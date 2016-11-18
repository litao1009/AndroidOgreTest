#include "OgreEnv.h"
#include "InputEvent.h"
#include "IFrameListener.h"
#include "ICameraFrameListener.h"
#include "OgreParticleFXPlugin.h"

#include "Ogre.h"

#include <vector>

class	OgreEnv::Imp : public Ogre::FrameListener
{
public:

	std::vector<IFrameListenerSPtr>	ChildList_;
	std::vector<Ogre::Camera*>		UpdateCameraList_;
	std::vector<Ogre::Plugin*>		Plugins_;

public:

	virtual bool frameStarted(const Ogre::FrameEvent& evt) override
	{
		auto smgrList = Ogre::Root::getSingletonPtr()->getSceneManagerIterator();
		for (auto& curSmgr : smgrList )
		{
			auto cameraList = curSmgr.second->getCameraIterator();
			for (auto& curCamera : cameraList )
			{
				UpdateCameraList_.push_back(curCamera);
			}
		}

		for (auto& curCamera : UpdateCameraList_ )
		{
			ICameraFrameListener::CameraFrameStart(curCamera, evt);
		}

		for (auto& curChild : ChildList_)
		{
			curChild->FrameStart(evt);
		}

		return true;
	}

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) override
	{
		for (auto& curCamera : UpdateCameraList_ )
		{
			ICameraFrameListener::CameraFrameQueue(curCamera, evt);
		}

		for (auto& curChild : ChildList_)
		{
			curChild->FrameQueue(evt);
		}

		return true;
	}

	virtual bool frameEnded(const Ogre::FrameEvent& evt) override
	{
		for (auto& curCamera : UpdateCameraList_ )
		{
			ICameraFrameListener::CameraFrameEnd(curCamera, evt);
		}

		for (auto& curChild : ChildList_)
		{
			curChild->FrameEnd(evt);
		}

		UpdateCameraList_.clear();

		return true;
	}
};

OgreEnv::OgreEnv():ImpUPtr_(new Imp)
{ }

OgreEnv::~OgreEnv()
{ }

OgreEnv& OgreEnv::GetInstance()
{
	static OgreEnv sins;
	return  sins;
}

void OgreEnv::Init()
{
	auto& imp_ = *ImpUPtr_;

	auto root = new Ogre::Root();

	auto particleFX = new Ogre::ParticleFXPlugin();

	imp_.Plugins_.push_back(particleFX);

	for (auto& curPlugin : imp_.Plugins_ )
	{
		root->installPlugin(curPlugin);
	}

	root->addFrameListener(ImpUPtr_.get());
}

void OgreEnv::UnInit()
{
	auto& imp_ = *ImpUPtr_;

	imp_.ChildList_.clear();

	delete Ogre::Root::getSingletonPtr();

	for (auto& curPlugin : imp_.Plugins_ )
	{
		delete curPlugin;
	}
}

void OgreEnv::RenderOneFrame()
{
	static auto s_LastTime = 0U;

	auto& root = *Ogre::Root::getSingletonPtr();

	auto curTime = root.getTimer()->getMicroseconds();
	auto frameTime = curTime - s_LastTime;
	auto limitFPS = 60;
	auto limitFrameTime = 1E6 / static_cast<float >(limitFPS);

	if ( frameTime < limitFrameTime )
	{
		return;
	}

	root.renderOneFrame();
}

void OgreEnv::OnInputEvent( const PointerState &evt )
{
	auto& imp_ = *ImpUPtr_;

	EventRecorder::GetStaticPointerState() = evt;

	auto smgrList = Ogre::Root::getSingletonPtr()->getSceneManagerIterator();
	for (auto& curSmgr : smgrList )
	{
		auto cameraList = curSmgr.second->getCameraIterator();
		for (auto& curCamera : cameraList )
		{
			ICameraFrameListener::CameraOnInputEvent(curCamera, evt);
		}
	}

	for (auto& curChild : imp_.ChildList_)
	{
		curChild->OnInputEvent(evt);
	}
}

void OgreEnv::AddFrameListener( const IFrameListenerSPtr &frameListener )
{
	auto& imp_ = *ImpUPtr_;

	imp_.ChildList_.push_back(frameListener);
}

void OgreEnv::RemoveFrameListener( const IFrameListenerSPtr &frameListener )
{
	auto& imp_ = *ImpUPtr_;

	auto itor = std::remove(imp_.ChildList_.begin(), imp_.ChildList_.end(), frameListener);
	imp_.ChildList_.erase(itor, imp_.ChildList_.end());
}