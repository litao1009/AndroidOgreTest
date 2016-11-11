#pragma once

#include "IFrameListenerFwd.h"
#include "InputEvent.h"

#include <memory>

namespace Ogre
{
	struct FrameEvent;
}

class IFrameListener : public std::enable_shared_from_this<IFrameListener>
{
	class	Imp;
	std::unique_ptr<Imp>	IFrameListenerUPtr_;

public:

	IFrameListener();

	virtual ~IFrameListener();

public:

	void	AddChild(const IFrameListenerSPtr& child);

	void	RemoveChild(const IFrameListenerSPtr& child);

public:

	void	FrameStart(const Ogre::FrameEvent& fevt);

	void	FrameQueue(const Ogre::FrameEvent& fevt);

	void	FrameEnd(const Ogre::FrameEvent& fevt);

public:

	void	OnInputEvent(const PointerState& ps);

	const EventRecorder&	GetEventRecorder() const;

public:

	virtual void	Load() {}

protected:

	virtual void	_FrameStart(const Ogre::FrameEvent& fevt) {}

	virtual void	_FrameStartPost(const Ogre::FrameEvent& fevt) {}

	virtual void	_FrameQueue(const Ogre::FrameEvent& fevt) {}

	virtual void	_FrameQueuePost(const Ogre::FrameEvent& fevt) {}

	virtual void	_FrameEnd(const Ogre::FrameEvent& fevt) {}

	virtual void	_FrameEndPost(const Ogre::FrameEvent& fevt) {}

	virtual void	_OnInputEvent(const PointerState& ps) {}
};