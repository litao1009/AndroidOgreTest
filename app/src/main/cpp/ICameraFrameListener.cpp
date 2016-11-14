#include "ICameraFrameListener.h"

#include "Ogre.h"

#include <vector>

using	ICameraFrameListenerImpList = std::vector<ICameraFrameListener*>;
using	ICameraFrameListenerImpListSPtr = std::shared_ptr<ICameraFrameListenerImpList>;

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

ICameraFrameListener::ICameraFrameListener( Ogre::Camera *camera )
{
	auto listPtr = ValidatevaUserBinding(camera);
	Camera_ = camera;

	if ( !listPtr->empty() )
	{
		listPtr->back()->Detach();
	}

	listPtr->push_back(this);
}

ICameraFrameListener::~ICameraFrameListener()
{
	auto listPtr = ValidatevaUserBinding(Camera_);

	assert(listPtr->back() == this);

	listPtr->pop_back();
}

Ogre::Camera* ICameraFrameListener::GetCamera() const
{
	return Camera_;
}

void ICameraFrameListener::CameraOnInputEvent( Ogre::Camera *camera, const PointerState &ps )
{
	auto listPtr = ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		listPtr->back()->OnInputEvent(ps);
	}
}

void ICameraFrameListener::CameraFrameStart( Ogre::Camera *camera, const Ogre::FrameEvent &fevt )
{
	auto listPtr = ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		auto& cur = listPtr->back();

		if ( !cur->Attached() )
		{
			cur->Attach();
		}

		cur->FrameStart(fevt);
	}
}

void ICameraFrameListener::CameraFrameQueue( Ogre::Camera *camera, const Ogre::FrameEvent &fevt )
{
	auto listPtr = ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		listPtr->back()->FrameQueue(fevt);
	}
}

void ICameraFrameListener::CameraFrameEnd( Ogre::Camera *camera, const Ogre::FrameEvent &fevt )
{
	auto listPtr = ValidatevaUserBinding(camera);

	if ( !listPtr->empty() )
	{
		listPtr->back()->FrameEnd(fevt);
	}
}

void ICameraFrameListener::Attach()
{
	if ( !Attach_ )
	{
		_Attach();

		Attach_ = true;
	}
}

void ICameraFrameListener::Detach()
{
	if ( Attach_ )
	{
		_Detach();

		Attach_ = false;
	}
}

bool ICameraFrameListener::Attached() const
{
	return Attach_;
}

