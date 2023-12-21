#pragma once

#include <vector>
#include "render/geometry_object.hpp"
//#include <memory>

class raptor_engine::render::geometry_object;

namespace raptor_engine
{
namespace render
{

class geometry_manager
{
public:
	geometry_manager();

	geometry_manager(const std::vector<geometry_object_sptr>& geometry_objects_);

	geometry_manager(geometry_manager&& manager_) noexcept;
	geometry_manager& operator=(geometry_manager&& manager_) noexcept;

	geometry_manager(const geometry_manager&)			 = delete;
	geometry_manager& operator=(const geometry_manager&) = delete;

	~geometry_manager();

public:
	void create(const std::vector<geometry_object_sptr>& geometry_objects_);

	void swap(geometry_manager& manager_);

	void reset() noexcept;

public:
	geometry_object_sptr add_geometry(const std::vector<float>& vertices_, const std::vector<unsigned int>& indices_);

public:
	const std::vector<geometry_object_sptr>& get_geometry_objects() const noexcept;

	const geometry_object_sptr& get_geometry_object(std::size_t index_) const;

private:
	class geometry_manager_pimpl;
	std::unique_ptr<geometry_manager_pimpl> pimpl;
};

using geometry_manager_uptr = std::unique_ptr<geometry_manager>;
using geometry_manager_sptr = std::shared_ptr<geometry_manager>;
using geometry_manager_wptr = std::weak_ptr<geometry_manager>;

} // namespace render
} // namespace raptor_engine
