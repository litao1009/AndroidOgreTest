#pragma once

#include "OISPrereqs.h"

namespace OIS
{
	class	MouseState;
	class	MultiTouchState;
}

class PointerState
{
public:

	enum EState
	{
		ES_None,
		ES_Pressed,
		ES_Released,
		ES_Moved
	};

public:

	PointerState() = default;

	PointerState(const OIS::MouseState& state);

	PointerState(const OIS::MultiTouchState& state);

public:

	PointerState operator - (const PointerState& rhs) const;

public:

	int width{}, height{};
	OIS::Axis X{}, Y{}, Z{};
	EState State_{ES_None};
};

class	PointerRecoder
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	PointerRecoder();

	~PointerRecoder();

public:

	bool	HasPressed() const;

	bool	HasReleased() const;

	bool	HasMoved() const;

	void	SetPressed();

	void	SetReleased();

	void	SetMoved();

	void	SetPointerState(const PointerState& state);

	const PointerState&		GetState() const;

	void	Reset();
};

class	EventRecorder
{
	PointerRecoder	PointerRecoder_;

public:

	void	Reset();

	void	UpdatePointerState(const PointerState& state);

	const PointerRecoder&	GetPointerRecorder() const;

	static	PointerState&	GetStaticPointerState();
};