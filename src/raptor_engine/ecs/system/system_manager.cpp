#include "ecs/system/system_manager.hpp"

#include <functional>
#include <map>

using namespace raptor_engine::ecs;

SystemManager::SystemManager()
{
	this->allocator =
		new SystemAllocator(ECS_SYSTEM_MEMORY_BUFFER_SIZE, Allocate(ECS_SYSTEM_MEMORY_BUFFER_SIZE, "SystemManager"));
}

SystemManager::~SystemManager()
{
	for (SystemWorkOrder::reverse_iterator it = this->workOrder.rbegin(); it != this->workOrder.rend(); ++it)
	{
		(*it)->~ISystem();
		*it = nullptr;
	}

	workOrder.clear();
	registry.clear();

	Free((void*)this->allocator->GetAddress());
	delete this->allocator;
	this->allocator = nullptr;
}

void SystemManager::UpdateSystemWorkOrder()
{
	// depth-first-search function
	static const std::function<
		void(SystemTypeId, std::vector<int>&, const std::vector<std::vector<bool>>&, std::vector<SystemTypeId>&)>
		dfs = [&](SystemTypeId							vertex,
				  std::vector<int>&						vertexState,
				  const std::vector<std::vector<bool>>& edges,
				  std::vector<SystemTypeId>&			output) {
			vertexState[vertex] = 1; // visited

			for (int i = 0; i < vertexState.size(); ++i) {
				if (edges[i][vertex] == true && vertexState[i] == 0)
					dfs(i, vertexState, edges, output);
			}

			vertexState[vertex] = 2; // done
			output.push_back(vertex);
		};

	const std::size_t NUM_SYSTEMS = this->dependencyMatrix.size();

	// create index array
	std::vector<int> indices(NUM_SYSTEMS);
	for (int i = 0; i < NUM_SYSTEMS; ++i)
		indices[i] = i;

	// determine vertex-groups
	std::vector<std::vector<SystemTypeId>> vertexGroups;
	std::vector<SystemPriority>			   groupPriority;

	while (indices.empty() == false) {
		SystemTypeId index = indices.back();
		indices.pop_back();

		if (index == -1)
			continue;

		std::vector<SystemTypeId> group;
		std::vector<SystemTypeId> member;

		SystemPriority currentGroupPriority = LOWEST_SYSTEM_PRIORITY;
		member.push_back(index);

		while (member.empty() == false) {
			index = member.back();
			member.pop_back();

			for (int i = 0; i < indices.size(); ++i) {
				if (indices[i] != -1 && (this->dependencyMatrix[i][index] == true ||
										 this->dependencyMatrix[index][i] == true)) {
					member.push_back(i);
					indices[i] = -1;
				}
			}

			group.push_back(index);

			ISystem* system = this->registry[index];
			currentGroupPriority =
				std::max((system != nullptr ? system->priority : NORMAL_SYSTEM_PRIORITY), currentGroupPriority);
		}

		vertexGroups.push_back(group);
		groupPriority.push_back(currentGroupPriority);
	}

	const std::size_t numVertexGroups = vertexGroups.size();

	// do a topological sort on groups w.r.t. to groups priority!
	std::vector<int> vertexStates(NUM_SYSTEMS, 0);

	std::multimap<SystemPriority, std::vector<SystemTypeId>> vertexGroupsSorted;

	for (int i = 0; i < numVertexGroups; ++i) {
		auto group = vertexGroups[i];

		std::vector<SystemTypeId> order;

		for (int j = 0; j < group.size(); ++j) {
			if (vertexStates[group[j]] == 0)
				dfs(group[j], vertexStates, this->dependencyMatrix, order);
		}

		std::reverse(order.begin(), order.end());

		// note: MAX - PRIORITY will frce the correct sorting behaviour in
		// multimap (by default a multimap sorts int values from low to high)
		vertexGroupsSorted.insert(std::pair<SystemPriority, std::vector<SystemTypeId>>(
			std::numeric_limits<SystemPriority>::max() - groupPriority[i],
			order));
	}

	// LogInfo("Update system work order:")

	// re-build system work order
	this->workOrder.clear();
	for (const auto& group : vertexGroupsSorted) {
		for (const auto& m : group.second) {
			ISystem* system = this->registry[m];
			if (system != nullptr) {
				this->workOrder.push_back(system);
				//LogInfo("\t%s", sys->GetSystemTypeName())
			}
		}
	}
}

SystemWorkStateMask SystemManager::GetSystemWorkState() const
{
	SystemWorkStateMask mask (this->workOrder.size());

	for (std::size_t i = 0; i < this->workOrder.size(); ++i)
	{
		mask[i] = this->workOrder[i]->isEnabled;
	}

	return mask;
}

void SystemManager::SetSystemWorkState(const SystemWorkStateMask& mask_)
{
	assert(mask_.size() == this->workOrder.size() && "Provided mask does not match size of current system array.");

	for (std::size_t i = 0; i < this->workOrder.size(); ++i)
	{
		this->workOrder[i]->isEnabled = mask_[i];
	}
}

void SystemManager::Update(f32 dt_)
{
	for (ISystem* system : this->workOrder)
	{
		system->timeSinceLastUpdate += dt_;

		system->isNeedUpdate = (system->updateInterval < 0.0f) ||
							   ((system->updateInterval > 0.0f) && (system->timeSinceLastUpdate > system->updateInterval));

		if (system->isEnabled == true && system->isNeedUpdate == true)
		{
			system->PreUpdate(dt_);
		}
	}

	for (ISystem* system : this->workOrder) {
		if (system->isEnabled == true && system->isNeedUpdate == true) {
			system->Update(dt_);

			// reset interval
			system->timeSinceLastUpdate = 0.0f;
		}
	}

	for (ISystem* system : this->workOrder) {
		if (system->isEnabled == true && system->isNeedUpdate == true) {
			system->PostUpdate(dt_);
		}
	}
}