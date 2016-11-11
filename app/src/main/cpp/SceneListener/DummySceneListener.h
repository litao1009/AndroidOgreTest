#pragma once

#include <Ogre.h>
#include "IFrameListener.h"

#include "OgrePrerequisites.h"

class DummySceneListener : public IFrameListener
{
	class 	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	DummySceneListener(Ogre::RenderWindow* rt);

	~DummySceneListener();

public:

	virtual void	Load();
};

