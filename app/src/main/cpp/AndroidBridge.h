#pragma once

#include "android_native_app_glue.h"

#include <memory>

class AndroidBridge
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

	AndroidBridge();

public:

	~AndroidBridge();

public:

	static	AndroidBridge&	GetInstance();

public:

	void	Init(android_app* state);

	void	Shutdown();

	void	Run();
};