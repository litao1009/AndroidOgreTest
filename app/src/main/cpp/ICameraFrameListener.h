#pragma once

#include <Ogre.h>
#include "IFrameListener.h"

namespace Ogre
{
	class	Camera;
}

class ICameraFrameListener : public IFrameListener
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	ICameraFrameListener(Ogre::Camera* camera);

	~ICameraFrameListener();

protected:

	virtual void	ReAttach() {}

public:

	static void	CameraFrameStart( Ogre::Camera *camera, const Ogre::FrameEvent &fevt );

	static	void	CameraFrameQueue( Ogre::Camera *camera, const Ogre::FrameEvent &fevt );

	static void	CameraFrameEnd( Ogre::Camera *camera, const Ogre::FrameEvent &fevt );

	static	void	CameraOnInputEvent( Ogre::Camera *camera, const PointerState &ps );

public:

	Ogre::Camera*	GetCamera() const;
};
