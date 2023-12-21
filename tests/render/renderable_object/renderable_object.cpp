#include "glad/glad.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO

#include "render/renderable_object.hpp"

using namespace testing; 
using namespace raptor_engine::render;

TEST(renderable_object, create_renderable_object_with_default_constructor)
{
	renderable_object object;

	ASSERT_EQ(object.get_geometry_object(), nullptr);
	ASSERT_EQ(object.get_shader_program(),  nullptr);
	ASSERT_EQ(object.get_draw_config(),     nullptr);
}

TEST(renderable_object, create_renderable_object_with_parametrized_constructor)
{
	geometry_object_sptr geom_object {std::make_shared<geometry_object>()};
	shader_program_sptr	 sh_program  {std::make_shared<shader_program>()};
	draw_config_sptr	 draw_cfg    {std::make_shared<draw_config>()};

	renderable_object object(geom_object, sh_program, draw_cfg);

	ASSERT_NE(object.get_geometry_object(), nullptr);
	ASSERT_NE(object.get_shader_program(),  nullptr);
	ASSERT_NE(object.get_draw_config(),     nullptr);
}

TEST(renderable_object, create_renderable_object_using_move_constructor) 
{
	std::function<renderable_object()> test_function = []() -> renderable_object {
		return std::move(renderable_object());
	};

	renderable_object object = test_function();

	ASSERT_EQ(object.get_geometry_object(), nullptr);
	ASSERT_EQ(object.get_shader_program(),  nullptr);
	ASSERT_EQ(object.get_draw_config(),     nullptr);
};

TEST(renderable_object, create_renderable_object_with_move_assignment)
{
	geometry_object_sptr geom_object {std::make_shared<geometry_object>()};
	shader_program_sptr	 sh_program  {std::make_shared<shader_program>()};
	draw_config_sptr	 draw_cfg    {std::make_shared<draw_config>()};

	renderable_object first_object {geom_object, sh_program, draw_cfg};
	renderable_object second_object;

	second_object = std::move(first_object);

	ASSERT_NE(second_object.get_geometry_object(), nullptr);
	ASSERT_NE(second_object.get_shader_program(),  nullptr);
	ASSERT_NE(second_object.get_draw_config(),     nullptr);
}