#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "render/shader_program.hpp"

using namespace testing; 
using namespace raptor_engine::render;

TEST(shader_program, create_shader_program_with_default_constructor)
{
	shader_program program;

	ASSERT_EQ(program.get_id(), 0);
}

TEST(shader_program, create_shader_program_with_parametrized_constructor)
{
	shader_program program {1, {0}};

	ASSERT_EQ(program.get_id(), 1);
	ASSERT_NE(program.get_g_uniforms().empty(), true);
}

TEST(shader_program, create_shader_program_using_move_constructor) {
	std::function<shader_program()> test_function = []() -> shader_program 
	{ 
		return std::move(shader_program(1, {0}));
	};

	shader_program program = test_function();

	ASSERT_EQ(program.get_id(), 1);
	ASSERT_NE(program.get_g_uniforms().empty(), true);
}

TEST(shader_program, create_shader_program_with_move_assignment)
{
	shader_program first_program {1, {0}};
	shader_program second_program;

	second_program = std::move(first_program);

	ASSERT_EQ(second_program.get_id(), 1);
	ASSERT_NE(second_program.get_g_uniforms().empty(), true);
}

TEST(shader_program, get_shader_program_global_uniforms)
{
	shader_program program;

	ASSERT_EQ(program.get_g_uniforms().empty(), true);
}