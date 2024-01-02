#include "glad/glad.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO

#include "render/texture_manager.hpp"

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

TEST(texture_manager, create_texture_manager_with_deafult_constructor)
{
	texture_manager manager {};

	ASSERT_EQ(manager.get_textures().empty(), true);
}

TEST_F(test_low_level_initializer, try_open_file)
{
	texture_manager manager {};
	manager.add_texture("resources/textures/container.jpg");

	ASSERT_EQ(manager.get_is_texture_file_opened(), true);
}

TEST_F(test_low_level_initializer, try_open_non_existent_texture_file)
{
	texture_manager manager {};
	manager.add_texture("non_existent_texture.jpg");

	ASSERT_EQ(manager.get_is_texture_file_opened(), false);
}

TEST_F(test_low_level_initializer, add_texture_to_manager)
{
	texture_manager manager {};
	manager.add_texture("resources/textures/container.jpg");

	ASSERT_EQ(manager.get_textures().empty(), false);
	ASSERT_EQ(manager.get_textures().back()->get_id(), 1);
}