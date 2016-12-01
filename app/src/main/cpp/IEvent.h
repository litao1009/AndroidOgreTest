#pragma once

#include <memory>

class 	IEvent
{
public:

	virtual ~IEvent () {}
};

using	IEventSPtr = std::shared_ptr<IEvent>;
using	IEventWPtr = std::weak_ptr<IEvent>;