#pragma once

#include <memory>  // for unique_ptr

#include "SDL_video.h"  // for SDL_Window

#include "defines.hpp"  // for u32
#include "structs/base_hardware_system_data.hpp"

using namespace raptor_engine::structs;

namespace raptor_enigne {
namespace systems {

// TODO: think about SIMPL.
/**
 * @brief hardware_system is the class responsible for communicating with
 * hardware, creating window and so on.
 *
 */
class base_hardware_system {
 public:
  /**
   * @brief Construct a new hardware system object with empty @see
   * hardware_system_data and uninitialized window. For initialization use @see
   * create(const hardware_system_data_sptr& hardware_system_info) method.
   *
   */
  base_hardware_system();

  // TODO: think about exceptions if initialization failed.
  /**
   * @brief Construct a new hardware system object with transferred @see
   * hardware_system_data and this constructor will initialize window if it is
   * possible, or write error message in log.
   *
   * @param hardware_system_info is a base info for initialization of window and
   * another systems.
   */
  base_hardware_system(const base_hardware_system_data_sptr& hardware_system_info);

  /**
   * @brief Construct a new hardware system object from anpother hardware_system
   */
  base_hardware_system(base_hardware_system&& system) noexcept;
  base_hardware_system& operator=(base_hardware_system&& system) noexcept;

  base_hardware_system(const base_hardware_system&)		  = delete;
  base_hardware_system& operator=(const base_hardware_system&) = delete;

  ~base_hardware_system();

 public:
  /**
   * @brief create is one way to initialize hardware_system object.
   *
   * @param hardware_system_info is a base info for initialization of window and
   * another systems.
   */
  void create(const base_hardware_system_data_sptr& hardware_system_info);

  /**
   * @brief swap is the method for swap two hardware_system objects with all
   * their data.
   */
  void swap(base_hardware_system& system) noexcept;

  /**
   * @brief reset merhod exchange this hardware_system on uninitialized and this
   * hardware_system_data and window will be destroyed.
   *
   */
  void reset() noexcept;

 private:
  class base_hardware_system_pimpl;
  std::unique_ptr<base_hardware_system_pimpl> pimpl;
};

using hardware_system_uptr = std::unique_ptr<base_hardware_system>;
using hardware_system_sptr = std::shared_ptr<base_hardware_system>;
using hardware_system_wptr = std::weak_ptr<base_hardware_system>;

}  // namespace systems
}  // namespace raptor_enigne