#include "ecs/system_manager.hpp"

#include <functional>
#include <map>

using namespace raptor_engine::ecs;

system_manager::system_manager()
{
	this->allocator =
		new system_allocator(ECS_SYSTEM_MEMORY_BUFFER_SIZE, allocate(ECS_SYSTEM_MEMORY_BUFFER_SIZE, "system_manager"));
}

system_manager::~system_manager()
{
	for (system_work_order::reverse_iterator it = this->work_order.rbegin(); it != this->work_order.rend(); ++it)
	{
		(*it)->~i_system();
		*it = nullptr;
	}

	work_order.clear();
	registry.clear();

	free((void*)this->allocator->get_address());
	delete this->allocator;
	this->allocator = nullptr;
}

void system_manager::update_system_work_order()
{
	// depth-first-search function
	static const std::function<
		void(system_type_id, std::vector<int>&, const std::vector<std::vector<bool>>&, std::vector<system_type_id>&)>
		dfs = [&](system_type_id						vertex,
				  std::vector<int>&						vertex_state,
				  const std::vector<std::vector<bool>>& edges,
				  std::vector<system_type_id>&			output) {
			vertex_state[vertex] = 1; // visited

			for (int i = 0; i < vertex_state.size(); ++i) {
				if (edges[i][vertex] == true && vertex_state[i] == 0)
					dfs(i, vertex_state, edges, output);
			}

			vertex_state[vertex] = 2; // done
			output.push_back(vertex);
		};

	const std::size_t NUM_SYSTEMS = this->dependency_matrix.size();

	// create index array
	std::vector<int> indices(NUM_SYSTEMS);
	for (int i = 0; i < NUM_SYSTEMS; ++i)
		indices[i] = i;

	// determine vertex-groups
	std::vector<std::vector<system_type_id>> vertex_groups;
	std::vector<system_priority>			 group_priority;

	while (indices.empty() == false) {
		system_type_id index = indices.back();
		indices.pop_back();

		if (index == -1)
			continue;

		std::vector<system_type_id> group;
		std::vector<system_type_id> member;

		system_priority current_group_priority = LOWEST_SYSTEM_PRIORITY;
		member.push_back(index);

		while (member.empty() == false) {
			index = member.back();
			member.pop_back();

			for (int i = 0; i < indices.size(); ++i) {
				if (indices[i] != -1 && (this->dependency_matrix[i][index] == true ||
										 this->dependency_matrix[index][i] == true)) {
					member.push_back(i);
					indices[i] = -1;
				}
			}

			group.push_back(index);

			i_system* sys = this->registry[index];
			current_group_priority =
				std::max((sys != nullptr ? sys->priority : NORMAL_SYSTEM_PRIORITY), current_group_priority);
		}

		vertex_groups.push_back(group);
		group_priority.push_back(current_group_priority);
	}

	const std::size_t num_vertex_groups = vertex_groups.size();

	// do a topological sort on groups w.r.t. to groups priority!
	std::vector<int> vertex_states(NUM_SYSTEMS, 0);

	std::multimap<system_priority, std::vector<system_type_id>> vertex_groups_sorted;

	for (int i = 0; i < num_vertex_groups; ++i) {
		auto g = vertex_groups[i];

		std::vector<system_type_id> order;

		for (int j = 0; j < g.size(); ++j) {
			if (vertex_states[g[j]] == 0)
				dfs(g[j], vertex_states, this->dependency_matrix, order);
		}

		std::reverse(order.begin(), order.end());

		// note: MAX - PRIORITY will frce the correct sorting behaviour in
		// multimap (by default a multimap sorts int values from low to high)
		vertex_groups_sorted.insert(std::pair<system_priority, std::vector<system_type_id>>(
			std::numeric_limits<system_priority>::max() - group_priority[i],
			order));
	}

	// LogInfo("Update system work order:")

	// re-build system work order
	this->work_order.clear();
	for (const auto& group : vertex_groups_sorted) {
		for (const auto& m : group.second) {
			i_system* sys = this->registry[m];
			if (sys != nullptr) {
				this->work_order.push_back(sys);
				//LogInfo("\t%s", sys->GetSystemTypeName())
			}
		}
	}
}

system_work_state_mask system_manager::get_system_work_state() const
{
	system_work_state_mask mask (this->work_order.size());

	for (std::size_t i = 0; i < this->work_order.size(); ++i)
	{
		mask[i] = this->work_order[i]->is_enabled;
	}

	return mask;
}

void system_manager::set_system_work_state(const system_work_state_mask& mask)
{
	assert(mask.size() == this->work_order.size() && "Provided mask does not match size of current system array.");

	for (std::size_t i = 0; i < this->work_order.size(); ++i)
	{
		this->work_order[i]->is_enabled = mask[i];
	}
}

void system_manager::update(f32 dt)
{
	for (i_system* s : this->work_order)
	{
		s->time_since_last_update += dt;

		s->is_need_update = (s->update_interval < 0.0f) ||
							((s->update_interval > 0.0f) && (s->time_since_last_update > s->update_interval));

		if (s->is_enabled == true && s->is_need_update == true)
		{
			s->pre_update(dt);
		}
	}

	for (i_system* s : this->work_order) {
		if (s->is_enabled == true && s->is_need_update == true) {
			s->update(dt);

			// reset interval
			s->time_since_last_update = 0.0f;
		}
	}

	for (i_system* s : this->work_order) {
		if (s->is_enabled == true && s->is_need_update == true) {
			s->post_update(dt);
		}
	}
}