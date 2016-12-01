#pragma once

#include "IEventFwd.h"

#include <memory>

struct AAssetManager;

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

	void	Init(AAssetManager* assetMgr);

	void	UnInit();

	void	PostEvent(const IEventSPtr& evt);
};
