#include "glad/glad.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO

#include "render/geometry_manager.hpp"

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

		window =
			SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

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

	SDL_Window*	  window	 = nullptr;
	SDL_GLContext gl_context = nullptr;
};

TEST(geometry_object, create_geometry_object_with_default_constructor)
{
	geometry_object object;

	ASSERT_EQ(object.get_vao(), nullptr);
	ASSERT_EQ(object.get_vbo(), nullptr);
	ASSERT_EQ(object.get_ebo(), nullptr);
}

TEST(geometry_object, create_geometry_object_with_parametrized_constructor)
{
	geometry_object object {std::make_shared<vertex_array_object>(),
							std::make_shared<vertex_buffer_object>(),
							std::make_shared<element_buffer_object>()};

	ASSERT_EQ(object.get_vao()->get_id(), 0);
	ASSERT_EQ(object.get_vbo()->get_id(), 0);
	ASSERT_EQ(object.get_ebo()->get_id(), 0);
}


TEST(geometry_object, create_geometry_object_using_move_constructor)
{
	std::function<geometry_object()> test_function = []() -> geometry_object {
		return std::move(geometry_object(std::make_shared<vertex_array_object>(),
										 std::make_shared<vertex_buffer_object>(),
										 std::make_shared<element_buffer_object>()));
	};

	geometry_object object = test_function();

	ASSERT_EQ(object.get_vao()->get_id(), 0);
	ASSERT_EQ(object.get_vbo()->get_id(), 0);
	ASSERT_EQ(object.get_ebo()->get_id(), 0);
}

TEST_F(test_low_level_initializer, create_geometry_object_with_move_assignment)
{
	auto vao = std::make_shared<vertex_array_object>();
	vao->generate_array();
	auto vbo = std::make_shared<vertex_buffer_object>();
	vbo->generate_buffer();
	auto ebo = std::make_shared<element_buffer_object>();
	ebo->generate_buffer();

	geometry_object first_object {vao, vbo, ebo};
	geometry_object second_object;

	second_object = std::move(first_object);

	ASSERT_NE(second_object.get_vao()->get_id(), 0);
	ASSERT_NE(second_object.get_vbo()->get_id(), 0);
	ASSERT_NE(second_object.get_ebo()->get_id(), 0);
}

TEST(geometry_manager, create_geometry_manager_object_with_default_constructor)
{
	geometry_manager manager;

	ASSERT_EQ(manager.vao, nullptr);
	ASSERT_EQ(manager.vbo, nullptr);
	ASSERT_EQ(manager.ebo, nullptr);
}

TEST_F(test_low_level_initializer, create_new_vao_with_add_geometry_method)
{
	geometry_manager manager;

	std::vector<float> vertices = {0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
	std::vector<unsigned int> indices  = {0U, 1U, 3U, 1U, 2U, 3U};
	manager.add_geometry(vertices, indices);

	ASSERT_NE(manager.vao->get_id(), 0);
}

TEST_F(test_low_level_initializer, create_new_vbo_with_add_geometry_method) 
{
	geometry_manager manager;

	std::vector<float> vertices = {0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
	std::vector<unsigned int> indices  = {0U, 1U, 3U, 1U, 2U, 3U};
	manager.add_geometry(vertices, indices);

	ASSERT_NE(manager.vbo->get_id(), 0);
}

TEST_F(test_low_level_initializer, create_new_ebo_with_add_geometry_method)
{
	geometry_manager manager;

	std::vector<float> vertices = {0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
	std::vector<unsigned int> indices  = {0U, 1U, 3U, 1U, 2U, 3U};
	manager.add_geometry(vertices, indices);

	ASSERT_NE(manager.ebo->get_id(), 0);
}