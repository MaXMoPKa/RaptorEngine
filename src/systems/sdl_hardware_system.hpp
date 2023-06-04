#pragma once

#include "structs/sdl_hardware_system_data.hpp"
#include "systems/base_hardware_system.hpp"

using namespace raptor_engine::structs;

namespace raptor_enigne {
namespace systems {

// TODO: think about SIMPL.
/**
 * @brief hardware_system is the class responsible for communicating with
 * hardware, creating window and so on.
 *
 */
class sdl_hardware_system : public base_hardware_system {
 public:
  /**
   * @brief Construct a new hardware system object with empty @see
   * hardware_system_data and uninitialized window. For initialization use @see
   * create(const hardware_system_data_sptr& hardware_system_info) method.
   *
   */
	 sdl_hardware_system();

  // TODO: think about exceptions if initialization failed.
  /**
   * @brief Construct a new hardware system object with transferred @see
   * hardware_system_data and this constructor will initialize window if it is
   * possible, or write error message in log.
   *
   * @param hardware_system_info is a base info for initialization of window and
   * another systems.
   */
	 sdl_hardware_system(const sdl_hardware_system_data_sptr& hardware_system_info);

  /**
   * @brief Construct a new hardware system object from anpother hardware_system
   */
	 sdl_hardware_system(sdl_hardware_system&& system) noexcept;
	 sdl_hardware_system& operator=(sdl_hardware_system&& system) noexcept;

  sdl_hardware_system(const sdl_hardware_system&)		= delete;
	 sdl_hardware_system& operator=(const sdl_hardware_system&) = delete;

  ~sdl_hardware_system();

 public:
  /**
   * @brief create is one way to initialize hardware_system object.
   *
   * @param hardware_system_info is a base info for initialization of window and
   * another systems.
   */
  void create(const sdl_hardware_system_data_sptr& hardware_system_info);

  /**
   * @brief swap is the method for swap two hardware_system objects with all
   * their data.
   */
  void swap(sdl_hardware_system& system) noexcept;

  /**
   * @brief reset merhod exchange this hardware_system on uninitialized and this
   * hardware_system_data and window will be destroyed.
   *
   */
  void reset() noexcept;

 public:
  /**
   * @return the raw pointer on @see SDL_Window object.
   */
  [[nodiscard]] SDL_Window const* get_window() const noexcept;

  // TODO: think about another way for return init flags.
  /**
   * @return the sdl flags for initialization, but only by one number.
   */
  [[nodiscard]] u32 get_sdl_init_flags() const noexcept;
  /**
   * @return the shared_ptr on @see window_data with base info about window.
   */
  [[nodiscard]] window_data_sptr get_window_data() const noexcept;

 private:

  class sdl_hardware_system_pimpl;
  std::unique_ptr<sdl_hardware_system_pimpl> pimpl;
};

using sdl_hardware_system_uptr = std::unique_ptr<sdl_hardware_system>;
using sdl_hardware_system_sptr = std::shared_ptr<sdl_hardware_system>;
using sdl_hardware_system_wptr = std::weak_ptr<sdl_hardware_system>;

}  // namespace systems
}  // namespace raptor_enigne