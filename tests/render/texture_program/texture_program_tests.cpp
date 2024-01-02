#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "render/texture_program.hpp"

using namespace testing; 
using namespace raptor_engine::render;

TEST(texture_program, create_texture_program_with_deafult_constructor)
{
	texture_program texture {};

	ASSERT_EQ(texture.get_id(), 0);
}

TEST(texture_program, create_texture_program_with_parametrized_constructor)
{
	texture_program texture {1};

	ASSERT_EQ(texture.get_id(), 1);
}

TEST(texture_program, create_texture_program_with_move_constructor)
{
	std::function<texture_program()> test_function = []() -> texture_program {
		return std::move(texture_program(1));
	};

	texture_program texture = test_function();

	ASSERT_EQ(texture.get_id(), 1);
}

TEST(texture_program, create_texture_program_with_move_assignment)
{
	texture_program first_texture{1};
	texture_program second_texture {};

	second_texture = std::move(first_texture);

	ASSERT_EQ(second_texture.get_id(), 1);
}