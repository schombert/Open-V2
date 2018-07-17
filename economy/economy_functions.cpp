#include "common\\common.h"
#include "economy_functions.h"
#include "world_state\\world_state.h"

namespace economy {
	void init_economic_scenario(world_state& ws) {
		ws.w.economy_s.current_prices.resize(ws.s.economy_m.aligned_32_goods_count);
		for(auto& g : ws.s.economy_m.goods) {
			ws.w.economy_s.current_prices[g.id] = g.base_price;
		}
	}
}
