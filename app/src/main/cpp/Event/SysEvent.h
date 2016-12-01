#pragma once

#include "IEvent.h"

struct ANativeWindow;

class 	SysEVT_SufaceCreated : public TEvent<SysEVT_SufaceCreated>
{
public:
	ANativeWindow*	NativeWindow{};
};


class 	SysEVT_MotionTouch : public TEvent<SysEVT_MotionTouch>
{
public:

	int Source{};
	int Action{};
	float X{};
	float Y{};
	float RawX{};
	float RawY{};
};