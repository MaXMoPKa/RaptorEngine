#include "shader_manager.hpp"

#include "glad/glad.h"

#include <fstream>
#include <sstream>

using namespace raptor_engine::render;

static const std::array<std::string, static_cast<size_t>(SHADER_GLOBAL_UNIFORMS::COUNT)> uniforms_names = {"g_time"};

class shader_manager::shader_manager_pimpl
{
public:
	shader_manager_pimpl()
		: shader_program {0}
#if TESTS
		  ,
		  is_vs_file_opened {false}, is_fs_file_opened {false}, vs_code {""}, fs_code {""}, vs_id {0}, fs_id {0},
		  is_vs_compiled_successful {false}, is_fs_compiled_successful {false},
		  is_shader_program_linked_successful {false}, global_uniforms_count {0}
#endif
	{

	}

public:
	void create()
	{

	}

	void swap(shader_manager_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->shader_program, pimpl.shader_program);

#if TESTS
		std::swap(this->is_vs_file_opened, pimpl.is_vs_file_opened);
		std::swap(this->is_fs_file_opened, pimpl.is_fs_file_opened);
		std::swap(this->vs_code, pimpl.vs_code);
		std::swap(this->fs_code, pimpl.fs_code);
		std::swap(this->vs_id, pimpl.vs_id);
		std::swap(this->fs_id, pimpl.fs_id);
		std::swap(this->is_vs_compiled_successful, pimpl.is_vs_compiled_successful);
		std::swap(this->is_fs_compiled_successful, pimpl.is_fs_compiled_successful);
		std::swap(this->is_shader_program_linked_successful, pimpl.is_shader_program_linked_successful);
		std::swap(this->global_uniforms_count, pimpl.global_uniforms_count);
#endif
	}

	void reset() noexcept
	{
		shader_manager_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void add_shaders(const std::string& vs_path, const std::string& fs_path)
	{
		std::string	  vertex_code;
		std::string	  fragment_code;
		std::ifstream vertex_shader_file;
		std::ifstream fragment_shader_file;
		vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
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

			vertex_code	  = vertex_shader_stream.str();
			fragment_code = fragment_shader_stream.str();

#if TESTS
			vs_code = vertex_code;
			fs_code = fragment_code;
#endif
		} catch (std::ifstream::failure& e) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s", e.what());
			return;
		}
		const char* vertex_shader_code	 = vertex_code.c_str();
		const char* fragment_shader_code = fragment_code.c_str();
		// vertex shader
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex_shader);
		// check for vs compile errors
		int	 success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		bool is_vs_compiled = check_compile_errors(vertex_shader, "VERTEX");
		// fragment shader
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment_shader);
		// check for fs compile errors
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		bool is_fs_compiled = check_compile_errors(fragment_shader, "FRAGMENT");
		// link shaders
		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		// check for linking errors
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		bool is_shader_program_linked = check_link_errors(shader_program, "PROGRAM");
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

#if TESTS
		vs_id = vertex_shader;
		fs_id = fragment_shader;

		is_vs_compiled_successful = is_vs_compiled;
		is_fs_compiled_successful = is_fs_compiled;

		is_shader_program_linked_successful = is_shader_program_linked;
#endif
		i32 uniform_location = -1;
		for (std::size_t i = 0; i < static_cast<size_t>(SHADER_GLOBAL_UNIFORMS::COUNT); ++i) {
			uniform_location = glGetUniformLocation(shader_program, uniforms_names[i].c_str());

#if TESTS
			if (uniform_location != -1) {
				++global_uniforms_count;
			}
#endif
		}
	}

public:
	u32 get_shader_program() const
	{
		return shader_program;
	}

private:
	bool check_compile_errors(GLuint shader, std::string type)
	{
		bool   result = true;
		GLint  success;
		GLchar infoLog[1024];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
							"ERROR::SHADER_COMPILATION_ERROR of type: %s \n %s \n -- "
							"--------------------------------------------------- --",
							type,
							infoLog);
			result = false;
		}

		return result;
	}

	bool check_link_errors(GLuint shader, std::string type)
	{
		bool   result = true;
		GLint  success;
		GLchar infoLog[1024];

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
							"ERROR::PROGRAM_LINKING_ERROR of type: %s \n %s \n -- "
							"--------------------------------------------------- --",
							type,
							infoLog);
			result = false;
		}

		return result;
	}

public:
#if TESTS
	bool get_is_vs_file_opened() const
	{
		return is_vs_file_opened;
	}
	bool get_is_fs_file_opened() const
	{
		return is_fs_file_opened;
	}

	std::string get_vs_code() const
	{
		return vs_code;
	}
	std::string get_fs_code() const
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

	bool get_is_vs_compiled_successful() const
	{
		return is_vs_compiled_successful;
	}
	bool get_is_fs_compiled_successful() const
	{
		return is_fs_compiled_successful;
	}

	unsigned int get_shader_program_id() const
	{
		return shader_program;
	}
	bool get_is_shader_program_linked_successful() const
	{
		return is_shader_program_linked_successful;
	}

	u32 get_global_uniforms_count() const
	{
		return global_uniforms_count;
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

	bool is_vs_compiled_successful;
	bool is_fs_compiled_successful;

	bool is_shader_program_linked_successful;

	u32 global_uniforms_count;
#endif
};

shader_manager::shader_manager() 
	: pimpl {std::make_unique<shader_manager_pimpl>()} 
{

}

shader_manager::~shader_manager() = default;

shader_manager::shader_manager(shader_manager&& manager) noexcept = default;

shader_manager& shader_manager::operator=(shader_manager&& manager) noexcept = default;

void shader_manager::create() 
{
	this->pimpl->create();
}

void shader_manager::swap(shader_manager& manager) noexcept 
{
	this->pimpl.swap(manager.pimpl);
}

void shader_manager::reset() noexcept 
{
	this->pimpl->reset();
}

void shader_manager::add_shaders(const std::string& vs_path, const std::string& fs_path) 
{
	this->pimpl->add_shaders(vs_path, fs_path);
}

u32 shader_manager::get_shader_program() const
{
	return this->pimpl->get_shader_program();
}

#if TESTS

bool shader_manager::get_is_vs_file_opened() const
{
	return this->pimpl->get_is_vs_file_opened();
}

bool shader_manager::get_is_fs_file_opened() const
{
	return this->pimpl->get_is_fs_file_opened();
}

std::string shader_manager::get_vs_code() const
{
	return this->pimpl->get_vs_code();
}

std::string shader_manager::get_fs_code() const
{
	return this->pimpl->get_fs_code();
}

unsigned int shader_manager::get_vs_id() const
{
	return this->pimpl->get_vs_id();
}

unsigned int shader_manager::get_fs_id() const
{
	return this->pimpl->get_fs_id();
}

bool shader_manager::get_is_vs_compiled_successful() const
{
	return this->pimpl->get_is_vs_compiled_successful();
}

bool shader_manager::get_is_fs_compiled_successful() const
{
	return this->pimpl->get_is_fs_compiled_successful();
}

unsigned int shader_manager::get_shader_program_id() const
{
	return this->pimpl->get_shader_program_id();
}

bool shader_manager::get_is_shader_program_linked_successful() const
{
	return this->pimpl->get_is_shader_program_linked_successful();
}

u32 shader_manager::get_global_uniforms_count() const
{
	return this->pimpl->get_global_uniforms_count();
}

#endif
