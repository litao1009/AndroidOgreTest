#pragma once

#include "IEvent.h"

#include <memory>
#include <list>

class EventQueue
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	using	EventList = std::list<IEventSPtr >;

public:

	EventQueue();

	~EventQueue();

public:

	void	SetMaxEventsSize(uint32_t maxNr);

	void	PushEvent(const IEventSPtr& event);

	IEventSPtr	PopEvent(bool block);

	EventList	PopAllEvent(bool block);

	bool	Start();

	void	Stop();

	bool	IsStop() const;

	void	Clean();
};