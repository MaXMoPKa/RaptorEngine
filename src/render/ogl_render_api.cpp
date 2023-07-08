#include "render/ogl_render_api.hpp"

using namespace raptor_engine::render;

class ogl_render_api::ogl_render_api_pimpl
{
public:
	ogl_render_api_pimpl() : gl_context {nullptr}, shader_program {}, vao {}, vbo {} { }

	ogl_render_api_pimpl(const sdl_window_sptr& window_ptr) : shader_program{}, vao{}, vbo{}
	{
		gl_context = SDL_GL_CreateContext(window_ptr.get());
		if (this->gl_context == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create GL Context");
		}

		if (int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD with error code: %n", &result);
		}

		const char* vertex_shader_source = "#version 330 core\n"
										   "layout (location = 0) in vec3 aPos;\n"
										   "void main()\n"
										   "{\n"
										   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
										   "}\0";
		const char* fragment_shader_source = "#version 330 core\n"
										     "out vec4 FragColor;\n"
										     "void main()\n"
										     "{\n"
										     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
										     "}\n\0";

		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		glCompileShader(vertex_shader);
		// check for shader compile errors
		int	 success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s", infoLog);
		}
		// fragment shader
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);
		// check for shader compile errors
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s", infoLog);
		}
		// link shaders
		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		// check for linking errors
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s", infoLog);
		}
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex
		// attributes(s).
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
		// vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
		// happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs
		// (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}

	~ogl_render_api_pimpl()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteProgram(shader_program);
	}

public:
	void create(const sdl_window_sptr& window_ptr)
	{
		ogl_render_api_pimpl tmp(window_ptr);
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
		glUseProgram(shader_program);
	}

	void bind_vao()
	{
		glBindVertexArray(vao);
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

	unsigned int shader_program;
	unsigned int vao, vbo;
};

ogl_render_api::ogl_render_api() : base_render_api(), pimpl{std::make_unique<ogl_render_api_pimpl>()} { }

ogl_render_api::ogl_render_api(const sdl_window_sptr& window_ptr)
	: base_render_api(window_ptr), pimpl {std::make_unique<ogl_render_api_pimpl>(window_ptr)} { }

ogl_render_api::ogl_render_api(ogl_render_api&& api) noexcept = default;

ogl_render_api& ogl_render_api::operator=(ogl_render_api&& api) noexcept = default;

ogl_render_api::~ogl_render_api() = default;

void ogl_render_api::create(const sdl_window_sptr& window_ptr)
{
	this->pimpl->create(window_ptr);
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