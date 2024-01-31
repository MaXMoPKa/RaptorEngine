#pragma once

#include "structs/init_engine_data.hpp"

//#include "systems/hardware_system.hpp"

#include "core/render_engine.hpp"


#include "ecs/event/event_delegate.hpp"
#include "ecs/event/event_handler.hpp"
#include "ecs/utils/timer.hpp"

#include "memory/memory_manager.hpp"

#include "ecs/component/component_manager.hpp"
#include "ecs/entity/entity_manager.hpp"
#include "ecs/system/system_manager.hpp"

//using namespace raptor_enigne::systems;
//using namespace raptor_engine::structs;
//using namespace raptor_engine::render;

namespace raptor_engine {

namespace ecs
{
namespace event
{
class IEvent;
} // namespace event
} // namespace ecs

using EngineUptr = std::unique_ptr<Engine>;
using EngineSptr = std::shared_ptr<Engine>;
using EngineWptr = std::weak_ptr<Engine>;

class Engine
{
public:
	Engine();
	
	Engine(const Engine&)            = delete;
	Engine& operator=(const Engine&) = delete;
	
	~Engine();

public:

	inline const ecs::util::TimerSptr& GetTimer()
	{
		return this->timer;
	}

	inline const ecs::SystemManagerSptr GetSystemManager()
	{
		return this->systemManager;
	}

	inline const ecs::ComponentManagerSptr GetComponentManager()
	{
		return this->componentManager;
	}

	inline const ecs::EntityManagerSptr& GetEntityManager()
	{
		return this->entityManager;
	}

public:
    template <typename E, typename... ARGS>
	void SendEvent(ARGS&&... args_)
	{
		this->eventHandler->Send<E>(std::forward<ARGS>(args_)...);
	}

	void Update(f32 dt_);

public:
	template<class E>
	inline void SubscribeEvent(ecs::event::IEventDelegate* const eventDelegate_)
	{
		this->eventHandler->AddEventCallback<E>(eventDelegate_);
	}

	void UnsubscribeEvent(ecs::event::IEventDelegate* eventDelegate_);

public:
	static const EngineSptr& GetInstance()
	{
		if (engineInstance == nullptr) {
			engineInstance = std::make_shared<Engine>();
		}

		return engineInstance;
	}

private:
	static EngineSptr engineInstance;

private:
	ecs::util::TimerSptr         timer;
	ecs::event::EventHandlerSptr eventHandler;
	ecs::SystemManagerSptr	     systemManager;
	ecs::ComponentManagerSptr	 componentManager;
	ecs::EntityManagerSptr	     entityManager;

};


///**
// * @brief engine class is the main class for work. It is accumulate systems and
// * another engines for useful user interface.
// *
// */
//class engine {
// public:
//  /**
//   * @brief Construct a new engine object with empty @see init_engine_data and
//   * uninitialized systems. For initialization use @see create(const
//   * init_engine_data_sptr& engine_info) method.
//   *
//   */
//  engine();
//
//  /**
//   * @brief Construct a new engine object eoth transferred @see init_engine_data and
//   * this contrucor will initialize systems if it is possible, or write error
//   * ,essage in log.
//   *
//   * @param engine_info is a base info for engine initialization.
//   */
//  engine(const init_engine_data_sptr& engine_info);
//
//  /**
//   * @brief Construct a new engine object from another engine object.
//   */
//  engine(engine&& engine) noexcept;
//  engine& operator=(engine&& engine) noexcept;
//
//  engine(const engine&) = delete;
//  engine& operator=(const engine&) = delete;
//
//  ~engine();
//
// public:
//  /**
//   * @brief create is one way to initialize engine object.
//   *
//   * @param engine_info is a base info for engine initialization.
//   */
//   void create(const init_engine_data_sptr& engine_info);
//
//  /**
//   * @brief swap is the method for swap two engine's object info.
//   */
//  void swap(engine& engine) noexcept;
//
//  /**
//   * @brief reset method exchange this engine object on uninitialized and this
//   * init_engine_data will be destroyed.
//   */
//  void reset() noexcept;
//
//public:
//  void run() noexcept;
//
// public:
//  /**
//   * @return the shared_ptr on @see init_engine_data with base info about engine.
//   */
//  [[nodiscard]] init_engine_data_sptr get_init_engine_data() const;
//
//  /**
//   * @return the shared_ptr on @see hardware_system object.
//   */
//  [[nodiscard]] hardware_system_sptr get_hardware_system() const;
//
//  [[nodiscard]] render_engine_sptr get_render_engine() ;
//
// private:
//  class engine_pimpl;
//  std::unique_ptr<engine_pimpl> pimpl;
//};
}  // namespace raptor_engine