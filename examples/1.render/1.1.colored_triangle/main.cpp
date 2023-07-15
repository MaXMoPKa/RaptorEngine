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
	      std::make_shared<hardware_system_data>(32U, window_info);

    engine_data_sptr engine_info = std::make_shared<engine_data>(hardware_system_info);

    engine engine{engine_info};

    engine.run();

    return 0;
}