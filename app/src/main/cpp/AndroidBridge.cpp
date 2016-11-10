#include "AndroidBridge.h"

#include <EGL/egl.h>
#include "Ogre.h"
#include "OgreGLES2Plugin.h"
#include "Compositor/OgreCompositorManager2.h"
#include "Android/OgreAndroidEGLWindow.h"
#include "Android/OgreAPKZipArchive.h"
#include "Android/OgreAPKFileSystemArchive.h"

class	AndroidBridge::Imp
{
public:

	std::unique_ptr<Ogre::Root>			Root_;
	std::unique_ptr<Ogre::GLES2Plugin>	GLES2Plugin_;
	android_app* 						AndroidApp_{};
	Ogre::RenderWindow*					MainWnd_{};

public:

	static void	HandleCMD(android_app* app, int32_t cmd)
	{
		switch (cmd)
		{
			case APP_CMD_SAVE_STATE:
				break;
			case APP_CMD_INIT_WINDOW:
			{
				auto& imp_ = *(AndroidBridge::GetInstance().ImpUPtr_);

				if ( nullptr == imp_.AndroidApp_ || nullptr == Ogre::Root::getSingletonPtr())
				{
					break;
				}

				auto config = AConfiguration_new();
				AConfiguration_fromAssetManager(config, imp_.AndroidApp_->activity->assetManager);

				if ( nullptr == imp_.MainWnd_ )
				{
					Ogre::NameValuePairList opt;
					opt["externalWindowHandle"] = std::to_string(reinterpret_cast<size_t>(imp_.AndroidApp_->window));
					opt["androidConfig"] = std::to_string(reinterpret_cast<size_t>(config));

					imp_.MainWnd_ = Ogre::Root::getSingletonPtr()->createRenderWindow("MainWnd", 0, 0, false, &opt);

					//input

					auto assetMgr = imp_.AndroidApp_->activity->assetManager;
					Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(assetMgr));
					Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(assetMgr));

					//Ogre::Root::getSingletonPtr()->initialise(true);

					//locate resources
					Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("Models", "APKFileSystem");
					Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("Materials", "APKFileSystem");
					Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("Materials/Programs", "APKFileSystem");
					Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("Textures", "APKFileSystem");

					//load resources
					Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();

					//dummp scene
					{
						auto smgr = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD, "DummyScene");
						auto camera = smgr->createCamera("MainCamera");
						camera->setAutoAspectRatio(true);
						auto comMgr = Ogre::Root::getSingletonPtr()->getCompositorManager2();
						comMgr->createBasicWorkspaceDef("DummyWorkspaceDef", Ogre::ColourValue(.6f, 0.6f, .6f), {});
						auto workspace = comMgr->addWorkspace(smgr, imp_.MainWnd_, camera, "DummyWorkspaceDef", true);

						auto light = smgr->createLight();
						smgr->setAmbientLight(Ogre::ColourValue(1.f, 1.f, 1.f));
						auto lightNode = smgr->getRootSceneNode()->createChildSceneNode();
						lightNode->setPosition({20.f, 80.f, 50.f});
						lightNode->attachObject(light);
						light->setDirection({-1.f, -1.f, -1.f});
						light->setDiffuseColour(1.f, 1.f, 1.f);

						auto head = smgr->createEntity("ogrehead.mesh");
						auto headNode = smgr->getRootSceneNode()->createChildSceneNode();
						headNode->attachObject(head);
						headNode->setPosition({0.f, 0.f, 0.f});

						auto camNode = smgr->getRootSceneNode()->createChildSceneNode();
						camera->detachFromParent();
						camNode->attachObject(camera);
						//camNode->setPosition({0.f, 0.f, 100.f});
						//camNode->setDirection({0.f, 0.f, -1.f});
						camera->setPosition(30.f, 90.f, 150.f);
						camera->lookAt({0.f, 0.f, 0.f});
						camera->setNearClipDistance(10.f);
						camera->setFarClipDistance(1000.f);
					}

					Ogre::Root::getSingletonPtr()->getRenderSystem()->_initRenderTargets();

					Ogre::Root::getSingletonPtr()->clearEventTimes();
				}
				else
				{
					static_cast<Ogre::AndroidEGLWindow*>(imp_.MainWnd_)->_createInternalResources(imp_.AndroidApp_->window, config);
				}

				AConfiguration_delete(config);
			}
				break;
			case APP_CMD_TERM_WINDOW:
			{
				auto& imp_ = *(AndroidBridge::GetInstance().ImpUPtr_);

				if ( nullptr != Ogre::Root::getSingletonPtr() && nullptr != imp_.MainWnd_)
				{
					static_cast<Ogre::AndroidEGLWindow*>(imp_.MainWnd_)->_destroyInternalResources();
				}
			}
				break;
		}
	}

	static int32_t	HandleInput(android_app* app, AInputEvent* evt)
	{
		if (AInputEvent_getType(evt) == AINPUT_EVENT_TYPE_MOTION)
		{
			int action = (int)(AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(evt));
		}
		else
		{

		}

		return 1;
	}
};

AndroidBridge::AndroidBridge():ImpUPtr_(new Imp())
{ }

AndroidBridge::~AndroidBridge()
{ }

AndroidBridge& AndroidBridge::GetInstance()
{
	static AndroidBridge ins;

	return  ins;
}

void AndroidBridge::Init( android_app *state )
{
	state->onAppCmd = &Imp::HandleCMD;
	state->onInputEvent = &Imp::HandleInput;

	auto& imp_ = *ImpUPtr_;

	imp_.AndroidApp_ = state;

	auto root = new Ogre::Root();
	imp_.Root_.reset(root);

	auto gles2Plugin = new Ogre::GLES2Plugin();
	root->installPlugin(gles2Plugin);
	imp_.GLES2Plugin_.reset(gles2Plugin);

	root->setRenderSystem(root->getAvailableRenderers().front());
	root->initialise(false);
}

void AndroidBridge::Shutdown()
{
	auto& imp_ = *ImpUPtr_;

	imp_.Root_.reset();
	imp_.GLES2Plugin_.reset();
}

void AndroidBridge::Run()
{
	auto& imp_ = *ImpUPtr_;

	int ident, events;
	android_poll_source* source;

	while (true)
	{
		while ((ident = ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0)
		{
			if (source != NULL)
			{
				source->process(imp_.AndroidApp_, source);
			}

			if (imp_.AndroidApp_->destroyRequested != 0)
			{
				return;
			}
		}

		if ( nullptr != imp_.MainWnd_ && imp_.MainWnd_->isActive() )
		{
			imp_.MainWnd_->windowMovedOrResized();
			Ogre::Root::getSingletonPtr()->renderOneFrame();
		}
	}
}