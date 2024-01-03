#include "core/engine.hpp"  // for hardware_system, systems

using namespace raptor_enigne::systems;
using namespace raptor_engine::structs;
using namespace raptor_engine;

int main()
{
    static const std::size_t WIDTH {1080U};
    static const std::size_t HEIGHT {720U};
    static const char		 TITLE[] {"Engine test window"};

    window_data_sptr window_info = std::make_shared<window_data>(TITLE,
															    SDL_WINDOWPOS_CENTERED,
															    SDL_WINDOWPOS_CENTERED,
															    WIDTH,
															    HEIGHT,
															    SDL_WINDOW_OPENGL);

    hardware_system_data_sptr hardware_system_info =
		std::make_shared<hardware_system_data>(32U /*SDL_INIT_VIDEO*/, window_info);

    render_objects objects = {
		{"colored_triangle.vs", 
         "colored_triangle.fs",
        { 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
          0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{0U, 1U, 3U,
         1U, 2U, 3U},
        // 0x1907 -- GL_RGB, 0x1908 -- GL_RGBA
		{std::make_shared<texture_config>("resources/textures/container.jpg", 0x1907, 0x1907), 
         std::make_shared<texture_config>("resources/textures/awesomeface.png", 0x1908, 0x1908)}
        }
	};

    draw_config_sptr draw_cfg = std::make_shared<draw_config>(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, false);

	scene_data_sptr scene_info = std::make_shared<scene_data>(objects, draw_cfg);

    init_engine_data_sptr engine_info = std::make_shared<init_engine_data>(hardware_system_info, scene_info);

    engine engine{engine_info};
    engine.run();

    return 0;
}