#include "IFrameListener.h"

#include <vector>

class	IFrameListener::Imp
{
public:

	EventRecorder						EventRecorder_;
	std::vector<IFrameListenerSPtr >	ChildList_;
};

IFrameListener::IFrameListener():IFrameListenerUPtr_(new Imp)
{

}

IFrameListener::~IFrameListener()
{ }

void IFrameListener::AddChild( const IFrameListenerSPtr &child )
{
	auto& imp_ = *IFrameListenerUPtr_;

	imp_.ChildList_.push_back(child);
}

void IFrameListener::RemoveChild( const IFrameListenerSPtr &child )
{
	auto& imp_ = *IFrameListenerUPtr_;

	auto itor = std::remove(imp_.ChildList_.begin(), imp_.ChildList_.end(), child);
	imp_.ChildList_.erase(itor, imp_.ChildList_.end());
}

void IFrameListener::FrameStart( const Ogre::FrameEvent &fevt )
{
	auto& imp_ = *IFrameListenerUPtr_;

	_FrameStart(fevt);

	for (auto& curChild : imp_.ChildList_)
	{
		curChild->FrameStart(fevt);
	}

	_FrameStartPost(fevt);
}

void IFrameListener::FrameQueue( const Ogre::FrameEvent &fevt )
{
	auto& imp_ = *IFrameListenerUPtr_;

	_FrameQueue(fevt);

	for (auto& curChild : imp_.ChildList_)
	{
		curChild->FrameQueue(fevt);
	}

	_FrameQueuePost(fevt);
}

void IFrameListener::FrameEnd( const Ogre::FrameEvent &fevt )
{
	auto& imp_ = *IFrameListenerUPtr_;

	_FrameEnd(fevt);

	for (auto& curChild : imp_.ChildList_)
	{
		curChild->FrameEnd(fevt);
	}

	_FrameEndPost(fevt);

	imp_.EventRecorder_.Reset();
}

void IFrameListener::OnInputEvent( const PointerState &ps )
{
	auto& imp_ = *IFrameListenerUPtr_;

	imp_.EventRecorder_.UpdatePointerState(ps);

	_OnInputEvent(ps);

	for (auto& curChild : imp_.ChildList_)
	{
		curChild->OnInputEvent(ps);
	}
}

const EventRecorder& IFrameListener::GetEventRecorder() const
{
	auto& imp_ = *IFrameListenerUPtr_;

	return imp_.EventRecorder_;
}