#pragma once

#include "defines.hpp"

#include "ecs/utils/platform.hpp"

namespace raptor_engine {
namespace ecs {

using SystemTypeId   = TypeId;
using SystemPriority = u16;

static const SystemTypeId INVALID_SYSTEM_ID = INVALID_TYPE_ID;

static const SystemPriority LOWEST_SYSTEM_PRIORITY	  = std::numeric_limits<SystemPriority>::min();
static const SystemPriority VERY_LOW_SYSTEM_PRIORITY  = 99;
static const SystemPriority LOW_SYSTEM_PRIORITY	      = 100;
static const SystemPriority NORMAL_SYSTEM_PRIORITY	  = 200;
static const SystemPriority MEDIUM_SYSTEM_PRIORITY	  = 300;
static const SystemPriority HIGH_SYSTEM_PRIORITY	  = 400;
static const SystemPriority VERY_HIGH_SYSTEM_PRIORITY = 401;
static const SystemPriority HIGHEST_SYSTEM_PRIORITY	  = std::numeric_limits<SystemPriority>::max();

class ISystem
{
	friend class SystemManager;

public:
	virtual ~ISystem() = default;

public:

	virtual inline const SystemTypeId GetStaticSystemTypeId() const = 0;
	virtual inline const char*		  GetSystemTypeName() const	    = 0;

	virtual void PreUpdate(f32 dt)  = 0;
	virtual void Update(f32 dt)		 = 0;
	virtual void PostUpdate(f32 dt) = 0;

protected:
	ISystem(SystemPriority priority_ = NORMAL_SYSTEM_PRIORITY, f32 updateIntervalMs = -1.0f);

private:
	f32            timeSinceLastUpdate;
	SystemPriority priority;
	f32			   updateInterval;
	u8			   isEnabled    : 1;
	u8			   isNeedUpdate : 1;
	u8			   reserved	    : 6;
};

using ISystemUptr = std::unique_ptr<ISystem>;
using ISystemSptr = std::shared_ptr<ISystem>;
using ISystemWptr = std::weak_ptr<ISystem>;

} // namespace ecs
} // namespace raptor_engine