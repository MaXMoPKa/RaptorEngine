#pragma once

#include <chrono>

#include "platform.hpp"

namespace raptor_engine 
{
namespace ecs 
{
namespace util
{

class Timer
{
    using Elapsed = std::chrono::duration<f32, std::milli>;

public:
	Timer()  = default;
	~Timer() = default;

public:
	void Tick(f32 ms) {};

	void Reset() {};

	[[nodiscard]] inline TimeStamp GetTimeStamp() const
	{
		return TimeStamp(this->elapsed.count());
	}

private:
	Elapsed elapsed;
};

using TimerUptr = std::unique_ptr<Timer>;
using TimerSptr = std::shared_ptr<Timer>;
using TimerWptr = std::weak_ptr<Timer>;

} // namespace util
} // namespace ecs
} // namespace raptor_engine