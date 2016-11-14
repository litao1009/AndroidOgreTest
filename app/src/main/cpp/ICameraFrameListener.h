#pragma once

#include <Ogre.h>
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

public:

	static void	CameraFrameStart( Ogre::Camera *camera, const Ogre::FrameEvent &fevt );

	static	void	CameraFrameQueue( Ogre::Camera *camera, const Ogre::FrameEvent &fevt );

	static void	CameraFrameEnd( Ogre::Camera *camera, const Ogre::FrameEvent &fevt );

	static	void	CameraOnInputEvent( Ogre::Camera *camera, const PointerState &ps );

public:

	Ogre::Camera*	GetCamera() const;

private:

	Ogre::Camera*	Camera_{};
};
