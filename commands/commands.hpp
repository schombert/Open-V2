#pragma once
#include "common\\common.h"
#include "commands.h"
#include "world_state\\world_state.h"

namespace commands {
	template<typename FIRST, typename ...REST>
	bool command_set<FIRST, REST...>::execute(world_state & ws) {
		const bool had_commands = command_list.flush([&ws](FIRST& e) {
			execute_command(e, ws);
		});
		return command_set<REST...>::execute(ws) | had_commands;
	}

	template<typename FIRST, typename ...REST>
	template<typename T, typename ...ARGS>
	void command_set<FIRST, REST...>::add(ARGS && ...args) {
		if constexpr(std::is_same_v<T, FIRST>) {
			command_list.emplace(std::forward<ARGS>(args)...);
		} else {
			command_set<REST...>::template add<T>(std::forward<ARGS>(args)...);
		}
	}
}
