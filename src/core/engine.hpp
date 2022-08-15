#pragma once

#include "structs/engine_data.hpp"
#include "systems/hardware_system.hpp"

using namespace raptor_enigne::systems;
using namespace raptor_engine::structs;

namespace raptor_engine {

/**
 * @brief engine class is the main class for work. It is accumulate systems and
 * another engines for useful user interface.
 *
 */
class engine {
 public:
  /**
   * @brief Construct a new engine object with empty @see engine_data and
   * uninitialized systems. For initialization use @see create(const
   * engine_data_sptr& engine_info) method.
   *
   */
  engine();

  /**
   * @brief Construct a new engine object eoth transferred @see engine_data and
   * this contrucor will initialize systems if it is possible, or write error
   * ,essage in log.
   *
   * @param engine_info is a base info for engine initialization.
   */
  engine(const engine_data_sptr& engine_info);

  /**
   * @brief Construct a new engine object from another engine object.
   */
  engine(engine&& engine) noexcept;
  engine& operator=(engine&& engine) noexcept;

  engine(const engine&) = delete;
  engine& operator=(const engine&) = delete;

  ~engine();

 public:
  /**
   * @brief create is one way to initialize engine object.
   *
   * @param engine_info is a base info for engine initialization.
   */
  void create(const engine_data_sptr& engine_info);

  /**
   * @brief swap is the method for swap two engine's object info.
   */
  void swap(engine& engine) noexcept;

  /**
   * @brief reset method exchange this engine object on uninitialized and this
   * engine_data will be destroyed.
   *
   */
  void reset() noexcept;

 public:
  /**
   * @return the shared_ptr on @see engine_data with base info about engine.
   */
  [[nodiscard]] engine_data_sptr get_engine_data() const;

  /**
   * @return the shared_ptr on @see hardware_system object.
   */
  [[nodiscard]] hardware_system_sptr get_hardware_system() const;

 private:
  class engine_pimpl;
  std::unique_ptr<engine_pimpl> pimpl;
};

}  // namespace raptor_engine