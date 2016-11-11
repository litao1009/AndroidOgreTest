#include "ICameraFrameListener.h"

#include "Ogre.h"

#include <map>

using	ICameraFrameListenerImpList = std::map<uint32_t, ICameraFrameListener* >;
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
	//auto listPtr = ValidatevaUserBinding(camera);

	BindingKey_ = reinterpret_cast<uint32_t>(static_cast<void*>(this));
	Camera_ = camera;

	//listPtr->emplace(BindingKey_, this);
}

ICameraFrameListener::~ICameraFrameListener()
{
	//auto listPtr = ValidatevaUserBinding(Camera_);

	//listPtr->erase(BindingKey_);
}

Ogre::Camera* ICameraFrameListener::GetCamera() const
{
	return Camera_;
}

