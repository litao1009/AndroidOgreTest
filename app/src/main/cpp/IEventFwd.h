#pragma once

#include <memory>

class	IEvent;
using	IEventSPtr = std::shared_ptr<IEvent>;
using	IEventWPtr = std::weak_ptr<IEvent>;