#pragma once

#include "defines.hpp"

#include "ecs/platform.hpp"

namespace raptor_engine {
namespace ecs {

using system_type_id  = type_id;
using system_priority = u16;

static const system_type_id INVALID_SYSTEM_ID = INVALID_TYPE_ID;

static const system_priority LOWEST_SYSTEM_PRIORITY    = std::numeric_limits<system_priority>::min();
static const system_priority VERY_LOW_SYSTEM_PRIORITY  = 99;
static const system_priority LOW_SYSTEM_PRIORITY	   = 100;
static const system_priority NORMAL_SYSTEM_PRIORITY	   = 200;
static const system_priority MEDIUM_SYSTEM_PRIORITY	   = 300;
static const system_priority HIGH_SYSTEM_PRIORITY	   = 400;
static const system_priority VERY_HIGH_SYSTEM_PRIORITY = 401;
static const system_priority HIGHEST_SYSTEM_PRIORITY   = std::numeric_limits<system_priority>::max();

class i_system
{
	friend class system_manager;

public:
	virtual ~i_system() = default;

public:
	virtual inline const system_type_id get_static_system_type_id() const = 0;
	virtual inline const char*			get_system_type_name() const	  = 0;

	virtual void pre_update(f32 dt)  = 0;
	virtual void update(f32 dt)		 = 0;
	virtual void post_update(f32 dt) = 0;

protected:
	i_system(system_priority priority_ = NORMAL_SYSTEM_PRIORITY, f32 update_interval_ms = -1.0f);

private:
	f32             time_since_last_update;
	system_priority priority;
	f32				update_interval;
	u8				is_enabled     : 1;
	u8				is_need_update : 1;
	u8				reserved	   : 6;
};

} // namespace ecs
} // namespace raptor_engine