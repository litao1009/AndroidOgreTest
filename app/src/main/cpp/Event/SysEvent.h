#pragma once

#include "IEvent.h"

struct ANativeWindow;

class 	SysEVT_SufaceCreated : public TEvent<SysEVT_SufaceCreated>
{
public:
	ANativeWindow*	NativeWindow{};
};
