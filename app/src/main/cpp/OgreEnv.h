#pragma once

#include <memory>
#include <functional>

#include "IFrameListenerFwd.h"
#include "InputEventFwd.h"

class OgreEnv
{
	class Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

	OgreEnv();

public:

public:

	~OgreEnv();

public:

	static OgreEnv&	GetInstance();

public:

	void	Init();

	void	UnInit();

	void	RenderOneFrame();

public:

	void	OnInputEvent(const PointerState& evt);

public:

	void	AddFrameListener(const IFrameListenerSPtr& frameListener);

	void	RemoveFrameListener(const IFrameListenerSPtr& frameListener);
};