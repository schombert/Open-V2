#include "map_modes.hpp"
#include "provinces/province_functions.hpp"
#include "nations/nations_functions.hpp"
#include "economy/economy_functions.hpp"
#include "population/population_functions.h"

namespace map_mode {
	state::state() : legends(std::make_unique<legend_gui>()) {}
	state::~state() {}

	void default_color_province(world_state const& ws, provinces::province_tag p, uint8_t* pcolors, uint8_t* scolors) {
		if(ws.s.province_m.province_container.get<province::is_sea>(p)) {
			pcolors[0] = 80ui8;
			pcolors[1] = 80ui8;
			pcolors[2] = 255ui8;
			scolors[0] = 80ui8;
			scolors[1] = 80ui8;
			scolors[2] = 255ui8;
		} else {
			pcolors[0] = 255ui8;
			pcolors[1] = 255ui8;
			pcolors[2] = 255ui8;
			scolors[0] = 255ui8;
			scolors[1] = 255ui8;
			scolors[2] = 255ui8;
		}
	}

	void update_map_colors(world_state& ws) {
		bool map_change_expected = true;
		if(ws.w.map_view.changed.compare_exchange_strong(map_change_expected, false)) {
			const auto pcolors = ws.w.map.colors.primary_color_data();
			const auto scolors = ws.w.map.colors.secondary_color_data();

			if(pcolors == nullptr || scolors == nullptr) {
				ws.w.map_view.changed.store(false, std::memory_order_release);
				return;
			}

			
			if(ws.w.map_view.mode == type::prices) {
				auto g = ws.w.map_view.legends->current_good;
				if(g) {
					auto price_range = economy::global_price_range(ws, g);

					for(int32_t i = 0; i < ws.w.province_s.province_state_container.size(); ++i) {
						if(auto sid = provinces::province_state(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)));
							bool(sid) && bool(ws.w.nation_s.states.get<::state::owner>(sid))) {


							auto local_prices = economy::state_current_prices(ws, sid);

							auto frac = (local_prices[g] + 0.01f - price_range.minimum) / (price_range.maximum + 0.01f - price_range.minimum );

							pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
							pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 2] = uint8_t(25);
							scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
							scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
							scolors[i * 3 + 2] = uint8_t(25);
						} else {
							default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
								pcolors + i * 3, scolors + i * 3);
						}
					}
				}
			} else if(ws.w.map_view.mode == type::production) {
				auto g = ws.w.map_view.legends->current_good;
				if(g) {
					auto prod_range = economy::global_production_range(ws, g);

					for(int32_t i = 0; i < ws.w.province_s.province_state_container.size(); ++i) {
						if(auto sid = provinces::province_state(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)));
							bool(sid) && bool(ws.w.nation_s.states.get<::state::owner>(sid))) {


							auto local_production = economy::state_current_production(ws, sid, g);

							auto frac = (local_production + 0.01f - prod_range.minimum) / (prod_range.maximum + 0.01f - prod_range.minimum);

							pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
							pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 2] = uint8_t(25);
							scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
							scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
							scolors[i * 3 + 2] = uint8_t(25);
						} else {
							default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
								pcolors + i * 3, scolors + i * 3);
						}
					}
				}
			} else if(ws.w.map_view.mode == type::purchasing) {
				auto g = ws.w.map_view.legends->current_good;
				if(auto selected_id = ws.w.trade_w.selected_state; g && ws.w.nation_s.states.is_valid_index(selected_id)) {
					auto purchasing_handle = ws.w.nation_s.state_purchases.get(selected_id, g);
					auto purchases_data_range = get_range(ws.w.economy_s.purchasing_arrays, purchasing_handle);

					if(std::begin(purchases_data_range) != std::end(purchases_data_range)) {
						auto max_purchases = *std::max_element(std::begin(purchases_data_range) + 1, std::end(purchases_data_range));


						for(int32_t i = 0; i < ws.w.province_s.province_state_container.size(); ++i) {
							if(auto sid = provinces::province_state(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)));
								bool(sid) && bool(ws.w.nation_s.states.get<::state::owner>(sid)) && purchases_data_range.first + to_index(sid) + 1 < purchases_data_range.second) {

								auto amount = purchases_data_range.first[to_index(sid) + 1];

								auto fraction = amount / (max_purchases + 0.000001f);
								if(fraction < 0)
									std::abort();
								pcolors[i * 3 + 0] = uint8_t(fraction * 205.0f + 50.0f);
								pcolors[i * 3 + 1] = uint8_t(fraction * 205.0f + 50.0f);
								pcolors[i * 3 + 2] = uint8_t(fraction * 205.0f + 50.0f);
								scolors[i * 3 + 0] = uint8_t(fraction * 205.0f + 50.0f);
								scolors[i * 3 + 1] = uint8_t(fraction * 205.0f + 50.0f);
								scolors[i * 3 + 2] = uint8_t(fraction * 205.0f + 50.0f);

								continue;
							}

							default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
								pcolors + i * 3, scolors + i * 3);
						}

					}
				}
			} else if(auto lprov = ws.w.map_view.legends ? ws.w.map_view.legends->current_province : provinces::province_tag();
				is_valid_index(lprov) && ws.w.map_view.mode == type::distance) {

				auto pcount = ws.s.province_m.province_container.size();
				for(int32_t i = 0; i < pcount; ++i) {
					auto distance = ws.w.province_s.province_distance_to[to_index(lprov) * pcount + i];
					pcolors[i * 3 + 0] = uint8_t(std::clamp((distance / 8'000.0f - 1.0f), 0.0f, 1.0f) * 255.0f);
					pcolors[i * 3 + 1] = uint8_t(std::clamp((1.0f - distance / 8'000.0f), 0.0f, 1.0f) * 255.0f);
					pcolors[i * 3 + 2] = uint8_t(100);
					scolors[i * 3 + 0] = uint8_t(std::clamp((distance / 8'000.0f - 1.0f), 0.0f, 1.0f) * 255.0f);
					scolors[i * 3 + 1] = uint8_t(std::clamp((1.0f - distance / 8'000.0f), 0.0f, 1.0f) * 255.0f);
					scolors[i * 3 + 2] = uint8_t(100);
				}
			} else if(ws.w.map_view.mode == type::culture) {
				auto pcount = ws.s.province_m.province_container.size();
				auto const c = ws.w.map_view.legends->current_culture;
				auto const col = c ? ws.s.culture_m.culture_container[c].color : graphics::color_rgb{ 0,0,0 };

				for(int32_t i = 0; i < pcount; ++i) {
					auto const p = provinces::province_tag(provinces::province_tag::value_base_t(i));
					if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
						if(c) {
							auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
							auto culture_pop = ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, c));
							auto const frac = culture_pop / (total_pop > 0 ? total_pop : 1.0f);

							pcolors[i * 3 + 0] = uint8_t(std::clamp(float(col.r) * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 1] = uint8_t(std::clamp(float(col.g) * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 2] = uint8_t(std::clamp(float(col.b) * frac, 0.0f, 255.0f));
							scolors[i * 3 + 0] = uint8_t(std::clamp(float(col.r) * frac, 0.0f, 255.0f));
							scolors[i * 3 + 1] = uint8_t(std::clamp(float(col.g) * frac, 0.0f, 255.0f));
							scolors[i * 3 + 2] = uint8_t(std::clamp(float(col.b) * frac, 0.0f, 255.0f));
						} else {
							auto dominant_culture = ws.w.province_s.province_state_container.get<province_state::dominant_culture>(p);
							if(dominant_culture) {
								auto const dcol = ws.s.culture_m.culture_container[dominant_culture].color;

								pcolors[i * 3 + 0] = dcol.r;
								pcolors[i * 3 + 1] = dcol.g;
								pcolors[i * 3 + 2] = dcol.b;
								scolors[i * 3 + 0] = dcol.r;
								scolors[i * 3 + 1] = dcol.g;
								scolors[i * 3 + 2] = dcol.b;
							} else {
								pcolors[i * 3 + 0] = uint8_t(0);
								pcolors[i * 3 + 1] = uint8_t(0);
								pcolors[i * 3 + 2] = uint8_t(0);
								scolors[i * 3 + 0] = uint8_t(0);
								scolors[i * 3 + 1] = uint8_t(0);
								scolors[i * 3 + 2] = uint8_t(0);
							}
						}
					} else {
						default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
							pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::region) {
				auto pcount = ws.s.province_m.province_container.size();
				int32_t const total_number_of_colors = 100;
				int32_t const rel_prime = find_best_relative_prime(total_number_of_colors);

				for(int32_t i = 0; i < pcount; ++i) {
					auto const p = provinces::province_tag(provinces::province_tag::value_base_t(i));

					if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
						auto const state_id = ws.s.province_m.province_container.get<province::state_id>(p);
						auto const val = (to_index(state_id) * rel_prime) % total_number_of_colors;
						auto const icolor = graphics::hue_to_rgb(float(val) / float(total_number_of_colors));

						pcolors[i * 3 + 0] = icolor.r;
						pcolors[i * 3 + 1] = icolor.g;
						pcolors[i * 3 + 2] = icolor.b;
						scolors[i * 3 + 0] = icolor.r;
						scolors[i * 3 + 1] = icolor.g;
						scolors[i * 3 + 2] = icolor.b;
					} else {
						default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
							pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::sphere) {
				for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
					if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {

						if(auto owner = provinces::province_owner(ws, p); owner) {
							if(auto sl = ws.w.nation_s.nations.get<nation::sphere_leader>(owner); sl || nations::is_great_power(ws, owner)) {
								auto owner_cc = ws.w.nation_s.nations.get<nation::current_color>(sl ? sl : owner);
								pcolors[i * 3 + 0] = owner_cc.r;
								pcolors[i * 3 + 1] = owner_cc.g;
								pcolors[i * 3 + 2] = owner_cc.b;
								scolors[i * 3 + 0] = owner_cc.r;
								scolors[i * 3 + 1] = owner_cc.g;
								scolors[i * 3 + 2] = owner_cc.b;
							} else {
								pcolors[i * 3 + 0] = 0ui8;
								pcolors[i * 3 + 1] = 0ui8;
								pcolors[i * 3 + 2] = 0ui8;
								scolors[i * 3 + 0] = 0ui8;
								scolors[i * 3 + 1] = 0ui8;
								scolors[i * 3 + 2] = 0ui8;
							}
						} else {
							pcolors[i * 3 + 0] = 0ui8;
							pcolors[i * 3 + 1] = 0ui8;
							pcolors[i * 3 + 2] = 0ui8;
							scolors[i * 3 + 0] = 0ui8;
							scolors[i * 3 + 1] = 0ui8;
							scolors[i * 3 + 2] = 0ui8;
						}
					} else {
						default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
							pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::population) {
				auto const range = ws.w.map_view.legends->showing_density == false ? 
					population::population_amount_range(ws) : population::population_density_range(ws);

				for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
					if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
						auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
						auto area = ws.s.province_m.province_container.get<province::area>(p);
						if(total_pop > 0 && area > 0) {
							auto const val = ws.w.map_view.legends->showing_density ? total_pop / area : total_pop;
							auto const frac = std::sqrt(val - range.min) / std::sqrt(range.max - range.min);

							pcolors[i * 3 + 0] = uint8_t(std::clamp(25.5f * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 1] = uint8_t(std::clamp(255.0f * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 2] = uint8_t(std::clamp(25.5f * frac, 0.0f, 255.0f));
							scolors[i * 3 + 0] = uint8_t(std::clamp(25.5f * frac, 0.0f, 255.0f));
							scolors[i * 3 + 1] = uint8_t(std::clamp(255.0f * frac, 0.0f, 255.0f));
							scolors[i * 3 + 2] = uint8_t(std::clamp(25.5f * frac, 0.0f, 255.0f));
						} else {
							pcolors[i * 3 + 0] = 0ui8;
							pcolors[i * 3 + 1] = 0ui8;
							pcolors[i * 3 + 2] = 0ui8;
							scolors[i * 3 + 0] = 0ui8;
							scolors[i * 3 + 1] = 0ui8;
							scolors[i * 3 + 2] = 0ui8;
						}
					} else {
						default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::relations) {
				auto const rel_with = ws.w.map_view.legends->current_nation;
				for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
					if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
						if(auto owner = provinces::province_owner(ws, p)) {
							auto const rel_value = nations::get_relationship(ws, rel_with, owner);
							float const frac = float(rel_value + 200) / 400.0f;
							pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
							pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 2] = uint8_t(25);
							scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
							scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
							scolors[i * 3 + 2] = uint8_t(25);
						} else {
							default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
						}
					} else {
						default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::migration) {
				if(ws.w.map_view.legends->showing_internal_migration) {
					tagged_vector<population::population_range, nations::country_tag, concurrent_allocator<population::population_range>> nation_ranges;
					auto const size = ws.w.nation_s.nations.size();
					nation_ranges.resize(size);
					std::fill_n(nation_ranges.data(), size, population::population_range{ 0.0f, 0.0f });

					for(int32_t i = 0; i < int32_t(size); ++i) {
						nations::country_tag n = nations::country_tag(nations::country_tag::value_base_t(i));
						auto prange = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
						for(auto p : prange) {
							auto p_amount = std::trunc(ws.w.province_s.province_state_container.get<province_state::old_migration_growth>(p));
							nation_ranges[n].min = std::min(nation_ranges[n].min, p_amount);
							nation_ranges[n].max = std::max(nation_ranges[n].max, p_amount);
						}
					}

					for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
						const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
						if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
							auto const owner = provinces::province_owner(ws, p);
							if(owner && to_index(owner) < nation_ranges.size()) {
								auto amount = std::trunc(ws.w.province_s.province_state_container.get<province_state::old_migration_growth>(p));

								float const frac = [range = nation_ranges[owner], amount]() {
									if(amount < 0)
										return range.min != 0 ? 0.5f * (1.0f - amount / range.min) : 0.5f;
									else if(amount > 0)
										return range.max != 0 ? 0.5f + (0.5f * amount / range.max) : 0.5f;
									else
										return 0.5f;
								}();

								pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
								pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
								pcolors[i * 3 + 2] = uint8_t(25);
								scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
								scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
								scolors[i * 3 + 2] = uint8_t(25);
							} else {
								pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								pcolors[i * 3 + 2] = uint8_t(25);
								scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								scolors[i * 3 + 2] = uint8_t(25);
							}
						} else {
							default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
						}
					}
				} else {
					tagged_vector<float, nations::country_tag, concurrent_allocator<float>> nation_totals;
					auto const size = ws.w.nation_s.nations.size();
					nation_totals.resize(size);
					std::fill_n(nation_totals.data(), size, 0.0f);
					population::population_range range{ 0.0f, 0.0f };

					for(int32_t i = 0; i < int32_t(size); ++i) {
						nations::country_tag n = nations::country_tag(nations::country_tag::value_base_t(i));
						auto prange = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
						for(auto p : prange) {
							nation_totals[n] += ws.w.province_s.province_state_container.get<province_state::old_immigration_growth>(p);
						}
						nation_totals[n] = std::trunc(nation_totals[n]);
						range.min = std::min(range.min, nation_totals[n]);
						range.max = std::max(range.max, nation_totals[n]);
					}

					for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
						const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
						if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
							auto const owner = provinces::province_owner(ws, p);
							if(owner && to_index(owner) < nation_totals.size()) {
								auto amount = nation_totals[owner];

								float const frac = [range, amount]() {
									if(amount < 0)
										return range.min != 0 ? 0.5f * (1.0f - amount / range.min) : 0.5f;
									else if(amount > 0)
										return range.max != 0 ? 0.5f + (0.5f * amount / range.max) : 0.5f;
									else
										return 0.5f;
								}();

								pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
								pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
								pcolors[i * 3 + 2] = uint8_t(25);
								scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * (1.0f - frac), 0.0f, 255.0f));
								scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * frac, 0.0f, 255.0f));
								scolors[i * 3 + 2] = uint8_t(25);
							} else {
								pcolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								pcolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								pcolors[i * 3 + 2] = uint8_t(25);
								scolors[i * 3 + 0] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								scolors[i * 3 + 1] = uint8_t(std::clamp(25.0f + 230.0f * 0.5f, 0.0f, 255.0f));
								scolors[i * 3 + 2] = uint8_t(25);
							}
						} else {
							default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
						}
					}

				}
			} else if(ws.w.map_view.mode == type::infrastructure) {
				for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));

					if(auto owner = provinces::province_owner(ws, p); owner) {
						auto rr_level = ws.w.province_s.province_state_container.get<province_state::railroad_level>(p);
						if(rr_level == 0) {
							pcolors[i * 3 + 0] = 255ui8;
							pcolors[i * 3 + 1] = 255ui8;
							pcolors[i * 3 + 2] = 255ui8;
							scolors[i * 3 + 0] = 255ui8;
							scolors[i * 3 + 1] = 255ui8;
							scolors[i * 3 + 2] = 255ui8;
						} else if(rr_level == 1) {
							pcolors[i * 3 + 0] = 224ui8;
							pcolors[i * 3 + 1] = 215ui8;
							pcolors[i * 3 + 2] = 96ui8;
							scolors[i * 3 + 0] = 224ui8;
							scolors[i * 3 + 1] = 215ui8;
							scolors[i * 3 + 2] = 96ui8;
						} else if(rr_level == 2) {
							pcolors[i * 3 + 0] = 224ui8;
							pcolors[i * 3 + 1] = 172ui8;
							pcolors[i * 3 + 2] = 94ui8;
							scolors[i * 3 + 0] = 224ui8;
							scolors[i * 3 + 1] = 172ui8;
							scolors[i * 3 + 2] = 94ui8;
						} else if(rr_level == 3) {
							pcolors[i * 3 + 0] = 224ui8;
							pcolors[i * 3 + 1] = 141ui8;
							pcolors[i * 3 + 2] = 116ui8;
							scolors[i * 3 + 0] = 224ui8;
							scolors[i * 3 + 1] = 141ui8;
							scolors[i * 3 + 2] = 116ui8;
						} else if(rr_level == 4) {
							pcolors[i * 3 + 0] = 224ui8;
							pcolors[i * 3 + 1] = 105ui8;
							pcolors[i * 3 + 2] = 38ui8;
							scolors[i * 3 + 0] = 224ui8;
							scolors[i * 3 + 1] = 105ui8;
							scolors[i * 3 + 2] = 38ui8;
						} else if(rr_level == 5) {
							pcolors[i * 3 + 0] = 224ui8;
							pcolors[i * 3 + 1] = 83ui8;
							pcolors[i * 3 + 2] = 101ui8;
							scolors[i * 3 + 0] = 224ui8;
							scolors[i * 3 + 1] = 83ui8;
							scolors[i * 3 + 2] = 101ui8;
						}

						if(commands::is_command_valid(
							commands::province_building(ws.w.local_player_nation, commands::province_building_type::province_railroad, p), ws) ||
							commands::is_command_valid(commands::foreign_invest_railroad(ws.w.local_player_nation, p), ws)) {

							scolors[i * 3 + 0] = 72ui8;
							scolors[i * 3 + 1] = 216ui8;
							scolors[i * 3 + 2] = 65ui8;
						}
						if(commands::is_command_valid(
							commands::province_building(ws.w.local_player_nation, commands::province_building_type::province_naval_base, p), ws)) {

							pcolors[i * 3 + 0] = 65ui8;
							pcolors[i * 3 + 1] = 155ui8;
							pcolors[i * 3 + 2] = 211ui8;
						}
					} else {
						default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
							pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::rgo) {
				int32_t max_size = 1;
				auto const cg = ws.w.map_view.legends->current_good;
				ws.s.province_m.for_each_land_province([&max_size, &ws, cg](provinces::province_tag o){
					if(!is_valid_index(cg) || cg == ws.w.province_s.province_state_container.get<province_state::rgo_production>(o))
						max_size = std::max(max_size, int32_t(ws.w.province_s.province_state_container.get<province_state::rgo_size>(o)));
				});
				for(size_t i = 1; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
					if(i < ws.s.province_m.first_sea_province) {
						auto prgo = ws.w.province_s.province_state_container.get<province_state::rgo_production>(p);
						if(prgo && (!is_valid_index(cg) || cg == prgo)) {
							float frac = cg ?
								0.2f + 0.8f * float(ws.w.province_s.province_state_container.get<province_state::rgo_size>(p)) / float(max_size)
								: 1.0f;
							auto clr = ws.s.economy_m.goods[prgo].color;

							pcolors[i * 3 + 0] = uint8_t(std::clamp(float(clr.r) * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 1] = uint8_t(std::clamp(float(clr.g) * frac, 0.0f, 255.0f));
							pcolors[i * 3 + 2] = uint8_t(std::clamp(float(clr.b) * frac, 0.0f, 255.0f));
							scolors[i * 3 + 0] = uint8_t(std::clamp(float(clr.r) * frac, 0.0f, 255.0f));
							scolors[i * 3 + 1] = uint8_t(std::clamp(float(clr.g) * frac, 0.0f, 255.0f));
							scolors[i * 3 + 2] = uint8_t(std::clamp(float(clr.b) * frac, 0.0f, 255.0f));
						} else {
							pcolors[i * 3 + 0] = 0ui8;
							pcolors[i * 3 + 1] = 0ui8;
							pcolors[i * 3 + 2] = 0ui8;
							scolors[i * 3 + 0] = 0ui8;
							scolors[i * 3 + 1] = 0ui8;
							scolors[i * 3 + 2] = 0ui8;
						}
					} else {
						default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
					}
				}
			} else if(ws.w.map_view.mode == type::voting) {
				auto const ide = ws.w.map_view.legends->current_ideology;
				for(size_t i = 1; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag p(static_cast<provinces::province_tag::value_base_t>(i));
					if(i < ws.s.province_m.first_sea_province) {
						if(ide) {
							auto const support = ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, ide));
							auto const total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
							if(total_pop > 0) {
								auto clr = ws.s.ideologies_m.ideology_container[ide].color;
								float frac = std::sqrt(support / total_pop);

								pcolors[i * 3 + 0] = uint8_t(std::clamp(float(clr.r) * frac + 255.0f * (1.0f - frac), 0.0f, 255.0f));
								pcolors[i * 3 + 1] = uint8_t(std::clamp(float(clr.g) * frac + 255.0f * (1.0f - frac), 0.0f, 255.0f));
								pcolors[i * 3 + 2] = uint8_t(std::clamp(float(clr.b) * frac + 255.0f * (1.0f - frac), 0.0f, 255.0f));
								scolors[i * 3 + 0] = uint8_t(std::clamp(float(clr.r) * frac + 255.0f * (1.0f - frac), 0.0f, 255.0f));
								scolors[i * 3 + 1] = uint8_t(std::clamp(float(clr.g) * frac + 255.0f * (1.0f - frac), 0.0f, 255.0f));
								scolors[i * 3 + 2] = uint8_t(std::clamp(float(clr.b) * frac + 255.0f * (1.0f - frac), 0.0f, 255.0f));
							} else {
								default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
							}
						} else {
							auto const dide = ws.w.province_s.province_state_container.get<province_state::dominant_ideology>(p);
							if(dide) {
								auto clr = ws.s.ideologies_m.ideology_container[dide].color;
								pcolors[i * 3 + 0] = clr.r;
								pcolors[i * 3 + 1] = clr.g;
								pcolors[i * 3 + 2] = clr.b;
								scolors[i * 3 + 0] = clr.r;
								scolors[i * 3 + 1] = clr.g;
								scolors[i * 3 + 2] = clr.b;
							} else {
								default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
							}
						}
					} else {
						default_color_province(ws, p, pcolors + i * 3, scolors + i * 3);
					}
				}
			} else { // default case: color by ownership
				for(size_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag this_province(static_cast<provinces::province_tag::value_base_t>(i));

					if(auto owner = provinces::province_owner(ws, this_province); owner) {
						auto owner_cc = ws.w.nation_s.nations.get<nation::current_color>(owner);
						pcolors[i * 3 + 0] = owner_cc.r;
						pcolors[i * 3 + 1] = owner_cc.g;
						pcolors[i * 3 + 2] = owner_cc.b;
						scolors[i * 3 + 0] = owner_cc.r;
						scolors[i * 3 + 1] = owner_cc.g;
						scolors[i * 3 + 2] = owner_cc.b;
					} else {
						default_color_province(ws, provinces::province_tag(provinces::province_tag::value_base_t(i)),
							pcolors + i * 3, scolors + i * 3);
					}
				}
			}

			ws.w.map.colors.update_ready();
		}
	}

	void change_mode(world_state& ws, type new_mode) {
		if(ws.w.map_view.mode != new_mode) {
			ui::hide(*(ws.w.map_view.legends->generic_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->generic_gradient_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->population_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->relations_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->migration_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->infrastructure_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->rgo_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->resource_legend_window.associated_object));
			ui::hide(*(ws.w.map_view.legends->voting_legend_window.associated_object));

			ws.w.map_view.mode = new_mode;
			ws.w.map_view.changed.store(true, std::memory_order_release);

			ws.w.bottombar_w.update_bottombar(ws.w.gui_m);

			if(ws.w.map_view.mode == type::political || ws.w.map_view.mode == type::sphere || ws.w.map_view.mode == type::region) {
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			} else if(ws.w.map_view.mode == type::culture) {
				ws.w.map_view.legends->current_culture = cultures::culture_tag();
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			} else if(ws.w.map_view.mode == type::population) {
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->population_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			} else if(ws.w.map_view.mode == type::relations) {
				ws.w.map_view.legends->current_nation = ws.w.local_player_nation;
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->relations_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			} else if(ws.w.map_view.mode == type::migration) {
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->migration_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			} else if(ws.w.map_view.mode == type::infrastructure) {
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->infrastructure_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(
					ws.w.map_view.legends->infrastructure_map_icons.data(),
					ws.w.map_view.legends->infrastructure_map_container,
					ws.s.province_m.first_sea_province);
			} else if(ws.w.map_view.mode == type::rgo) {
				ws.w.map_view.legends->current_good = economy::goods_tag();
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->rgo_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(
					ws.w.map_view.legends->rgo_map_icons.data(),
					ws.w.map_view.legends->rgo_map_container,
					ws.s.province_m.first_sea_province);
			} else if(ws.w.map_view.mode == type::prices || ws.w.map_view.mode == type::production) {
				ws.w.map_view.legends->current_good = economy::goods_tag(economy::goods_tag::value_base_t(1));
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->resource_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			} else if(ws.w.map_view.mode == type::voting) {
				ws.w.map_view.legends->current_ideology = ideologies::ideology_tag();
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->voting_legend_window.associated_object));
				ws.w.map.associate_map_icon_set(nullptr, nullptr, 0);
			}
		}
	}

	void on_selection(world_state& ws, provinces::province_tag p) {
		if(is_valid_index(p) && p != provinces::province_tag(0) && to_index(p) < ws.s.province_m.first_sea_province) {
			if(ws.w.map_view.mode == type::political || ws.w.map_view.mode == type::sphere || ws.w.map_view.mode == type::region
				|| ws.w.map_view.mode == type::population || ws.w.map_view.mode == type::migration || ws.w.map_view.mode == type::infrastructure
				|| ws.w.map_view.mode == type::prices || ws.w.map_view.mode == type::production || ws.w.map_view.mode == type::voting) {
				sound::play_interface_sound(ws, ws.s.sound_m.click_sound);
				ws.w.province_w.show_province_window(ws.w.gui_m, p);
			} else if(ws.w.map_view.mode == type::culture) {
				auto dominant_culture = ws.w.province_s.province_state_container.get<province_state::dominant_culture>(p);
				if(dominant_culture) {
					ws.w.map_view.legends->current_culture = dominant_culture;
					ui::hide(*(ws.w.map_view.legends->generic_legend_window.associated_object));
					ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_gradient_legend_window.associated_object));
				} else {
					ws.w.map_view.legends->current_culture = cultures::culture_tag();
					ui::hide(*(ws.w.map_view.legends->generic_gradient_legend_window.associated_object));
					ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_legend_window.associated_object));
				}
				ws.w.map_view.changed.store(true, std::memory_order_release);
			} else if(ws.w.map_view.mode == type::relations) {
				auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(p);
				if(owner) 
					ws.w.map_view.legends->current_nation = owner;
				else
					ws.w.map_view.legends->current_nation = ws.w.local_player_nation;
				ws.w.map_view.changed.store(true, std::memory_order_release);
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->relations_legend_window.associated_object));
			} else if(ws.w.map_view.mode == type::rgo) {
				auto prgo = ws.w.province_s.province_state_container.get<province_state::rgo_production>(p);
				ws.w.map_view.legends->current_good = prgo;
				ws.w.map_view.changed.store(true, std::memory_order_release);
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->rgo_legend_window.associated_object));
			}
		} else {
			if(ws.w.map_view.mode == type::political || ws.w.map_view.mode == type::sphere || ws.w.map_view.mode == type::region
				|| ws.w.map_view.mode == type::population || ws.w.map_view.mode == type::migration || ws.w.map_view.mode == type::infrastructure
				|| ws.w.map_view.mode == type::prices || ws.w.map_view.mode == type::production || ws.w.map_view.mode == type::voting) {
				ws.w.province_w.hide_province_window(ws.w.gui_m);
			} else if(ws.w.map_view.mode == type::culture) {
				ws.w.map_view.legends->current_culture = cultures::culture_tag();
				ui::hide(*(ws.w.map_view.legends->generic_gradient_legend_window.associated_object));
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_legend_window.associated_object));
				ws.w.map_view.changed.store(true, std::memory_order_release);
			} else if(ws.w.map_view.mode == type::relations) {
				ws.w.map_view.legends->current_nation = ws.w.local_player_nation;
				ws.w.map_view.changed.store(true, std::memory_order_release);
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->relations_legend_window.associated_object));
			} else if(ws.w.map_view.mode == type::rgo) {
				ws.w.map_view.legends->current_good = economy::goods_tag();
				ws.w.map_view.changed.store(true, std::memory_order_release);
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->rgo_legend_window.associated_object));
			}
		}
	}

	void on_mouse_over(world_state& ws, provinces::province_tag p) {
		if(ws.w.map_view.mode == type::political || ws.w.map_view.mode == type::sphere || ws.w.map_view.mode == type::region) {
			if(ws.w.map_view.legends->current_province != p) {
				ws.w.map_view.legends->current_province = p;
				ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_legend_window.associated_object));
			}
		} else if(ws.w.map_view.mode == type::culture) {
			if(ws.w.map_view.legends->current_province != p) {
				ws.w.map_view.legends->current_province = p;

				if(ws.w.map_view.legends->current_culture)
					ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_gradient_legend_window.associated_object));
				else
					ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->generic_legend_window.associated_object));
			}
		} else if(ws.w.map_view.mode == type::population) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->population_legend_window.associated_object));
		} else if(ws.w.map_view.mode == type::relations) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->relations_legend_window.associated_object));
		} else if(ws.w.map_view.mode == type::migration) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->migration_legend_window.associated_object));
		} else if(ws.w.map_view.mode == type::infrastructure) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->infrastructure_legend_window.associated_object));
		} else if(ws.w.map_view.mode == type::rgo) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->rgo_legend_window.associated_object));
		} else if(ws.w.map_view.mode == type::prices || ws.w.map_view.mode == type::production) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->resource_legend_window.associated_object));
		} else if(ws.w.map_view.mode == type::voting) {
			ws.w.map_view.legends->current_province = p;
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->voting_legend_window.associated_object));
		}
	}

	void state::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_generic"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->generic_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_generic_gradient"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->generic_gradient_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_population"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->population_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_relations"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->relations_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_migration"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->migration_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_infrastructure"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->infrastructure_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_rgo"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->rgo_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_resource"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->resource_legend_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_legend_voting"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, ws.w.map_view.legends->voting_legend_window);

		{
			auto i_con = ws.w.gui_m.gui_objects.emplace();
			i_con.object.flags.store(ui::gui_object::enabled, std::memory_order_release);
			ui::add_to_back(ws.w.gui_m, ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, i_con);

			legends->infrastructure_map_container = &(i_con.object);
			legends->infrastructure_map_icons.resize(ws.s.province_m.first_sea_province);
			legends->infrastructure_icons_windows.resize(ws.s.province_m.first_sea_province);

			auto const win_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_icon_infra"]);

			for(int32_t i = 0; i < ws.s.province_m.first_sea_province; ++i) {
				auto obj = ui::create_static_element(ws, win_tag,
					i_con, legends->infrastructure_icons_windows[i]);
				legends->infrastructure_icons_windows[i].tag = provinces::province_tag(provinces::province_tag::value_base_t(i));
				legends->infrastructure_map_icons[i] = &(obj.object);
			}
		}
		{
			auto i_con = ws.w.gui_m.gui_objects.emplace();
			i_con.object.flags.store(ui::gui_object::enabled, std::memory_order_release);
			ui::add_to_back(ws.w.gui_m, ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, i_con);

			legends->rgo_map_container = &(i_con.object);
			legends->rgo_map_icons.resize(ws.s.province_m.first_sea_province);
			legends->rgo_map_icons_objects.resize(ws.s.province_m.first_sea_province);

			auto const ico_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_map_icon_resource"]);

			for(int32_t i = 0; i < ws.s.province_m.first_sea_province; ++i) {
				auto obj = ui::create_static_element(ws, ico_tag,
					i_con, legends->rgo_map_icons_objects[i]);
				legends->rgo_map_icons_objects[i].tag = provinces::province_tag(provinces::province_tag::value_base_t(i));
				legends->rgo_map_icons[i] = &(obj.object);
			}
		}
	}
	void generic_legend_title::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(ws.w.map_view.mode == type::political) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_political, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::culture) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_culture , fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::region) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_region, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::sphere) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_sphere, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::population) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_population, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::relations) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_relations, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::migration) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_migration, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::infrastructure) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_infrastructure, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::rgo) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_rgo, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::prices) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_prices, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::production) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_production, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == type::voting) {
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_legend_voting, fmt, ws, box, lm);
		}
	}
	void generic_legend_contents::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(ws.w.map_view.mode == type::political) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
				auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(p);
				if(owner) {
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
					cursor = ui::add_text(cursor, scenario::fixed_ui::owner_label, fmt, ws, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					cursor = ui::add_text(cursor, ws.w.nation_s.nations.get<nation::name>(owner), fmt, ws, box, lm);
				}
			}
		} else if(ws.w.map_view.mode == type::culture) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);

				auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
				if(total_pop > 0) {
					if(auto const c = ws.w.map_view.legends->current_culture; c) {
						auto culture_pop = ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, c));
						cursor = ui::add_text(cursor, text_data::percent{ culture_pop / total_pop }, fmt, ws, box, lm);
						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
						cursor = ui::add_text(cursor, ws.s.culture_m.culture_container[c].name, fmt, ws, box, lm);
					} else {
						auto dominant_culture = ws.w.province_s.province_state_container.get<province_state::dominant_culture>(p);
						if(dominant_culture) {
							auto culture_pop = ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, dominant_culture));
							cursor = ui::add_text(cursor, text_data::percent{ culture_pop / total_pop }, fmt, ws, box, lm);
							cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
							cursor = ui::add_text(cursor, ws.s.culture_m.culture_container[dominant_culture].name, fmt, ws, box, lm);
						}
					}
				}
			}
		} else if(ws.w.map_view.mode == type::region) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
				auto rgn = ws.s.province_m.province_container.get<province::state_id>(p);
				if(rgn) {
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
					cursor = ui::add_text(cursor, scenario::fixed_ui::region_label, fmt, ws, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					cursor = ui::add_text(cursor, ws.s.province_m.state_names[rgn], fmt, ws, box, lm);
				}
			}
		} else if(ws.w.map_view.mode == type::sphere) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(p);
				if(owner) {
					auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(owner), fmt, ws, box, lm);
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
					cursor = ui::add_text(cursor, scenario::fixed_ui::sphere_label, fmt, ws, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);

					if(auto const sl = ws.w.nation_s.nations.get<nation::sphere_leader>(owner); sl)
						cursor = ui::add_text(cursor, ws.w.nation_s.nations.get<nation::name>(sl), fmt, ws, box, lm);
					else
						cursor = ui::add_text(cursor, scenario::fixed_ui::none, fmt, ws, box, lm);
				}
			}
		} else if(ws.w.map_view.mode == type::population) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
		
				auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
				cursor = ui::add_text(cursor, scenario::fixed_ui::population_label, fmt, ws, box, lm);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
				cursor = ui::add_text(cursor, text_data::integer{total_pop}, fmt, ws, box, lm);
			}
		} else if(ws.w.map_view.mode == type::relations) {
			auto const rel_with = ws.w.map_view.legends->current_nation;
			auto const owner = ws.w.province_s.province_state_container.get<province_state::owner>(ws.w.map_view.legends->current_province);
			if(rel_with && owner) {
				text_data::text_replacement repl[] = {
					text_data::text_replacement(text_data::value_type::nation, ws.w.nation_s.nations.get<nation::name>(rel_with)),
					text_data::text_replacement(text_data::value_type::target, ws.w.nation_s.nations.get<nation::name>(owner)),
					text_data::text_replacement(text_data::value_type::value, text_data::integer{ nations::get_relationship(ws, rel_with, owner)})
				};
				ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::map_relation_explanation, fmt, ws, box, lm, repl, 3);
			}
		} else if(ws.w.map_view.mode == type::migration) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				if(to_index(p) < ws.s.province_m.first_sea_province) {
					if(ws.w.map_view.legends->showing_internal_migration) {
						auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
						auto amount = ws.w.province_s.province_state_container.get<province_state::old_migration_growth>(p);

						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
						cursor = ui::add_text(cursor,
							scenario::fixed_ui::migration_internal_label,
							fmt, ws, box, lm);
						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
						cursor = ui::add_text(cursor, text_data::integer{ int32_t(amount) }, fmt, ws, box, lm);
					} else if(auto const owner = provinces::province_owner(ws, p); owner) {
						auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(owner), fmt, ws, box, lm);

						float amount = 0.0f;
						
						auto provs = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(owner));
						for(auto ip : provs)
							amount += ws.w.province_s.province_state_container.get<province_state::old_immigration_growth>(ip);
						
						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
						cursor = ui::add_text(cursor,
							scenario::fixed_ui::migration_external_label,
							fmt, ws, box, lm);
						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
						cursor = ui::add_text(cursor, text_data::integer{ int32_t(amount) }, fmt, ws, box, lm);
					}
				}
			}
		} else if(ws.w.map_view.mode == type::infrastructure || ws.w.map_view.mode == type::rgo) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
			}
		} else if(ws.w.map_view.mode == type::prices) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto const g = ws.w.map_view.legends->current_good;
				if(auto const sid = provinces::province_state(ws, p); g && bool(sid) && bool(ws.w.nation_s.states.get<::state::owner>(sid))) {
					auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.states.get<::state::name>(sid), fmt, ws, box, lm);
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
					cursor = ui::add_text(cursor,
						scenario::fixed_ui::price_label,
						fmt, ws, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					cursor = ui::add_text(cursor, text_data::currency{ economy::state_current_prices(ws, sid)[g] }, fmt, ws, box, lm);
				}
			}
		} else if(ws.w.map_view.mode == type::production) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto const g = ws.w.map_view.legends->current_good;
				if(auto const sid = provinces::province_state(ws, p); g && bool(sid) && bool(ws.w.nation_s.states.get<::state::owner>(sid))) {
					auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.states.get<::state::name>(sid), fmt, ws, box, lm);
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
					cursor = ui::add_text(cursor,
						scenario::fixed_ui::production_label,
						fmt, ws, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					cursor = ui::add_text(cursor, text_data::fp_two_places{ economy::state_current_production(ws, sid, g) }, fmt, ws, box, lm);
				}
			}
		} else if(ws.w.map_view.mode == type::voting) {
			if(auto const p = ws.w.map_view.legends->current_province; is_valid_index(p) && p != provinces::province_tag(0)) {
				auto cursor = ui::add_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container.get<province_state::name>(p), fmt, ws, box, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);

				auto const ide = ws.w.map_view.legends->current_ideology;
				auto const dide = ide ? ide : ws.w.province_s.province_state_container.get<province_state::dominant_ideology>(p);

				if(dide) {
					auto const support = ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, dide));
					auto const total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(p);

					if(total_pop > 0) {
						cursor = ui::add_text(cursor, text_data::percent{ support / total_pop }, fmt, ws, box, lm);
						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
						cursor = ui::add_text(cursor, ws.s.ideologies_m.ideology_container[dide].name, fmt, ws, box, lm);
					}
				}
			}
		}
	}
	void gradient_bar::update(ui::tinted_icon<gradient_bar>& self, world_state & ws) {
		if(ws.w.map_view.mode == map_mode::type::culture) {
			if(auto const c = ws.w.map_view.legends->current_culture; c) {
				auto const col = ws.s.culture_m.culture_container[c].color;
				self.set_color(ws.w.gui_m, float(col.r) / 255.0f, float(col.g) / 255.0f, float(col.b) / 255.0f);
			}
		} else if(ws.w.map_view.mode == map_mode::type::population) {
			self.set_color(ws.w.gui_m, 0.1f, 1.0f, 0.1f);
		} else if(ws.w.map_view.mode == map_mode::type::rgo) {
			auto const cg = ws.w.map_view.legends->current_good;
			auto clr = cg ? ws.s.economy_m.goods[cg].color : graphics::color_rgb{ 255ui8, 255ui8, 255ui8 };
			self.set_color(ws.w.gui_m, float(clr.r) / 255.0f, float(clr.g) / 255.0f, float(clr.b) / 255.0f);
		}
	}
	void generic_legend_min::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(ws.w.map_view.mode == map_mode::type::culture) {
			ui::add_text(ui::xy_pair{ 0,0 }, u"0%", fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::population) {
			auto const range = ws.w.map_view.legends->showing_density == false ?
				population::population_amount_range(ws) : population::population_density_range(ws);
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{range.min}, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::relations) {
			ui::add_text(ui::xy_pair{ 0,0 }, u"-200", fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::migration) {
			//auto const range = ws.w.map_view.legends->showing_internal_migration ?
			//	population::internal_migration_range(ws) : population::external_migration_range(ws);
			//ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ range.min }, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::infrastructure) {
			ui::add_text(ui::xy_pair{ 0,0 }, u"0", fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::prices) {
			if(auto g = ws.w.map_view.legends->current_good; g) {
				auto range = economy::global_price_range(ws, g);
				ui::add_text(ui::xy_pair{ 0,0 }, text_data::currency{ range.minimum }, fmt, ws, box, lm);
			}
		} else if(ws.w.map_view.mode == map_mode::type::production) {
			if(auto g = ws.w.map_view.legends->current_good; g) {
				auto range = economy::global_production_range(ws, g);
				ui::add_text(ui::xy_pair{ 0,0 }, text_data::fp_two_places{ range.minimum }, fmt, ws, box, lm);
			}
		}
	}
	void generic_legend_max::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(ws.w.map_view.mode == map_mode::type::culture) {
			ui::add_text(ui::xy_pair{ 0,0 }, u"100%", fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::population) {
			auto const range = ws.w.map_view.legends->showing_density == false ?
				population::population_amount_range(ws) : population::population_density_range(ws);
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ range.max }, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::relations) {
			ui::add_text(ui::xy_pair{ 0,0 }, u"+200", fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::migration) {
			//auto const range = ws.w.map_view.legends->showing_internal_migration ?
			//	population::internal_migration_range(ws) : population::external_migration_range(ws);
			//ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ range.max }, fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::infrastructure) {
			ui::add_text(ui::xy_pair{ 0,0 }, u"5", fmt, ws, box, lm);
		} else if(ws.w.map_view.mode == map_mode::type::prices) {
			if(auto g = ws.w.map_view.legends->current_good; g) {
				auto range = economy::global_price_range(ws, g);
				ui::add_text(ui::xy_pair{ 0,0 }, text_data::currency{ range.maximum }, fmt, ws, box, lm);
			}
		} else if(ws.w.map_view.mode == map_mode::type::production) {
			if(auto g = ws.w.map_view.legends->current_good; g) {
				auto range = economy::global_production_range(ws, g);
				ui::add_text(ui::xy_pair{ 0,0 }, text_data::fp_two_places{ range.maximum }, fmt, ws, box, lm);
			}
		}
	}
	void population_amount_button::update(ui::simple_button<population_amount_button>& self, world_state & ws) {
		if(ws.w.map_view.legends->showing_density)
			self.set_frame(ws.w.gui_m, 0);
		else
			self.set_frame(ws.w.gui_m, 1);
	}
	void population_amount_button::button_function(ui::simple_button<population_amount_button>& self, world_state & ws) {
		ws.w.map_view.legends->showing_density = false;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->population_legend_window.associated_object));
	}
	void population_density_button::update(ui::simple_button<population_density_button>& self, world_state & ws) {
		if(ws.w.map_view.legends->showing_density)
			self.set_frame(ws.w.gui_m, 1);
		else
			self.set_frame(ws.w.gui_m, 0);
	}
	void population_density_button::button_function(ui::simple_button<population_density_button>& self, world_state & ws) {
		ws.w.map_view.legends->showing_density = true;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->population_legend_window.associated_object));
	}
	void external_button::update(ui::simple_button<external_button>& self, world_state & ws) {
		if(ws.w.map_view.legends->showing_internal_migration)
			self.set_frame(ws.w.gui_m, 0);
		else
			self.set_frame(ws.w.gui_m, 1);
	}
	void external_button::button_function(ui::simple_button<external_button>& self, world_state & ws) {
		ws.w.map_view.legends->showing_internal_migration = false;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->migration_legend_window.associated_object));
	}
	void internal_button::update(ui::simple_button<internal_button>& self, world_state & ws) {
		if(ws.w.map_view.legends->showing_internal_migration)
			self.set_frame(ws.w.gui_m, 1);
		else
			self.set_frame(ws.w.gui_m, 0);
	}
	void internal_button::button_function(ui::simple_button<internal_button>& self, world_state & ws) {
		ws.w.map_view.legends->showing_internal_migration = true;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->migration_legend_window.associated_object));
	}
	void railroad_color::update(ui::tinted_icon<railroad_color>& self, world_state & ws) {
		self.set_color(ws.w.gui_m, 72.0f / 255.0f, 216.0f / 255.0f, 65.0f / 255.0f);
	}
	void naval_base_color::update(ui::tinted_icon<naval_base_color>& self, world_state & ws) {
		self.set_color(ws.w.gui_m, 65.0f / 255.0f, 115.0f / 255.0f, 211.0f / 255.0f);
	}
	ui::window_tag rgo_items_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_rgo_legend_list_item"]);
	}
	void rgo_item_button::button_function(ui::simple_button<rgo_item_button>& self, world_state & ws) {
		ws.w.map_view.legends->current_good = tag;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->rgo_legend_window.associated_object));
	}
	void rgo_map_icon::update(ui::dynamic_icon<rgo_map_icon>& self, world_state & ws) {
		auto prgo = ws.w.province_s.province_state_container.get<province_state::rgo_production>(tag);
		if(prgo)
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[prgo].icon);
		else
			self.set_frame(ws.w.gui_m, 0);
	}
	void resource_item_button::button_function(ui::simple_button<resource_item_button>& self, world_state & ws) {
		ws.w.map_view.legends->current_good = tag;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->resource_legend_window.associated_object));
	}
	ui::window_tag resource_items_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_resource_legend_list_item"]);
	}
	ui::window_tag ideology_items_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_voting_legend_list_item"]);
	}
	void ideology_item_button::button_function(ui::simple_button<ideology_item_button>& self, world_state & ws) {
		ws.w.map_view.legends->current_ideology = tag;
		ws.w.map_view.changed.store(true, std::memory_order_release);
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.map_view.legends->voting_legend_window.associated_object));
	}
}
