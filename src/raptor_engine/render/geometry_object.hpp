#pragma once

#include <vector>
#include <memory>

#include "render/vertex_array_object.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/element_buffer_object.hpp"

namespace raptor_engine
{
namespace render
{

class geometry_object
{
public:
	geometry_object();

	geometry_object(vertex_array_object_sptr vao_, 
					vertex_buffer_object_sptr vbo_, 
					element_buffer_object_sptr ebo_);

	geometry_object(geometry_object&& object_) noexcept;
	geometry_object& operator=(geometry_object&& object_) noexcept;

	geometry_object(const geometry_object&)			   = delete;
	geometry_object& operator=(const geometry_object&) = delete;

	~geometry_object();

public:
	void create(vertex_array_object_sptr vao_, 
				vertex_buffer_object_sptr vbo_, 
				element_buffer_object_sptr ebo_);

	void swap(geometry_object& object_) noexcept;

	void reset() noexcept;

public:
	const vertex_array_object_sptr get_vao() const;

	const vertex_buffer_object_sptr get_vbo() const;

	const element_buffer_object_sptr get_ebo() const;

public:
	void set_vao(vertex_array_object_sptr new_vao_);

	void set_vbo(vertex_buffer_object_sptr new_vbo_);

	void set_ebo(element_buffer_object_sptr new_ebo_);

private:
	class geometry_object_pimpl;
	std::unique_ptr<geometry_object_pimpl> pimpl;
};

using geometry_object_uptr = std::unique_ptr<geometry_object>;
using geometry_object_sptr = std::shared_ptr<geometry_object>;
using geometry_object_wptr = std::weak_ptr<geometry_object>;

} // namespace render
} // namespace raptor_engine