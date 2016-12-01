#include "IEvent.h"


uint32_t IEvent::GetTypeID() const
{
	return typeid(*this).hash_code();
}