#include "glad/glad.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO

#include "render/shader_manager.hpp"

using namespace testing; 
using namespace raptor_engine::render;

class test_low_level_initializer : public Test
{
public:

	static constexpr std::size_t WIDTH {1080U};
	static constexpr std::size_t HEIGHT {810U};
	static constexpr char		 TITLE[] = "Engine test window";

protected:

	void SetUp()
	{
		const auto result = SDL_Init(SDL_INIT_VIDEO);

		if (result != 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
			return;
		}

		window = SDL_CreateWindow(TITLE,
								  SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED,
								  WIDTH,
								  HEIGHT,
								  SDL_WINDOW_OPENGL);

		if (window == nullptr) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
			return;
		}

		gl_context = SDL_GL_CreateContext(window);
		if (this->gl_context == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create GL Context");
		}

		if (int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD with error code: %n", &result);
		}
	}

	void TearDown() 
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

private:
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context = nullptr;
};

TEST(shader_manager, create_shader_manager_instance_with_default_constructor)
{
	shader_manager manager{};

	ASSERT_EQ(manager.get_shader_program(), 0);
}

TEST_F(test_low_level_initializer, try_open_shader_files)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	ASSERT_EQ(manager.get_is_vs_file_opened(), true);
	ASSERT_EQ(manager.get_is_fs_file_opened(), true);
}

TEST(shader_manager, try_open_non_existent_shader_files)
{
	shader_manager manager;
	manager.add_shaders("non_existent_test.vs", "non_existent_test.fs");

	ASSERT_EQ(manager.get_is_vs_file_opened(), false);
	ASSERT_EQ(manager.get_is_fs_file_opened(), false);
}

TEST_F(test_low_level_initializer, read_data_from_shader_files)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	std::string vs_code{"#version 330 core\n\n"
						"layout (location = 0) in vec3 aPos;\n\n"
						"void main()\n"
						"{\n"
						"    gl_Position = vec4(aPos, 1.0);\n"
						"}\0"
	};
	std::string fs_code {"#version 330 core\n\n"
						 "out vec4 FragColor;\n\n"
						 "uniform float g_time;\n\n"
						 "void main()\n"
						 "{\n"
						 "    FragColor = vec4(0.0f, sin(g_time) / 2.0f + 0.5f, 0.0f, 1.0f);\n"
						 "}\0"
	};

	ASSERT_EQ(manager.get_vs_code(), vs_code);
	ASSERT_EQ(manager.get_fs_code(), fs_code);
}

TEST_F(test_low_level_initializer, create_shaders_from_read_sources)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	ASSERT_NE(manager.get_vs_id(), 0);
	ASSERT_NE(manager.get_fs_id(), 0);
}

TEST_F(test_low_level_initializer, compile_shaders)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	ASSERT_EQ(manager.get_is_vs_compiled_successful(), true);
	ASSERT_EQ(manager.get_is_fs_compiled_successful(), true);
}

TEST_F(test_low_level_initializer, link_shaders)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	ASSERT_EQ(manager.get_is_shader_program_linked_successful(), true);
}

TEST_F(test_low_level_initializer, create_shader_manager_instance_with_parametrized_constructor)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	ASSERT_NE(manager.get_shader_program(), 0);
}

TEST_F(test_low_level_initializer, get_shaders_uniforms)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	ASSERT_NE(manager.get_global_uniforms_count(), 0);
}

TEST_F(test_low_level_initializer, create_shader_manager_with_create_method)
{
	shader_manager manager;
	manager.create();

	ASSERT_EQ(manager.get_shader_program(), 0);
}

TEST_F(test_low_level_initializer, swap_two_shader_manager_instances_with_swap_method)
{
	shader_manager first_manager;
	
	shader_manager second_manager;
	second_manager.add_shaders("test.vs", "test.fs");

	first_manager.swap(second_manager);

	ASSERT_NE(first_manager.get_shader_program(), 0);
	ASSERT_EQ(second_manager.get_shader_program(), 0);
}

TEST_F(test_low_level_initializer, clear_shader_manager_instance_with_reset_method)
{
	shader_manager manager;
	manager.add_shaders("test.vs", "test.fs");

	manager.reset();

	ASSERT_EQ(manager.get_shader_program(), 0);
}