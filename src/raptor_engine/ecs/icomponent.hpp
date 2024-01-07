#pragma once

#include "ecs/platform.hpp"
#include "ecs/handle.hpp"

namespace raptor_engine {
namespace ecs {

using entity_id = util::handle64;

using component_id = object_id;

class icomponent
{
public:
	icomponent();

	virtual ~icomponent();

public:
	bool operator==(const icomponent& other_) const;
	bool operator!=(const icomponent& other_) const;

private:
	const component_id get_id() const;

	const entity_id get_owner_id() const;

	bool get_is_active() const;

	void set_is_active(bool is_active_);

protected:
	component_id hash_value;
	component_id id;
	entity_id	 owner_id;
	bool		 is_active;
};


} // namespace ecs
} // namespace raptor_engine