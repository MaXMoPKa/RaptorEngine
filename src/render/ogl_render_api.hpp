#pragma once

#include "glad/glad.h"

#include <memory>

#include "SDL_log.h"

#include "render/base_render_api.hpp"

namespace raptor_engine {
namespace render {

class ogl_render_api : public base_render_api
{
public:

	ogl_render_api() : base_render_api {}, gl_context {nullptr} { }

	ogl_render_api(const sdl_window_sptr& window_ptr) : base_render_api {window_ptr}, gl_context {nullptr} 
	{ 
		init();
	}

	ogl_render_api(ogl_render_api&& api) noexcept            = default;
	ogl_render_api& operator=(ogl_render_api&& api) noexcept = default;

	ogl_render_api(const ogl_render_api&)			 = delete;
	ogl_render_api& operator=(const ogl_render_api&) = delete;
 
	virtual ~ogl_render_api() 
	{	
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		//glDeleteProgram(shaderProgram);
	}

public:

	void create(const sdl_window_sptr& window_ptr) override
	{
		ogl_render_api tmp(window_ptr);
		this->swap(tmp);
	}

	void swap(ogl_render_api& api) noexcept
	{
		if (this == &api) 
		{
			return;
		}

		base_render_api::swap(api);

		std::swap(this->gl_context, api.gl_context);
	}

	void reset() noexcept override
	{
		ogl_render_api tmp {};
		this->swap(tmp);
	}

public:

	void clear_color() override
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void use_shader_program() override 
	{
		glUseProgram(shaderProgram);
	}

	void bind_vao() override
	{
		glBindVertexArray(VAO);
	}

	void draw_arrays() override
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void swap_window() override
	{ 
		SDL_GL_SwapWindow(this->window.lock().get());
	}

private:
	void init()
	{		
		gl_context = SDL_GL_CreateContext(this->window.lock().get());
		if (this->gl_context == 0) { 
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create GL Context");
		}

		if (int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD with error code: %n", &result);
		}

		const char* vertexShaderSource	  = "#version 330 core\n"
										 "layout (location = 0) in vec3 aPos;\n"
										 "void main()\n"
										 "{\n"
										 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
										 "}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
										   "out vec4 FragColor;\n"
										   "void main()\n"
										   "{\n"
										   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
										   "}\n\0";

		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int	 success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s", infoLog);
		}
		// fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s", infoLog);
		}
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s", infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex
		// attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

private:
	SDL_GLContext gl_context;

	unsigned int shaderProgram;
	unsigned int VAO, VBO;
};

using ogl_render_api_uptr = std::unique_ptr<ogl_render_api>;
using ogl_render_api_sptr = std::shared_ptr<ogl_render_api>;
using ogl_render_api_wptr = std::weak_ptr<ogl_render_api>;

} // namespace render
} // namespace raptor_engine