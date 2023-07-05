#include <cstddef>     // for size_t
#include <functional>  // for function
#include <memory>      // for __shared_ptr_access, mak...
#include <utility>     // for move

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SDL.h"    // for SDL_INIT_VIDEO, SDL_INIT_AUDIO
#include "SDL_video.h"  // for SDL_WINDOW_OPENGL, SDL_WINDOW...

#include "glad/glad.h"

#include "defines.hpp"                  // for u32
#include "structs/window_data.hpp"      // for window_data, window_data::WIN...
#include "core/engine.hpp"  // for hardware_system, systems

using namespace testing;
using namespace raptor_enigne::systems;
using namespace raptor_engine::structs;
using namespace raptor_engine;

TEST(learn_opengl, colored_triangle)
{
  //GTEST_SKIP() << "Skipping single test";

  static const std::size_t WIDTH {1080U};
  static const std::size_t HEIGHT {810U};
  static const char		   TITLE[] {"Engine test window"};

  window_data_sptr window_info = std::make_shared<window_data>(TITLE,
															   SDL_WINDOWPOS_CENTERED,
															   SDL_WINDOWPOS_CENTERED,
															   WIDTH,
															   HEIGHT,
															   SDL_WINDOW_OPENGL);

  hardware_system_data_sptr hardware_system_info =
	  std::make_shared<hardware_system_data>(32U, window_info);

  engine_data_sptr engine_info = std::make_shared<engine_data>(hardware_system_info);

  engine engine{engine_info};

 engine.run();

 /* u32 sdl_init_flags {SDL_INIT_AUDIO | SDL_INIT_VIDEO};

  const char title[]{"Test title"};
  const auto x_pos{SDL_WINDOWPOS_UNDEFINED};
  const auto y_pos{SDL_WINDOWPOS_UNDEFINED};
  const std::size_t width{960};
  const std::size_t height{720};
  const auto flags{SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP};

  window_data_sptr window_info =
      std::make_shared<window_data>(title, x_pos, y_pos, width, height, flags);

  hardware_system_data_sptr hardware_system_info =
      std::make_shared<hardware_system_data>(sdl_init_flags, window_info);

  hardware_system hardware_sys{hardware_system_info};

  ASSERT_THAT(hardware_sys.get_sdl_init_flags(), Eq(sdl_init_flags));
  ASSERT_NE(hardware_sys.get_window(), nullptr);
  ASSERT_THAT(hardware_sys.get_window_data()->title, Eq(title));
  ASSERT_THAT(hardware_sys.get_window_data()->x_pos, Eq(x_pos));
  ASSERT_THAT(hardware_sys.get_window_data()->y_pos, Eq(y_pos));
  ASSERT_THAT(hardware_sys.get_window_data()->width, Eq(width));
  ASSERT_THAT(hardware_sys.get_window_data()->height, Eq(height));
  ASSERT_THAT(hardware_sys.get_window_data()->flags, Eq(flags));

  const char *vertexShaderSource   = "#version 330 core\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "void main()\n"
									 "{\n"
									 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
									 "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
									 "out vec4 FragColor;\n"
									 "void main()\n"
									 "{\n"
									 "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									 "}\n\0";

  //if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
	//  EXPECT_TRUE(false) << "Failed to initialize GLAD";
  //}

  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
	  glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	  EXPECT_TRUE(false) << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
	  glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	  EXPECT_TRUE(false) << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
  }
  // link shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
	  glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	  EXPECT_TRUE(false) << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
  // buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
  // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when
  // it's not directly necessary.
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  bool window_should_close = false;
  auto precess_events	   = [&window_should_close]() { 
	  SDL_Event event;
	  while (SDL_PollEvent(&event))
	  {
		  switch (event.type) {
			  case SDL_QUIT: {
				  window_should_close = true;
				  break;
			  }
			  case SDL_KEYDOWN: {
				  switch (event.key.keysym.sym) {
					  case SDLK_ESCAPE: {
						  window_should_close = true;
						  break;
					  }
				  }
				  break;
			  }
		  }
	  }
  };
  while (!window_should_close)
  {
	  // input
	  // -----
	  precess_events();

	  // render
	  // ------
	  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	  glClear(GL_COLOR_BUFFER_BIT);

	  glUseProgram(shaderProgram);
	  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do
							  // so to keep things a bit more organized
	  glDrawArrays(GL_TRIANGLES, 0, 3);

	  engine.get_render_engine()->post_update();

	  //hardware_sys.update();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // SDL2: terminate, clearing all previously allocated SDL2 resources.
  // ------------------------------------------------------------------*/
}