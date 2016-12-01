#include "NativeBridge.h"

#include "EventQueue.h"


#include <EGL/egl.h>
#include "Ogre.h"
#include "OgreGLES2Plugin.h"
#include "Compositor/OgreCompositorManager2.h"
#include "Android/OgreAndroidEGLWindow.h"
#include "Android/OgreAPKZipArchive.h"
#include "Android/OgreAPKFileSystemArchive.h"
#include "OIS.h"
#include "OgreEnv.h"
#include "Event/SysEvent.h"
#include "SceneListener/DummySceneListener.h"


#include <thread>
#include <atomic>
#include <sstream>
#include <unordered_map>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Test", __VA_ARGS__))

class 	NativeBridge::Imp
{
public:

	std::unique_ptr<std::thread>	Thread_;
	EventQueue						EventQueue_;
	std::atomic_bool				Stop_{false};

	std::unique_ptr<Ogre::GLES2Plugin>	GLES2Plugin_;
	Ogre::RenderWindow*					MainWnd_{};

	AAssetManager*						AssetMgr_{};

	std::unordered_map<uint32_t, std::function<void(const IEventSPtr&)>>	EventDispathMap_;

public:

	void	Run()
	{
		while ( !Stop_ )
		{
			while ( true )
			{
				auto block = MainWnd_ == nullptr;

				auto eventList  = EventQueue_.PopAllEvent(block);
				if ( eventList.empty() )
				{
					break;
				}

				for ( auto& curEvent : eventList )
				{
					auto itor = EventDispathMap_.find(curEvent->GetTypeID());
					if ( itor != EventDispathMap_.end() )
					{
						auto& func = itor->second;
						func(curEvent);
					}
				}
			}

			if ( MainWnd_ )
			{
				OgreEnv::GetInstance().RenderOneFrame();
			}

			std::stringstream ss;
			ss << std::this_thread::get_id();
			LOGI("%s", ss.str().c_str());
		}
	}
};

NativeBridge::NativeBridge():ImpUPtr_(new Imp())
{
	auto& imp_ = *ImpUPtr_;

	imp_.EventDispathMap_.emplace(SysEVT_SufaceCreated::GetStaticTypeID(), [&imp_](const IEventSPtr& evt)
	{
		auto sysEvt = SysEVT_SufaceCreated::CastFrom(evt);

		auto config = AConfiguration_new();
		AConfiguration_fromAssetManager(config, imp_.AssetMgr_);

		if ( nullptr == imp_.MainWnd_ )
		{
			Ogre::NameValuePairList opt;
			opt["externalWindowHandle"] = std::to_string(reinterpret_cast<size_t>(sysEvt->NativeWindow));
			opt["androidConfig"] = std::to_string(reinterpret_cast<size_t>(config));

			imp_.MainWnd_ = Ogre::Root::getSingletonPtr()->createRenderWindow("MainWnd", 0, 0, false, &opt);

			//input
			Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(imp_.AssetMgr_));
			Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(imp_.AssetMgr_));

			//Ogre::Root::getSingletonPtr()->initialise(true);

			//locate resources
			Ogre::ConfigFile cfg;
			{
				auto rscAsset = AAssetManager_open(imp_.AssetMgr_, "resources.cfg", AASSET_MODE_BUFFER);
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
			static_cast<Ogre::AndroidEGLWindow*>(imp_.MainWnd_)->_createInternalResources(sysEvt->NativeWindow, config);
		}

		AConfiguration_delete(config);
	});
}

NativeBridge::~NativeBridge()
{
}

NativeBridge& NativeBridge::GetInstance()
{
	static NativeBridge sIns;

	return sIns;
}

void NativeBridge::Init(AAssetManager* assetMgr)
{
	auto& imp_ = *ImpUPtr_;
	imp_.AssetMgr_ = assetMgr;

	OgreEnv::GetInstance().Init();

	auto root = Ogre::Root::getSingletonPtr();

	auto gles2Plugin = new Ogre::GLES2Plugin();
	root->installPlugin(gles2Plugin);
	imp_.GLES2Plugin_.reset(gles2Plugin);

	root->setRenderSystem(root->getAvailableRenderers().front());
	root->initialise(false);

	imp_.EventQueue_.Start();
	imp_.Thread_.reset(new std::thread([&imp_]()
	{
		imp_.Run();
	}));
}

void NativeBridge::UnInit()
{
	auto& imp_ = *ImpUPtr_;

	imp_.Stop_ = true;
	imp_.EventQueue_.Stop();

	imp_.Thread_->join();

	OgreEnv::GetInstance().UnInit();

	imp_.GLES2Plugin_.reset();
}

void NativeBridge::PostEvent( const IEventSPtr &evt )
{
	auto& imp_ = *ImpUPtr_;

	imp_.EventQueue_.PushEvent(evt);
}