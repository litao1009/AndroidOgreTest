#include "AndroidBridge.h"

#include <EGL/egl.h>
#include "Ogre.h"
#include "OgreGLES2Plugin.h"
#include "Compositor/OgreCompositorManager2.h"
#include "Android/OgreAndroidEGLWindow.h"
#include "Android/OgreAPKZipArchive.h"
#include "Android/OgreAPKFileSystemArchive.h"
#include "OIS.h"
#include "OgreEnv.h"
#include "InputEvent.h"

#include "SceneListener/DummySceneListener.h"

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidBridge", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidBridge", __VA_ARGS__))

class	AndroidBridge::Imp
{
public:

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
		case APP_CMD_START:
		{
			auto i = 0;
		}
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
				Ogre::ConfigFile cfg;
				{
					auto rscAsset = AAssetManager_open(assetMgr, "resources.cfg", AASSET_MODE_BUFFER);
					assert(rscAsset);
					auto length = AAsset_getLength(rscAsset);
					auto membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
					memcpy(membuf, AAsset_getBuffer(rscAsset), length);
					AAsset_close(rscAsset);
					Ogre::DataStreamPtr srcData(new Ogre::MemoryDataStream(membuf, length, true, true));
					cfg.load(srcData);
				}

				auto sectionItor = cfg.getSectionIterator();
				for ( auto& curSec : sectionItor )
				{
					auto sec = curSec.first;
					for ( auto& curSet : *(curSec.second) )
					{
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(curSet.second, curSet.first, sec);
					}
				}

				//load resources
				Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();

				auto dummyScene = std::make_shared<DummySceneListener>(imp_.MainWnd_);
				OgreEnv::GetInstance().AddFrameListener(dummyScene);

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

			AndroidBridge::GetInstance().Shutdown();
		}
			break;
		case APP_CMD_WINDOW_RESIZED:
		{
			int i = 0;
		}
			break;
		case APP_CMD_DESTROY:
		{

		}
			break;
		}
	}

	static int32_t	HandleInput(android_app* app, AInputEvent* evt)
	{
		auto& imp_ = *(AndroidBridge::GetInstance().ImpUPtr_);

		auto eventType = AInputEvent_getType(evt);

		switch (eventType)
		{
			case AINPUT_EVENT_TYPE_MOTION:
			{
				PointerState ps;

				auto source = AInputEvent_getSource(evt);
				switch (source)
				{
					case AINPUT_SOURCE_TOUCHSCREEN:
					{
						auto action = AMotionEvent_getAction(evt);

						ps.X.rel = AMotionEvent_getX(evt, 0);
						ps.Y.rel = AMotionEvent_getY(evt, 0);
						ps.X.abs = AMotionEvent_getRawX(evt, 0);
						ps.Y.abs = AMotionEvent_getRawY(evt, 0);
						ps.width = imp_.MainWnd_->getWidth();
						ps.height = imp_.MainWnd_->getHeight();

						switch (action)
						{
							case AMOTION_EVENT_ACTION_DOWN:
							{
								ps.State_ = PointerState::ES_Pressed;
							}
								break;
							case AMOTION_EVENT_ACTION_UP:
							{
								ps.State_ = PointerState::ES_Released;
							}
								break;
							case AMOTION_EVENT_ACTION_MOVE:
							{
								ps.State_ = PointerState::ES_Moved;
							}
								break;
						}

						OgreEnv::GetInstance().OnInputEvent(ps);
					}
						break;
					case AINPUT_SOURCE_TRACKBALL:
					{

					}
						break;
				}
			}
				break;
			case AINPUT_EVENT_TYPE_KEY:
			{

			}
				break;
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

	OgreEnv::GetInstance().Init();

	auto root = Ogre::Root::getSingletonPtr();

	auto gles2Plugin = new Ogre::GLES2Plugin();
	root->installPlugin(gles2Plugin);
	imp_.GLES2Plugin_.reset(gles2Plugin);

	root->setRenderSystem(root->getAvailableRenderers().front());
	root->initialise(false);
}

void AndroidBridge::Shutdown()
{
	auto& imp_ = *ImpUPtr_;

	OgreEnv::GetInstance().UnInit();

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
			OgreEnv::GetInstance().RenderOneFrame();
		}
	}
}