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


#include <thread>
#include <atomic>
#include <sstream>

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

public:

	void	Run()
	{
		while ( !Stop_ )
		{
			auto evt = EventQueue_.PopEventNonBlock();
			std::stringstream ss;
			ss << std::this_thread::get_id();
			LOGI("%s", ss.str().c_str());
		}
	}
};

NativeBridge::NativeBridge():ImpUPtr_(new Imp())
{
}

NativeBridge::~NativeBridge()
{
}

NativeBridge& NativeBridge::GetInstance()
{
	static NativeBridge sIns;

	return sIns;
}

void NativeBridge::Init()
{
	auto& imp_ = *ImpUPtr_;

	OgreEnv::GetInstance().Init();

	auto root = Ogre::Root::getSingletonPtr();

	auto gles2Plugin = new Ogre::GLES2Plugin();
	root->installPlugin(gles2Plugin);
	imp_.GLES2Plugin_.reset(gles2Plugin);

	root->setRenderSystem(root->getAvailableRenderers().front());
	root->initialise(false);
}

void NativeBridge::UnInit()
{
	auto& imp_ = *ImpUPtr_;

	OgreEnv::GetInstance().UnInit();

	imp_.GLES2Plugin_.reset();

	//imp_.Stop_ = true;

	//imp_.Thread_->join();
}
