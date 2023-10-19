#pragma once

#include <fstream>

#include "glad/glad.h"

#include "SDL2/SDL_log.h"

#include "defines.hpp"

class shader_manager
{
public:
	shader_manager() 
		: shader_program {0}
#if TESTS
		, is_vs_file_opened {false}
		, is_fs_file_opened {false}
		, vs_code {}
		, fs_code {}
		, vs_id {}
		, fs_id {}
#endif
	{

	}

	shader_manager(const std::string& vs_path, const std::string& fs_path) 
		: shader_program {0}
#if TESTS
		, is_vs_file_opened {false}
		, is_fs_file_opened {false}
		, vs_code {}
		, fs_code {}
		, vs_id {}
		, fs_id {}
#endif
	{
		std::string	  vertex_code;
		std::string	  fragment_code;
		std::ifstream vertex_shader_file;
		std::ifstream fragment_shader_file;
		vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try 
		{
			vertex_shader_file.open(vs_path);
			fragment_shader_file.open(fs_path);

#if TESTS
			is_vs_file_opened = true;
			is_fs_file_opened = true;
#endif
			std::stringstream vertex_shader_stream;
			std::stringstream fragment_shader_stream;

			vertex_shader_stream << vertex_shader_file.rdbuf();
			fragment_shader_stream << fragment_shader_file.rdbuf();

			vertex_shader_file.close();
			fragment_shader_file.close();

			vertex_code = vertex_shader_stream.str();
			fragment_code = fragment_shader_stream.str();

#if TESTS
			vs_code = vertex_code;
			fs_code = fragment_code;
#endif
		} 
		catch (std::ifstream::failure& e) 
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s", e.what());
		}
		const char* vertex_shader_code	 = vertex_code.c_str();
		const char* fragment_shader_code = fragment_code.c_str();

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex_shader);

		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment_shader);

#if TESTS
		vs_id = vertex_shader;
		fs_id = fragment_shader;
#endif

	}

public:
	u32 get_shader_program()
	{
		return shader_program;
	}

#if TESTS
	bool get_is_vs_file_opened() const
	{
		return is_vs_file_opened;
	}
	bool get_is_fs_file_opened() const
	{
		return is_fs_file_opened;
	}

	std::string_view get_vs_code() const
	{
		return vs_code;
	}
	std::string_view get_fs_code() const
	{
		return fs_code;
	}

	unsigned int get_vs_id() const
	{
		return vs_id;
	}
	unsigned int get_fs_id() const
	{
		return fs_id;
	}
#endif

private:
	u32 shader_program;

#if TESTS
	bool is_vs_file_opened;
	bool is_fs_file_opened;

	std::string vs_code;
	std::string fs_code;

	unsigned int vs_id;
	unsigned int fs_id;
#endif
};