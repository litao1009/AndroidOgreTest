#pragma once

#include "IEventFwd.h"

#include <memory>
#include <functional>

class 	IEvent : public std::enable_shared_from_this<IEvent>
{
public:

	virtual ~IEvent () {}

public:

	uint32_t GetTypeID() const;
};

template <typename T>
class 	TEvent : public IEvent
{
public:

	static uint32_t GetStaticTypeID()
	{
		return typeid(T).hash_code();
	}

	static std::shared_ptr<T>	CastFrom(const IEventSPtr& evt)
	{
		return std::static_pointer_cast<T>(evt);
	}

	static std::shared_ptr<T>	Create()
	{
		return std::make_shared<T>();
	}
};