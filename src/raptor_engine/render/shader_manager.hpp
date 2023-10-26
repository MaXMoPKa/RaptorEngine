#pragma once

#include <iostream>
#include <array>

#include "SDL2/SDL_log.h"

#include "defines.hpp"

namespace raptor_engine
{
namespace render
{

enum class SHADER_GLOBAL_UNIFORMS : u32
{
	TIME,

	COUNT
};

/**
 * @brief shader manager class is the main class for work with shaders.
 */
class shader_manager
{
public:
	/**
	 * @brief Construct a new shader manager object.
	 */
	shader_manager();

	 /**
	  * @brief Construct a new shader manager object from another shader manager object.
	  */
	shader_manager(shader_manager&& manager) noexcept;
	shader_manager& operator=(shader_manager&& manager) noexcept;

	shader_manager(const shader_manager&)			 = delete;
	shader_manager& operator=(const shader_manager&) = delete;

	~shader_manager();

public:
	/**
	 * @brief create is one way to initialize shader manager object.
	 */
	void create();

	/**
	 * @brief swap is the method for swap two shader manager's object info.
	 */
	void swap(shader_manager& manager) noexcept;

	/**
	 * @brief reset method exchange this shader manager object on uninitialized.
	 */
	void reset() noexcept;

public:
	/**
	 * @brief add_shaders method that add new shader program.
	 * 
	 * @param vs_path is a path to vertex shader.
	 * 
	 * @param fs_path is a path to fragment shader.
	 */
	void add_shaders(const std::string& vs_path, const std::string& fs_path);

public:
	/**
	 * @return the id of the shader program.
	 */
	u32 get_shader_program() const;

#if TESTS
	/**
	 * @return true if vertex shader file was opened successful and false in another way.
	 */
	bool get_is_vs_file_opened() const;
	/**
	 * @return true if fragment shader file was opened successful and false in another way.
	 */
	bool get_is_fs_file_opened() const;

	/**
	 * @return vertex shader code if vertex shader file was opened successful and empty string in another way.
	 */
	std::string get_vs_code() const;
	/**
	 * @return fragment shader code if fragment shader file was opened successful and empty string in another way.
	 */
	std::string get_fs_code() const;

	/**
	 * @return index of vertex shader program if it was created successfull and 0 in another way.
	 */
	unsigned int get_vs_id() const;
	/**
	 * @return index of fragment shader program if it was created successfull and 0 in another way.
	 */
	unsigned int get_fs_id() const;

	/**
	 * @return true if vertex shader was compiled successful and false in another way.
	 */
	bool get_is_vs_compiled_successful() const;
	/**
	 * @return true if fragment shader was compiled successful and false in another way.
	 */
	bool get_is_fs_compiled_successful() const;

	/**
	 * @return index of created shader program and 0 in another way.
	 */
	unsigned int get_shader_program_id() const;

	/**
	 * @return true if shader program was linked succsessful and false in another way.
	 */
	bool get_is_shader_program_linked_successful() const;

	/**
	 * @return count of global uniforms or 0 if there aren't.
	 */
	u32 get_global_uniforms_count() const;
#endif

private:
	class shader_manager_pimpl;
	std::unique_ptr<shader_manager_pimpl> pimpl;
};

} // namespace render
} // namespace raptor_engine