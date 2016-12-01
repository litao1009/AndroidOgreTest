#include "EventQueue.h"

#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>


class EventQueue::Imp
{
public:

	EventList					Queue_;

	std::atomic_uint_least32_t	MaxSize_{1000};
	std::atomic_bool			StopFlag_{true};
	std::mutex					Mutex_;
	std::condition_variable		EmptyCondition_;
	std::condition_variable		FullCondition_;
};

EventQueue::EventQueue() :ImpUPtr_(new Imp())
{

}

EventQueue::~EventQueue()
{

}

void EventQueue::SetMaxEventsSize(uint32_t maxNr)
{
	auto& imp_ = *ImpUPtr_;

	imp_.MaxSize_ = maxNr;
}

void EventQueue::PushEvent(const IEventSPtr& event)
{
	auto& imp_ = *ImpUPtr_;

	{
		std::unique_lock<std::mutex> lock(imp_.Mutex_);

		while ( imp_.Queue_.size() > imp_.MaxSize_ && !imp_.StopFlag_ )
		{
			imp_.FullCondition_.wait(lock);
		}

		imp_.Queue_.push_front(event);
	}

	imp_.EmptyCondition_.notify_one();
}

IEventSPtr EventQueue::PopEvent(bool block)
{
	auto& imp_ = *ImpUPtr_;

	if ( imp_.StopFlag_ )
	{
		return nullptr;
	}

	IEventSPtr event;
	{
		std::unique_lock<std::mutex> lock(imp_.Mutex_);

		if ( block )
		{
			while ( imp_.Queue_.empty() && !imp_.StopFlag_ )
			{
				imp_.EmptyCondition_.wait(lock);
			}

			event = imp_.Queue_.back();
			imp_.Queue_.pop_back();
		}
		else
		{
			if ( imp_.Queue_.empty() )
			{
				return nullptr;
			}

			event = imp_.Queue_.back();
			imp_.Queue_.pop_back();
		}
	}

	imp_.FullCondition_.notify_one();

	return event;
}

EventQueue::EventList EventQueue::PopAllEvent(bool block)
{
	auto& imp_ = *ImpUPtr_;

	if ( imp_.StopFlag_ )
	{
		return {};
	}

	EventList tmp;
	{
		std::unique_lock<std::mutex> lock(imp_.Mutex_);

		if ( block )
		{
			while ( imp_.Queue_.empty() && !imp_.StopFlag_ )
			{
				imp_.EmptyCondition_.wait(lock);
			}
		}

		{
			using namespace std;
			swap(tmp, imp_.Queue_);
		}
	}

	imp_.FullCondition_.notify_all();

	return tmp;
}

bool EventQueue::Start()
{
	auto& imp_ = *ImpUPtr_;

	auto ret = imp_.StopFlag_.exchange(false);

	return ret;
}

void EventQueue::Stop()
{
	auto& imp_ = *ImpUPtr_;

	auto ret = imp_.StopFlag_.exchange(true);

	if ( !ret )
	{
		imp_.EmptyCondition_.notify_all();
	}
}

bool EventQueue::IsStop() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.StopFlag_;
}

void EventQueue::Clean()
{
	auto& imp_ = *ImpUPtr_;

	std::unique_lock<std::mutex> lock(imp_.Mutex_);

	imp_.Queue_.clear();
}