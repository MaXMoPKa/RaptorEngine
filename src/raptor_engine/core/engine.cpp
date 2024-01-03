#include "engine.hpp"

#include "render/texture_manager.hpp"
#include "render/geometry_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/shader.hpp"
#include "render/renderable_object.hpp"

using namespace raptor_engine;
using namespace render;

class engine::engine_pimpl
{
public:

	engine_pimpl() : hardware_sys(std::make_shared<hardware_system>()) { }

	engine_pimpl(const init_engine_data_sptr& engine_info)
		: engine_info {engine_info}
		, hardware_sys {std::make_shared<hardware_system>(engine_info->hardware_system_info)} 
		, render_eng {std::make_shared<render_engine>(std::make_shared<init_render_engine_data>(
					  std::make_shared<high_level_renderer_data>(this->hardware_sys->get_window()),
					  high_level_renderer_type::FORWARD_LDR_RENDERER,
					  engine_info->scene_info))}
		, sh_manager {std::make_shared<shader_manager>()} 
		, geom_manager {std::make_shared<geometry_manager>()}
		, tex_manager {std::make_shared<texture_manager>()}
	{
		  auto scene_info = engine_info->scene_info;

		  std::vector<renderable_object_sptr> renderable_objects;

		  for (auto& object : scene_info->objects) 
		  {
			  shader_program_sptr sh_program = sh_manager->add_shaders(object.vs_path, object.fs_path);
			  geometry_object_sptr geom_object = geom_manager->add_geometry(object.vertices, object.indices);
			  
			  std::vector<texture_program_sptr> textures;
			  if (!object.textures.empty())
			  {
				  for (auto& tex : object.textures) 
				  {
					  textures.emplace_back(tex_manager->add_texture(tex));
				  }
			  }

			  renderable_objects.emplace_back(
				  std::make_shared<renderable_object>(geom_object, sh_program, scene_info->draw_config, textures));
		  }

		  render_eng->set_renderable_objects(renderable_objects);

	}

public:

	void create(const init_engine_data_sptr& engine_info)
	{
		engine_pimpl tmp {engine_info};
		this->swap(tmp);
	}

	void swap(engine_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->hardware_sys, pimpl.hardware_sys);
		std::swap(this->render_eng, pimpl.render_eng);
		std::swap(this->engine_info, pimpl.engine_info);
	}

	void reset() noexcept
	{
		engine_pimpl tmp {};
		this->swap(tmp);
	}

 public:

    void run() noexcept
	{
		bool window_should_close = false;
		while (!window_should_close) 
		{ 
			this->hardware_sys->process_events(window_should_close);
			
			u64 time = this->hardware_sys->get_time();
			
			this->render_eng->pre_update();
			this->render_eng->update(time);
			this->render_eng->post_update();
		}
	}

 public:
  [[nodiscard]] init_engine_data_sptr get_init_engine_data() const {
    return this->engine_info;
  }

  [[nodiscard]] hardware_system_sptr get_hardware_system() const {
    return this->hardware_sys;
  }

  [[nodiscard]] render_engine_sptr get_render_engine()
  {
	return this->render_eng;
  }

 private:
    init_engine_data_sptr engine_info;

    hardware_system_sptr hardware_sys {};

    render_engine_sptr render_eng {};

    shader_manager_sptr sh_manager {};

	geometry_manager_sptr geom_manager {};

	texture_manager_sptr tex_manager {};
};

engine::engine() : pimpl{std::make_unique<engine_pimpl>()} {}

engine::engine(const init_engine_data_sptr& engine_info)
	: pimpl {std::make_unique<engine_pimpl>(engine_info)}
{ }

engine::engine(engine&& engine) noexcept = default;
engine& engine::operator=(engine&& engine) noexcept = default;

engine::~engine() = default;

void engine::create(const init_engine_data_sptr& engine_info)
{
  this->pimpl->create(engine_info);
}

void engine::swap(engine& engine) noexcept { this->pimpl.swap(engine.pimpl); }

void engine::reset() noexcept { this->pimpl->reset(); }

void engine::run() noexcept 
{
  this->pimpl->run();
}

init_engine_data_sptr engine::get_init_engine_data() const {
  return this->pimpl->get_init_engine_data();
}

hardware_system_sptr engine::get_hardware_system() const {
  return this->pimpl->get_hardware_system();
}


render_engine_sptr engine::get_render_engine()
{
  return this->pimpl->get_render_engine();
}