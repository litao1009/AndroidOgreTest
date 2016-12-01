#pragma once

#include <memory>

#include "IEvent.h"

class EventQueue
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	EventQueue();

	~EventQueue();

public:

	void	SetMaxEventsSize(uint32_t maxNr);

	void	PushEvent(const IEventSPtr& event);

	IEventSPtr	PopEvent();

	IEventSPtr 	PopEventNonBlock();

	bool	Start();

	void	Stop();

	bool	IsStop() const;

	void	Clean();
};