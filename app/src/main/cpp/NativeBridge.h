#pragma once

#include <memory>

class NativeBridge
{
	class Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	static	NativeBridge&	GetInstance();

public:

	NativeBridge();

	~NativeBridge ();

public:

	void	Init();

	void	UnInit();
};
