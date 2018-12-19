#include "triggers.h"
#include "codes.h"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"
#include "military\\military_functions.hpp"
#include "population\\population_functions.hpp"
#include "ideologies\\ideologies_functions.h"
#include "issues\\issues_functions.h"
#include "provinces\\province_functions.h"
#include "modifiers\\modifier_functions.h"
#include "economy\\economy_functions.h"
#include "concurrency_tools\\ve.h"

namespace triggers {
	int32_t get_trigger_payload_size(const uint16_t* data) {
		const auto payload_mask = uint16_t(-int16_t((trigger_codes::no_payload & data[0]) == 0));
		return payload_mask & data[1];
	}
	int32_t trigger_scope_data_payload(uint16_t code) {
		const auto masked_code = code & trigger_codes::code_mask;
		if ((masked_code == trigger_codes::x_provinces_in_variable_region) |
			(masked_code == trigger_codes::tag_scope) |
			(masked_code == trigger_codes::integer_scope))
			return 1;
		return 0;
	}
	void add_float_to_payload(std::vector<uint16_t>& v, float f) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			float f;
		} pack_float;
		pack_float.f = f;

		v.push_back(pack_float.v.low);
		v.push_back(pack_float.v.high);
	}
	float read_float_from_payload(const uint16_t* data) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			float f;
		} pack_float;

		pack_float.v.low = data[0];
		pack_float.v.high = data[1];

		return pack_float.f;
	}

	void add_int32_t_to_payload(std::vector<uint16_t>& v, int32_t i) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			int32_t i;
		} pack_int;
		pack_int.i = i;

		v.push_back(pack_int.v.low);
		v.push_back(pack_int.v.high);
	}
	int32_t read_int32_t_from_payload(const uint16_t* data) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			int32_t i;
		} pack_int;
		
		pack_int.v.low = data[0];
		pack_int.v.high = data[1];

		return pack_int.i;
	}

	namespace {


#define TRIGGER_PARAMTERS uint16_t const* tval, world_state const& ws, const_parameter primary_slot, const_parameter this_slot, const_parameter from_slot

		struct single_type {
			using return_type = bool;
			using parameter_type = const_parameter;
		};
		struct contiguous_type {
			using return_type = ve::mask_vector;
			using parameter_type = ve::contiguous_tags<union_tag>;
		};
		struct gathered_type {
			using return_type = ve::mask_vector;
			using parameter_type = ve::int_vector;
		};

		template<typename T>
		struct value_to_type_s;

		template<>
		struct value_to_type_s<ve::contiguous_tags<union_tag>> {
			using type = contiguous_type;
		};

		template<>
		struct value_to_type_s<const_parameter> {
			using type = single_type;
		};
		template<>
		struct value_to_type_s<provinces::province_tag> {
			using type = single_type;
		};
		template<>
		struct value_to_type_s<nations::country_tag> {
			using type = single_type;
		};
		template<>
		struct value_to_type_s<nations::state_tag> {
			using type = single_type;
		};
		template<>
		struct value_to_type_s<population::pop_tag> {
			using type = single_type;
		};
		template<>
		struct value_to_type_s<population::rebel_faction_tag> {
			using type = single_type;
		};
		template<typename T>
		struct value_to_type_s<expanded_tag<T>> : public value_to_type_s<T> {};

		template<>
		struct value_to_type_s<ve::int_vector> {
			using type = gathered_type;
		};
		template<typename T>
		struct value_to_type_s<ve::tagged_vector<T>> {
			using type = gathered_type;
		};
		template<>
		struct value_to_type_s<ve::union_tag_vector> {
			using type = gathered_type;
		};

		template<typename T>
		using value_to_type = typename value_to_type_s<T>::type;

#define TRIGGER_FUNCTION(function_name) template<typename primary_type, typename this_type, typename from_type> \
	auto __vectorcall function_name(uint16_t const* tval, world_state const& ws, typename primary_type::parameter_type primary_slot, \
	typename this_type::parameter_type this_slot, typename from_type::parameter_type from_slot) -> typename primary_type::return_type


		__forceinline nations::country_tag to_nation(const_parameter v) { return v; }
		__forceinline nations::state_tag to_state(const_parameter v) { return v; }
		__forceinline provinces::province_tag to_province(const_parameter v) { return v; }
		__forceinline population::pop_tag to_pop(const_parameter v) { return v; }
		__forceinline population::rebel_faction_tag to_rebel(const_parameter v) { return v; }

		__forceinline ve::contiguous_tags<nations::country_tag> to_nation(ve::contiguous_tags<union_tag> v) {
			return ve::contiguous_tags<nations::country_tag>(v.value);
		}
		__forceinline ve::contiguous_tags<nations::state_tag> to_state(ve::contiguous_tags<union_tag> v) {
			return ve::contiguous_tags<nations::state_tag>(v.value);
		}
		__forceinline ve::contiguous_tags<provinces::province_tag> to_province(ve::contiguous_tags<union_tag> v) {
			return ve::contiguous_tags<provinces::province_tag>(v.value);
		}
		__forceinline ve::contiguous_tags<population::pop_tag> to_pop(ve::contiguous_tags<union_tag> v) {
			return ve::contiguous_tags<population::pop_tag>(v.value);
		}
		__forceinline ve::contiguous_tags<population::rebel_faction_tag> to_rebel(ve::contiguous_tags<union_tag> v) {
			return ve::contiguous_tags<population::rebel_faction_tag>(v.value);
		}

		__forceinline ve::tagged_vector<nations::country_tag> to_nation(ve::union_tag_vector v) { return v; }
		__forceinline ve::tagged_vector<nations::state_tag> to_state(ve::union_tag_vector v) { return v; }
		__forceinline ve::tagged_vector<provinces::province_tag> to_province(ve::union_tag_vector v) { return v; }
		__forceinline ve::tagged_vector<population::pop_tag> to_pop(ve::union_tag_vector v) { return v; }
		__forceinline ve::tagged_vector<population::rebel_faction_tag> to_rebel(ve::union_tag_vector v) { return v; }

		template<typename index, typename container, typename T>
		__forceinline auto to_value(container const& c, T v) {
			return ve::load(v, c.template get_row<index>());
		}
		template<int32_t index, typename container, typename T>
		__forceinline auto to_indexed_value(container const& c, T v) {
			return ve::load(v, c.template get_row<index>(0));
		}
		template<typename container, typename T, typename ITYPE>
		__forceinline auto to_vindexed_value(container const& c, T v, ITYPE i) {
			return ve::load(v, c.template get_row(i, 0));
		}

	TRIGGER_FUNCTION(test_trigger_generic);

	TRIGGER_FUNCTION(apply_disjuctively) {
		const auto source_size = 1 + get_trigger_payload_size(tval);
		auto sub_units_start = tval + 2 + trigger_scope_data_payload(tval[0]);

		typename primary_type::return_type result;
		while(sub_units_start < tval + source_size) {
			result = result | test_trigger_generic<primary_type, this_type, from_type>(sub_units_start, ws, primary_slot, this_slot, from_slot);

			auto compressed_res = ve::compress_mask(result);
			if(compressed_res == primary_type::full_mask)
				return result;

			sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
		}
		return result;
	}

	TRIGGER_FUNCTION(apply_conjuctively) {
		const auto source_size = 1 + get_trigger_payload_size(tval);
		auto sub_units_start = tval + 2 + trigger_scope_data_payload(tval[0]);

		typename primary_type::return_type result = !(typename primary_type::return_type());
		while(sub_units_start < tval + source_size) {
			result = result & test_trigger_generic<primary_type, this_type, from_type>(sub_units_start, ws, primary_slot, this_slot, from_slot);
			
			auto compressed_res = ve::compress_mask(result);
			if(compressed_res == primary_type::rempty_mask)
				return result;

			sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
		}
		return result;
	}

	TRIGGER_FUNCTION(apply_subtriggers) {
		if((*tval & trigger_codes::is_disjunctive_scope) != 0)
			return apply_disjuctively(tval, ws, primary_slot, this_slot, from_slot);
		else
			return apply_conjuctively(tval, ws, primary_slot, this_slot, from_slot);
	}

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

	TRIGGER_FUNCTION(tf_none) {
		return !(typename primary_type::return_type());
	}

	TRIGGER_FUNCTION(tf_generic_scope) {
		return apply_subtriggers<primary_type, this_type, from_type>(tval, ws, primary_slot, this_slot, from_slot);
	}

	auto existance_accumulator(world_state const& ws, uint16_t const* tval, const_parameter this_slot, const_parameter from_slot) {
		return ve::make_true_accumulator([&ws, tval, this_slot, from_slot](ve::int_vector v) {
			return apply_subtriggers<gathered_type, single_type, single_type>(tval, ws, v.value, this_slot, from_slot);
		});
	}

	auto universal_accumulator(world_state const& ws, uint16_t const* tval, const_parameter this_slot, const_parameter from_slot) {
		return ve::make_false_accumulator([&ws, tval, this_slot, from_slot](ve::int_vector v) {
			return apply_subtriggers<gathered_type, single_type, single_type>(tval, ws, v.value, this_slot, from_slot);
		});
	}

	TRIGGER_FUNCTION(tf_x_neighbor_province_scope) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto prov_id = to_province(p_slot);

			if(!ws.w.province_s.province_state_container.is_valid_index(prov_id))
				return false;
			
			auto p_same_range = ws.s.province_m.same_type_adjacency.get_row(prov_id);
			auto p_diff_range = ws.s.province_m.coastal_adjacency.get_row(prov_id);

			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p = p_same_range.first; !accumulator.result && p != p_same_range.second; ++p)
					accumulator.add_value(p->value);
				for(auto p = p_diff_range.first; !accumulator.result && p != p_diff_range.second; ++p)
					accumulator.add_value(p->value);
				accumulator.flush();

				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto p = p_same_range.first; accumulator.result && p != p_same_range.second; ++p)
					accumulator.add_value(p->value);
				for(auto p = p_diff_range.first; accumulator.result && p != p_diff_range.second; ++p)
					accumulator.add_value(p->value);
				accumulator.flush();

				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_neighbor_country_scope_nation) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);

			if(!ws.w.nation_s.nations.is_valid_index(nid))
				return false;

			auto n_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(nid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);
				for(auto n : n_range) {
					accumulator.add_value(n.value);
					if(accumulator.result)
						return true;
				}
				accumulator.flush();

				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);
				for(auto n : n_range) {
					accumulator.add_value(n.value);
					if(!accumulator.result)
						return false;
				}
				accumulator.flush();

				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_neighbor_country_scope_pop) {
		auto pop_province = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto province_owner = to_value<province::owner>(ws.w.province_s.province_state_container, pop_province);

		return tf_x_neighbor_country_scope_nation<value_to_type<decltype(province_owner)>, this_type, from_type>(tval, ws, province_owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_x_war_countries_scope_nation) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);

			if(!ws.w.nation_s.nations.is_valid_index(nid))
				return false;

			auto opposing_countries = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(nid));

			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto n : opposing_countries) {
					accumulator.add_value(n.value);
					if(accumulator.result)
						return true;
				}
				accumulator.flush();

				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto n : opposing_countries) {
					accumulator.add_value(n.value);
					if(!accumulator.result)
						return false;
				}
				accumulator.flush();

				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_war_countries_scope_pop) {
		auto pop_province = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto province_owner = to_value<province::owner>(ws.w.province_s.province_state_container, pop_province);

		return tf_x_war_countries_scope_nation<value_to_type<decltype(province_owner)>, this_type, from_type>(tval, ws, province_owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_x_greater_power_scope) {
		return ve::apply(this_slot, from_slot, [&ws, tval](const_parameter t_slot, const_parameter f_slot) {
			auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
			int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);

			if(*tval & trigger_codes::is_existance_scope) {
				int32_t count = 0;
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
					if(is_valid_index(*n)) {
						if(nations::is_great_power(ws, *n)) {
							++count;

							accumulator.add_value(n->value);
							if(accumulator.result)
								return true;
						}
					}
				}

				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				int32_t count = 0;
				for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
					if(is_valid_index(*n)) {
						if(nations::is_great_power(ws, *n)) {
							++count;

							accumulator.add_value(n->value);
							if(!accumulator.result)
								return false;
						}
					}
				}

				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_owned_province_scope_state) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto sid = to_state(p_slot.state);
			if(!ws.w.nation_s.states.is_valid_index(sid))
				return false;

			auto region_id = ws.w.nation_s.states.get<state::region_id>(sid);
			if(!is_valid_index(region_id))
				return false;

			auto region_provinces = ws.s.province_m.states_to_province_index.get_row(region_id);
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : region_provinces) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == to_state(primary_slot)) {
						accumulator.add_value(p.value);
						if(accumulator.result)
							return true;
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : region_provinces) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == to_state(primary_slot)) {
						accumulator.add_value(p.value);
						if(!accumulator.result)
							return false;
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_owned_province_scope_nation) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot.nation);

			if(!ws.w.nation_s.nations.is_valid_index(nid))
				return false;

			auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(nid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : owned_range) {
					accumulator.add_value(p.value);
					if(accumulator.result)
						return true;
				}

				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : owned_range) {
					accumulator.add_value(p.value);
					if(!accumulator.result)
						return false;
				}

				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_core_scope_province) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto pid = to_province(p_slot);
			if(!ws.w.province_s.province_state_container.is_valid_index(pid))
				return false;

			auto core_range = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : core_range) {
					if(is_valid_index(p)) {
						auto core_holder = ws.w.culture_s.tags_to_holders[p];
						accumulator.add_value(core_holder.value);
						if(accumulator.result)
							return true;
					}
				}

				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : core_range) {
					if(is_valid_index(p)) {
						auto core_holder = ws.w.culture_s.tags_to_holders[p];
						accumulator.add_value(core_holder.value);
						if(!accumulator.result)
							return false;
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_core_scope_nation) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);
			if(!ws.w.nation_s.nations.is_valid_index(nid))
				return false;

			auto tag = ws.w.nation_s.nations.get<nation::tag>(nid);
			if(!is_valid_index(tag))
				return false;

			auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : core_range) {
					accumulator.add_value(p.value);
					if(accumulator.result)
						return true;
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : core_range) {
					accumulator.add_value(p.value);
					if(!accumulator.result)
						return false;
				}

				accumulator.flush();
				return accumulator.result;
			}
		});
	}

	TRIGGER_FUNCTION(tf_x_state_scope) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);
			if(!ws.w.nation_s.states.is_valid_index(nid))
				return false;

			auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(nid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto s = states.first; s != states.second; ++s) {
					accumulator.add_value(s->state.value);
					if(accumulator.result)
						return true;
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto s = states.first; s != states.second; ++s) {
					accumulator.add_value(s->state.value);
					if(!accumulator.result)
						return false;
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_substate_scope) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);
			if(!ws.w.nation_s.states.is_valid_index(nid))
				return false;

			auto nation_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(nid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto i = nation_range.first; i != nation_range.second; ++i) {
					if(ws.w.nation_s.nations.get<nation::is_substate>(*i)) {
						accumulator.add_value(i->value);
						if(accumulator.result)
							return true;
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto i = nation_range.first; i != nation_range.second; ++i) {
					if(ws.w.nation_s.nations.get<nation::is_substate>(*i)) {
						accumulator.add_value(i->value);
						if(!accumulator.result)
							return false;
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_sphere_member_scope) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);
			if(!ws.w.nation_s.states.is_valid_index(nid))
				return false;

			auto nation_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(nid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto i = nation_range.first; i != nation_range.second; ++i) {
					accumulator.add_value(i->value);
					if(accumulator.result)
						return true;
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto i = nation_range.first; i != nation_range.second; ++i) {
					accumulator.add_value(i->value);
					if(!accumulator.result)
						return false;
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_pop_scope_province) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto pid = to_province(p_slot);
			if(!ws.w.province_s.province_state_container.is_valid_index(pid))
				return false;

			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(pid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto i : pop_range) {
					accumulator.add_value(i.value);
					if(accumulator.result)
						return true;
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto i : pop_range) {
					accumulator.add_value(i.value);
					if(!accumulator.result)
						return false;
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_pop_scope_state) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto sid = to_state(p_slot);
			if(!ws.w.nation_s.states.is_valid_index(sid))
				return false;

			auto region_id = ws.w.nation_s.states.get<state::region_id>(sid);
			if(!is_valid_index(region_id))
				return false;

			auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto j = province_range.first; j != province_range.second; ++j) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(*j) == to_state(primary_slot)) {
						auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(*j));
						for(auto i : pop_range) {
							accumulator.add_value(i.value);
							if(accumulator.result)
								return true;
						}
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto j = province_range.first; j != province_range.second; ++j) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(*j) == to_state(primary_slot)) {
						auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(*j));
						for(auto i : pop_range) {
							accumulator.add_value(i.value);
							if(!accumulator.result)
								return false;
						}
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_pop_scope_nation) {
		return ve::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = to_nation(p_slot);
			if(!ws.w.nation_s.nations.is_valid_index(nid))
				return false;

			auto province_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(nid));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto j = province_range.first; j != province_range.second; ++j) {
					if(is_valid_index(*j)) {
						auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(*j));
						for(auto i : pop_range) {
							accumulator.add_value(i.value);
							if(accumulator.result)
								return true;
						}
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto j = province_range.first; j != province_range.second; ++j) {
					if(is_valid_index(*j)) {
						auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(*j));
						for(auto i : pop_range) {
							accumulator.add_value(i.value);
							if(!accumulator.result)
								return false;
						}
					}
				}
				
				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_x_provinces_in_variable_region) {
		auto region = trigger_payload(*(tval + 2)).state;
		auto provinces = ws.s.province_m.states_to_province_index.get_row(region);

		return ve::apply(this_slot, from_slot, [&ws, tval, provinces](const_parameter t_slot, const_parameter f_slot) {
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto i = provinces.first; i != provinces.second; ++i) {
					accumulator.add_value(i->value);
					if(accumulator.result)
						return true;
				}
				
				accumulator.flush();
				return accumulator.result;
			} else {
				auto accumulator = universal_accumulator(ws, tval, t_slot, f_slot);

				for(auto i = provinces.first; i != provinces.second; ++i) {
					accumulator.add_value(i->value);
					if(!accumulator.result)
						return false;
				}

				accumulator.flush();
				return accumulator.result;
			}
		});
	}
	TRIGGER_FUNCTION(tf_owner_scope_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(primary_slot));
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_owner_scope_province) {
		auto owner = to_value<province_state::owner>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_controller_scope) {
		auto owner = to_value<province_state::controller>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_location_scope) {
		auto location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		return is_non_zero(location) & apply_subtriggers<value_to_type<decltype(location)>, this_type, from_type>(tval, ws, location, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(primary_slot));
		return is_non_zero(owner) &apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_pop) {
		auto location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto owner = to_value<province_state::owner>(ws.w.province_s.province_state_container, location);
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_capital_scope) {
		auto capital = to_value<nation::current_capital>(ws.w.nation_s.nations, to_nation(primary_slot));
		return is_non_zero(capital) & apply_subtriggers<value_to_type<decltype(capital)>, this_type, from_type>(tval, ws, capital, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_this_scope) {
		return apply_subtriggers<this_type, this_type, from_type>(tval, ws, this_slot, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_from_scope) {
		return apply_subtriggers<from_type, this_type, from_type>(tval, ws, from_slot, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_sea_zone_scope) {
		auto sea_zones = ve::apply(primary_slot, [&ws](const_parameter p_slot) {
			auto pid = to_province(p_slot);
			if(!ws.w.province_s.province_state_container.is_valid_index(pid))
				return provinces::province_tag();
			auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(pid);
			if(sea_zones.first != sea_zones.second)
				return apply_subtriggers<single_type, single_type, single_type>(tval, ws, *sea_zones.first, const_parameter(), const_parameter());
			return provinces::province_tag();
		});
		return is_valid_index(sea_zones) & apply_subtriggers<value_to_type<decltype(sea_zones)>, this_type, from_type>(tval, ws, sea_zones, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_cultural_union_scope) {
		auto cultures = to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto union_tags = ve::load(cultures, ws.s.culture_m.cultures_to_tags.view());
		auto group_holders = ve::load(union_tags, ws.w.culture_s.tags_to_holders.view());
		return is_valid_index(group_holders) & apply_subtriggers<value_to_type<decltype(group_holders)>, this_type, from_type>(tval, ws, group_holders, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_overlord_scope) {
		auto so = to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot));
		return is_valid_index(so) & apply_subtriggers<value_to_type<decltype(so)>, this_type, from_type>(tval, ws, so, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_sphere_owner_scope) {
		auto so = to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot));
		return is_valid_index(so) & apply_subtriggers<value_to_type<decltype(so)>, this_type, from_type>(tval, ws, so, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_independence_scope) {
		auto rtags = to_value<rebel_faction::independence_tag>(ws.w.population_s.rebel_factions, to_rebel(from_slot));
		auto inations = ve::load(rtags, ws.w.culture_s.tags_to_holders.view());
		return is_valid_index(inations) & apply_subtriggers<value_to_type<decltype(inations)>, this_type, from_type>(tval, ws, inations, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_flashpoint_tag_scope) {
		auto ctags = to_value<state::crisis_tag>(ws.w.nation_s.states, to_state(from_slot));
		auto fp_nations = ve::load(ctags, ws.w.culture_s.tags_to_holders.view());
		return is_valid_index(fp_nations) & apply_subtriggers<value_to_type<decltype(fp_nations)>, this_type, from_type>(tval, ws, fp_nations, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_crisis_state_scope) {
		auto cstate = ve::widen_to<decltype(primary_slot)>(ws.w.current_crisis.state);
		if(is_valid_index(ws.w.current_crisis.state))
			return apply_subtriggers<value_to_type<decltype(cstate)>, this_type, from_type>(tval, ws, cstate, this_slot, from_slot);
		return false;
	}
	TRIGGER_FUNCTION(tf_state_scope_province) {
		auto state_instance = to_value<province_state::state_instance>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		return is_valid_index(state_instance) & apply_subtriggers<value_to_type<decltype(state_instance)>, this_type, from_type>(tval, ws, state_instance, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_state_scope_pop) {
		auto location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto prov_state = to_value<province_state::state_instance>(ws.w.province_s.province_state_container, location);
		return is_valid_index(prov_state) & apply_subtriggers<value_to_type<decltype(prov_state)>, this_type, from_type>(tval, ws, prov_state, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_tag_scope) {
		auto tag = trigger_payload(tval[2]).tag;
		auto tag_holder = ws.w.culture_s.tags_to_holders[tag];
		if(tag_holder) {
			auto wtag_holder = ve::widen_to<decltype(primary_slot)>(tag_holder);
			return apply_subtriggers<value_to_type<decltype(wtag_holder)>, this_type, from_type>(tval, ws, wtag_holder, this_slot, from_slot);
		} else {
			return false;
		}
	}
	TRIGGER_FUNCTION(tf_integer_scope) {
		auto wprov = ve::widen_to<decltype(primary_slot)>(provinces::province_tag(tval[2]));
		return apply_subtriggers<value_to_type<decltype(wprov)>, this_type, from_type>(tval, ws, wprov, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_nation) {
		return apply_subtriggers<primary_type, this_type, from_type>(tval, ws, primary_slot, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_province) {
		auto owner = to_value<province_state::owner>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		return is_valid_index(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_cultural_union_scope_pop) {
		auto cultures = to_value<pop::culture>(ws.w.nation_s.nations, to_pop(primary_slot));
		auto union_tags = ve::load(cultures, ws.s.culture_m.cultures_to_tags.view());
		auto group_holders = ve::load(union_tags, ws.w.culture_s.tags_to_holders.view());

		return is_valid_index(group_holders) & apply_subtriggers<value_to_type<decltype(group_holders)>, this_type, from_type>(tval, ws, group_holders, this_slot, from_slot);
	}

	template<typename primary_type, typename this_type, typename from_type>
	struct scope_container {
		constexpr static typename primary_type::return_type (__vectorcall *scope_functions[])(uint16_t const*, world_state const&,
			typename primary_type::parameter_type, typename this_type::parameter_type, typename from_type::parameter_type) = {
			tf_generic_scope<primary_type, this_type, from_type>, //constexpr uint16_t generic_scope = 0x0000; // or & and
			tf_x_neighbor_province_scope<primary_type, this_type, from_type>, //constexpr uint16_t x_neighbor_province_scope = 0x0001;
			tf_x_neighbor_country_scope_nation<primary_type, this_type, from_type>, //constexpr uint16_t x_neighbor_country_scope_nation = 0x0002;
			tf_x_neighbor_country_scope_pop<primary_type, this_type, from_type>, //constexpr uint16_t x_neighbor_country_scope_pop = 0x0003;
			tf_x_war_countries_scope_nation<primary_type, this_type, from_type>, //constexpr uint16_t x_war_countries_scope_nation = 0x0004;
			tf_x_war_countries_scope_pop<primary_type, this_type, from_type>, //constexpr uint16_t x_war_countries_scope_pop = 0x0005;
			tf_x_greater_power_scope<primary_type, this_type, from_type>, //constexpr uint16_t x_greater_power_scope = 0x0006;
			tf_x_owned_province_scope_state<primary_type, this_type, from_type>, //constexpr uint16_t x_owned_province_scope_state = 0x0007;
			tf_x_owned_province_scope_nation<primary_type, this_type, from_type>, //constexpr uint16_t x_owned_province_scope_nation = 0x0008;
			tf_x_core_scope_province<primary_type, this_type, from_type>, //constexpr uint16_t x_core_scope_province = 0x0009;
			tf_x_core_scope_nation<primary_type, this_type, from_type>, //constexpr uint16_t x_core_scope_nation = 0x000A;
			tf_x_state_scope<primary_type, this_type, from_type>, //constexpr uint16_t x_state_scope = 0x000B;
			tf_x_substate_scope<primary_type, this_type, from_type>, //constexpr uint16_t x_substate_scope = 0x000C;
			tf_x_sphere_member_scope<primary_type, this_type, from_type>, //constexpr uint16_t x_sphere_member_scope = 0x000D;
			tf_x_pop_scope_province<primary_type, this_type, from_type>, //constexpr uint16_t x_pop_scope_province = 0x000E;
			tf_x_pop_scope_state<primary_type, this_type, from_type>, //constexpr uint16_t x_pop_scope_state = 0x000F;
			tf_x_pop_scope_nation<primary_type, this_type, from_type>, //constexpr uint16_t x_pop_scope_nation = 0x0010;
			tf_x_provinces_in_variable_region<primary_type, this_type, from_type>, //constexpr uint16_t x_provinces_in_variable_region = 0x0011; // variable name
			tf_owner_scope_state<primary_type, this_type, from_type>, //constexpr uint16_t owner_scope_state = 0x0012;
			tf_owner_scope_province<primary_type, this_type, from_type>, //constexpr uint16_t owner_scope_province = 0x0013;
			tf_controller_scope<primary_type, this_type, from_type>, //constexpr uint16_t controller_scope = 0x0014;
			tf_location_scope<primary_type, this_type, from_type>, //constexpr uint16_t location_scope = 0x0015;
			tf_country_scope_state<primary_type, this_type, from_type>, //constexpr uint16_t country_scope_state = 0x0016;
			tf_country_scope_pop<primary_type, this_type, from_type>, //constexpr uint16_t country_scope_pop = 0x0017;
			tf_capital_scope<primary_type, this_type, from_type>, //constexpr uint16_t capital_scope = 0x0018;
			tf_this_scope<primary_type, this_type, from_type>, //constexpr uint16_t this_scope_pop = 0x0019;
			tf_this_scope<primary_type, this_type, from_type>, //constexpr uint16_t this_scope_nation = 0x001A;
			tf_this_scope<primary_type, this_type, from_type>, //constexpr uint16_t this_scope_state = 0x001B;
			tf_this_scope<primary_type, this_type, from_type>, //constexpr uint16_t this_scope_province = 0x001C;
			tf_from_scope<primary_type, this_type, from_type>, //constexpr uint16_t from_scope_pop = 0x001D;
			tf_from_scope<primary_type, this_type, from_type>, //constexpr uint16_t from_scope_nation = 0x001E;
			tf_from_scope<primary_type, this_type, from_type>, //constexpr uint16_t from_scope_state = 0x001F;
			tf_from_scope<primary_type, this_type, from_type>, //constexpr uint16_t from_scope_province = 0x0020;
			tf_sea_zone_scope<primary_type, this_type, from_type>, //constexpr uint16_t sea_zone_scope = 0x0021;
			tf_cultural_union_scope<primary_type, this_type, from_type>, //constexpr uint16_t cultural_union_scope = 0x0022;
			tf_overlord_scope<primary_type, this_type, from_type>, //constexpr uint16_t overlord_scope = 0x0023;
			tf_sphere_owner_scope<primary_type, this_type, from_type>, //constexpr uint16_t sphere_owner_scope = 0x0024;
			tf_independence_scope<primary_type, this_type, from_type>, //constexpr uint16_t independence_scope = 0x0025;
			tf_flashpoint_tag_scope<primary_type, this_type, from_type>, //constexpr uint16_t flashpoint_tag_scope = 0x0026;
			tf_crisis_state_scope<primary_type, this_type, from_type>, //constexpr uint16_t crisis_state_scope = 0x0027;
			tf_state_scope_pop<primary_type, this_type, from_type>, //constexpr uint16_t state_scope_pop = 0x0028;
			tf_state_scope_province<primary_type, this_type, from_type>, //constexpr uint16_t state_scope_province = 0x0029;
			tf_tag_scope<primary_type, this_type, from_type>, //constexpr uint16_t tag_scope = 0x002A; // variable name
			tf_integer_scope<primary_type, this_type, from_type>, //constexpr uint16_t integer_scope = 0x002B; // variable name
			tf_country_scope_nation<primary_type, this_type, from_type>, //constexpr uint16_t country_scope_nation = 0x002C;
			tf_country_scope_province<primary_type, this_type, from_type>, //constexpr uint16_t country_scope_province = 0x002D;
			tf_cultural_union_scope_pop<primary_type, this_type, from_type>, //constexpr uint16_t cultural_union_scope_pop = 0x002E;
		};
	};

	template<typename A, typename B>
	[[nodiscard]] auto compare_values(uint16_t trigger_code, A value_a, B value_b) {
		switch(trigger_code & trigger_codes::association_mask) {
			case trigger_codes::association_eq:
				return value_a == value_b;
			case trigger_codes::association_gt:
				return value_a > value_b;
			case trigger_codes::association_lt:
				return value_a < value_b;
			case trigger_codes::association_le:
				return value_a <= value_b;
			case trigger_codes::association_ne:
				return value_a != value_b;
			case trigger_codes::association_ge:
				return value_a >= value_b;
			default:
				return value_a >= value_b;
		}
	}

	template<typename A, typename B>
	[[nodiscard]] auto compare_values_eq(uint16_t trigger_code, A value_a, B value_b) {
		switch(trigger_code & trigger_codes::association_mask) {
			case trigger_codes::association_eq:
				return value_a == value_b;
			case trigger_codes::association_gt:
				return value_a != value_b;
			case trigger_codes::association_lt:
				return value_a != value_b;
			case trigger_codes::association_le:
				return value_a == value_b;
			case trigger_codes::association_ne:
				return value_a != value_b;
			case trigger_codes::association_ge:
				return value_a == value_b;
			default:
				return value_a == value_b;
		}
	}

	template<typename A>
	[[nodiscard]] auto compare_to_true(uint16_t trigger_code, A value_a) {
		switch(trigger_code & trigger_codes::association_mask) {
			case trigger_codes::association_eq:
				return value_a;
			case trigger_codes::association_gt:
				return !value_a;
			case trigger_codes::association_lt:
				return !value_a;
			case trigger_codes::association_le:
				return value_a;
			case trigger_codes::association_ne:
				return !value_a;
			case trigger_codes::association_ge:
				return value_a;
			default:
				return value_a;
		}
	}
	template<typename A>
	[[nodiscard]] auto compare_to_false(uint16_t trigger_code, A value_a) {
		switch(trigger_code & trigger_codes::association_mask) {
			case trigger_codes::association_eq:
				return !value_a;
			case trigger_codes::association_gt:
				return value_a;
			case trigger_codes::association_lt:
				return value_a;
			case trigger_codes::association_le:
				return !value_a;
			case trigger_codes::association_ne:
				return value_a;
			case trigger_codes::association_ge:
				return !value_a;
			default:
				return !value_a;
		}
	}

	TRIGGER_FUNCTION(tf_year) {
		return compare_values(tval[0], ve::widen_to<decltype(primary_slot)>(int32_t(tag_to_date(ws.w.current_date).year())), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_month) {
		return compare_values(tval[0], ve::widen_to<decltype(primary_slot)>(int32_t(tag_to_date(ws.w.current_date).month())), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_port) {
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<province::is_coastal>(ws.s.province_m.province_container, to_prov(primary_slot))
				& !to_value<province::is_sea>(ws.s.province_m.province_container, to_prov(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_rank) {
		// note: comparison revesed since rank 1 is "greater" than rank 1 + N
		return compare_values(tval[0], int32_t(tval[2]), to_value<nation::overall_rank>(ws.w.nation_s.nations, to_nation(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_technology) {
		auto technology = trigger_payload(tval[2]).tech;
		return compare_to_true(tval[0], ve::widen_mask(to_vindexed_value(ws.w.nation_s.active_technologies, to_nation(primary_slot), technology)));
	}
	TRIGGER_FUNCTION(tf_strata_rich) {
		return compare_to_true(tval[0], ve::widen_mask(
			!to_value<pop::is_middle>(ws.w.population_s.pops, to_pop(primary_slot)) & !to_value<pop::is_poor>(ws.w.population_s.pops, to_pop(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_strata_middle) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<pop::is_middle>(ws.w.population_s.pops, to_pop(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_strata_poor) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<pop::is_poor>(ws.w.population_s.pops, to_pop(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_life_rating_province) {
		return compare_values(tval[0], provinces::get_life_rating(ws, to_prov(primary_slot)), float(trigger_payload(tval[2]).signed_value));
	}
	TRIGGER_FUNCTION(tf_life_rating_state) {
		auto state_caps = to_value<state::state_capital>(ws.w.nation_s.states, to_state(primary_slot));
		return compare_values(tval[0], provinces::get_life_rating(ws, state_caps), float(trigger_payload(tval[2]).signed_value));
	}

	auto empty_province_accumulator(world_state const& ws) {
		return ve::make_true_accumulator([&ws](ve::tagged_vector<provinces::province_tag> v) {
			auto owners = to_value<province_state::owner>(ws.w.province_s.province_state_container, v);
			return ve::is_valid_index(owners);
		});
	}

	TRIGGER_FUNCTION(tf_has_empty_adjacent_state_province) {
		auto results = ve::apply(primary_slot, [&ws](const_parameter p_slot) {
			auto acc = empty_province_accumulator(ws);

			auto adj_range = ws.s.province_m.same_type_adjacency.get_row(to_prov(p_slot));
			for(auto p : adj_range) {
				acc.add_value(p.value);
				if(acc.result)
					return true;
			}

			acc.flush();
			return acc.result;
		});
		
		return compare_to_true(tval[0], results);
	}
	TRIGGER_FUNCTION(tf_has_empty_adjacent_state_state) {
		auto results = ve::apply(primary_slot, [&ws](const_parameter p_slot) {
			auto acc = empty_province_accumulator(ws);

			auto state_id = to_state(p_slot);
			auto region_id = ws.w.nation_s.states.get<state::region_id>(state_id);

			if(!is_valid_index(region_id))
				return false;

			auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);

			for(auto p : province_range) {
				if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == state_id) {
					auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
					for(auto q : adj_range) {
						acc.add_value(q.value);
						if(acc.result)
							return true;

					}
				}
			}

			acc.flush();
			return acc.result;
		});

		return compare_to_true(tval[0], results);
	}
	TRIGGER_FUNCTION(tf_state_id_province) {
		provinces::province_tag pid(tval[2]);
		return compare_values_eq(tval[0],
			to_value<province::state_id>(ws.s.province_m.province_container, to_prov(primary_slot)),
			ws.s.province_m.province_container.get<province::state_id>(pid));
	}
	TRIGGER_FUNCTION(tf_state_id_state) {
		provinces::province_tag pid(tval[2]);
		return compare_values_eq(tval[0],
			to_value<state::region_id>(ws.w.nation_s.states, to_state(primary_slot)),
			ws.s.province_m.province_container.get<province::state_id>(pid));
	}
	TRIGGER_FUNCTION(tf_cash_reserves) {
		auto ratio = economy::money_qnty_type(read_float_from_payload(tval + 2));
		auto target = population::desired_needs_spending(ws, to_pop(primary_slot));
		auto money = to_value<pop::money>(ws.w.population_s.pops, to_pop(primary_slot));
		return compare_values(tval[0], ve::select(target != 0.0f, money / target, 100.0f), ratio);
	}
	TRIGGER_FUNCTION(tf_unemployment_nation) {
		auto employed = ve::apply(primary_slot, [&ws](const_parameter p_slot) {
			if(!ws.w.nation_s.nations.is_valid_index(to_nation(p_slot)))
				return 0.0f;
			else
				return ws.w.nation_s.nation_demographics.get(to_nation(p_slot), population::total_employment_tag);
		});
		auto total_pop = to_value<nation::total_core_population>(ws.w.nation_s.nations, to_nation(primary_slot));
		return compare_values(tval[0],
			ve::select(total_pop > 0.0f, 1.0f - (employed / total_pop), 0.0f),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_unemployment_state) {
		auto employed = ve::apply(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter, const_parameter) {
			if(!ws.w.nation_s.states.is_valid_index(to_state(p_slot)))
				return 0.0f;
			else
				return ws.w.nation_s.state_demographics.get(to_state(p_slot), population::total_employment_tag);
		});
		auto total_pop = to_value<state::total_population>(ws.w.nation_s.states, to_state(primary_slot));
		return compare_values(tval[0],
			ve::select(total_pop > 0.0f, 1.0f - (employed / total_pop), 0.0f),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_unemployment_province) {
		auto employed = ve::apply(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter, const_parameter) {
			if(!ws.w.province_s.province_state_container.is_valid_index(to_prov(p_slot)))
				return 0.0f;
			else
				return ws.w.province_s.province_demographics.get(to_prov(p_slot), population::total_employment_tag);
		});
		auto total_pop = to_value<province_state::total_population>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		return compare_values(tval[0],
			ve::select(total_pop > 0.0f, 1.0f - (employed / total_pop), 0.0f),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_unemployment_pop) {
		auto employed = ve::apply(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter, const_parameter) {
			if(!ws.w.population_s.pops.is_valid_index(to_pop(p_slot)))
				return 0.0f;
			else
				return ws.w.population_s.pop_demographics.get(to_pop(p_slot), population::total_employment_tag);
		});
		auto total_pop = to_value<pop::size>(ws.w.population_s.pops, to_pop(primary_slot));
		return compare_values(tval[0],
			ve::select(total_pop > 0.0f, 1.0f - (employed / total_pop), 0.0f),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_is_slave_nation) {
		return compare_to_true(tval[0],
			ve::bit_test(to_value<nation::current_rules>(ws.w.nation_s.nations, to_nation(primary_slot)), int32_t(issues::rules::slavery_allowed))
		);
	}
	TRIGGER_FUNCTION(tf_is_slave_state) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<state::is_slave_state>(ws.w.nation_s.states, to_state(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_is_slave_province) {
		auto states = to_value<province_state::state_instance>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		return tf_is_slave_state<value_to_type<decltype(states)>, single_type, single_type>(tval, ws, states, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_is_slave_pop) {
		return compare_values_eq(tval[0], to_value<pop::type>(ws.w.population_s.pops, to_pop(primary_slot)), ws.s.population_m.slave);
	}
	TRIGGER_FUNCTION(tf_is_independant) {
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), nations::country_tag());
	}
	TRIGGER_FUNCTION(tf_has_national_minority_province) {
		auto owners = to_value<province_state::owner>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		auto pcultures = to_value<nation::primary_culture>(ws.w.nation_s.nations, owners);
		auto prov_populations = to_value<province_state::total_population>(ws.w.province_s.province_state_container, to_prov(primary_slot));

		auto majority_pop = ve::apply(primary_slot, pcultures, [&ws](const_parameter p_slot, cultures::culture_tag c) {
			if(!ws.w.province_s.province_state_container.is_valid_index(to_prov(p_slot)))
				return 0.0f;
			else
				return ws.w.province_s.province_demographics.get(to_prov(p_slot), population::to_demo_tag(ws, c));
		});

		return compare_to_false(tval[0], majority_pop == prov_populations);
	}
	TRIGGER_FUNCTION(tf_has_national_minority_state) {
		auto owners = to_value<state::owner>(ws.w.nation_s.states, to_state(primary_slot));
		auto pcultures = to_value<nation::primary_culture>(ws.w.nation_s.nations, owners);
		auto populations = to_value<state::total_population>(ws.w.nation_s.states, to_state(primary_slot));

		auto majority_pop = ve::apply(primary_slot, pcultures, [&ws](const_parameter p_slot, cultures::culture_tag c) {
			if(!ws.w.nation_s.states.is_valid_index(to_state(p_slot)))
				return 0.0f;
			else
				return ws.w.nation_s.state_demographics.get(to_state(p_slot), population::to_demo_tag(ws, c));
		});

		return compare_to_false(tval[0], majority_pop == populations);
	}
	TRIGGER_FUNCTION(tf_has_national_minority_nation) {
		auto populations = to_value<nation::total_core_population>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto majority_pop = ve::apply(primary_slot, [&ws](const_parameter p_slot) {
			if(!ws.w.nation_s.nations.is_valid_index(to_nation(p_slot)))
				return 0.0f;
			else
				return ws.w.nation_s.nation_demographics.get(
					to_nation(p_slot),
					population::to_demo_tag(ws, ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(p_slot))));
		});

		return compare_to_false(tval[0], majority_pop == populations);
	}
	TRIGGER_FUNCTION(tf_government_nation) {
		return compare_values_eq(tval[0],
			to_value<nation::current_government>(ws.w.nation_s.nations, to_nation(primary_slot)),
			trigger_payload(tval[2]).small.values.government);
	}
	TRIGGER_FUNCTION(tf_government_pop) {
		auto location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto owners = to_value<province::owner>(ws.w.province_s.province_state_container, location);

		return is_valid_index(owners) & tf_government_nation<primary_type, single_type, single_type>(tval, ws, owners, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_capital) {
		return compare_values_eq(tval[0],
			to_value<nation::current_capital>(ws.w.nation_s.nations, to_nation(primary_slot)),
			provinces::province_tag(tval[2]));
	}
	TRIGGER_FUNCTION(tf_tech_school) {
		return compare_values_eq(tval[0],
			to_value<nation::tech_school>(ws.w.nation_s.nations, to_nation(primary_slot)),
			trigger_payload(tval[2]).nat_mod);
	}
	TRIGGER_FUNCTION(tf_primary_culture) {
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(primary_slot)),
			trigger_payload(tval[2]).culture);
	}
	TRIGGER_FUNCTION(tf_accepted_culture) {
		auto is_accepted = ve::apply(primary_slot, [&ws](const_parameter p_slot) {
			contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(to_nation(primary_slot)));
		});
		return compare_to_true(tval[0], is_accepted);
	}
	TRIGGER_FUNCTION(tf_culture_pop) {
		return compare_values_eq(tval[0],
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)),
			trigger_payload(tval[2]).culture);
	}
	TRIGGER_FUNCTION(tf_culture_state) {
		return compare_values_eq(tval[0],
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)),
			trigger_payload(tval[2]).culture);
	}
	TRIGGER_FUNCTION(tf_culture_province) {
		return compare_values_eq(tval[0],
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			trigger_payload(tval[2]).culture);
	}
	TRIGGER_FUNCTION(tf_culture_nation) {
		auto c = trigger_payload(tval[2]).culture;
		return compare_to_true(tval[0], nations::is_culture_accepted(ws, c, to_nation(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_pop_reb) {
		return compare_values_eq(tval[0],
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(to_pop(primary_slot))),
			to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_state_reb) {
		return compare_values_eq(tval[0],
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)),
			to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_province_reb) {
		return compare_values_eq(tval[0],
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_nation_reb) {
		auto c = to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot));
		return compare_to_true(tval[0], nations::is_culture_accepted(ws, c, to_nation(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_this_nation) {
		auto pc = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		return compare_to_true(tval[0], nations::is_culture_accepted(ws, pc, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_from_nation) {
		auto pc = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		return compare_to_true(tval[0], nations::is_culture_accepted(ws, pc, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_to_true(is_valid_index(owner) & tf_culture_this_nation<primary_type, this_type, single_type>(tval, ws, primary_slot, owner, const_parameter()));
	}
	TRIGGER_FUNCTION(tf_culture_this_pop) {
		auto loc = to_value<pop::location>(ws.w.population_s.pops, to_pop(this_slot));
		auto owner = to_value<province_state::owner>(ws.w.province_s.province_state_container, loc);
		return compare_to_true(is_valid_index(owner) & tf_culture_this_nation<primary_type, this_type, single_type>(tval, ws, primary_slot, owner, const_parameter()));
	}
	TRIGGER_FUNCTION(tf_culture_this_province) {
		auto owner = to_value<province_state::owner>(ws.w.province_s.province_state_container, to_prov(this_slot));
		return compare_to_true(is_valid_index(owner) & tf_culture_this_nation<primary_type, this_type, single_type>(tval, ws, primary_slot, owner, const_parameter()));
	}
	TRIGGER_FUNCTION(tf_culture_group_nation) {
		return compare_values_eq(tval[0], nations::national_culture_group(ws, to_nation(primary_slot)), trigger_payload(tval[2]).culture_group);
	}
	TRIGGER_FUNCTION(tf_culture_group_pop) {
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());
		return compare_values_eq(tval[0], cgroups, trigger_payload(tval[2]).culture_group);
	}
	TRIGGER_FUNCTION(tf_culture_group_reb_nation) {
		auto rc = to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot));
		auto rcg = ve::load(rc, ws.s.culture_m.cultures_to_groups.view());

		return compare_values_eq(tval[0], rcg, nations::national_culture_group(ws, to_nation(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_group_reb_pop) {
		auto rc = to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot));
		auto rcg = ve::load(rc, ws.s.culture_m.cultures_to_groups.view());

		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());

		return compare_values_eq(tval[0], rcg, cgroups);
	}
	TRIGGER_FUNCTION(tf_culture_group_nation_this_nation) {
		return compare_values_eq(tval[0],
			nations::national_culture_group(ws, to_nation(primary_slot)),
			nations::national_culture_group(ws, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_group_pop_this_nation) {
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());
		return compare_values_eq(tval[0], cgroups, nations::national_culture_group(ws, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_group_nation_from_nation) {
		return compare_values_eq(tval[0],
			nations::national_culture_group(ws, to_nation(primary_slot)),
			nations::national_culture_group(ws, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_group_pop_from_nation) {
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());
		return compare_values_eq(tval[0], cgroups, nations::national_culture_group(ws, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_culture_group_nation_this_province) {
		auto owners = to_value<province_state::owner>(ws.w.province_s.province_state_container, to_prov(this_slot));
		return compare_values_eq(tval[0],
			nations::national_culture_group(ws, to_nation(primary_slot)),
			nations::national_culture_group(ws, owners));
	}
	TRIGGER_FUNCTION(tf_culture_group_pop_this_province) {
		auto owners = to_value<province_state::owner>(ws.w.province_s.province_state_container, to_prov(this_slot));
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());
		return compare_values_eq(tval[0], cgroups, nations::national_culture_group(ws, owners));
	}
	TRIGGER_FUNCTION(tf_culture_group_nation_this_state) {
		auto owners = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values_eq(tval[0],
			nations::national_culture_group(ws, to_nation(primary_slot)),
			nations::national_culture_group(ws, owners));
	}
	TRIGGER_FUNCTION(tf_culture_group_pop_this_state) {
		auto owners = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());
		return compare_values_eq(tval[0], cgroups, nations::national_culture_group(ws, owners));
	}
	TRIGGER_FUNCTION(tf_culture_group_nation_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0],
			nations::national_culture_group(ws, to_nation(primary_slot)),
			nations::national_culture_group(ws, owner));
	}
	TRIGGER_FUNCTION(tf_culture_group_pop_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));
		auto cgroups = ve::load(pculture, ws.s.culture_m.cultures_to_groups.view());
		return compare_values_eq(tval[0], cgroups, nations::national_culture_group(ws, owner));
	}
	TRIGGER_FUNCTION(tf_religion) {
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			trigger_payload(tval[2]).small.values.religion);
	}
	TRIGGER_FUNCTION(tf_religion_reb) {
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<rebel_faction::religion>(ws.w.population_s.rebel_factions, to_rebel(from_slot));
	}
	TRIGGER_FUNCTION(tf_religion_from_nation) {
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::religion>(ws.w.nation_s.nations, to_nation(from_slot));
	}
	TRIGGER_FUNCTION(tf_religion_this_nation) {
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::religion>(ws.w.nation_s.nations, to_nation(this_slot));
	}
	TRIGGER_FUNCTION(tf_religion_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::religion>(ws.w.nation_s.nations, owner);
	}
	TRIGGER_FUNCTION(tf_religion_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::religion>(ws.w.nation_s.nations, owner);
	}
	TRIGGER_FUNCTION(tf_religion_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0],
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::religion>(ws.w.nation_s.nations, owner);
	}
	TRIGGER_FUNCTION(tf_terrain_province) {
		return compare_values_eq(tval[0],
			to_value<province_state::terrain>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			trigger_payload(tval[2]).prov_mod);
	}
	TRIGGER_FUNCTION(tf_terrain_pop) {
		auto location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		return compare_values_eq(tval[0],
			to_value<province_state::terrain>(ws.w.province_s.province_state_container, location),
			trigger_payload(tval[2]).prov_mod);
	}
	TRIGGER_FUNCTION(tf_trade_goods) {
		return compare_values_eq(tval[0],
			to_value<province_state::rgo_production>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			trigger_payload(tval[2]).small.values.good);
	}

	TRIGGER_FUNCTION(tf_is_secondary_power_nation) {
		return compare_to_true(tval[0],
			to_value<nation::overall_rank>(ws.w.nation_s.nations, to_nation(primary_slot)) <= int32_t(ws.s.modifiers_m.global_defines.colonial_rank));
	}
	TRIGGER_FUNCTION(tf_is_secondary_power_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_to_true(tval[0],
			is_valid_index(owner) & 
			(to_value<nation::overall_rank>(ws.w.nation_s.nations, to_nation(owner)) <= int32_t(ws.s.modifiers_m.global_defines.colonial_rank)));
	}
	TRIGGER_FUNCTION(tf_has_faction_nation) {
		auto rebel_type = trigger_payload(tval[2]).small.values.rebel;
		auto result = ve::apply(to_nation(primary_slot), [&ws, rebel_type](nations::country_tag n) {
			auto nation_factions = get_range(ws.w.population_s.rebel_faction_arrays, ws.w.nation_s.nations.get<nation::active_rebel_factions>(n));
			for(auto f : nation_factions) {
				if(is_valid_index(f) && ws.w.population_s.rebel_factions.get<rebel_faction::type>(f) == rebel_type) {
					return true;
				}
			}
			return false;
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_faction_pop) {
		auto pop_faction = to_value<pop::rebel_faction>(ws.w.population_s.pops, to_pop(primary_slot));
		auto faction_type = to_value<rebel_faction::type>(ws.w.population_s.rebel_factions, pop_faction);
		return compare_values_eq(tval[0], faction_type, trigger_payload(tval[2]).small.values.rebel);
	}

	auto unowned_core_accumulator(world_state const& ws, nations::country_tag n) {
		return ve::make_true_accumulator([&ws, n](ve::tagged_vector<provinces::province_tag> v) {
			auto owners = to_value<province_state::owner>(ws.w.province_s.province_state_container, v);
			return owners != n;
		});
	}

	TRIGGER_FUNCTION(tf_has_unclaimed_cores) {
		auto nation_tag = to_value<nation::tag>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto result = ve::apply(to_nation(primary_slot), nation_tag, [&ws](nations::country_tag n, cultures::national_tag t) {
			if(is_valid_index(t)) {
				auto acc = unowned_core_accumulator(ws, n);
				auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[t].core_provinces);

				for(auto p : core_range) {
					acc.add_value(p.value);
					if(acc.result)
						return false;
				}
				acc.flush();
				return !(acc.result);
			}
			return false;
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_bool) {
		return compare_values_eq(tval[0], nations::union_holder_of(ws, to_nation(primary_slot)), to_nation(to_nation(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_this_self_pop) {
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops.get<pop::culture>, to_pop(primary_slot));
		auto pop_union = nations::union_holder_for(ws, pculture);

		return compare_values_eq(tval[0], to_nation(this_slot), pop_union);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_pop_this_pop) {
		auto nation = population::get_pop_owner(ws, to_pop(primary_slot));
		auto pculture = to_value<pop::culture>(ws.w.population_s.pops.get<pop::culture>, to_pop(primary_slot));
		auto pop_union = nations::union_holder_for(ws, pculture);

		return compare_values_eq(tval[0], nation, pop_union);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_this_nation) {
		auto main_union = nations::union_holder_of(ws, to_nation(primary_slot));
		return compare_values_eq(tval[0], to_nation(this_slot), main_union);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_this_pop) {
		auto pop_owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto main_union = nations::union_holder_of(ws, to_nation(primary_slot));
		return compare_values_eq(tval[0], pop_owner, main_union);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_this_state) {
		auto state_owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto main_union = nations::union_holder_of(ws, to_nation(primary_slot));
		return compare_values_eq(tval[0], state_owner, main_union);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_this_province) {
		auto prov_owner = provinces::province_owner(ws, to_prov(this_slot));
		auto main_union = nations::union_holder_of(ws, to_nation(primary_slot));
		return compare_values_eq(tval[0], prov_owner, main_union);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_this_rebel) {
		auto rc = to_value<rebel_faction::culture>(ws.w.population_s.rebel_factions, to_rebel(from_slot));
		return compare_values_eq(tval[0], to_nation(primary_slot), == nations::union_holder_for(ws, rc));
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_tag_nation) {
		return compare_values_eq(tval[0], nations::union_tag_of(ws, to_nation(primary_slot)), trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_tag_this_pop) {
		auto pop_owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0], nations::union_tag_of(ws, pop_owner), trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_tag_this_state) {
		auto state_owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values_eq(tval[0], nations::union_tag_of(ws, state_owner), trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_tag_this_province) {
		auto prov_owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0], nations::union_tag_of(ws, prov_owner), trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_is_cultural_union_tag_this_nation) {
		return compare_values_eq(tval[0], nations::union_tag_of(ws, to_nation(this_slot)), trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_can_build_factory_nation) {
		return compare_to_true(tval[0],
			ve::bit_test(to_value<nation::current_rules>(ws.w.nation_s.nations, to_nation(primary_slot)), int32_t(issues::rules::pop_build_factory)));
	}
	TRIGGER_FUNCTION(tf_can_build_factory_province) {
		auto p_owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_to_true(tval[0],
			ve::bit_test(to_value<nation::current_rules>(ws.w.nation_s.nations, p_owner), int32_t(issues::rules::pop_build_factory)));
	}
	TRIGGER_FUNCTION(tf_can_build_factory_pop) {
		auto p_owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_to_true(tval[0],
			ve::bit_test(to_value<nation::current_rules>(ws.w.nation_s.nations, p_owner), int32_t(issues::rules::pop_build_factory)));
	}
	TRIGGER_FUNCTION(tf_war_nation) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<nation::is_at_war>(ws.w.nation_s.nations, to_nation(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_war_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_to_true(tval[0], ve::widen_mask(to_value<nation::is_at_war>(ws.w.nation_s.nations, owner)));
	}
	TRIGGER_FUNCTION(tf_war_exhaustion_nation) {
		return compare_values(tval[0], to_value<nation::war_exhaustion>(ws.w.nation_s.nations, to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_war_exhaustion_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_values(tval[0], to_value<nation::war_exhaustion>(ws.w.nation_s.nations, owner), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_war_exhaustion_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_values(tval[0], to_value<nation::war_exhaustion>(ws.w.nation_s.nations, owner), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_blockade) {
		auto cpc = ve::to_float(to_value<nation::central_province_count>(ws.w.nation_s.nations, to_nation(primary_slot)));
		return compare_values(tval[0],
			ve::select(cpc != 0.0f, ve::to_float(to_value<nation::blockaded_count>(ws.w.nation_s.nations, to_nation(primary_slot))) / cpc, 0.0f),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_owns) {
		provinces::province_tag prov(tval[2]);
		return compare_values_eq(tval[0], provinces::province_owner(ws, prov), to_nation(primary_slot));
	}
	TRIGGER_FUNCTION(tf_controls) {
		provinces::province_tag prov(tval[2]);
		return compare_values_eq(tval[0], provinces::province_controller(ws, prov), to_nation(primary_slot));
	}
	TRIGGER_FUNCTION(tf_is_core_integer) {
		provinces::province_tag prov(tval[2]);
		auto tag = to_value<nation::tag>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto result = ve::apply(tag, [&ws, prov](cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_this_nation) {
		auto tag = to_value<nation::tag>(ws.w.nation_s.nations, to_nation(this_slot));
		auto result = ve::apply(to_prov(primary_slot), tag,
			[&ws](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto tag = to_value<nation::tag>(ws.w.nation_s.nations, owner);
		auto result = ve::apply(to_prov(primary_slot), tag,
			[&ws](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		auto tag = to_value<nation::tag>(ws.w.nation_s.nations, owner);
		auto result = ve::apply(to_prov(primary_slot), tag,
			[&ws](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto tag = to_value<nation::tag>(ws.w.nation_s.nations, owner);
		auto result = ve::apply(to_prov(primary_slot), tag,
			[&ws](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_from_nation) {
		auto tag = to_value<nation::tag>(ws.w.nation_s.nations, to_nation(from_slot));
		auto result = ve::apply(to_prov(primary_slot), tag,
			[&ws](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_reb) {
		auto tag = to_value<rebel_faction::independence_tag>(ws.w.nation_s.nations, to_rebel(from_slot));
		auto result = ve::apply(to_prov(primary_slot), tag,
			[&ws](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_is_core_tag) {
		auto result = ve::apply(to_prov(primary_slot),
			[&ws, tag = trigger_payload(tval[2]).tag](provinces::province_tag prov, cultures::national_tag t) {
			return contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				t);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_num_of_revolts) {
		return compare_values(tval[0], 
			to_value<nation::rebel_controlled_provinces>(ws.w.nation_s.nations, to_nation(primary_slot)), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_revolt_percentage) {
		auto cpc = ve::to_float(to_value<nation::central_province_count>(ws.w.nation_s.nations, to_nation(primary_slot)));
		auto rpc = ve::to_float(to_value<nation::rebel_controlled_provinces>(ws.w.nation_s.nations, to_nation(primary_slot)));
		return compare_values(tval[0], ve::select(cpc != 0.0f, rpc / cpc, 0.0f), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_num_of_cities_int) {
		return compare_values(tval[0],
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(primary_slot)),
			int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_num_of_cities_from_nation) {
		return compare_values(tval[0],
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_num_of_cities_this_nation) {
		return compare_values(tval[0],
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_num_of_cities_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values(tval[0],
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::province_count>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_num_of_cities_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values(tval[0],
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::province_count>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_num_of_cities_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values(tval[0],
			to_value<nation::province_count>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::province_count>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_num_of_ports) {
		return compare_values(tval[0], to_value<nation::num_connected_ports>(ws.w.nation_s.nations, to_nation(primary_slot)), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_num_of_allies) {
		auto count_allies = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return int32_t(get_size(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(n)));
		});
		return compare_values(tval[0], count_allies, int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_num_of_vassals) {
		auto count_vassals = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return int32_t(get_size(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(n)));
		});
		return compare_values(tval[0], count_vassals, int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_owned_by_tag) {
		auto holders = ve::load(trigger_payload(tval[2]).tag, ws.w.culture_s.tags_to_holders.view());
		return compare_to_true(tval[0], is_valid_index(holders) & (holder == provinces::province_owner(ws, to_prov(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_owned_by_this_nation) {
		return compare_values_eq(tval[0], to_nation(this_slot), provinces::province_owner(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_owned_by_from_nation) {
		return compare_values_eq(tval[0], to_nation(from_slot), provinces::province_owner(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_owned_by_this_province) {
		return compare_values_eq(tval[0], provinces::province_owner(ws, to_prov(this_slot)), provinces::province_owner(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_owned_by_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values_eq(tval[0], owner, provinces::province_owner(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_owned_by_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0], owner, provinces::province_owner(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_exists_bool) {
		return compare_to_true(tval[0], is_valid_index(to_value<nation::current_capital>(ws.w.nation_s.nations, to_nation(to_nation(primary_slot)))));
	}
	TRIGGER_FUNCTION(tf_exists_tag) {
		auto holder = ve::load(trigger_payload(tval[2]).tag, ws.w.culture_s.tags_to_holders.view());
		return compare_to_true(tval[0], is_valid_index(to_value<nation::current_capital>(ws.w.nation_s.nations, holder)));
	}
	TRIGGER_FUNCTION(tf_has_country_flag) {
		auto result = ve::apply(to_nation(primary_slot), [&ws, flg = trigger_payload(tval[2]).nat_flag](nations::country_tag n) {
			return contains_item(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(n), flg);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_country_flag_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		auto result = ve::apply(owner, [&ws, flg = trigger_payload(tval[2]).nat_flag](nations::country_tag n) {
			return contains_item(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(n), flg);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_country_flag_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		auto result = ve::apply(owner, [&ws, flg = trigger_payload(tval[2]).nat_flag](nations::country_tag n) {
			return contains_item(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(n), flg);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_continent_province) {
		return compare_values_eq(tval[0],
			to_value<province::continent>(ws.s.province_m.province_container, to_prov(primary_slot)), trigger_payload(tval[2]).prov_mod);
	}
	TRIGGER_FUNCTION(tf_continent_state) {
		auto state_caps = to_value<state::state_capital>(ws.w.nation_s.states, to_state(primary_slot));
		return compare_values_eq(tval[0],
			to_value<province::continent>(ws.s.province_m.province_container, state_caps), trigger_payload(tval[2]).prov_mod);
	}
	TRIGGER_FUNCTION(tf_continent_nation) {
		auto nat_caps = to_value<nation::current_capital>(ws.w.nation_s.nations, to_nation(primary_slot));
		return compare_values_eq(tval[0],
			to_value<province::continent>(ws.s.province_m.province_container, nat_caps), trigger_payload(tval[2]).prov_mod);
	}
	TRIGGER_FUNCTION(tf_continent_pop) {
		auto prov_id = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		return compare_values_eq(tval[0],
			to_value<province::continent>(ws.s.province_m.province_container, prov_id), trigger_payload(tval[2]).prov_mod);
	}
	TRIGGER_FUNCTION(tf_continent_nation_this) {
		return tf_continent_nation<this_type, single_type, single_type>(tval, ws, this_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_state_this) {
		return tf_continent_state<this_type, single_type, single_type>(tval, ws, this_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_province_this) {
		return tf_continent_province<this_type, single_type, single_type>(tval, ws, this_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_pop_this) {
		return tf_continent_pop<this_type, single_type, single_type>(tval, ws, this_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_nation_from) {
		return tf_continent_nation<from_type, single_type, single_type>(tval, ws, from_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_state_from) {
		return tf_continent_state<from_type, single_type, single_type>(tval, ws, from_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_province_from) {
		return tf_continent_province<from_type, single_type, single_type>(tval, ws, from_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_continent_pop_from) {
		return tf_continent_pop<from_type, single_type, single_type>(tval, ws, from_slot, const_parameter(), const_parameter());
	}
	TRIGGER_FUNCTION(tf_casus_belli_tag) {
		auto tag_holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto result = ve::apply(to_nation(primary_slot), [&ws, tag_holder](nations::country_tag n) {
			return military::can_use_cb_against(ws, n, tag_holder);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_casus_belli_from) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(from_slot), [&ws, tag_holder](nations::country_tag n, nations::country_tag target) {
			return military::can_use_cb_against(ws, n, target);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_casus_belli_this_nation) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(this_slot), [&ws, tag_holder](nations::country_tag n, nations::country_tag target) {
			return military::can_use_cb_against(ws, n, target);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_casus_belli_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws, tag_holder](nations::country_tag n, nations::country_tag target) {
			return military::can_use_cb_against(ws, n, target);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_casus_belli_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws, tag_holder](nations::country_tag n, nations::country_tag target) {
			return military::can_use_cb_against(ws, n, target);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_casus_belli_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws, tag_holder](nations::country_tag n, nations::country_tag target) {
			return military::can_use_cb_against(ws, n, target);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_military_access_tag) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		return compare_to_true(tval[0], military::has_military_access_with(ws, to_nation(primary_slot), holder));
	}
	TRIGGER_FUNCTION(tf_military_access_from) {
		return compare_to_true(tval[0], military::has_military_access_with(ws, to_nation(primary_slot), to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_military_access_this_nation) {
		return compare_to_true(tval[0], military::has_military_access_with(ws, to_nation(primary_slot), to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_military_access_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_to_true(tval[0], military::has_military_access_with(ws, to_nation(primary_slot), owner));
	}
	TRIGGER_FUNCTION(tf_military_access_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_to_true(tval[0], military::has_military_access_with(ws, to_nation(primary_slot), owner));
	}
	TRIGGER_FUNCTION(tf_military_access_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_to_true(tval[0], military::has_military_access_with(ws, to_nation(primary_slot), owner));
	}
	TRIGGER_FUNCTION(tf_prestige_value) {
		return compare_values(tval[0], nations::get_prestige(ws, to_nation(primary_slot), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_prestige_from) {
		return compare_values(tval[0], nations::get_prestige(ws, to_nation(primary_slot)), nations::get_prestige(ws, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_prestige_this_nation) {
		return compare_values(tval[0], nations::get_prestige(ws, to_nation(primary_slot), nations::get_prestige(ws, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_prestige_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values(tval[0], nations::get_prestige(ws, to_nation(primary_slot), nations::get_prestige(ws, owner));
	}
	TRIGGER_FUNCTION(tf_prestige_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values(tval[0], nations::get_prestige(ws, to_nation(primary_slot), nations::get_prestige(ws, owner));
	}
	TRIGGER_FUNCTION(tf_prestige_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values(tval[0], nations::get_prestige(ws, to_nation(primary_slot), nations::get_prestige(ws, owner));
	}
	TRIGGER_FUNCTION(tf_badboy) {
		return compare_values(tval[0], to_value<nation::infamy>(ws.w.nation_s.nations, to_nation(to_nation(primary_slot))), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_has_building_fort) {
		return compare_to_true(tval[0], to_value<province_state::fort_level>(ws.w.province_s.province_state_container, to_prov(primary_slot)) != 0);
	}
	TRIGGER_FUNCTION(tf_has_building_railroad) {
		return compare_to_true(tval[0], to_value<province_state::railroad_level>(ws.w.province_s.province_state_container, to_prov(primary_slot)) != 0);
	}
	TRIGGER_FUNCTION(tf_has_building_naval_base) {
		return compare_to_true(tval[0], to_value<province_state::naval_base_level>(ws.w.province_s.province_state_container, to_prov(primary_slot)) != 0);
	}

	TRIGGER_FUNCTION(tf_has_building_factory) {
		return compare_to_true(tval[0], nations::has_factory(ws, to_state(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_has_building_state) {
		return compare_to_true(tval[0], nations::has_factory(ws, to_state(primary_slot), trigger_payload(tval[2]).small.values.factory));
	}
	TRIGGER_FUNCTION(tf_has_building_state_from_province) {
		auto state = provinces::province_state(ws, to_prov(primary_slot));
		return compare_to_true(tval[0], nations::has_factory(ws, state, trigger_payload(tval[2]).small.values.factory));
	}
	TRIGGER_FUNCTION(tf_has_building_factory_from_province) {
		auto state = provinces::province_state(ws, to_prov(primary_slot));
		return compare_to_true(tval[0], nations::has_factory(ws, state));
	}
	TRIGGER_FUNCTION(tf_empty) {
		return compare_to_true(tval[0], !is_valid_index(provinces::province_owner(ws, to_prov(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_is_blockaded) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<province_state::is_blockaded>(ws.w.province_s.province_state_container, to_prov(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_has_country_modifier) {
		const auto mod = trigger_payload(tval[2]).nat_mod;
		auto result = ve::apply(to_nation(primary_slot), [&ws, mod](nations::country_tag n) {
			return modifiers::has_national_modifier(ws, n, mod);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_country_modifier_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		const auto mod = trigger_payload(tval[2]).nat_mod;
		auto result = ve::apply(owner, [&ws, mod](nations::country_tag n) {
			return modifiers::has_national_modifier(ws, n, mod);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_province_modifier) {
		const auto mod = trigger_payload(tval[2]).prov_mod;
		auto result = ve::apply(to_prov(primary_slot), [&ws, mod](provicnes::province_tag p) {
			return modifiers::has_provincial_modifier(ws, p, mod);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_region) {
		return compare_values_eq(tval[0],
			to_value<province::state_id>(ws.s.province_m.province_container, to_prov(primary_slot)),
			trigger_payload(tval[2]).state);
	}
	TRIGGER_FUNCTION(tf_tag_tag) {
		return compare_values_eq(tval[0],
			to_value<nation::tag>(ws.w.nation_s.nations, to_nation(primary_slot)),
			trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_tag_this_nation) {
		return compare_values_eq(tval[0], to_nation(primary_slot, to_nation(this_slot));
	}
	TRIGGER_FUNCTION(tf_tag_this_province) {
		return compare_values_eq(tval[0],
			to_nation(primary_slot),
			provinces::province_owner(ws, to_prov(this_slot)));
	}
	TRIGGER_FUNCTION(tf_tag_from_nation) {
		return compare_values_eq(tval[0], to_nation(primary_slot), to_nation(from_slot));
	}
	TRIGGER_FUNCTION(tf_tag_from_province) {
		return compare_values_eq(tval[0], to_nation(primary_slot), provinces::province_owner(ws, to_prov(from_slot)));
	}
	TRIGGER_FUNCTION(tf_tag_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_values_eq(tval[0], owner, trigger_payload(tval[2]).tag);
	}
	TRIGGER_FUNCTION(tf_neighbour_tag) {
		auto tag_holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto result = ve::apply(to_nation(primary_slot), [&ws, tag_holder](nations::country_tag n) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(n), tag_holder);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_neighbour_this) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(this_slot),
			[&ws, tag_holder](nations::country_tag primary, nations::country_tag other) {
				return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(primary), other);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_neighbour_from) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(from_slot),
			[&ws, tag_holder](nations::country_tag primary, nations::country_tag other) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(primary), other);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_units_in_province_value) {
		auto result = ve::apply(to_prov(primary_slot), [&ws](provinces::province_tag p) {
			return int32_t(military::total_units_in_province(ws, p));
		});
		return compare_values(tval[0], result, int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_units_in_province_from) {
		auto result = ve::apply(to_prov(primary_slot), to_nation(from_slot), [&ws](provinces::province_tag p, nations::country_tag n) {
			return military::has_units_in_province(ws, n, p);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_units_in_province_this_nation) {
		auto result = ve::apply(to_prov(primary_slot), to_nation(this_slot), [&ws](provinces::province_tag p, nations::country_tag n) {
			return military::has_units_in_province(ws, n, p);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_units_in_province_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		auto result = ve::apply(to_prov(primary_slot), owner, [&ws](provinces::province_tag p, nations::country_tag n) {
			return military::has_units_in_province(ws, n, p);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_units_in_province_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto result = ve::apply(to_prov(primary_slot), owner, [&ws](provinces::province_tag p, nations::country_tag n) {
			return military::has_units_in_province(ws, n, p);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_units_in_province_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto result = ve::apply(to_prov(primary_slot), owner, [&ws](provinces::province_tag p, nations::country_tag n) {
			return military::has_units_in_province(ws, n, p);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_war_with_tag) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto result = ve::apply(to_nation(primary_slot), [&ws, holder](nations::country_tag a) {
			return military::in_war_against(ws, a, holder);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_war_with_from) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(from_slot), [&ws](nations::country_tag a, nations::country_tag b) {
			return military::in_war_against(ws, a, b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_war_with_this_nation) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(this_slot), [&ws](nations::country_tag a, nations::country_tag b) {
			return military::in_war_against(ws, a, b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_war_with_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws](nations::country_tag a, nations::country_tag b) {
			return military::in_war_against(ws, a, b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_war_with_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws](nations::country_tag a, nations::country_tag b) {
			return military::in_war_against(ws, a, b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_war_with_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws](nations::country_tag a, nations::country_tag b) {
			return military::in_war_against(ws, a, b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_unit_in_battle) {
		return compare_to_true(tval[0], military::province_is_contested(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_total_amount_of_divisions) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return int32_t(military::total_active_divisions(ws, n));
		});
		return compare_values(tval[0], result, int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_money) {
		return compare_values(tval[0], nations::national_treasury(ws, to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_lost_national) {
		auto fraction = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return nations::fraction_of_cores_owned(ws, n);
		});
		return compare_values(tval[0], 1.0f - fraction, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_is_vassal) {
		return compare_to_true(tval[0], is_valid_index(ws.w.nation_s.nations.get<nation::overlord>(to_nation(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_ruling_party_ideology_nation) {
		auto ri = to_value<nation::ruling_ideology>(ws.w.nation_s.nations, to_nation(primary_slot));
		return compare_values_eq(tval[0], ri, trigger_payload(tval[2]).small.values.ideology);
	}
	TRIGGER_FUNCTION(tf_ruling_party_ideology_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		auto ri = to_value<nation::ruling_ideology>(ws.w.nation_s.nations, owner);
		return compare_values_eq(tval[0], ri, trigger_payload(tval[2]).small.values.ideology);
	}
	TRIGGER_FUNCTION(tf_ruling_party) {
		auto rp = to_value<nation::ruling_party>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto party_names = ve::apply(rp, [&ws](government::party_tag p) {
			return ws.s.governments_m.parties[p].name;
		});
		return compare_values_eq(tval[0], party_names, trigger_payload(tval[2]).text);
	}
	TRIGGER_FUNCTION(tf_is_ideology_enabled) {
		return compare_to_true(tval[0], ideologies::ideology_enabled(ws, trigger_payload(tval[2]).small.values.ideology));
	}
	TRIGGER_FUNCTION(tf_political_reform_want_nation) {
		return compare_values(tval[0],
			to_value<nation::political_interest_fraction>(ws.w.nation_s.nations, to_nation(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_political_reform_want_pop) {
		return compare_values(tval[0],
			to_value<pop::political_interest>(ws.w.population_s.pops, to_pop(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_social_reform_want_nation) {
		return compare_values(tval[0],
			to_value<nation::social_interest_fraction>(ws.w.nation_s.nations, to_nation(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_social_reform_want_pop) {
		return compare_values(tval[0],
			to_value<pop::social_interest>(ws.w.population_s.pops, to_pop(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_total_amount_of_ships) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return int32_t(military::total_active_ships(ws, n));
		});
		return compare_values(tval[0], result, int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_plurality) {
		return compare_values(tval[0],
			to_value<nation::plurality>(ws.w.nation_s.nations, to_nation(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_corruption) {
		auto cpc = ve::to_float(to_value<central_province_count>(ws.w.nation_s.nations, to_nation(primary_slot)));
		auto cc = ve::to_float(to_value<nation::crime_count>(ws.w.nation_s.nations, to_nation(primary_slot)))
		
		return compare_values(tval[0], ve::select(cpc > 0.0f, cc / cpc, 0.0f), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_is_state_religion_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::national_religion>(ws.w.nation_s.nations, owner),
			to_value<pop::religion>(ws.w.population_s.pops, to_pop(primary_slot)));
		
	}
	TRIGGER_FUNCTION(tf_is_state_religion_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::national_religion>(ws.w.nation_s.nations, owner),
			to_value<province_state::dominant_religion>(ws.w.province_s.province_state_container, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_state_religion_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::national_religion>(ws.w.nation_s.nations, owner),
			to_value<state::dominant_religion>(ws.w.nation_s.states, to_state(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, owner),
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, owner),
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, owner),
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_nation_this_pop) {
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(this_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_nation_this_nation) {
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_nation_this_state) {
		auto this_owner = nations::state_owner(ws, to_state(this_slot));
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_nation_this_province) {
		auto this_owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0],
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_state_this_nation) {
		return compare_values_eq(tval[0],
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_state_this_pop) {
		auto this_owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0],
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_state_this_state) {
		auto this_owner = nations::state_owner(ws, to_state(this_slot));
		return compare_values_eq(tval[0],
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_state_this_province) {
		auto this_owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0],
			to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_province_this_nation) {
		return compare_values_eq(tval[0],
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_province_this_pop) {
		auto this_owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0],
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_province_this_state) {
		auto this_owner = nations::state_owner(ws, to_state(this_slot));
		return compare_values_eq(tval[0],
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_province_this_province) {
		auto this_owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0],
			to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_pop_this_nation) {
		return compare_values_eq(tval[0],
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_pop_this_pop) {
		auto this_owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0],
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_pop_this_state) {
		auto this_owner = nations::state_owner(ws, to_state(this_slot));
		return compare_values_eq(tval[0],
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_primary_culture_pop_this_province) {
		auto this_owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0],
			to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)),
			to_value<nation::primary_culture>(ws.w.nation_s.nations, this_owner));
	}
	TRIGGER_FUNCTION(tf_is_accepted_culture_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_to_true(tval[0], nations::is_culture_accepted(ws, to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot)), owner));
	}
	TRIGGER_FUNCTION(tf_is_accepted_culture_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_to_true(tval[0],
			nations::is_culture_accepted(ws, to_value<province_state::dominant_culture>(ws.w.province_s.province_state_container, to_prov(primary_slot)), owner));
	}
	TRIGGER_FUNCTION(tf_is_accepted_culture_state) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
		return compare_to_true(tval[0],
			nations::is_culture_accepted(ws, to_value<state::dominant_culture>(ws.w.nation_s.states, to_state(primary_slot)), owner));
	}
	TRIGGER_FUNCTION(tf_is_coastal) {
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<province::is_coastal>(ws.s.province_m.province_container, to_prov(primary_slot))),
			);
	}
	TRIGGER_FUNCTION(tf_in_sphere_tag) {
		return compare_values_eq(tval[0],
			to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot)),
			ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag]);
	}
	TRIGGER_FUNCTION(tf_in_sphere_from) {
		return compare_values_eq(tval[0],
			to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_nation(from_slot));
	}
	TRIGGER_FUNCTION(tf_in_sphere_this_nation) {
		return compare_values_eq(tval[0],
			to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_nation(this_slot));
	}
	TRIGGER_FUNCTION(tf_in_sphere_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0],
			to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot)),
			owner);
	}
	TRIGGER_FUNCTION(tf_in_sphere_this_state) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		return compare_values_eq(tval[0],
			to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot)),
			owner);
	}
	TRIGGER_FUNCTION(tf_in_sphere_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0],
			to_value<nation::sphere_leader>(ws.w.nation_s.nations, to_nation(primary_slot)),
			owner);
	}
	TRIGGER_FUNCTION(tf_produces_nation) {
		auto good = trigger_payload(tval[2]).small.values.good;

		auto result = ve::apply(to_nation(primary_slot), [&ws, good](nations::country_tag n) {
			auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(n));
			for(auto s = states.first; s != states.second; ++s) {
				if(ws.w.nation_s.state_production.get(s->state, good) > 0.0f)
					return true;
			}
			return false;
		});

		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_produces_province) {
		return compare_values(tval[0],
			(to_value<province_state::rgo_production>(ws.w.province_s.province_state_container, to_prov(primary_slot)) == trigger_payload(tval[2]).small.values.good) |
			(to_value<province_state::artisan_production>(ws.w.province_s.province_state_container, to_prov(primary_slot)) == trigger_payload(tval[2]).small.values.good),
			true);
	}
	TRIGGER_FUNCTION(tf_produces_state) {
		auto good = trigger_payload(tval[2]).small.values.good;

		auto result = ve::apply(to_state(primary_slot), [&ws, good](nations::state_tag s) {
			return ws.w.nation_s.states.is_valid_index(s) ? ws.w.nation_s.state_production.get(s, good) > 0.0f ? false;
		});

		return compare_to_true(tval[0], result);
	}
	
	TRIGGER_FUNCTION(tf_produces_pop) {
		auto pop_location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto good = trigger_payload(tval[2]).small.values.good;

		return compare_to_true(tval[0], (to_value<pop::type>(ws.w.population_s.pops, to_pop(primary_slot)) == ws.s.population_m.artisan)
			& (to_value<province_state::artisan_production>(ws.w.province_s.province_state_container, pop_location) == good));
	}
	TRIGGER_FUNCTION(tf_average_militancy_nation) {
		auto total_pop = to_value<nation::total_core_population>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto mil_amount = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return ws.w.nation_s.nations.is_valid_index(n) ?
				ws.w.nation_s.nation_demographics.get(n, population::militancy_demo_tag(ws)) : 0.0f;
		});
		auto val = ve::select(total_pop > 0.0f, 10.0f * mil_amount / total_pop, 0.0f);
		return compare_values(tval[0], val, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_average_militancy_state) {
		auto total_pop = to_value<state::total_population>(ws.w.nation_s.states, to_state(primary_slot));
		auto mil_amount = ve::apply(to_state(primary_slot), [&ws](nations::state_tag s) {
			return ws.w.nation_s.states.is_valid_index(s) ?
				ws.w.nation_s.state_demographics.get(s, population::militancy_demo_tag(ws)) : 0.0f;
		});
		auto val = ve::select(total_pop > 0.0f, 10.0f * mil_amount / total_pop, 0.0f);
		return compare_values(tval[0], val, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_average_militancy_province) {
		auto total_pop = to_value<province_state::total_population>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		auto mil_amount = ve::apply(to_state(primary_slot), [&ws](provinces::province_tag p) {
			return ws.w.province_s.province_state_container.is_valid_index(p) ?
				ws.w.province_s.province_demographics.get(p, population::militancy_demo_tag(ws)) : 0.0f;
		});
		auto val = ve::select(total_pop > 0.0f, 10.0f * mil_amount / total_pop, 0.0f);
		return compare_values(tval[0], val, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_average_consciousness_nation) {
		auto total_pop = to_value<nation::total_core_population>(ws.w.nation_s.nations, to_nation(primary_slot));
		auto con_amount = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return ws.w.nation_s.nations.is_valid_index(n) ?
				ws.w.nation_s.nation_demographics.get(n, population::consciousness_demo_tag(ws)) : 0.0f;
		});
		auto val = ve::select(total_pop > 0.0f, 10.0f * con_amount / total_pop, 0.0f);
		return compare_values(tval[0], val, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_average_consciousness_state) {
		auto total_pop = to_value<state::total_population>(ws.w.nation_s.states, to_state(primary_slot));
		auto con_amount = ve::apply(to_state(primary_slot), [&ws](nations::state_tag s) {
			return ws.w.nation_s.states.is_valid_index(s) ?
				ws.w.nation_s.state_demographics.get(s, population::consciousness_demo_tag(ws)) : 0.0f;
		});
		auto val = ve::select(total_pop > 0.0f, 10.0f * con_amount / total_pop, 0.0f);
		return compare_values(tval[0], val, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_average_consciousness_province) {
		auto total_pop = to_value<province_state::total_population>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		auto con_amount = ve::apply(to_state(primary_slot), [&ws](provinces::province_tag p) {
			return ws.w.province_s.province_state_container.is_valid_index(p) ?
				ws.w.province_s.province_demographics.get(p, population::consciousness_demo_tag(ws)) : 0.0f;
		});
		auto val = ve::select(total_pop > 0.0f, 10.0f * con_amount / total_pop, 0.0f);
		return compare_values(tval[0], val, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_is_next_reform_nation) {
		auto reform_id = trigger_payload(tval[2]).small.values.option;
		auto reform_parent = ws.s.issues_m.options[reform_id].parent_issue;
		auto active_option = to_vindexed_value(ws.w.nation_s.active_issue_options, to_nation(primary_slot), reform_parent);

		return compare_values_eq(tval[0], to_int(active_option) + 1, to_int(reform_id));
	}
	TRIGGER_FUNCTION(tf_is_next_reform_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		auto reform_id = trigger_payload(tval[2]).small.values.option;
		auto reform_parent = ws.s.issues_m.options[reform_id].parent_issue;
		auto active_option = to_vindexed_value(ws.w.nation_s.active_issue_options, owner, reform_parent);

		return compare_values_eq(tval[0], to_int(active_option) + 1, to_int(reform_id));
	}
	TRIGGER_FUNCTION(tf_rebel_power_fraction) {
		// note: virtually unused
		return compare_to_true(tval[0], false);
	}
	TRIGGER_FUNCTION(tf_recruited_percentage_nation) {
		auto value = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return military::recruited_pop_fraction(ws, n);
		});
		return compare_values(tval[0], value, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_recruited_percentage_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		auto value = ve::apply(owner, [&ws](nations::country_tag n) {
			return military::recruited_pop_fraction(ws, n);
		});
		return compare_values(tval[0], value, read_float_from_payload(tval + 2));
	}

	auto has_culture_core_accumulator(world_state const& ws, cultures::culture_tag c) {
		return ve::make_true_accumulator([&ws, c](ve::tagged_vector<cultures::national_tag> v) {
			auto holders = ve::load(v, ws.w.culture_s.tags_to_holders.view());
			auto holder_pculture = ve::load(holders, ws.w.nation_s.nations.get_row<nation::primary_culture>());
			return holder_pculture == c;
		});
	}

	TRIGGER_FUNCTION(tf_has_culture_core) {
		auto location = to_value<pop::location>(ws.w.population_s.pops, to_pop(primary_slot));
		auto culture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(primary_slot));

		auto result = ve::apply(location, culture, [&ws](province::province_tag p, cultures::culture_tag c) {
			auto cores = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(p));
			auto acc = has_culture_core_accumulator(ws, c);
			for(auto tag : cores) {
				acc.add_value(tag.value);
				if(acc.result)
					return true;
			}
			acc.flush();
			return acc.result;
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_culture_core_province_this_pop) {
		auto culture = to_value<pop::culture>(ws.w.population_s.pops, to_pop(this_slot));

		auto result = ve::apply(to_prov(primary_slot), culture, [&ws](province::province_tag p, cultures::culture_tag c) {
			auto cores = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(p));
			auto acc = has_culture_core_accumulator(ws, c);
			for(auto tag : cores) {
				acc.add_value(tag.value);
				if(acc.result)
					return true;
			}
			acc.flush;
			return acc.result;
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_nationalism) {
		return compare_values(tval[0], to_value<province_state::nationalism>(ws.w.province_s.province_state_container, to_prov(primary_slot)), float(tval[2]));
	}
	TRIGGER_FUNCTION(tf_is_overseas) {
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<province_state::is_overseas>(ws.w.province_s.province_state_container, to_prov(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_controlled_by_rebels) {
		return compare_to_true(tval[0],
			to_value<province_state::rebel_controller>(ws.w.province_s.province_state_container, to_prov(primary_slot)) != population::rebel_faction_tag());
	}
	TRIGGER_FUNCTION(tf_controlled_by_tag) {
		return compare_values_eq(tval[0],
			provinces::province_controller(ws, to_prov(primary_slot)), ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag]);
	}
	TRIGGER_FUNCTION(tf_controlled_by_from) {
		return compare_values_eq(tval[0], provinces::province_controller(ws, to_prov(primary_slot)), to_nation(from_slot));
	}
	TRIGGER_FUNCTION(tf_controlled_by_this_nation) {
		return compare_values_eq(tval[0], provinces::province_controller(ws, to_prov(primary_slot)), to_nation(this_slot));
	}
	TRIGGER_FUNCTION(tf_controlled_by_this_province) {
		return compare_values_eq(tval[0], provinces::province_controller(ws, to_prov(primary_slot)), provinces::province_owner(ws, to_prov(this_slot)));
	}
	TRIGGER_FUNCTION(tf_controlled_by_this_state) {
		return compare_values_eq(tval[0], provinces::province_controller(ws, to_prov(primary_slot)), nations::state_owner(ws, to_state(this_slot)));
	}
	TRIGGER_FUNCTION(tf_controlled_by_this_pop) {
		return compare_values_eq(tval[0], provinces::province_controller(ws, to_prov(primary_slot)), population::get_pop_owner(ws, to_pop(this_slot)));
	}
	TRIGGER_FUNCTION(tf_controlled_by_owner) {
		return compare_values_eq(tval[0], provinces::province_owner(ws, to_prov(primary_slot)), provinces::province_controller(ws, to_prov(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_controlled_by_reb) {
		return compare_values_eq(tval[0],
			to_value<province_state::rebel_controller>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			to_rebel(from_slot));
	}
	TRIGGER_FUNCTION(tf_is_canal_enabled) {
		return compare_to_true(tval[0], ws.w.province_s.is_canal_enabled[tval[2]-1] != 0ui8);
	}
	TRIGGER_FUNCTION(tf_is_state_capital) {
		auto id = to_prov(primary_slot);
		auto sid = to_value<province_state::state_instance>(ws.w.province_s.province_state_container, to_prov(primary_slot));
		
		return compare_values_eq(tval[0],
			to_value<state::state_capital>(ws.w.nation_s.states, sid),
			to_prov(primary_slot));
	}
	TRIGGER_FUNCTION(tf_truce_with_tag) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto result = ve::apply(to_nation(primary_slot), [&ws, holder](nations::country_tag a) {
			return contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(a), nations::truce{ date_tag(), holder });
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_truce_with_from) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(from_slot), [&ws, holder](nations::country_tag a, nations::country_tag b) {
			return contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(a), nations::truce{ date_tag(), b });
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_truce_with_this_nation) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(this_slot), [&ws, holder](nations::country_tag a, nations::country_tag b) {
			return contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(a), nations::truce{ date_tag(), b });
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_truce_with_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws, holder](nations::country_tag a, nations::country_tag b) {
			return contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(a), nations::truce{ date_tag(), b });
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_truce_with_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws, holder](nations::country_tag a, nations::country_tag b) {
			return contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(a), nations::truce{ date_tag(), b });
		});
	}
	TRIGGER_FUNCTION(tf_truce_with_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws, holder](nations::country_tag a, nations::country_tag b) {
			return contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(a), nations::truce{ date_tag(), b });
		});
	}
	TRIGGER_FUNCTION(tf_total_pops_nation) {
		return compare_values(tval[0],
			to_value<nation::total_core_population>(ws.w.nation_s.nations, to_nation(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_total_pops_state) {
		return compare_values(tval[0],
			to_value<state::total_population>(ws.w.nation_s.states, to_state(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_total_pops_province) {
		return compare_values(tval[0],
			to_value<province_state::total_population>(ws.w.province_s.province_state_container, to_prov(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_total_pops_pop) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		return compare_values(tval[0],
			to_value<state::total_population>(ws.w.nation_s.states, provinces::province_state(ws, location)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_has_pop_type_nation) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		auto amount = ve::apply(to_nation(primary_slot), [&ws, type](nations::country_tag n) {
			return ws.w.nation_s.nation_demographics.get(n, population::to_demo_tag(ws, type));
		});
		return compare_to_true(tval[0], amount > 0.0f);
	}
	TRIGGER_FUNCTION(tf_has_pop_type_state) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		auto amount = ve::apply(to_state(primary_slot), [&ws, type](nations::state_tag s) {
			return ws.w.nation_s.state_demographics.get(s, population::to_demo_tag(ws, type));
		});
		return compare_to_true(tval[0], amount > 0.0f);
	}
	TRIGGER_FUNCTION(tf_has_pop_type_province) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		auto amount = ve::apply(to_prov(primary_slot), [&ws, type](provinces::province_tag p) {
			return ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, type));
		});
		return compare_to_true(tval[0], amount > 0.0f);

	}
	TRIGGER_FUNCTION(tf_has_pop_type_pop) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		return compare_values_eq(tval[0], to_value<pop::type>(ws.w.population_s.pops, to_pop(primary_slot)), type);
	}
	TRIGGER_FUNCTION(tf_has_empty_adjacent_province) {
		auto result = ve::apply(to_prov(primary_slot), [&ws](provinces::province_tag p) {
			if(is_valid_index(p)) {
				auto acc = empty_province_accumulator(ws);
				auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
				for(auto a : adj_range) {
					acc.add_value(a.value);
					if(acc.result)
						return true;
				}
				acc.flush();
				return acc.result;
			} else {
				return false;
			}
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_leader) {
		auto name = trigger_payload(tval[2]).text;
		auto result = ve::apply(to_nation(primary_slot), [&ws, name](nations::country_tag n) {
			return military::has_named_leader(ws, n, name);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_ai) {
		return compare_to_false(tval[0], ve::widen_mask(to_value<nation::is_not_ai_controlled>(ws.w.nation_s.nations, to_nation(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_can_create_vassals) {
		return compare_to_true(tval[0], true); // stub for apparently unused 
	}
	TRIGGER_FUNCTION(tf_is_possible_vassal) {
		auto tag = trigger_payload(tval[2]).tag;
		auto result = ve::apply(to_nation(primary_slot), [&ws, tag](nations::country_tag n) {
			can_release_as_vassal(ws, n, tag);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_province_id) {
		return compare_values_eq(tval[0], to_prov(primary_slot), provinces::province_tag(tval[2]));
	}
	TRIGGER_FUNCTION(tf_vassal_of_tag) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), tag_holder);
	}
	TRIGGER_FUNCTION(tf_vassal_of_from) {
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), to_nation(from_slot));
	}
	TRIGGER_FUNCTION(tf_vassal_of_this_nation) {
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), to_nation(this_slot));
	}
	TRIGGER_FUNCTION(tf_vassal_of_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), owner);
	}
	TRIGGER_FUNCTION(tf_vassal_of_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), owner);
	}
	TRIGGER_FUNCTION(tf_vassal_of_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values_eq(tval[0], to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)), owner);
	}
	TRIGGER_FUNCTION(tf_substate_of_tag) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_substate>(ws.w.nation_s.nations, to_nation(primary_slot)))
			& (to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)) == tag_holder));
	}
	TRIGGER_FUNCTION(tf_substate_of_from) {
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_substate>(ws.w.nation_s.nations, to_nation(primary_slot)))
			& (to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)) == to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_substate_of_this_nation) {
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_substate>(ws.w.nation_s.nations, to_nation(primary_slot)))
			& (to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)) == to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_substate_of_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_substate>(ws.w.nation_s.nations, to_nation(primary_slot)))
			& (to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)) == owner));
	}
	TRIGGER_FUNCTION(tf_substate_of_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_substate>(ws.w.nation_s.nations, to_nation(primary_slot)))
			& (to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)) == owner));
	}
	TRIGGER_FUNCTION(tf_substate_of_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_substate>(ws.w.nation_s.nations, to_nation(primary_slot)))
			& (to_value<nation::overlord>(ws.w.nation_s.nations, to_nation(primary_slot)) == owner));
	}
	TRIGGER_FUNCTION(tf_alliance_with_tag) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		auto result = ve::apply(to_nation(primary_slot), [&ws, holder](nations::culture_tag n) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(holder), n);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_alliance_with_from) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(from_slot), [&ws](nations::culture_tag a, nations::culture_tag b) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_alliance_with_this_nation) {
		auto result = ve::apply(to_nation(primary_slot), to_nation(this_slot), [&ws](nations::culture_tag a, nations::culture_tag b) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_alliance_with_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws](nations::culture_tag a, nations::culture_tag b) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_alliance_with_this_state) {
		nations::country_tag owner = to_value<state::owner(>ws.w.nation_s.states, to_state(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws](nations::culture_tag a, nations::culture_tag b) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_alliance_with_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		auto result = ve::apply(to_nation(primary_slot), owner, [&ws](nations::culture_tag a, nations::culture_tag b) {
			return contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_has_recently_lost_war) {
		auto last_lost = to_value<nation::last_lost_war>(ws.w.nation_s.nations, to_nation(primary_slot));
		return compare_to_true(tval[0], is_valid_index(last_lost) & ((to_int(ws.w.current_date) - to_int(last_lost)) < (365 * 5 + 1)));
	}
	TRIGGER_FUNCTION(tf_is_mobilised) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<nation::is_mobilized>(ws.w.nation_s.nations, to_nation(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_mobilisation_size) {
		return compare_values(tval[0],
			to_indexed_value<modifiers::national_offsets::mobilisation_size>(ws.w.nation_s.modifier_values, to_nation(primary_slot)),
			read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_crime_higher_than_education_nation) {
		return compare_to_true(tval[0],
			to_value<nation::f_administrative_spending>(ws.w.nation_s.nations, to_nation(primary_slot))
			>= to_value<nation::f_education_spending>(ws.w.nation_s.nations, to_nation(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_crime_higher_than_education_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(primary_slot));
		return compare_to_true(tval[0],
			to_value<nation::f_administrative_spending>(ws.w.nation_s.nations, owner)
			>= to_value<nation::f_education_spending>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_crime_higher_than_education_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_to_true(tval[0],
			to_value<nation::f_administrative_spending>(ws.w.nation_s.nations, owner)
			>= to_value<nation::f_education_spending>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_crime_higher_than_education_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_to_true(tval[0],
			to_value<nation::f_administrative_spending>(ws.w.nation_s.nations, owner)
			>= to_value<nation::f_education_spending>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_agree_with_ruling_party) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		auto ruling_ideology = to_value<nation::ruling_ideology>(ws.w.nation_s.nations, owner);
		auto population_size = to_value<pop::size>(ws.w.population_s.pops, to_pop(primary_slot));
		auto ruling_support = ve::apply(to_pop(primary_slot), ruling_ideology, [&ws](population::pop_tag p, ideologies::ideology_tag i) {
			return ws.w.population_s.pop_demographics.get(pop_id, population::to_demo_tag(ws, i));
		});
		return compare_values(tval[0], ve::select(population_size > 0.0f, ruling_support / population_size, 0.0f), read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_is_colonial_state) {
		return compare_to_true(tval[0], nations::is_colonial_or_protectorate(ws, to_state(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_is_colonial_province) {
		auto si = provinces::province_state(ws, to_prov(primary_slot));
		return compare_to_true(tval[0], nations::is_colonial_or_protectorate(ws, si));
	}
	TRIGGER_FUNCTION(tf_has_factories) {
		auto result = ve::apply(to_state(primary_slot), [&ws](nations::state_tag s) {
			auto& factories = ws.w.nation_s.states.get<state::factories>(s);
			for(int32_t i = state::factories_count; i--; ) {
				if(factories[i].type)
					return true;
			}
			return false;
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_in_default_tag) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		return compare_to_true(tval[0],
			economy::is_bankrupt(ws, holder));
	}
	TRIGGER_FUNCTION(tf_in_default_from) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return economy::is_bankrupt(ws, n);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_in_default_this_nation) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return economy::is_bankrupt(ws, n);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_in_default_this_province) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return economy::is_bankrupt(ws, n);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_in_default_this_state) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return economy::is_bankrupt(ws, n);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_in_default_this_pop) {
		auto result = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			return economy::is_bankrupt(ws, n);
		});
		return compare_to_true(tval[0], result);
	}
	TRIGGER_FUNCTION(tf_total_num_of_ports) {
		return compare_values(tval[0], to_value<nation::num_ports>(ws.w.nation_s.nations, to_nation(primary_slot)), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_always) {
		return compare_to_true(tval[0], true);
	}
	TRIGGER_FUNCTION(tf_election) {
		return compare_to_true(tval[0],
			ve::widen_mask(to_value<nation::is_holding_election>(ws.w.nation_s.nations, to_nation(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_has_global_flag) {
		return compare_to_true(tval[0], ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] != 0.0f);
	}
	TRIGGER_FUNCTION(tf_is_capital) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_values_eq(tval[0], to_value<nation::current_capital>(ws.w.nation_s.nations, owner), to_prov(primary_slot));
	}
	TRIGGER_FUNCTION(tf_nationalvalue_nation) {
		return compare_values_eq(tval[0],
			to_value<nation::national_value>(ws.w.nation_s.nations, to_nation(primary_slot)),
			trigger_payload(tval[2]).nat_mod);
	}
	TRIGGER_FUNCTION(tf_nationalvalue_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::national_value>(ws.w.nation_s.nations, owner),
			trigger_payload(tval[2]).nat_mod);
	}
	TRIGGER_FUNCTION(tf_nationalvalue_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_values_eq(tval[0],
			to_value<nation::national_value>(ws.w.nation_s.nations, owner),
			trigger_payload(tval[2]).nat_mod);
	}
	TRIGGER_FUNCTION(tf_industrial_score_value) {
		return compare_values(tval[0],
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			int32_t(trigger_payload(tval[2]).signed_value));
	}
	TRIGGER_FUNCTION(tf_industrial_score_from_nation) {
		return compare_values(tval[0],
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_industrial_score_this_nation) {
		return compare_values(tval[0],
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_industrial_score_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values(tval[0],
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::industrial_score>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_industrial_score_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values(tval[0],
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::industrial_score>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_industrial_score_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values(tval[0],
			to_value<nation::industrial_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::industrial_score>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_military_score_value) {
		return compare_values(tval[0],
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			int32_t(trigger_payload(tval[2]).signed_value));
	}
	TRIGGER_FUNCTION(tf_military_score_from_nation) {
		return compare_values(tval[0],
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(from_slot)));
	}
	TRIGGER_FUNCTION(tf_military_score_this_nation) {
		return compare_values(tval[0],
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(this_slot)));
	}
	TRIGGER_FUNCTION(tf_military_score_this_pop) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		return compare_values(tval[0],
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::military_score>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_military_score_this_state) {
		auto owner = to_value<state::owner>(ws.w.nation_s.states, to_state(this_slot));
		return compare_values(tval[0],
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::military_score>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_military_score_this_province) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		return compare_values(tval[0],
			to_value<nation::military_score>(ws.w.nation_s.nations, to_nation(primary_slot)),
			to_value<nation::military_score>(ws.w.nation_s.nations, owner));
	}
	TRIGGER_FUNCTION(tf_civilized_nation) {
		return compare_to_true(tval[0], ve::widen_mask(to_value<nation::is_civilized>(ws.w.nation_s.nations, to_nation(primary_slot))));
	}
	TRIGGER_FUNCTION(tf_civilized_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		return compare_to_true(tval[0], ve::widen_mask(to_value<nation::is_civilized>(ws.w.nation_s.nations, owner)));
	}
	TRIGGER_FUNCTION(tf_civilized_province) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		return compare_to_true(tval[0], ve::widen_mask(to_value<nation::is_civilized>(ws.w.nation_s.nations, owner)));
	}
	TRIGGER_FUNCTION(tf_national_provinces_occupied) {
		auto fraction_occupied = ve::apply(to_nation(primary_slot), [&ws](nations::country_tag n) {
			auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
			if(owned_range.first != owned_range.second) {
				float count_owned_uncontrolled = 0.0f;
				for(auto p : owned_range) {
					if(provinces::province_controller(ws, p) != n)
						++count_owned_uncontrolled;
				}
				return count_owned_uncontrolled / float(owned_range.second - owned_range.first);
			} else {
				return 0.0f;
			}
		});
		return compare_values(tval[0], fraction_occupied, read_float_from_payload(tval + 2));
	}
	TRIGGER_FUNCTION(tf_is_greater_power_nation) {
		return compare_to_true(tval[0], nations::is_great_power(ws, to_nation(primary_slot)));
	}
	bool tf_is_greater_power_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return compare_values(tval[0], nations::is_great_power(ws, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_greater_power_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner)
			return compare_values(tval[0], nations::is_great_power(ws, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rich_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::rich_tax>(to_nation(primary_slot)), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_middle_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::middle_tax>(to_nation(primary_slot)), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_poor_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::poor_tax>(to_nation(primary_slot)), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_social_spending>(to_nation(primary_slot)), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_social_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return tf_social_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner)
			return tf_social_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_military_spending>(to_nation(primary_slot)), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_military_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_administrative_spending>(to_nation(primary_slot)), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_administration_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_education_spending>(to_nation(primary_slot)), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_education_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_colonial_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_colonial_nation>(to_nation(primary_slot)), true);
	}
	bool tf_pop_majority_religion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_religion>(to_nation(primary_slot)) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_religion_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_religion>(to_state(primary_slot)) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_religion_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_religion>(to_prov(primary_slot)) == trigger_payload(tval[2]).small.values.religion,
			true);
	}
	bool tf_pop_majority_culture_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_culture>(to_nation(primary_slot)) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_culture_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_culture>(to_state(primary_slot)) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_culture_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_culture>(to_prov(primary_slot)) == trigger_payload(tval[2]).culture,
			true);
	}
	bool tf_pop_majority_issue_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_issue>(to_nation(primary_slot)) == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_issue>(to_state(primary_slot)) == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_issue>(to_prov(primary_slot)) == trigger_payload(tval[2]).small.values.option,
			true);
	}
	bool tf_pop_majority_issue_pop(TRIGGER_PARAMTERS) {
		auto id = to_pop(primary_slot);
		if(ws.w.population_s.pops.is_valid_index(id))
			return compare_values(tval[0], population::is_dominant_issue(ws, id, trigger_payload(tval[2]).small.values.option), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_pop_majority_ideology_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_ideology>(to_nation(primary_slot)) == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_ideology>(to_state(primary_slot)) == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_ideology>(to_prov(primary_slot)) == trigger_payload(tval[2]).small.values.ideology,
			true);
	}
	bool tf_pop_majority_ideology_pop(TRIGGER_PARAMTERS) {
		auto id = to_pop(primary_slot);
		if(ws.w.population_s.pops.is_valid_index(id))
			return compare_values(tval[0], population::is_dominant_ideology(ws, id, trigger_payload(tval[2]).small.values.ideology), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_poor_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::poor_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::poor_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws)));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::poor_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(to_pop(primary_slot));
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor)
			return compare_values(tval[0], population::get_militancy_direct(ws, to_pop(primary_slot)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::middle_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::middle_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws)));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::middle_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(to_pop(primary_slot));
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle)
			return compare_values(tval[0], population::get_militancy_direct(ws, to_pop(primary_slot)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::rich_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::rich_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws)));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::rich_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(to_pop(primary_slot));
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich)
			return compare_values(tval[0], population::get_militancy_direct(ws, to_pop(primary_slot)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_tax_above_poor(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::rich_tax>(to_nation(primary_slot)) > ws.w.nation_s.nations.get<nation::poor_tax>(to_nation(primary_slot)), true);
	}
	bool tf_culture_has_union_tag_pop(TRIGGER_PARAMTERS) {
		auto pop_culture = ws.w.population_s.pops.get<pop::culture>(to_pop(primary_slot));
		if(is_valid_index(pop_culture)) {
			auto group = ws.s.culture_m.culture_container[pop_culture].group;
			return compare_values(tval[0], is_valid_index(ws.s.culture_m.culture_groups[group].union_tag), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_culture_has_union_tag_nation(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], is_valid_index(ws.s.culture_m.culture_groups[group].union_tag), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_tag(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == trigger_payload(tval[2]).tag, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_from(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == ws.w.nation_s.nations.get<nation::tag>(to_nation(from_slot)), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_this_nation(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == ws.w.nation_s.nations.get<nation::tag>(to_nation(this_slot)), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_nation(TRIGGER_PARAMTERS) {
		auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
		auto this_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot));
		return compare_values(tval[0],
			is_valid_index(prim_culture) &&
			is_valid_index(this_culture) &&
			ws.s.culture_m.culture_container[prim_culture].group == ws.s.culture_m.culture_container[this_culture].group, true);
	}
	bool tf_this_culture_union_this_union_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_minorities_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto accepted_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot))));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(to_nation(primary_slot)));
			for(auto c : accepted_range)
				accepted_pop += ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_minorities_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
		if(ws.w.nation_s.states.is_valid_index(id) && is_valid_index(owner)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto accepted_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, ws.w.nation_s.nations.get<nation::primary_culture>(owner)));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(owner));
			for(auto c : accepted_range)
				accepted_pop += ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_minorities_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner) {
			auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
			auto accepted_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, ws.w.nation_s.nations.get<nation::primary_culture>(owner)));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(owner));
			for(auto c : accepted_range)
				accepted_pop += ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_revanchism_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::revanchism>(to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_revanchism_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::revanchism>(owner), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_crime(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.province_s.province_state_container.get<province_state::crime>(to_prov(primary_slot))), true);
	}
	bool tf_num_of_substates(TRIGGER_PARAMTERS) {
		auto vassal_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(to_nation(primary_slot)));
		uint32_t count_substates = 0ui32;
		for(auto v : vassal_range) {
			if(is_valid_index(v) && ws.w.nation_s.nations.get<nation::is_substate>(v))
				++count_substates;
		}
		return compare_values(tval[0], count_substates, uint32_t(tval[2]));
	}
	bool tf_num_of_vassals_no_substates(TRIGGER_PARAMTERS) {
		auto vassal_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(to_nation(primary_slot)));
		uint32_t count_non_substates = 0ui32;
		for(auto v : vassal_range) {
			if(is_valid_index(v) && ws.w.nation_s.nations.get<nation::is_substate>(v))
				++count_non_substates;
		}
		return compare_values(tval[0], count_non_substates, uint32_t(tval[2]));
	}
	bool tf_brigades_compare_this(TRIGGER_PARAMTERS) {
		auto main_brigades = military::total_active_divisions(ws, to_nation(primary_slot));
		auto this_brigades = military::total_active_divisions(ws, to_nation(this_slot));
		return compare_values(tval[0],
			this_brigades != 0 ? float(main_brigades) / float(this_brigades) : 1'000'000.0f,
			read_float_from_payload(tval + 2));
	}
	bool tf_brigades_compare_from(TRIGGER_PARAMTERS) {
		auto main_brigades = military::total_active_divisions(ws, to_nation(primary_slot));
		auto from_brigades = military::total_active_divisions(ws, to_nation(from_slot));
		return compare_values(tval[0],
			from_brigades != 0 ? float(main_brigades) / float(from_brigades) : 1'000'000.0f,
			read_float_from_payload(tval + 2));
	}
	bool tf_constructing_cb_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(to_nation(primary_slot)) == tag_holder, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(to_nation(primary_slot)) == to_nation(from_slot), true);
	}
	bool tf_constructing_cb_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(to_nation(primary_slot)) == to_nation(this_slot), true);
	}
	bool tf_constructing_cb_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(to_nation(primary_slot)) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(to_nation(primary_slot)) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(to_nation(primary_slot)) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_discovered(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_discovered>(to_nation(primary_slot)), true);
	}
	bool tf_constructing_cb_progress(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_progress>(to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_civilization_progress(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.modifier_values.get<modifiers::national_offsets::civilization_progress_modifier>(to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_constructing_cb_type(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_type>(to_nation(primary_slot)) == trigger_payload(tval[2]).small.values.cb_type, true);
	}
	bool tf_is_our_vassal_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(tag_holder) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(to_nation(from_slot)) == to_nation(primary_slot), true);
	}
	bool tf_is_our_vassal_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(to_nation(this_slot)) == to_nation(primary_slot), true);
	}
	bool tf_is_our_vassal_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(owner) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(owner) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(owner) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_substate(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_substate>(to_nation(primary_slot)), true);
	}
	bool tf_great_wars_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.great_wars_enabled, true);
	}
	bool tf_can_nationalize(TRIGGER_PARAMTERS) {
		auto influencer_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(to_nation(primary_slot)));
		auto this_id = to_nation(primary_slot);
		for(auto c : influencer_range) {
			if(ws.w.nation_s.nations.is_valid_index(c) && nations::get_foreign_investment(ws, c, this_id) != 0.0f)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_part_of_sphere(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.nation_s.nations.get<nation::sphere_leader>(to_nation(primary_slot))), true);
	}
	bool tf_is_sphere_leader_of_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(holder) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(to_nation(from_slot)) == to_nation(primary_slot), true);
	}
	bool tf_is_sphere_leader_of_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(to_nation(this_slot)) == to_nation(primary_slot), true);
	}
	bool tf_is_sphere_leader_of_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(owner) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(owner) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(owner) == to_nation(primary_slot), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_number_of_states(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(to_nation(primary_slot))), uint32_t(tval[2]));
	}
	bool tf_war_score(TRIGGER_PARAMTERS) {
		//stub for apparently unused trigger
		return compare_values(tval[0], true, true);
	}
	bool tf_is_releasable_vassal_from(TRIGGER_PARAMTERS) {
		auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(from_slot));
		if(is_valid_index(tag))
			return compare_values(tval[0], !(ws.w.culture_s.national_tags_state[tag].is_not_releasable), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_releasable_vassal_other(TRIGGER_PARAMTERS) {
		auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot));
		if(is_valid_index(tag))
			return compare_values(tval[0], !(ws.w.culture_s.national_tags_state[tag].is_not_releasable), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_recent_imigration(TRIGGER_PARAMTERS) {
		auto last_immigration = ws.w.province_s.province_state_container.get<province_state::last_immigration>(to_prov(primary_slot));
		if(!is_valid_index(last_immigration))
			return compare_values(tval[0], std::numeric_limits<uint16_t>::max(), tval[2]);
		else
			return compare_values(tval[0], to_index(ws.w.current_date) - to_index(last_immigration), int32_t(tval[2]));
	}
	bool tf_province_control_days(TRIGGER_PARAMTERS) {
		auto last_control_change = ws.w.province_s.province_state_container.get<province_state::last_controller_change>(to_prov(primary_slot));
		if(!is_valid_index(last_control_change))
			return compare_values(tval[0], std::numeric_limits<uint16_t>::max(), tval[2]);
		else
			return compare_values(tval[0], to_index(ws.w.current_date) - to_index(last_control_change), int32_t(tval[2]));
	}
	bool tf_is_disarmed(TRIGGER_PARAMTERS) {
		auto disarmed_date = ws.w.nation_s.nations.get<nation::disarmed_until>(to_nation(primary_slot));
		return compare_values(tval[0], is_valid_index(disarmed_date) && ws.w.current_date < disarmed_date, true);
	}
	bool tf_big_producer(TRIGGER_PARAMTERS) {
		// stub: used only rarely in ai chances and would be expensive to test
		return compare_values(tval[0], false, true);
	}
	bool tf_someone_can_form_union_tag_from(TRIGGER_PARAMTERS) {
		// stub: apparently unused
		return compare_values(tval[0], false, true);
	}
	bool tf_someone_can_form_union_tag_other(TRIGGER_PARAMTERS) {
		// stub: apparently unused
		return compare_values(tval[0], false, true);
	}
	bool tf_social_movement_strength(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::social_movement_support>(to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_political_movement_strength(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::political_movement_support>(to_nation(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_can_build_factory_in_capital_state(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_social_movement(TRIGGER_PARAMTERS) {
		auto mt = ws.w.population_s.pops.get<pop::movement>(to_pop(primary_slot));
		if(is_valid_index(mt))
			return compare_values(tval[0], ws.w.population_s.pop_movements[mt].type == population::movement_type::social, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_political_movement(TRIGGER_PARAMTERS) {
		auto mt = ws.w.population_s.pops.get<pop::movement>(to_pop(primary_slot));
		if(is_valid_index(mt))
			return compare_values(tval[0], ws.w.population_s.pop_movements[mt].type == population::movement_type::political, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_cultural_sphere(TRIGGER_PARAMTERS) {
		auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
		auto culture_group = is_valid_index(prim_culture) ? ws.s.culture_m.culture_container[prim_culture].group : cultures::culture_group_tag();
		auto sphere_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(to_nation(primary_slot)));
		for(auto c : sphere_range) {
			if(is_valid_index(c)) {
				auto sc = ws.w.nation_s.nations.get<nation::primary_culture>(c);
				if(is_valid_index(sc) && culture_group == ws.s.culture_m.culture_container[sc].group)
					return compare_values(tval[0], true, true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_world_wars_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.world_wars_enabled, true);
	}
	bool tf_has_pop_culture_pop_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(to_pop(primary_slot)) == ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot)), true);
	}
	bool tf_has_pop_culture_state_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot));
		auto id = to_state(primary_slot);
		if(is_valid_index(culture) && ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_province_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot));
		auto id = to_prov(primary_slot);
		if(is_valid_index(culture))
			return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_nation_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot));
		auto id = to_nation(primary_slot);
		if(is_valid_index(culture) && ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(to_pop(primary_slot)) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_has_pop_culture_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
	}
	bool tf_has_pop_culture_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_pop_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(to_pop(primary_slot)) == ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot)), true);
	}
	bool tf_has_pop_religion_state_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot));
		auto id = to_state(primary_slot);
		if(is_valid_index(religion) && ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_province_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot));
		auto id = to_prov(primary_slot);
		if(is_valid_index(religion))
			return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_nation_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot));
		auto id = to_nation(primary_slot);
		if(is_valid_index(religion) && ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(to_pop(primary_slot)) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_has_pop_religion_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
	}
	bool tf_has_pop_religion_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_life_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::needs_satisfaction>(to_pop(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_everyday_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::needs_satisfaction>(to_pop(primary_slot)), read_float_from_payload(tval + 2) + 1.0f);
	}
	bool tf_luxury_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::needs_satisfaction>(to_pop(primary_slot)), read_float_from_payload(tval + 2) + 2.0f);
	}
	bool tf_consciousness_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], population::get_consciousness_direct(ws, to_pop(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		auto con = ws.w.province_s.province_demographics.get(id, population::consciousness_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) * 10.0f / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto con = ws.w.nation_s.state_demographics.get(id, population::consciousness_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::consciousness_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], population::get_literacy_direct(ws, to_pop(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_literacy_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		auto con = ws.w.province_s.province_demographics.get(id, population::literacy_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto con = ws.w.nation_s.state_demographics.get(id, population::literacy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::literacy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], population::get_militancy_direct(ws, to_pop(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_militancy_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		auto con = ws.w.province_s.province_demographics.get(id, population::militancy_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) * 10.0f / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto con = ws.w.nation_s.state_demographics.get(id, population::militancy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::militancy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_trade_goods_in_state_state(TRIGGER_PARAMTERS) {
		auto state_region = ws.w.nation_s.states.get<state::region_id>(to_state(primary_slot));
		if(is_valid_index(state_region)) {
			auto prov_range = ws.s.province_m.states_to_province_index.get_row(state_region);
			for(auto p : prov_range) {
				if(provinces::province_state(ws, p) == to_state(primary_slot)
					&& ws.w.province_s.province_state_container.get<province_state::rgo_production>(p) == trigger_payload(tval[2]).small.values.good) {

					return compare_values(tval[0], true, true);
				}
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_trade_goods_in_state_province(TRIGGER_PARAMTERS) {
		auto si = provinces::province_state(ws, to_prov(primary_slot));
		if(si)
			return tf_trade_goods_in_state_state(tval, ws, si, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_flashpoint(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.nation_s.states.get<state::crisis_tag>(to_state(primary_slot))), true);
	}
	bool tf_flashpoint_tension(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::current_tension>(to_state(primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_crisis_exist(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.type != current_state::crisis_type::none, true);
	}
	bool tf_is_liberation_crisis(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.type == current_state::crisis_type::liberation, true);
	}
	bool tf_is_claim_crisis(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.type == current_state::crisis_type::claim, true);
	}
	bool tf_crisis_temperature(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.temperature, read_float_from_payload(tval + 2));
	}
	bool tf_involved_in_crisis_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		return compare_values(tval[0],
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, id) ||
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, id) ||
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, id),
			true);
	}
	bool tf_involved_in_crisis_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return tf_involved_in_crisis_nation(nullptr, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rich_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_rich_strata_life_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_rich_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_rich_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_middle_strata_life_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_middle_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_middle_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_poor_strata_life_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_poor_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_poor_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_diplomatic_influence_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[3]).tag].holder;
		if(holder) 
			return compare_values(tval[0], nations::get_influence_value(ws, to_nation(primary_slot), holder), int32_t(tval[2]));
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_diplomatic_influence_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_influence_value(ws, to_nation(primary_slot), to_nation(this_slot)), int32_t(tval[2]));
	}
	bool tf_diplomatic_influence_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return tf_diplomatic_influence_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_diplomatic_influence_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_influence_value(ws, to_nation(primary_slot), to_nation(from_slot)), int32_t(tval[2]));
	}
	bool tf_diplomatic_influence_from_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(from_slot));
		if(owner)
			return tf_diplomatic_influence_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_pop_unemployment_nation(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_state(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_province(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type));
		if(total_pop != 0)
			return compare_values(tval[0], 1.0f - float(ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_pop(TRIGGER_PARAMTERS) {
		auto id = to_pop(primary_slot);
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_size = ws.w.population_s.pops.get<pop::size>(id);
			if(total_size != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.population_s.pop_demographics.get(id, population::total_employment_tag)) / float(total_size), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_nation_this_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(to_pop(this_slot));
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_state_this_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(to_pop(this_slot));
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_province_this_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(to_pop(this_slot));
		auto id = to_prov(primary_slot);
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type));
		if(total_pop != 0)
			return compare_values(tval[0], 1.0f - float(ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_relation_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[3]).tag].holder;
		if(holder)
			return compare_values(tval[0], nations::get_relationship(ws, to_nation(primary_slot), holder), int32_t(trigger_payload(tval[2]).signed_value));
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_relation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_relationship(ws, to_nation(primary_slot), to_nation(this_slot)), int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_relation_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return tf_relation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_relation_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_relationship(ws, to_nation(primary_slot), to_nation(from_slot)), int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_relation_from_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(from_slot));
		if(owner)
			return tf_relation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_check_variable(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], ws.w.nation_s.national_variables.get(id, triggers::trigger_payload(tval[3]).nat_var), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_upper_house(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.upper_house.get(id, triggers::trigger_payload(tval[3]).small.values.ideology)) / 100.0f, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_by_type_nation(TRIGGER_PARAMTERS) {
		return tf_pop_unemployment_nation(tval, ws, primary_slot, nullptr, nullptr);
	}
	bool tf_unemployment_by_type_state(TRIGGER_PARAMTERS) {
		return tf_pop_unemployment_state(tval, ws, primary_slot, nullptr, nullptr);
	}
	bool tf_unemployment_by_type_province(TRIGGER_PARAMTERS) {
		return tf_pop_unemployment_province(tval, ws, primary_slot, nullptr, nullptr);
	}
	bool tf_unemployment_by_type_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_unemployment_by_type_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_nation_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_from_nation_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_province_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_from_province_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_nation_from_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(to_prov(from_slot), trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_generic(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(to_prov(primary_slot), trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_from_nation_scope_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(to_prov(primary_slot), trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_from_province_scope_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(to_prov(primary_slot), trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_can_build_in_province_railroad_no_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(to_prov(primary_slot))) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(to_prov(primary_slot)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(to_nation(from_slot)),
			true);
	}
	bool tf_can_build_in_province_railroad_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(to_prov(primary_slot))) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(to_prov(primary_slot)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(to_nation(from_slot)),
			true);
	}
	bool tf_can_build_in_province_railroad_no_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(to_prov(primary_slot))) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(to_prov(primary_slot)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(to_nation(this_slot)),
			true);
	}
	bool tf_can_build_in_province_railroad_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(to_prov(primary_slot))) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(to_prov(primary_slot)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(to_nation(this_slot)),
			true);
	}
	bool tf_can_build_in_province_fort_no_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(to_prov(primary_slot))) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(to_nation(from_slot)),
			true);
	}
	bool tf_can_build_in_province_fort_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(to_prov(primary_slot))) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(to_nation(from_slot)),
			true);
	}
	bool tf_can_build_in_province_fort_no_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(to_prov(primary_slot))) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(to_nation(this_slot)),
			true);
	}
	bool tf_can_build_in_province_fort_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(to_prov(primary_slot))) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(to_nation(this_slot)),
			true);
	}
	bool tf_can_build_in_province_naval_base_no_limit_from_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(to_prov(primary_slot));
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(from_slot)),
				true);
		} else if(auto si = provinces::province_state(ws, to_prov(primary_slot)); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(to_prov(primary_slot)) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(from_slot)) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(to_prov(primary_slot));
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(from_slot)),
				true);
		} else if(auto si = provinces::province_state(ws, to_prov(primary_slot)); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(to_prov(primary_slot)) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(from_slot)) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_no_limit_this_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(to_prov(primary_slot));
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(this_slot)),
				true);
		} else if(auto si = provinces::province_state(ws, to_prov(primary_slot)); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(to_prov(primary_slot)) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(this_slot)) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(to_prov(primary_slot));
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(this_slot)),
				true);
		} else if(auto si = provinces::province_state(ws, to_prov(primary_slot)); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(to_prov(primary_slot)) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(to_nation(this_slot)) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_railway_in_capital_yes_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_railway_in_capital_yes_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_railway_in_capital_no_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_railway_in_capital_no_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_yes_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_yes_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_no_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_no_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_work_available_nation(TRIGGER_PARAMTERS) {
		auto count_workers = tval[1] - 1;
		auto id = to_nation(primary_slot);

		if(!ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], false, true);

		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			if(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type)) != 0 &&
				ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type)) == 0) {
				return compare_values(tval[0], false, true);
			}
		}
		return compare_values(tval[0], true, true);
	}
	bool tf_work_available_state(TRIGGER_PARAMTERS) {
		auto count_workers = tval[1] - 1;
		auto id = to_state(primary_slot);

		if(!ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], false, true);

		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			if(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type)) != 0 &&
				ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type)) == 0) {
				return compare_values(tval[0], false, true);
			}
		}
		return compare_values(tval[0], true, true);
	}
	bool tf_work_available_province(TRIGGER_PARAMTERS) {
		auto count_workers = tval[1] - 1;
		auto id = to_prov(primary_slot);

		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			if(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type)) != 0 &&
				ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type)) == 0) {
				return compare_values(tval[0], false, true);
			}
		}
		return compare_values(tval[0], true, true);
	}
	bool tf_variable_ideology_name_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);

		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_pop(TRIGGER_PARAMTERS) {
		auto id = to_pop(primary_slot);
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_pop = ws.w.population_s.pops.get<pop::size>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.population_s.pop_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);

		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_pop(TRIGGER_PARAMTERS) {
		auto id = to_pop(primary_slot);
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_pop = ws.w.population_s.pops.get<pop::size>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.population_s.pop_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_group_name_nation(TRIGGER_PARAMTERS) {
		auto option = trigger_payload(tval[2]).small.values.option;
		auto issue = ws.s.issues_m.options[option].parent_issue;

		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], ws.w.nation_s.active_issue_options.get(id, issue) == option, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(primary_slot));
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_pop_type_name_nation(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_state(TRIGGER_PARAMTERS) {
		auto id = to_state(primary_slot);
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_province(TRIGGER_PARAMTERS) {
		auto id = to_prov(primary_slot);

		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_pop(TRIGGER_PARAMTERS) {
		if(ws.w.population_s.pops.get<pop::type>(to_pop(primary_slot)) == trigger_payload(tval[2]).small.values.pop_type)
			return compare_values(tval[0], 1.0f, read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_good_name(TRIGGER_PARAMTERS) {
		auto id = to_nation(primary_slot);
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.national_stockpiles.get(id, trigger_payload(tval[2]).small.values.good)), read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_religion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(to_nation(primary_slot)) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_religion_nation_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(to_nation(primary_slot)) == ws.w.population_s.rebel_factions[to_rebel(from_slot)].religion, true);
	}
	bool tf_religion_nation_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(to_nation(primary_slot)) == ws.w.nation_s.nations.get<nation::national_religion>(to_nation(from_slot)), true);
	}
	bool tf_religion_nation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(to_nation(primary_slot)) == ws.w.nation_s.nations.get<nation::national_religion>(to_nation(this_slot)), true);
	}
	bool tf_religion_nation_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_nation_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, to_prov(this_slot));
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_nation_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, to_pop(this_slot));
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}

	static bool(*trigger_functions[])(TRIGGER_PARAMTERS) = {
		tf_none,
		tf_year,
		tf_month,
		tf_port,
		tf_rank,
		tf_technology,
		tf_strata_rich,
		tf_life_rating_province,
		tf_life_rating_state,
		tf_has_empty_adjacent_state_province,
		tf_has_empty_adjacent_state_state,
		tf_state_id_province,
		tf_state_id_state,
		tf_cash_reserves,
		tf_unemployment_nation,
		tf_unemployment_state,
		tf_unemployment_province,
		tf_unemployment_pop,
		tf_is_slave_nation,
		tf_is_slave_state,
		tf_is_slave_province,
		tf_is_slave_pop,
		tf_is_independant,
		tf_has_national_minority_province,
		tf_has_national_minority_state,
		tf_has_national_minority_nation,
		tf_government_nation,
		tf_government_pop,
		tf_capital,
		tf_tech_school,
		tf_primary_culture,
		tf_accepted_culture,
		tf_culture_pop,
		tf_culture_state,
		tf_culture_province,
		tf_culture_nation,
		tf_culture_pop_reb,
		tf_culture_state_reb,
		tf_culture_province_reb,
		tf_culture_nation_reb,
		tf_culture_from_nation,
		tf_culture_this_nation,
		tf_culture_this_state,
		tf_culture_this_pop,
		tf_culture_this_province,
		tf_culture_group_nation,
		tf_culture_group_pop,
		tf_culture_group_reb_nation,
		tf_culture_group_reb_pop,
		tf_culture_group_nation_from_nation,
		tf_culture_group_pop_from_nation,
		tf_culture_group_nation_this_nation,
		tf_culture_group_pop_this_nation,
		tf_culture_group_nation_this_province,
		tf_culture_group_pop_this_province,
		tf_culture_group_nation_this_state,
		tf_culture_group_pop_this_state,
		tf_culture_group_nation_this_pop,
		tf_culture_group_pop_this_pop,
		tf_religion,
		tf_religion_reb,
		tf_religion_from_nation,
		tf_religion_this_nation,
		tf_religion_this_state,
		tf_religion_this_province,
		tf_religion_this_pop,
		tf_terrain_province,
		tf_terrain_pop,
		tf_trade_goods,
		tf_is_secondary_power_pop,
		tf_is_secondary_power_nation,
		tf_has_faction_nation,
		tf_has_faction_pop,
		tf_has_unclaimed_cores,
		tf_is_cultural_union_bool,
		tf_is_cultural_union_this_self_pop,
		tf_is_cultural_union_this_pop,
		tf_is_cultural_union_this_state,
		tf_is_cultural_union_this_province,
		tf_is_cultural_union_this_nation,
		tf_is_cultural_union_this_rebel,
		tf_is_cultural_union_tag_nation,
		tf_is_cultural_union_tag_this_pop,
		tf_is_cultural_union_tag_this_state,
		tf_is_cultural_union_tag_this_province,
		tf_is_cultural_union_tag_this_nation,
		tf_can_build_factory_pop,
		tf_war_pop,
		tf_war_nation,
		tf_war_exhaustion_nation,
		tf_blockade,
		tf_owns,
		tf_controls,
		tf_is_core_integer,
		tf_is_core_this_nation,
		tf_is_core_this_state,
		tf_is_core_this_province,
		tf_is_core_this_pop,
		tf_is_core_from_nation,
		tf_is_core_reb,
		tf_is_core_tag,
		tf_num_of_revolts,
		tf_revolt_percentage,
		tf_num_of_cities_int,
		tf_num_of_cities_from_nation,
		tf_num_of_cities_this_nation,
		tf_num_of_cities_this_state,
		tf_num_of_cities_this_province,
		tf_num_of_cities_this_pop,
		tf_num_of_ports,
		tf_num_of_allies,
		tf_num_of_vassals,
		tf_owned_by_tag,
		tf_owned_by_from_nation,
		tf_owned_by_this_nation,
		tf_owned_by_this_province,
		tf_owned_by_this_state,
		tf_owned_by_this_pop,
		tf_exists_bool,
		tf_exists_tag,
		tf_has_country_flag,
		tf_continent_nation,
		tf_continent_state,
		tf_continent_province,
		tf_continent_pop,
		tf_continent_nation_this,
		tf_continent_state_this,
		tf_continent_province_this,
		tf_continent_pop_this,
		tf_continent_nation_from,
		tf_continent_state_from,
		tf_continent_province_from,
		tf_continent_pop_from,
		tf_casus_belli_tag,
		tf_casus_belli_from,
		tf_casus_belli_this_nation,
		tf_casus_belli_this_state,
		tf_casus_belli_this_province,
		tf_casus_belli_this_pop,
		tf_military_access_tag,
		tf_military_access_from,
		tf_military_access_this_nation,
		tf_military_access_this_state,
		tf_military_access_this_province,
		tf_military_access_this_pop,
		tf_prestige_value,
		tf_prestige_from,
		tf_prestige_this_nation,
		tf_prestige_this_state,
		tf_prestige_this_province,
		tf_prestige_this_pop,
		tf_badboy,
		tf_has_building_state,
		tf_has_building_fort,
		tf_has_building_railroad,
		tf_has_building_naval_base,
		tf_empty,
		tf_is_blockaded,
		tf_has_country_modifier,
		tf_has_province_modifier,
		tf_region,
		tf_tag_tag,
		tf_tag_this_nation,
		tf_tag_this_province,
		tf_tag_from_nation,
		tf_tag_from_province,
		tf_neighbour_tag,
		tf_neighbour_this,
		tf_neighbour_from,
		tf_units_in_province_value,
		tf_units_in_province_from,
		tf_units_in_province_this_nation,
		tf_units_in_province_this_province,
		tf_units_in_province_this_state,
		tf_units_in_province_this_pop,
		tf_war_with_tag,
		tf_war_with_from,
		tf_war_with_this_nation,
		tf_war_with_this_province,
		tf_war_with_this_state,
		tf_war_with_this_pop,
		tf_unit_in_battle,
		tf_total_amount_of_divisions,
		tf_money,
		tf_lost_national,
		tf_is_vassal,
		tf_ruling_party_ideology_pop,
		tf_ruling_party_ideology_nation,
		tf_ruling_party,
		tf_is_ideology_enabled,
		tf_political_reform_want_nation,
		tf_political_reform_want_pop,
		tf_social_reform_want_nation,
		tf_social_reform_want_pop,
		tf_total_amount_of_ships,
		tf_plurality,
		tf_corruption,
		tf_is_state_religion_pop,
		tf_is_state_religion_province,
		tf_is_state_religion_state,
		tf_is_primary_culture_pop,
		tf_is_primary_culture_province,
		tf_is_primary_culture_state,
		tf_is_primary_culture_nation_this_pop,
		tf_is_primary_culture_nation_this_nation,
		tf_is_primary_culture_nation_this_state,
		tf_is_primary_culture_nation_this_province,
		tf_is_primary_culture_state_this_pop,
		tf_is_primary_culture_state_this_nation,
		tf_is_primary_culture_state_this_state,
		tf_is_primary_culture_state_this_province,
		tf_is_primary_culture_province_this_pop,
		tf_is_primary_culture_province_this_nation,
		tf_is_primary_culture_province_this_state,
		tf_is_primary_culture_province_this_province,
		tf_is_primary_culture_pop_this_pop,
		tf_is_primary_culture_pop_this_nation,
		tf_is_primary_culture_pop_this_state,
		tf_is_primary_culture_pop_this_province,
		tf_is_accepted_culture_pop,
		tf_is_accepted_culture_province,
		tf_is_accepted_culture_state,
		tf_is_coastal,
		tf_in_sphere_tag,
		tf_in_sphere_from,
		tf_in_sphere_this_nation,
		tf_in_sphere_this_province,
		tf_in_sphere_this_state,
		tf_in_sphere_this_pop,
		tf_produces_nation,
		tf_produces_state,
		tf_produces_province,
		tf_produces_pop,
		tf_average_militancy_nation,
		tf_average_militancy_state,
		tf_average_militancy_province,
		tf_average_consciousness_nation,
		tf_average_consciousness_state,
		tf_average_consciousness_province,
		tf_is_next_reform_nation,
		tf_is_next_reform_pop,
		tf_rebel_power_fraction,
		tf_recruited_percentage_nation,
		tf_recruited_percentage_pop,
		tf_has_culture_core,
		tf_nationalism,
		tf_is_overseas,
		tf_controlled_by_rebels,
		tf_controlled_by_tag,
		tf_controlled_by_from,
		tf_controlled_by_this_nation,
		tf_controlled_by_this_province,
		tf_controlled_by_this_state,
		tf_controlled_by_this_pop,
		tf_controlled_by_owner,
		tf_controlled_by_reb,
		tf_is_canal_enabled,
		tf_is_state_capital,
		tf_truce_with_tag,
		tf_truce_with_from,
		tf_truce_with_this_nation,
		tf_truce_with_this_province,
		tf_truce_with_this_state,
		tf_truce_with_this_pop,
		tf_total_pops_nation,
		tf_total_pops_state,
		tf_total_pops_province,
		tf_total_pops_pop,
		tf_has_pop_type_nation,
		tf_has_pop_type_state,
		tf_has_pop_type_province,
		tf_has_pop_type_pop,
		tf_has_empty_adjacent_province,
		tf_has_leader,
		tf_ai,
		tf_can_create_vassals,
		tf_is_possible_vassal,
		tf_province_id,
		tf_vassal_of_tag,
		tf_vassal_of_from,
		tf_vassal_of_this_nation,
		tf_vassal_of_this_province,
		tf_vassal_of_this_state,
		tf_vassal_of_this_pop,
		tf_alliance_with_tag,
		tf_alliance_with_from,
		tf_alliance_with_this_nation,
		tf_alliance_with_this_province,
		tf_alliance_with_this_state,
		tf_alliance_with_this_pop,
		tf_has_recently_lost_war,
		tf_is_mobilised,
		tf_mobilisation_size,
		tf_crime_higher_than_education_nation,
		tf_crime_higher_than_education_state,
		tf_crime_higher_than_education_province,
		tf_crime_higher_than_education_pop,
		tf_agree_with_ruling_party,
		tf_is_colonial_state,
		tf_is_colonial_province,
		tf_has_factories,
		tf_in_default_tag,
		tf_in_default_from,
		tf_in_default_this_nation,
		tf_in_default_this_province,
		tf_in_default_this_state,
		tf_in_default_this_pop,
		tf_total_num_of_ports,
		tf_always,
		tf_election,
		tf_has_global_flag,
		tf_is_capital,
		tf_nationalvalue_nation,
		tf_industrial_score_value,
		tf_industrial_score_from_nation,
		tf_industrial_score_this_nation,
		tf_industrial_score_this_pop,
		tf_industrial_score_this_state,
		tf_industrial_score_this_province,
		tf_military_score_value,
		tf_military_score_from_nation,
		tf_military_score_this_nation,
		tf_military_score_this_pop,
		tf_military_score_this_state,
		tf_military_score_this_province,
		tf_civilized_nation,
		tf_civilized_pop,
		tf_civilized_province,
		tf_national_provinces_occupied,
		tf_is_greater_power_nation,
		tf_is_greater_power_pop,
		tf_rich_tax,
		tf_middle_tax,
		tf_poor_tax,
		tf_social_spending_nation,
		tf_social_spending_pop,
		tf_social_spending_province,
		tf_colonial_nation,
		tf_pop_majority_religion_nation,
		tf_pop_majority_religion_state,
		tf_pop_majority_religion_province,
		tf_pop_majority_culture_nation,
		tf_pop_majority_culture_state,
		tf_pop_majority_culture_province,
		tf_pop_majority_issue_nation,
		tf_pop_majority_issue_state,
		tf_pop_majority_issue_province,
		tf_pop_majority_issue_pop,
		tf_pop_majority_ideology_nation,
		tf_pop_majority_ideology_state,
		tf_pop_majority_ideology_province,
		tf_pop_majority_ideology_pop,
		tf_poor_strata_militancy_nation,
		tf_poor_strata_militancy_state,
		tf_poor_strata_militancy_province,
		tf_poor_strata_militancy_pop,
		tf_middle_strata_militancy_nation,
		tf_middle_strata_militancy_state,
		tf_middle_strata_militancy_province,
		tf_middle_strata_militancy_pop,
		tf_rich_strata_militancy_nation,
		tf_rich_strata_militancy_state,
		tf_rich_strata_militancy_province,
		tf_rich_strata_militancy_pop,
		tf_rich_tax_above_poor,
		tf_culture_has_union_tag_pop,
		tf_culture_has_union_tag_nation,
		tf_this_culture_union_tag,
		tf_this_culture_union_from,
		tf_this_culture_union_this_nation,
		tf_this_culture_union_this_province,
		tf_this_culture_union_this_state,
		tf_this_culture_union_this_pop,
		tf_this_culture_union_this_union_nation,
		tf_this_culture_union_this_union_province,
		tf_this_culture_union_this_union_state,
		tf_this_culture_union_this_union_pop,
		tf_minorities_nation,
		tf_minorities_state,
		tf_minorities_province,
		tf_revanchism_nation,
		tf_revanchism_pop,
		tf_has_crime,
		tf_num_of_substates,
		tf_num_of_vassals_no_substates,
		tf_brigades_compare_this,
		tf_brigades_compare_from,
		tf_constructing_cb_tag,
		tf_constructing_cb_from,
		tf_constructing_cb_this_nation,
		tf_constructing_cb_this_province,
		tf_constructing_cb_this_state,
		tf_constructing_cb_this_pop,
		tf_constructing_cb_discovered,
		tf_constructing_cb_progress,
		tf_civilization_progress,
		tf_constructing_cb_type,
		tf_is_our_vassal_tag,
		tf_is_our_vassal_from,
		tf_is_our_vassal_this_nation,
		tf_is_our_vassal_this_province,
		tf_is_our_vassal_this_state,
		tf_is_our_vassal_this_pop,
		tf_substate_of_tag,
		tf_substate_of_from,
		tf_substate_of_this_nation,
		tf_substate_of_this_province,
		tf_substate_of_this_state,
		tf_substate_of_this_pop,
		tf_is_substate,
		tf_great_wars_enabled,
		tf_can_nationalize,
		tf_part_of_sphere,
		tf_is_sphere_leader_of_tag,
		tf_is_sphere_leader_of_from,
		tf_is_sphere_leader_of_this_nation,
		tf_is_sphere_leader_of_this_province,
		tf_is_sphere_leader_of_this_state,
		tf_is_sphere_leader_of_this_pop,
		tf_number_of_states,
		tf_war_score,
		tf_is_releasable_vassal_from,
		tf_is_releasable_vassal_other,
		tf_has_recent_imigration,
		tf_province_control_days,
		tf_is_disarmed,
		tf_big_producer,
		tf_someone_can_form_union_tag_from,
		tf_someone_can_form_union_tag_other,
		tf_social_movement_strength,
		tf_political_movement_strength,
		tf_can_build_factory_in_capital_state,
		tf_social_movement,
		tf_political_movement,
		tf_has_cultural_sphere,
		tf_world_wars_enabled,
		tf_has_pop_culture_pop_this_pop,
		tf_has_pop_culture_state_this_pop,
		tf_has_pop_culture_province_this_pop,
		tf_has_pop_culture_nation_this_pop,
		tf_has_pop_culture_pop,
		tf_has_pop_culture_state,
		tf_has_pop_culture_province,
		tf_has_pop_culture_nation,
		tf_has_pop_religion_pop_this_pop,
		tf_has_pop_religion_state_this_pop,
		tf_has_pop_religion_province_this_pop,
		tf_has_pop_religion_nation_this_pop,
		tf_has_pop_religion_pop,
		tf_has_pop_religion_state,
		tf_has_pop_religion_province,
		tf_has_pop_religion_nation,
		tf_life_needs,
		tf_everyday_needs,
		tf_luxury_needs,
		tf_consciousness_pop,
		tf_consciousness_province,
		tf_consciousness_state,
		tf_consciousness_nation,
		tf_literacy_pop,
		tf_literacy_province,
		tf_literacy_state,
		tf_literacy_nation,
		tf_militancy_pop,
		tf_militancy_province,
		tf_militancy_state,
		tf_militancy_nation,
		tf_military_spending_pop,
		tf_military_spending_province,
		tf_military_spending_state,
		tf_military_spending_nation,
		tf_administration_spending_pop,
		tf_administration_spending_province,
		tf_administration_spending_state,
		tf_administration_spending_nation,
		tf_education_spending_pop,
		tf_education_spending_province,
		tf_education_spending_state,
		tf_education_spending_nation,
		tf_trade_goods_in_state_state,
		tf_trade_goods_in_state_province,
		tf_has_flashpoint,
		tf_flashpoint_tension,
		tf_crisis_exist,
		tf_is_liberation_crisis,
		tf_is_claim_crisis,
		tf_crisis_temperature,
		tf_involved_in_crisis_pop,
		tf_involved_in_crisis_nation,
		tf_rich_strata_life_needs_nation,
		tf_rich_strata_life_needs_state,
		tf_rich_strata_life_needs_province,
		tf_rich_strata_life_needs_pop,
		tf_rich_strata_everyday_needs_nation,
		tf_rich_strata_everyday_needs_state,
		tf_rich_strata_everyday_needs_province,
		tf_rich_strata_everyday_needs_pop,
		tf_rich_strata_luxury_needs_nation,
		tf_rich_strata_luxury_needs_state,
		tf_rich_strata_luxury_needs_province,
		tf_rich_strata_luxury_needs_pop,
		tf_middle_strata_life_needs_nation,
		tf_middle_strata_life_needs_state,
		tf_middle_strata_life_needs_province,
		tf_middle_strata_life_needs_pop,
		tf_middle_strata_everyday_needs_nation,
		tf_middle_strata_everyday_needs_state,
		tf_middle_strata_everyday_needs_province,
		tf_middle_strata_everyday_needs_pop,
		tf_middle_strata_luxury_needs_nation,
		tf_middle_strata_luxury_needs_state,
		tf_middle_strata_luxury_needs_province,
		tf_middle_strata_luxury_needs_pop,
		tf_poor_strata_life_needs_nation,
		tf_poor_strata_life_needs_state,
		tf_poor_strata_life_needs_province,
		tf_poor_strata_life_needs_pop,
		tf_poor_strata_everyday_needs_nation,
		tf_poor_strata_everyday_needs_state,
		tf_poor_strata_everyday_needs_province,
		tf_poor_strata_everyday_needs_pop,
		tf_poor_strata_luxury_needs_nation,
		tf_poor_strata_luxury_needs_state,
		tf_poor_strata_luxury_needs_province,
		tf_poor_strata_luxury_needs_pop,
		tf_diplomatic_influence_tag,
		tf_diplomatic_influence_this_nation,
		tf_diplomatic_influence_this_province,
		tf_diplomatic_influence_from_nation,
		tf_diplomatic_influence_from_province,
		tf_pop_unemployment_nation,
		tf_pop_unemployment_state,
		tf_pop_unemployment_province,
		tf_pop_unemployment_pop,
		tf_pop_unemployment_nation_this_pop,
		tf_pop_unemployment_state_this_pop,
		tf_pop_unemployment_province_this_pop,
		tf_relation_tag,
		tf_relation_this_nation,
		tf_relation_this_province,
		tf_relation_from_nation,
		tf_relation_from_province,
		tf_check_variable,
		tf_upper_house,
		tf_unemployment_by_type_nation,
		tf_unemployment_by_type_state,
		tf_unemployment_by_type_province,
		tf_unemployment_by_type_pop,
		tf_party_loyalty_nation_province_id,
		tf_party_loyalty_from_nation_province_id,
		tf_party_loyalty_province_province_id,
		tf_party_loyalty_from_province_province_id,
		tf_party_loyalty_nation_from_province,
		tf_party_loyalty_from_nation_scope_province,
		tf_can_build_in_province_railroad_no_limit_from_nation,
		tf_can_build_in_province_railroad_yes_limit_from_nation,
		tf_can_build_in_province_railroad_no_limit_this_nation,
		tf_can_build_in_province_railroad_yes_limit_this_nation,
		tf_can_build_in_province_fort_no_limit_from_nation,
		tf_can_build_in_province_fort_yes_limit_from_nation,
		tf_can_build_in_province_fort_no_limit_this_nation,
		tf_can_build_in_province_fort_yes_limit_this_nation,
		tf_can_build_in_province_naval_base_no_limit_from_nation,
		tf_can_build_in_province_naval_base_yes_limit_from_nation,
		tf_can_build_in_province_naval_base_no_limit_this_nation,
		tf_can_build_in_province_naval_base_yes_limit_this_nation,
		tf_can_build_railway_in_capital_yes_whole_state_yes_limit,
		tf_can_build_railway_in_capital_yes_whole_state_no_limit,
		tf_can_build_railway_in_capital_no_whole_state_yes_limit,
		tf_can_build_railway_in_capital_no_whole_state_no_limit,
		tf_can_build_fort_in_capital_yes_whole_state_yes_limit,
		tf_can_build_fort_in_capital_yes_whole_state_no_limit,
		tf_can_build_fort_in_capital_no_whole_state_yes_limit,
		tf_can_build_fort_in_capital_no_whole_state_no_limit,
		tf_work_available_nation,
		tf_work_available_state,
		tf_work_available_province,
		tf_variable_ideology_name_nation,
		tf_variable_ideology_name_state,
		tf_variable_ideology_name_province,
		tf_variable_ideology_name_pop,
		tf_variable_issue_name_nation,
		tf_variable_issue_name_state,
		tf_variable_issue_name_province,
		tf_variable_issue_name_pop,
		tf_variable_issue_group_name_nation,
		tf_variable_issue_group_name_state,
		tf_variable_issue_group_name_province,
		tf_variable_issue_group_name_pop,
		tf_variable_pop_type_name_nation,
		tf_variable_pop_type_name_state,
		tf_variable_pop_type_name_province,
		tf_variable_pop_type_name_pop,
		tf_variable_good_name,
		tf_strata_middle,
		tf_strata_poor,
		tf_party_loyalty_from_province_scope_province,
		tf_can_build_factory_nation,
		tf_can_build_factory_province,
		tf_nationalvalue_pop,
		tf_nationalvalue_province,
		tf_war_exhaustion_pop,
		tf_has_culture_core_province_this_pop,
		tf_tag_pop,
		tf_has_country_flag_pop,
		tf_has_country_flag_province,
		tf_has_country_modifier_province,
		tf_religion_nation,
		tf_religion_nation_reb,
		tf_religion_nation_from_nation,
		tf_religion_nation_this_nation,
		tf_religion_nation_this_state,
		tf_religion_nation_this_province,
		tf_religion_nation_this_pop,
		tf_war_exhaustion_province,
		tf_is_greater_power_province,
		tf_is_cultural_union_pop_this_pop,
		tf_has_building_factory,
		tf_has_building_state_from_province,
		tf_has_building_factory_from_province,
		tf_party_loyalty_generic,
	};
	}

	bool test_trigger(TRIGGER_PARAMTERS) {
		if((*tval & trigger_codes::is_scope) != 0) {
			return scope_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot);
		} else {
			return trigger_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot);
		}
	}

#ifdef __llvm__
#pragma clang diagnostic pop
#endif

}
