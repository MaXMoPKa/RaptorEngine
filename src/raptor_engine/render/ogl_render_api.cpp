#include "render/ogl_render_api.hpp"

#include "render/shader.hpp"
#include "render/vertex_array_object.hpp"
#include "render/vertex_buffer_object.hpp"

using namespace raptor_engine::render;

class ogl_render_api::ogl_render_api_pimpl
{
public:
	ogl_render_api_pimpl() : gl_context {nullptr}, shader_program {}, vao {}, vbo {} { }

	ogl_render_api_pimpl(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info) : vao {}, vbo {}
	{
		gl_context = SDL_GL_CreateContext(window_ptr.get());
		if (this->gl_context == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create GL Context");
		}

		if (int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD with error code: %n", &result);
		}

		for (auto& object : scene_info->objects) 
		{
			shader_program = std::make_shared<shader>(std::make_shared<shader_data>(object.vs_path, object.fs_path));

			vao = std::make_shared<vertex_array_object>();
			vao->use();

			vbo = std::make_shared<vertex_buffer_object>(
				std::make_shared<vertex_buffer_object_data>(object.vertices.size() * sizeof(float),
															object.vertices.data(),
															GL_STATIC_DRAW,
															0,
															3,
															GL_FLOAT,
															GL_FALSE,
															3 * sizeof(float),
															(void*)0));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
			// happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind
			// VAOs (nor VBOs) when it's not directly necessary.
			glBindVertexArray(0);
		}
	}

	~ogl_render_api_pimpl() = default;

public:

	void create(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
	{
		ogl_render_api_pimpl tmp(window_ptr, scene_info);
		this->swap(tmp);
	}

	void swap(ogl_render_api_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->gl_context, pimpl.gl_context);
	}

	void reset() noexcept
	{
		ogl_render_api_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void clear_color()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void use_shader_program()
	{
		shader_program->use();
	}

	void bind_vao()
	{
		vao->use();
	}

	void draw_arrays()
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void swap_window(const sdl_window_sptr& window_ptr)
	{
		SDL_GL_SwapWindow(window_ptr.get());
	}

private:
	SDL_GLContext gl_context;

	shader_sptr	shader_program;

	vertex_array_object_sptr vao;
	vertex_buffer_object_sptr vbo;
};

ogl_render_api::ogl_render_api() : base_render_api(), pimpl{std::make_unique<ogl_render_api_pimpl>()} { }

ogl_render_api::ogl_render_api(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
	: base_render_api(window_ptr), pimpl {std::make_unique<ogl_render_api_pimpl>(window_ptr, scene_info)} { }

ogl_render_api::ogl_render_api(ogl_render_api&& api) noexcept = default;

ogl_render_api& ogl_render_api::operator=(ogl_render_api&& api) noexcept = default;

ogl_render_api::~ogl_render_api() = default;

void ogl_render_api::create(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
{
	this->pimpl->create(window_ptr, scene_info);
}

void ogl_render_api::swap(ogl_render_api& api) noexcept 
{
	base_render_api::swap(api);
	this->pimpl.swap(api.pimpl);
}

void ogl_render_api::reset() noexcept 
{
	this->pimpl->reset();
}

void ogl_render_api::clear_color()
{
	this->pimpl->clear_color();
}

void ogl_render_api::use_shader_program()
{
	this->pimpl->use_shader_program();
}

void ogl_render_api::bind_vao()
{
	this->pimpl->bind_vao();
}

void ogl_render_api::draw_arrays()
{
	this->pimpl->draw_arrays();
}

void ogl_render_api::swap_window()
{
	this->pimpl->swap_window(this->get_window().lock());
}