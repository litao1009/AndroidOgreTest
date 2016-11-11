#pragma once

#include "IFrameListener.h"

namespace Ogre
{
	class	Camera;
}

class ICameraFrameListener : public IFrameListener
{

public:

	ICameraFrameListener(Ogre::Camera* camera);

	~ICameraFrameListener();

public:

	Ogre::Camera*	GetCamera() const;

private:

	Ogre::Camera*	Camera_{};
	uint32_t 		BindingKey_{};
};
