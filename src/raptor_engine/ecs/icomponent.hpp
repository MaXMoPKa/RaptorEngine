#pragma once

#include "ecs/platform.hpp"
#include "ecs/handle.hpp"

namespace raptor_engine {
namespace ecs {

using entity_id = util::handle64;

static const entity_id INVALID_ENTITY_ID = util::handle64::INVALID_HANDLE;

using component_id = object_id;

static const component_id INVALID_COMPONENT_ID = INVALID_OBJECT_ID;

class i_component
{
public:
	i_component();

	virtual ~i_component();

public:
	bool operator==(const i_component& other_) const;
	bool operator!=(const i_component& other_) const;

public:
	const component_id get_hash() const;

	const component_id get_id() const;

	const entity_id get_owner_id() const;

	bool get_is_active() const;

	void set_hash(const component_id hash_);

	void set_id(const component_id id_);

	void set_owner_id(const entity_id owner_id_);

	void set_is_active(bool is_active_);

protected:
	component_id hash_value;
	component_id id;
	entity_id	 owner_id;
	bool		 is_active;
};


} // namespace ecs
} // namespace raptor_engine