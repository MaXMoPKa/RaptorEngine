#pragma once

#include <memory>  // for unique_ptr

#include "defines.hpp"  // for u32

#include "structs/hardware_system_data.hpp"

using namespace raptor_engine::structs;

namespace raptor_enigne {
namespace systems {

// TODO: think about SIMPL.
/**
 * @brief hardware_system is the class responsible for communicating with
 * hardware, creating window and so on.
 *
 */
class hardware_system {
 public:
  /**
   * @brief Construct a new hardware system object with empty @see
   * hardware_system_data and uninitialized window. For initialization use @see
   * create(const hardware_system_data_sptr& hardware_system_info) method.
   *
   */
  hardware_system();

  // TODO: think about exceptions if initialization failed.
  /**
   * @brief Construct a new hardware system object with transferred @see
   * hardware_system_data and this constructor will initialize window if it is
   * possible, or write error message in log.
   *
   * @param hardware_system_info is a base info for initialization of window and
   * another systems.
   */
  hardware_system(const hardware_system_data_sptr& hardware_system_info);

  /**
   * @brief Construct a new hardware system object from anpother hardware_system
   */
  hardware_system(hardware_system&& system) noexcept;
  hardware_system& operator=(hardware_system&& system) noexcept;

  hardware_system(const hardware_system&) = delete;
  hardware_system& operator=(const hardware_system&) = delete;

  ~hardware_system();

 public:
  /**
   * @brief create is one way to initialize hardware_system object.
   *
   * @param hardware_system_info is a base info for initialization of window and
   * another systems.
   */
  void create(const hardware_system_data_sptr& hardware_system_info);

  /**
   * @brief swap is the method for swap two hardware_system objects with all
   * their data.
   */
  void swap(hardware_system& system) noexcept;

  /**
   * @brief reset merhod exchange this hardware_system on uninitialized and this
   * hardware_system_data and window will be destroyed.
   *
   */
  void reset() noexcept;

 public:
  void process_events(bool& window_should_close) noexcept;

public:
  u64 get_time() const noexcept;

 public:
  /**
   * @return the raw pointer on @see SDL_Window object.
   */
  [[nodiscard]] sdl_window_sptr get_window() const noexcept;

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
  class hardware_system_pimpl;
  std::unique_ptr<hardware_system_pimpl> pimpl;
};

using hardware_system_uptr = std::unique_ptr<hardware_system>;
using hardware_system_sptr = std::shared_ptr<hardware_system>;
using hardware_system_wptr = std::weak_ptr<hardware_system>;

}  // namespace systems
}  // namespace raptor_enigne