#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "render/shader_manager.hpp"

using namespace testing;

TEST(shader_manager, create_shader_manager_instance_with_default_constructor)
{
	shader_manager manager{};

	ASSERT_EQ(manager.get_shader_program(), 0);
}

TEST(sahder_manager, DISABLED_create_shader_manager_instance_with_parametrized_constructor)
{
	shader_manager manager {"test.vs", "test.fs"};

	ASSERT_NE(manager.get_shader_program(), 0);
}

TEST(shader_manager, try_open_shader_files)
{
	shader_manager manager {"test.vs", "test.fs"};

	ASSERT_EQ(manager.get_is_vs_file_opened(), true);
	ASSERT_EQ(manager.get_is_fs_file_opened(), true);
}

TEST(shader_manager, try_open_non_existent_shader_files)
{
	shader_manager manager {"non_existent_test.vs", "non_existent_test.fs"};

	ASSERT_EQ(manager.get_is_vs_file_opened(), false);
	ASSERT_EQ(manager.get_is_fs_file_opened(), false);
}

TEST(shader_manager, read_data_from_shader_files)
{
	shader_manager manager {"test.vs", "test.fs"};

	std::string vs_code{""};
	std::string fs_code {""};

	ASSERT_EQ(manager.get_vs_code(), vs_code);
	ASSERT_EQ(manager.get_fs_code(), fs_code);
}

TEST(shader_manager, create_shaders_from_read_sources) 
{
	shader_manager manager {"test.vs", "test.fs"};

	ASSERT_NE(manager.get_vs_id(), 0);
	ASSERT_NE(manager.get_fs_id(), 0);
}