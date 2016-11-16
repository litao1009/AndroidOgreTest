#include "ICameraFrameListener.h"

#include "Ogre.h"

#include <vector>

using	ICameraFrameListenerImpList = std::vector<ICameraFrameListener*>;
using	ICameraFrameListenerImpListSPtr = std::shared_ptr<ICameraFrameListenerImpList>;

class ICameraFrameListener::Imp
{
public:

	class 	StateSaver
	{
	public:

		float 			NearClipDistance_{};
		float 			FarClipDistance_{};
		float 			AspectRadio_{};
		bool 			AutoAspectRadio_{};
		bool 			UseFixedYawAxis_{};
		Ogre::Radian	Fovy_{};
		Ogre::ProjectionType ProjectType_{Ogre::PT_PERSPECTIVE};
		Ogre::Vector3	FixedYawAxis_{};
		Ogre::Quaternion	Orientation_ = Ogre::Quaternion::IDENTITY;

	public:

		void	Save(const Ogre::Camera* camera)
		{
			NearClipDistance_ = camera->getNearClipDistance();
			FarClipDistance_ = camera->getFarClipDistance();
			AutoAspectRadio_ = camera->getAutoAspectRatio();
			AspectRadio_ = camera->getAspectRatio();
			Fovy_ = camera->getFOVy();
			UseFixedYawAxis_ = camera->getUseFixedYawAxis();
			FixedYawAxis_ = camera->getFixedAxis();
			ProjectType_ = camera->getProjectionType();
			Orientation_ = camera->getOrientation();
		}

		void	Restore(Ogre::Camera* camera)
		{
			camera->setNearClipDistance(NearClipDistance_);
			camera->setFarClipDistance(FarClipDistance_);
			camera->setAutoAspectRatio(AutoAspectRadio_);
			camera->setAspectRatio(AspectRadio_);
			camera->setFOVy(Fovy_);
			camera->setFixedYawAxis(UseFixedYawAxis_, FixedYawAxis_);
			camera->setProjectionType(ProjectType_);
			camera->setOrientation(Orientation_);
		}
	};

public:

	Ogre::Camera*	Camera_{};
	StateSaver		PreviousState_;

public:

	static ICameraFrameListenerImpListSPtr	ValidatevaUserBinding(Ogre::Camera *camera)
	{
		auto& userBindings = camera->Ogre::MovableObject::getUserObjectBindings();

		if ( userBindings.getUserAny("ICameraListener").isEmpty() )
		{
			auto listPtr = std::make_shared<ICameraFrameListenerImpList>();
			userBindings.setUserAny("ICameraListener", Ogre::Any(listPtr));
		}

		auto listPtr = userBindings.getUserAny("ICameraListener").get<ICameraFrameListenerImpListSPtr>();

		return listPtr;
	}

};



ICameraFrameListener::ICameraFrameListener( Ogre::Camera *camera ):ImpUPtr_(new Imp)
{
	auto& imp_ = *ImpUPtr_;

	imp_.Camera_ = camera;
	imp_.PreviousState_.Save(camera);

	camera->detachFromParent();

	auto listPtr = Imp::ValidatevaUserBinding(camera);
	listPtr->push_back(this);
}

ICameraFrameListener::~ICameraFrameListener()
{
	auto& imp_ = *ImpUPtr_;

	imp_.PreviousState_.Restore(imp_.Camera_);

	auto listPtr = Imp::ValidatevaUserBinding(imp_.Camera_);
	assert(listPtr->back() == this);
	listPtr->pop_back();
}

Ogre::Camera* ICameraFrameListener::GetCamera() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.Camera_;
}

void ICameraFrameListener::CameraOnInputEvent( Ogre::Camera *camera, const PointerState &ps )
{
	auto listPtr = Imp::ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		listPtr->back()->OnInputEvent(ps);
	}
}

void ICameraFrameListener::CameraFrameStart( Ogre::Camera *camera, const Ogre::FrameEvent &fevt )
{
	auto listPtr = Imp::ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		auto& cur = listPtr->back();

		auto& imp_ = *(cur->ImpUPtr_);
		if ( !imp_.Camera_->isAttached() )
		{
			cur->_Attach();
		}

		cur->FrameStart(fevt);
	}
}

void ICameraFrameListener::CameraFrameQueue( Ogre::Camera *camera, const Ogre::FrameEvent &fevt )
{
	auto listPtr = Imp::ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		listPtr->back()->FrameQueue(fevt);
	}
}

void ICameraFrameListener::CameraFrameEnd( Ogre::Camera *camera, const Ogre::FrameEvent &fevt )
{
	auto listPtr = Imp::ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		listPtr->back()->FrameEnd(fevt);
	}
}