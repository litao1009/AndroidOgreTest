#include "InputEvent.h"

#include "OIS.h"

#include <bitset>

PointerState::PointerState( const OIS::MouseState &state )
{
	width = state.width;
	height = state.height;
	X = state.X;
	Y = state.Y;
	Z = state.Z;
}

PointerState::PointerState( const OIS::MultiTouchState &state )
{
	width = state.width;
	height = state.height;
	X = state.X;
	Y = state.Y;
	Z = state.Z;

	switch (state.touchType)
	{
		case  OIS::MT_Pressed:
			State_ = ES_Pressed;
			break;
		case  OIS::MT_Released:
			State_ = ES_Released;
			break;
		case  OIS::MT_Moved:
			State_ = ES_Moved;
			break;
	}
}


class	PointerRecoder::Imp
{
public:

	enum	EFlag
	{
		EF_Pressed,
		EF_Released,
		EF_Move,
		EF_COUNT
	};

	std::bitset<EF_COUNT>	Flags_;
	PointerState			State_;
};

PointerRecoder::PointerRecoder():ImpUPtr_(new Imp)
{ }

PointerRecoder::~PointerRecoder()
{ }

bool PointerRecoder::HasPressed() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.Flags_.test(Imp::EF_Pressed);
}

void PointerRecoder::SetPressed()
{
	auto& imp_ = *ImpUPtr_;

	imp_.Flags_.set(Imp::EF_Pressed);
}

bool PointerRecoder::HasReleased() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.Flags_.test(Imp::EF_Released);
}

void PointerRecoder::SetReleased()
{
	auto& imp_ = *ImpUPtr_;

	imp_.Flags_.set(Imp::EF_Pressed);
}

bool PointerRecoder::HasMoved() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.Flags_.test(Imp::EF_Move);
}

void PointerRecoder::SetMoved()
{
	auto& imp_ = *ImpUPtr_;

	imp_.Flags_.set(Imp::EF_Move);
}

const PointerState& PointerRecoder::GetState() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.State_;
}
PointerState PointerState::operator-( const PointerState &rhs ) const
{
	PointerState ret;
	ret.X.rel = X.rel - rhs.X.rel;
	ret.Y.rel = Y.rel - rhs.Y.rel;
	ret.X.abs = X.abs - rhs.X.abs;
	ret.Y.abs = Y.abs - rhs.Y.abs;

	return ret;
}

void PointerRecoder::SetPointerState( const PointerState &state )
{
	auto& imp_ = *ImpUPtr_;

	imp_.State_ = state;
}

void PointerRecoder::Reset()
{
	auto& imp_ = *ImpUPtr_;

	imp_.Flags_.reset();
}


const PointerRecoder& EventRecorder::GetPointerRecorder() const
{
	return PointerRecoder_;
}

void EventRecorder::Reset()
{
	PointerRecoder_.Reset();
}

void EventRecorder::UpdatePointerState( const PointerState &state )
{
	PointerRecoder_.SetPointerState(state);

	switch (state.State_)
	{
		case PointerState::ES_Pressed:
		{
			PointerRecoder_.SetPressed();
		}
			break;
		case PointerState::ES_Released:
		{
			PointerRecoder_.SetReleased();
		}
			break;
		case PointerState::ES_Moved:
		{
			PointerRecoder_.SetMoved();
		}
	}
}

PointerState& EventRecorder::GetStaticPointerState()
{
	static PointerState sins;

	return sins;
}