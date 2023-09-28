#pragma once

#include "glad/glad.h"

#include <utility>
#include <fstream>
#include <sstream>

#include "SDL2/SDL_log.h"

namespace raptor_engine {
namespace render {

struct shader_data
{
public:

	shader_data() : vertex_file_path {}, fragment_file_path {} { }
	shader_data(const std::string& vertex_path, const std::string& fragment_path) : 
		vertex_file_path {vertex_path}, 
		fragment_file_path {fragment_path}
	{ }

	~shader_data() = default;

public:
	std::string vertex_file_path;
	std::string fragment_file_path;
};

using shader_data_uptr = std::unique_ptr<shader_data>;
using shader_data_sptr = std::shared_ptr<shader_data>;
using shader_data_wptr = std::weak_ptr<shader_data>;

class shader
{
public:
	shader() : shader_program {} { }

	shader(const shader_data_sptr& shader_data)
	{
		std::string	  vertex_code;
		std::string	  fragment_code;
		std::ifstream vertex_shader_file;
		std::ifstream fragment_shader_file;
		// ensure ifstream objects can throw exceptions:
		vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// open files
			vertex_shader_file.open(shader_data->vertex_file_path);
			fragment_shader_file.open(shader_data->fragment_file_path);
			std::stringstream vertex_shader_stream, fragment_shader_stream;
			// read file's buffer contents into streams
			vertex_shader_stream << vertex_shader_file.rdbuf();
			fragment_shader_stream << fragment_shader_file.rdbuf();
			// close file handlers
			vertex_shader_file.close();
			fragment_shader_file.close();
			// convert stream into string
			vertex_code	 = vertex_shader_stream.str();
			fragment_code = fragment_shader_stream.str();
		} catch (std::ifstream::failure& e) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s", e.what());
		}
		const char* vertex_shader_code = vertex_code.c_str();
		const char* fragment_shader_code = fragment_code.c_str();

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex_shader);
		// check for shader compile errors
		int	 success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		check_compile_errors(vertex_shader, "VERTEX");
		// fragment shader
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment_shader);
		// check for shader compile errors
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		check_compile_errors(fragment_shader, "FRAGMENT");
		// link shaders
		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		// check for linking errors
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		check_compile_errors(shader_program, "PROGRAM");
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	shader(shader&& sh) noexcept = default;
	shader& operator=(shader&& sh) noexcept = default;

	shader(const shader&) = delete;
	shader& operator=(const shader&) = delete;

	~shader()
	{
		glDeleteProgram(shader_program);
	}

public:
	void create(/*const shader_data_sptr& shader_data*/)
	{
		shader tmp {};
		this->swap(tmp);
	}

	void swap(shader& sh) noexcept
	{
		if (this == &sh)
		{
			return;
		}

		std::swap(this->shader_program, sh.shader_program);
	}

	void reset() noexcept
	{
		shader tmp {};
		this->swap(tmp);
	}

public:
	void use()
	{
		glUseProgram(shader_program);
	}

private:
	void check_compile_errors(GLuint shader, std::string type)
	{
		GLint  success;
		GLchar infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, 
								"ERROR::SHADER_COMPILATION_ERROR of type: %s \n %s \n -- "
								"--------------------------------------------------- --",
								type,
								infoLog);
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
								"ERROR::PROGRAM_LINKING_ERROR of type: %s \n %s \n -- "
								"--------------------------------------------------- --",
								type,
								infoLog);
			}
		}
	}

private:
	unsigned int shader_program;
};

using shader_uptr = std::unique_ptr<shader>;
using shader_sptr = std::shared_ptr<shader>;
using shader_wptr = std::weak_ptr<shader>;

}
}