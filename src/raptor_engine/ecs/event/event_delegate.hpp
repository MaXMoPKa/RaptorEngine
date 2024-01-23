#pragma once

#include "ecs/event/i_event_delegate.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

class IEvent;

template <typename Class, typename EventType>
class EventDelegate : public IEventDelegate
{
	typedef void (Class::*Callback)(const EventType* const);

public:

	EventDelegate(Class* receiver_, Callback& callbackFunction_)
		: receiver(receiver_)
		, callback(callbackFunction_)
	{
	}
	virtual ~EventDelegate() = default;

public:
	virtual IEventDelegate* clone() override
	{
		return new EventDelegate(this->receiver, this->callback);
	}

	virtual inline void invoke(const IEvent* const e) override
	{
		(receiver->*callback)(reinterpret_cast<const EventType* const>(e));
	}

	virtual inline EventDelegateId GetId() const override
	{
		static const EventDelegateId DELEGATE_ID {typeid(Class).hash_code() ^ typeid(Callback).hash_code()};
		return DELEGATE_ID;
	}

	virtual inline u64 GetStaticEventTypeId() const override
	{
		static const u64 SEID {EventType::STATIC_EVENT_TYPE_ID};
		return SEID;
	}

	virtual bool operator==(const IEventDelegate* other_) const override
	{
		if (other_ == nullptr || this->GetId() != other_->GetId())
		{
			return false;
		}

		EventDelegate* delegate = (EventDelegate*)other_;

		return ((this->callback == delegate->callback) && (this->receiver == delegate->receiver));
	}

private:

	inline void SetReceiver(Class* receiver_)
	{
		this->receiver = receiver_;
	}
	inline auto& GetReceiver()
	{
		return this->receiver;
	}
	inline const auto& GetReceiver() const
	{
		return this->receiver;
	}

	inline void SetCallback(const Callback& callback_)
	{
		this->callback = callback_;
	}
	inline auto& GetCallback()
	{
		return this->callback;
	}
	inline const auto& GetCallback() const
	{
		return this->callback;
	}

private:
	Class* receiver;
	Callback callback;
};

} // namespace event
} // namespace ecs
} // namespace raptor_engine