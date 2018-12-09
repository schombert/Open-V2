#include "triggers.h"
#include "codes.h"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"
#include "military\\military_functions.h"
#include "population\\population_function.h"
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
		const_parameter nth_item(uint32_t i, const_parameter p) { return p; }
		const_parameter nth_item(uint32_t i, uint32_t p) { return const_parameter(int32_t(i + p)); }
		const_parameter nth_item(uint32_t i, ve::int_vector p) { return const_parameter(p[i]); }

		bool is_non_zero(const_parameter p) {
			return p.value != 0;
		}
		ve::fp_vector is_non_zero(ve::int_vector p) {
			return ve::is_non_zero(p);
		}

#define TRIGGER_PARAMTERS uint16_t const* tval, world_state const& ws, const_parameter primary_slot, const_parameter this_slot, const_parameter from_slot

		struct single_type {
			using return_type = bool;
			using parameter_type = const_parameter;

			constexpr static bool full_mask = true;
			constexpr static bool empty_mask = false;

			template<typename F>
			__forceinline static bool apply(const_parameter p, const_parameter this_slot, const_parameter from_slot, F&& f) { return f(p, this_slot, from_slot); }
			template<typename F>
			__forceinline static const_parameter apply_for_index(const_parameter p, const_parameter this_slot, const_parameter from_slot, F&& f) { return f(p, this_slot, from_slot); }
			template<typename F>
			__forceinline static bool generate(F&& f, const_parameter this_slot, const_parameter from_slot) { return f(this_slot, from_slot); }
			
			__forceinline static const_parameter widen(const_parameter p) { return p; }
			__forceinline static float widen(float p) { return p; }
			__forceinline static int32_t widen(int32_t p) { return p; }
			__forceinline static bool widen(bool p) { return p; }
		};
		struct contiguous_type {
			using return_type = ve::fp_vector;
			using parameter_type = uint32_t;

			constexpr static int32_t full_mask = ve::full_mask;
			constexpr static int32_t empty_mask = ve::empty_mask;

			template<typename F, typename this_type, typename from_type>
			__forceinline static ve::fp_vector apply(uint32_t p, this_type t, from_type fr, F&& f) {
				return ve::generate([&f, p, t, fr](uint32_t i) {
					return f(nth_item(i, p), nth_item(i, t), nth_item(i, fr));
				});
			}
			template<typename F, typename this_type, typename from_type>
			__forceinline static ve::int_vector apply_for_index(uint32_t p, this_type t, from_type fr, F&& f) {
				return ve::generate([&f, p, t, fr](uint32_t i) {
					return f(nth_item(i, p), nth_item(i, t), nth_item(i, fr)).value;
				});
			}
			template<typename F, typename this_type, typename from_type>
			__forceinline static ve::fp_vector generate(this_type t, from_type fr, F&& f) {
				return ve::generate([&f, t, fr](uint32_t i) {
					return f(nth_item(i, t), nth_item(i, fr));
				});
			}

			__forceinline static ve::int_vector widen(const_parameter p) { return ve::int_vector(p.value); }
			__forceinline static ve::fp_vector widen(float p) { return ve::fp_vector(p); }
			__forceinline static ve::int_vector widen(int32_t p) { return ve::int_vector(p); }
			__forceinline static ve::fp_vector widen(bool p) { return ve::fp_vector(p); }
		};
		struct gathered_type {
			using return_type = ve::fp_vector;
			using parameter_type = ve::int_vector;

			constexpr static int32_t full_mask = ve::full_mask;
			constexpr static int32_t empty_mask = ve::empty_mask;

			template<typename F, typename this_type, typename from_type>
			__forceinline static ve::fp_vector apply(ve::int_vector p, this_type t, from_type fr, F&& f) {
				return ve::generate([&f, p, t, fr](uint32_t i) {
					return f(nth_item(i, p), nth_item(i, t), nth_item(i, fr));
				});
			}
			template<typename F, typename this_type, typename from_type>
			__forceinline static ve::int_vector apply_for_index(ve::int_vector p, this_type t, from_type fr, F&& f) {
				return ve::generate([&f, p, t, fr](uint32_t i) {
					return f(nth_item(i, p), nth_item(i, t), nth_item(i, fr)).value;
				});
			}
			template<typename F, typename this_type, typename from_type>
			__forceinline static ve::fp_vector generate(this_type t, from_type fr, F&& f) {
				return ve::generate([&f, t, fr](uint32_t i) {
					return f(nth_item(i, t), nth_item(i, fr));
				});
			}

			__forceinline static ve::int_vector widen(const_parameter p) { return ve::int_vector(p.value); }
			__forceinline static ve::fp_vector widen(float p) { return ve::fp_vector(p); }
			__forceinline static ve::int_vector widen(int32_t p) { return ve::int_vector(p); }
			__forceinline static ve::fp_vector widen(bool p) { return ve::fp_vector(p); }
		};

		template<typename T>
		struct value_to_type_s;

		template<>
		struct value_to_type_s<uint32_t> {
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
		using value_to_type = typename value_to_type_s<T>::type;

#define TRIGGER_FUNCTION(function_name) template<typename primary_type, typename this_type, typename from_type> \
	auto __vectorcall function_name(uint16_t const* tval, world_state const& ws, typename primary_type::parameter_type primary_slot, \
	typename this_type::parameter_type this_slot, typename from_type::parameter_type from_slot) -> typename primary_type::return_type

	template<typename index>
	__forceinline auto nation_value(world_state const& ws, const_parameter v) {
		return ws.w.nation_s.nations.get<index>(ws, v.nation);
	}
	template<typename index>
	__forceinline auto nation_value(world_state const& ws, uint32_t v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.nation_s.nations.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation(v).load(ws.w.nation_s.nations.get_row<index>());
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation(v).load((int32_t const*)(ws.w.nation_s.nations.get_row<index>()));
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}
	template<typename index>
	__forceinline auto nation_value(world_state const& ws, ve::int_vector v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.nation_s.nations.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation::gather_load(ws.w.nation_s.nations.get_row<index>(), v);
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation::gather_load((int32_t const*)(ws.w.nation_s.nations.get_row<index>()), v);
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}

	template<typename index>
	__forceinline auto state_value(world_state const& ws, const_parameter v) {
		return ws.w.nation_s.states.get<index>(ws, v.state);
	}
	template<typename index>
	__forceinline auto state_value(world_state const& ws, uint32_t v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.nation_s.states.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation(v).load(ws.w.nation_s.states.get_row<index>());
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation(v).load((int32_t const*)(ws.w.nation_s.states.get_row<index>()));
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}
	template<typename index>
	__forceinline auto state_value(world_state const& ws, ve::int_vector v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.nation_s.states.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation::gather_load(ws.w.nation_s.states.get_row<index>(), v);
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation::gather_load((int32_t const*)(ws.w.nation_s.states.get_row<index>()), v);
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}

	template<typename index>
	__forceinline auto pop_value(world_state const& ws, const_parameter v) {
		return ws.w.population_s.pops.get<index>(ws, v.pop);
	}
	template<typename index>
	__forceinline auto pop_value(world_state const& ws, uint32_t v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.population_s.pops.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation(v).load(ws.w.population_s.pops.get_row<index>());
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation(v).load((int32_t const*)(ws.w.population_s.pops.get_row<index>()));
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}
	template<typename index>
	__forceinline auto pop_value(world_state const& ws, ve::int_vector v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.population_s.pops.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation::gather_load(ws.w.population_s.pops.get_row<index>(), v);
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation::gather_load((int32_t const*)(ws.w.population_s.pops.get_row<index>()), v);
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}

	template<typename index>
	__forceinline auto province_state_value(world_state const& ws, const_parameter v) {
		return ws.w.province_s.province_state_container.get<index>(ws, v.prov);
	}
	template<typename index>
	__forceinline auto province_state_value(world_state const& ws, uint32_t v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.province_s.province_state_container.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation(v).load(ws.w.province_s.province_state_container.get_row<index>());
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation(v).load((int32_t const*)(ws.w.province_s.province_state_container.get_row<index>()));
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}
	template<typename index>
	__forceinline auto province_state_value(world_state const& ws, ve::int_vector v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.province_s.province_state_container.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation::gather_load(ws.w.province_s.province_state_container.get_row<index>(), v);
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation::gather_load((int32_t const*)(ws.w.province_s.province_state_container.get_row<index>()), v);
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}


	template<typename index>
	__forceinline auto province_value(world_state const& ws, const_parameter v) {
		return ws.s.province_m.provinces.get<index>(ws, v.prov);
	}
	template<typename index>
	__forceinline auto province_value(world_state const& ws, uint32_t v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.s.province_m.provinces.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation(v).load(ws.s.province_m.provinces.get_row<index>());
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation(v).load((int32_t const*)(ws.s.province_m.provinces.get_row<index>()));
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}
	template<typename index>
	__forceinline auto province_value(world_state const& ws, ve::int_vector v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.s.province_m.provinces.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation::gather_load(ws.s.province_m.provinces.get_row<index>(), v);
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation::gather_load((int32_t const*)(ws.s.province_m.provinces.get_row<index>()), v);
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}

	template<typename index>
	__forceinline auto rebel_value(world_state const& ws, const_parameter v) {
		return ws.w.province_s.province_state_container.get<index>(ws, v.rebel);
	}
	template<typename index>
	__forceinline auto rebel_value(world_state const& ws, uint32_t v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.population_s.rebel_factions.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation(v).load(ws.w.population_s.rebel_factions.get_row<index>());
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation(v).load((int32_t const*)(ws.w.population_s.rebel_factions.get_row<index>()));
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
	}
	template<typename index>
	__forceinline auto rebel_value(world_state const& ws, ve::int_vector v) {
		using ctype = std::remove_cv_t<std::remove_reference_t<decltype(*(ws.w.population_s.rebel_factions.get_row<index>()))>>;
		if constexpr(std::is_same_v<ctype, float>)
			return ve::full_vector_operation::gather_load(ws.w.population_s.rebel_factions.get_row<index>(), v);
		else if constexpr(sizeof(ctype) == 4)
			return ve::full_vector_operation::gather_load((int32_t const*)(ws.w.population_s.rebel_factions.get_row<index>()), v);
		else
			static_assert(std::is_same_v<ctype, float> || sizeof(ctype) == 4);
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

		typename primary_type::return_type result = ~typename primary_type::return_type();
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
		return ~typename primary_type::return_type();
	}

	TRIGGER_FUNCTION(tf_generic_scope) {
		return apply_subtriggers<primary_type, this_type, from_type>(tval, ws, primary_slot, this_slot, from_slot);
	}

	auto existance_accumulator(world_state const& ws, uint16_t const* tval, const_parameter this_slot, const_parameter from_slot) {
		return ve::make_true_accumulator([&ws, tval, this_slot, from_slot](ve::int_vector v) {
			return apply_subtriggers<gathered_type, single_type, single_type>(tval, ws, v, this_slot, from_slot);
		});
	}

	auto universal_accumulator(world_state const& ws, uint16_t const* tval, const_parameter this_slot, const_parameter from_slot) {
		return ve::make_false_accumulator([&ws, tval, this_slot, from_slot](ve::int_vector v) {
			return apply_subtriggers<gathered_type, single_type, single_type>(tval, ws, v, this_slot, from_slot);
		});
	}

	TRIGGER_FUNCTION(tf_x_neighbor_province_scope) {
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto prov_id = p_slot.prov;

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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;

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
		auto pop_province = pop_value<pop::location>(ws, primary_slot);
		auto province_owner = province_state_value<province::owner>(ws, pop_province);

		return tf_x_neighbor_country_scope_nation<value_to_type<decltype(province_owner)>, this_type, from_type>(tval, ws, province_owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_x_war_countries_scope_nation) {
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;

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
		auto pop_province = pop_value<pop::location>(ws, primary_slot);
		auto province_owner = province_state_value<province::owner>(ws, pop_province);

		return tf_x_war_countries_scope_nation<value_to_type<decltype(province_owner)>, this_type, from_type>(tval, ws, province_owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_x_greater_power_scope) {
		return primary_type::generate(this_slot, from_slot, [&ws, tval](const_parameter t_slot, const_parameter f_slot) {
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto sid = p_slot.state;
			if(!ws.w.nation_s.states.is_valid_index(sid))
				return false;

			auto region_id = ws.w.nation_s.states.get<state::region_id>(sid);
			if(!is_valid_index(region_id))
				return false;

			auto region_provinces = ws.s.province_m.states_to_province_index.get_row(region_id);
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : region_provinces) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == primary_slot.state) {
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
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == primary_slot.state) {
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;

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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto pid = p_slot.prov;
			if(!ws.w.province_s.province_state_container.is_valid_index(pid))
				return false;

			auto core_range = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(primary_slot.prov));
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto p : core_range) {
					if(is_valid_index(p)) {
						auto core_holder = ws.w.culture_s.national_tags_state[p].holder;
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
						auto core_holder = ws.w.culture_s.national_tags_state[p].holder;
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto pid = p_slot.prov;
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto sid = p_slot.state;
			if(!ws.w.nation_s.states.is_valid_index(sid))
				return false;

			auto region_id = ws.w.nation_s.states.get<state::region_id>(sid);
			if(!is_valid_index(region_id))
				return false;

			auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);
			if(*tval & trigger_codes::is_existance_scope) {
				auto accumulator = existance_accumulator(ws, tval, t_slot, f_slot);

				for(auto j = province_range.first; j != province_range.second; ++j) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(*j) == primary_slot.state) {
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
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(*j) == primary_slot.state) {
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
		return primary_type::apply(primary_slot, this_slot, from_slot, [&ws, tval](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;
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

		return primary_type::generate(this_slot, from_slot, [&ws, tval, provinces](const_parameter t_slot, const_parameter f_slot) {
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
		auto owner = state_value<state::owner>(ws, primary_slot);
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_owner_scope_province) {
		auto owner = province_state_value<province_state::owner>(ws, primary_slot);
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_controller_scope) {
		auto owner = province_state_value<province_state::controller>(ws, primary_slot);
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_location_scope) {
		auto location = pop_value<pop::location>(ws, primary_slot);
		return is_non_zero(location) & apply_subtriggers<value_to_type<decltype(location)>, this_type, from_type>(tval, ws, location, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_state) {
		auto owner = state_value<state::owner>(ws, primary_slot);
		return is_non_zero(owner) &apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_pop) {
		auto location = pop_value<pop::location>(ws, primary_slot);
		auto owner = province_state_value<province_state::owner>(location);
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_capital_scope) {
		auto capital = nation_value<nation::current_capital>(ws, primary_slot);
		return is_non_zero(capital) & apply_subtriggers<value_to_type<decltype(capital)>, this_type, from_type>(tval, ws, capital, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_this_scope) {
		return apply_subtriggers<this_type, this_type, from_type>(tval, ws, this_slot, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_from_scope) {
		return apply_subtriggers<from_type, this_type, from_type>(tval, ws, from_slot, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_sea_zone_scope) {
		auto sea_zones = primary_type::apply_for_index(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto pid = p_slot.prov;
			if(!ws.w.province_s.province_state_container.is_valid_index(pid))
				return provinces::province_tag();
			auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(pid);
			if(sea_zones.first != sea_zones.second)
				return apply_subtriggers<single_type, single_type, single_type>(tval, ws, *sea_zones.first, t_slot, f_slot);
			return provinces::province_tag();
		});
		return is_non_zero(sea_zones) & apply_subtriggers<value_to_type<decltype(sea_zones)>, this_type, from_type>(tval, ws, sea_zones, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_cultural_union_scope) {
		auto union_nations = primary_type::apply_for_index(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto nid = p_slot.nation;
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(nid);
			if(!is_valid_index(prim_culture))
				return nations::country_tag();

			auto culture_group = ws.s.culture_m.culture_container[prim_culture].group;
			if(is_valid_index(culture_group)) {
				auto union_tag = ws.s.culture_m.culture_groups[culture_group].union_tag;
				if(is_valid_index(union_tag)) {
					return ws.w.culture_s.national_tags_state[union_tag].holder;
				}
			}
			return nations::country_tag();
		});
		return is_non_zero(union_nations) & apply_subtriggers<value_to_type<decltype(union_nations)>, this_type, from_type>(tval, ws, union_nations, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_overlord_scope) {
		auto so = nation_value<nation::overlord>(ws, primary_slot);
		return is_non_zero(so) & apply_subtriggers<value_to_type<decltype(so)>, this_type, from_type>(tval, ws, so, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_sphere_owner_scope) {
		auto so = nation_value<nation::sphere_leader>(ws, primary_slot);
		return is_non_zero(so) & apply_subtriggers<value_to_type<decltype(so)>, this_type, from_type>(tval, ws, so, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_independence_scope) {
		auto inations = primary_type::apply_for_index(from_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto rtag = ws.w.population_s.rebel_factions.get<rebel_faction::independence_tag>(p_slot.rebel);
			if(is_valid_index(rtag))
				return ws.w.culture_s.national_tags_state[rtag].holder;
			else
				return nations::country_tag();
		});
		return is_non_zero(inations) & apply_subtriggers<value_to_type<decltype(inations)>, this_type, from_type>(tval, ws, inations, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_flashpoint_tag_scope) {
		auto fp_nations = primary_type::apply_for_index(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto ctag = ws.w.nation_s.states.get<state::crisis_tag>(p_slot.state);
			if(is_valid_index(ctag))
				return ws.w.culture_s.national_tags_state[ctag].holder;
			else
				return nations::country_tag();
		});
		return is_non_zero(fp_nations) & apply_subtriggers<value_to_type<decltype(fp_nations)>, this_type, from_type>(tval, ws, fp_nations, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_crisis_state_scope) {
		auto cstate = primary_type::widen(ws.w.current_crisis.state);
		if(is_valid_index(ws.w.current_crisis.state))
			return apply_subtriggers<value_to_type<decltype(cstate)>, this_type, from_type>(tval, ws, cstate, this_slot, from_slot);
		return typename primary_type::return_type();
	}
	TRIGGER_FUNCTION(tf_state_scope_province) {
		auto state_instance = province_state_value<province_state::state_instance>(ws, primary_slot);
		return is_non_zero(state_instance) & apply_subtriggers<value_to_type<decltype(state_instance)>, this_type, from_type>(tval, ws, state_instance, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_state_scope_pop) {
		auto pop_province = pop_value<pop::location>(ws, primary_slot);
		auto prov_state = province_state_value<province_state::state_instance>(ws, pop_province);
		return is_non_zero(prov_state) & apply_subtriggers<value_to_type<decltype(prov_state)>, this_type, from_type>(tval, ws, prov_state, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_tag_scope) {
		auto tag = trigger_payload(tval[2]).tag;
		auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder;
		if(tag_holder) {
			auto wtag_holder = primary_type::widen(tag_holder);
			return apply_subtriggers<value_to_type<decltype(wtag_holder)>, this_type, from_type>(tval, ws, wtag_holder, this_slot, from_slot);
		} else {
			return false;
		}
	}
	TRIGGER_FUNCTION(tf_integer_scope) {
		auto wprov = primary_type::widen(provinces::province_tag(tval[2]));
		return apply_subtriggers<value_to_type<decltype(wprov)>, this_type, from_type>(tval, ws, wprov, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_nation) {
		return apply_subtriggers<primary_type, this_type, from_type>(tval, ws, primary_slot, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_country_scope_province) {
		auto owner = province_state_value<province_state::owner>(ws, primary_slot.prov);
		return is_non_zero(owner) & apply_subtriggers<value_to_type<decltype(owner)>, this_type, from_type>(tval, ws, owner, this_slot, from_slot);
	}
	TRIGGER_FUNCTION(tf_cultural_union_scope_pop) {
		auto unations = primary_type::apply_for_index(primary_slot, this_slot, from_slot, [&ws](const_parameter p_slot, const_parameter t_slot, const_parameter f_slot) {
			auto prim_culture = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
			if(!is_valid_index(prim_culture))
				return nations::country_tag();

			auto culture_group = ws.s.culture_m.culture_container[prim_culture].group;
			if(!is_valid_index(culture_group))
				return nations::country_tag();

			auto union_tag = ws.s.culture_m.culture_groups[culture_group].union_tag;
			if(!is_valid_index(union_tag))
				return nations::country_tag();

			return ws.w.culture_s.national_tags_state[union_tag].holder;
		});
		return is_non_zero(unations) & apply_subtriggers<value_to_type<decltype(unations)>, this_type, from_type>(tval, ws, unations, this_slot, from_slot);
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

	template<typename A>
	[[nodiscard]] auto compare_to_true(uint16_t trigger_code, A value_a) {
		switch(trigger_code & trigger_codes::association_mask) {
			case trigger_codes::association_eq:
				return value_a;
			case trigger_codes::association_gt:
				return ~value_a;
			case trigger_codes::association_lt:
				return ~value_a;
			case trigger_codes::association_le:
				return value_a;
			case trigger_codes::association_ne:
				return ~value_a;
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
				return ~value_a;
			case trigger_codes::association_gt:
				return value_a;
			case trigger_codes::association_lt:
				return value_a;
			case trigger_codes::association_le:
				return ~value_a;
			case trigger_codes::association_ne:
				return value_a;
			case trigger_codes::association_ge:
				return ~value_a;
			default:
				return ~value_a;
		}
	}

	TRIGGER_FUNCTION(tf_year) {
		return compare_values(tval[0], primary_type::widen(int32_t(tag_to_date(ws.w.current_date).year())), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_month) {
		return compare_values(tval[0], primary_type::widen(int32_t(tag_to_date(ws.w.current_date).month())), int32_t(tval[2]));
	}
	TRIGGER_FUNCTION(tf_port) {
		return compare_to_true(tval[0], ve::widen_mask(province_value<province::is_coastal>(primary_slot) & ~province_value<province::is_sea>(primary_slot)));
	}
	TRIGGER_FUNCTION(tf_rank) {
		// note: comparison revesed since rank 1 is "greater" than rank 1 + N
		return compare_values(tval[0], int32_t(tval[2]), nation_value<nation::overall_rank>(primary_slot));
	}
	TRIGGER_FUNCTION(tf_technology) {
		auto technology = trigger_payload(tval[2]).tech;
		auto nation_id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto tech_row = ws.w.nation_s.active_technologies.get_row(nation_id);
			auto has_tech = bit_vector_test(tech_row, technology);
			return compare_values(tval[0], has_tech, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_strata_rich(TRIGGER_PARAMTERS) {
		auto pop_type = ws.w.population_s.pops.get<pop::type>(primary_slot.pop);
		if(is_valid_index(pop_type))
			return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_strata_middle(TRIGGER_PARAMTERS) {
		auto pop_type = ws.w.population_s.pops.get<pop::type>(primary_slot.pop);
		if(is_valid_index(pop_type))
			return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_strata_poor(TRIGGER_PARAMTERS) {
		auto pop_type = ws.w.population_s.pops.get<pop::type>(primary_slot.pop);
		if(is_valid_index(pop_type))
			return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_life_rating_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::get_life_rating(ws, primary_slot.prov), float(trigger_payload(tval[2]).signed_value));
	}
	bool tf_life_rating_state(TRIGGER_PARAMTERS) {
		auto state = primary_slot.state;
		auto region_id = ws.w.nation_s.states.get<state::region_id>(state);
		if(!is_valid_index(region_id))
			return compare_values(tval[0], 0, int32_t(trigger_payload(tval[2]).signed_value));

		auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);
		int32_t min_life_rating = std::numeric_limits<int32_t>::max();
		nations::for_each_province(ws, state, [&ws, &min_life_rating](provinces::province_tag p) {
			min_life_rating = std::min(min_life_rating, int32_t(provinces::get_life_rating(ws, p)));
		});
		return compare_values(tval[0], min_life_rating, int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_has_empty_adjacent_state_province(TRIGGER_PARAMTERS) {
		auto adj_range = ws.s.province_m.same_type_adjacency.get_row(primary_slot.prov);
		for(auto p : adj_range) {
			if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))) 
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_empty_adjacent_state_state(TRIGGER_PARAMTERS) {
		auto state_id = primary_slot.state;
		auto region_id = ws.w.nation_s.states.get<state::region_id>(state_id);
		if(!is_valid_index(region_id))
			return compare_values(tval[0], false, true);

		auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);

		for(auto p : province_range) {
			if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == state_id) {
				auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
				for(auto q : adj_range) {
					if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(q))
						&& (ws.w.province_s.province_state_container.get<province_state::state_instance>(q) != state_id)) {
						return compare_values(tval[0], true, true);
					}
					
				}
			}
		}

		return compare_values(tval[0], false, true);
	}
	bool tf_state_id_province(TRIGGER_PARAMTERS) {
		provinces::province_tag pid(tval[2]);
		auto current_prov = primary_slot.prov;
		auto same_region = ws.s.province_m.province_container.get<province::state_id>(current_prov) == ws.s.province_m.province_container.get<province::state_id>(pid);
		return compare_values(tval[0], same_region, true);
	}
	bool tf_state_id_state(TRIGGER_PARAMTERS) {
		provinces::province_tag pid(tval[2]);
		auto current_region = ws.w.nation_s.states.get<state::region_id>(primary_slot.state);
		auto same_region = current_region == ws.s.province_m.province_container.get<province::state_id>(pid);
		return compare_values(tval[0], same_region, true);
	}
	bool tf_cash_reserves(TRIGGER_PARAMTERS) {
		auto ratio = economy::money_qnty_type(read_float_from_payload(tval + 2));
		auto target = population::desired_needs_spending(ws, primary_slot.pop);
		auto money = economy::money_qnty_type(ws.w.population_s.pops.get<pop::money>(primary_slot.pop));
		return compare_values(tval[0], target != economy::money_qnty_type(0) ? money / target : economy::money_qnty_type(100), ratio);
	}
	bool tf_unemployment_nation(TRIGGER_PARAMTERS) {
		auto nation_id = primary_slot.nation;
		if(!ws.w.nation_s.nations.is_valid_index(nation_id))
			return compare_values(tval[0], false, true);
		float unemployed_fraction = 1.0f - float(ws.w.nation_s.nation_demographics.get(nation_id, population::total_employment_tag)) / float(ws.w.nation_s.nations.get<nation::total_core_population>(nation_id));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_state(TRIGGER_PARAMTERS) {
		auto state_id = primary_slot.state;
		if(!ws.w.nation_s.states.is_valid_index(state_id))
			return compare_values(tval[0], false, true);
		float unemployed_fraction = 1.0f - float(ws.w.nation_s.state_demographics.get(state_id, population::total_employment_tag)) / float(ws.w.nation_s.states.get<state::total_population>(state_id));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_province(TRIGGER_PARAMTERS) {
		auto prov_id = primary_slot.prov;
		float unemployed_fraction = 1.0f - float(ws.w.province_s.province_demographics.get(prov_id, population::total_employment_tag)) / float(ws.w.province_s.province_state_container.get<province_state::total_population>(prov_id));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_pop(TRIGGER_PARAMTERS) {
		auto pop_id = primary_slot.pop;
		if(!ws.w.population_s.pops.is_valid_index(pop_id))
			return compare_values(tval[0], false, true);
		float unemployed_fraction = 1.0f - float(ws.w.population_s.pop_demographics.get(pop_id, population::total_employment_tag)) / float(ws.w.population_s.pops.get<pop::size>(pop_id));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_is_slave_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (ws.w.nation_s.nations.get<nation::current_rules>(primary_slot.nation).rules_value & issues::rules::slavery_allowed), true);
	}
	bool tf_is_slave_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::is_slave_state>(primary_slot.state), true);
	}
	bool tf_is_slave_province(TRIGGER_PARAMTERS) {
		auto state = provinces::province_state(ws, primary_slot.prov);
		if(state)
			return tf_is_slave_state(tval, ws, state, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_slave_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::type>(primary_slot.pop) == ws.s.population_m.slave, true);
	}
	bool tf_is_independant(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == nations::country_tag(), true);
	}
	bool tf_has_national_minority_province(TRIGGER_PARAMTERS) {
		auto powner = provinces::province_owner(ws, primary_slot.prov);
		auto pid = primary_slot.prov;

		if(powner) {
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(powner);
			if(is_valid_index(prim_culture)) {
				return compare_values(tval[0],
					ws.w.province_s.province_demographics.get(pid, population::to_demo_tag(ws, prim_culture)) ==
						ws.w.province_s.province_state_container.get<province_state::total_population>(pid),
					true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_national_minority_state(TRIGGER_PARAMTERS) {
		auto sowner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		auto sid = primary_slot.state;

		if(bool(sowner) & ws.w.nation_s.states.is_valid_index(sid)) {
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(sowner);
			if(is_valid_index(prim_culture)) {
				return compare_values(tval[0],
					ws.w.nation_s.state_demographics.get(sid, population::to_demo_tag(ws, prim_culture)) ==
					ws.w.nation_s.states.get<state::total_population>(sid),
					true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_national_minority_nation(TRIGGER_PARAMTERS) { 
		auto nid = primary_slot.nation;

		if(ws.w.nation_s.nations.is_valid_index(nid)) {
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
			if(is_valid_index(prim_culture)) {
				return compare_values(tval[0],
					ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, prim_culture)) ==
					ws.w.nation_s.nations.get<nation::total_core_population>(nid),
					true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_government_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::current_government>(primary_slot.nation) == trigger_payload(tval[2]).small.values.government, true);
	}
	bool tf_government_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto owner = provinces::province_owner(ws, location);
			if(owner) {
				return tf_government_nation(tval, ws, owner, nullptr, nullptr);
			}
		}
		return false;
	}
	bool tf_capital(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::current_capital>(primary_slot.nation) == provinces::province_tag(tval[2]), true);
	}
	bool tf_tech_school(TRIGGER_PARAMTERS) { 
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::tech_school>(primary_slot.nation) == trigger_payload(tval[2]).nat_mod, true);
	}
	bool tf_primary_culture(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_accepted_culture(TRIGGER_PARAMTERS) { 
		auto has_culture = contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(primary_slot.nation), trigger_payload(tval[2]).culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(primary_slot.pop) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_culture_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_culture>(primary_slot.state) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_culture_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::dominant_culture>(primary_slot.prov) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_culture_nation(TRIGGER_PARAMTERS) {
		auto has_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation) == trigger_payload(tval[2]).culture
			|| contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(primary_slot.nation), trigger_payload(tval[2]).culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_pop_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(primary_slot.pop) == ws.w.population_s.rebel_factions[from_slot.rebel].culture, true);
	}
	bool tf_culture_state_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_culture>(primary_slot.state) == ws.w.population_s.rebel_factions[from_slot.rebel].culture, true);
	}
	bool tf_culture_province_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::dominant_culture>(primary_slot.prov) == ws.w.population_s.rebel_factions[from_slot.rebel].culture, true);
	}
	bool tf_culture_nation_reb(TRIGGER_PARAMTERS) {
		auto has_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation) == ws.w.population_s.rebel_factions[from_slot.rebel].culture
			|| contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(primary_slot.nation), ws.w.population_s.rebel_factions[from_slot.rebel].culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_this_nation(TRIGGER_PARAMTERS) {
		auto pc = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
		auto has_culture = ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation) == pc
			|| contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(this_slot.nation), pc);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_from_nation(TRIGGER_PARAMTERS) {
		return tf_culture_this_nation(tval, ws, primary_slot, from_slot, nullptr);
	}
	bool tf_culture_this_state(TRIGGER_PARAMTERS) {
		auto state_owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(state_owner)
			return tf_culture_this_nation(tval, ws, primary_slot, state_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_this_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(this_slot.pop);
		if(is_valid_index(location)) {
			auto prov_owner = provinces::province_owner(ws, location);
			if(prov_owner)
				return tf_culture_this_nation(tval, ws, primary_slot, prov_owner, nullptr);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_this_province(TRIGGER_PARAMTERS) {
		auto prov_owner = provinces::province_owner(ws, this_slot.prov);
		if(prov_owner)
			return tf_culture_this_nation(tval, ws, primary_slot, prov_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation(TRIGGER_PARAMTERS) {
		auto cg = trigger_payload(tval[2]).culture_group;
		auto nation_pculture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(nation_pculture))
			return compare_values(tval[0], ws.s.culture_m.culture_container[nation_pculture].group == cg, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop(TRIGGER_PARAMTERS) {
		auto cg = trigger_payload(tval[2]).culture_group;
		auto pculture = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
		if(is_valid_index(pculture))
			return compare_values(tval[0], ws.s.culture_m.culture_container[pculture].group == cg, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_reb_nation(TRIGGER_PARAMTERS) { 
		auto rc = ws.w.population_s.rebel_factions[from_slot.rebel].culture;
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto nation_pculture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
			if(is_valid_index(nation_pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[nation_pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_reb_pop(TRIGGER_PARAMTERS) {
		auto rc = ws.w.population_s.rebel_factions[from_slot.rebel].culture;
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto pculture = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
			if(is_valid_index(pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_this_nation(TRIGGER_PARAMTERS) {
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto nation_pculture = ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation);
			if(is_valid_index(nation_pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[nation_pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_nation(TRIGGER_PARAMTERS) {
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto pculture = ws.w.population_s.pops.get<pop::culture>(this_slot.pop);
			if(is_valid_index(pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_from_nation(TRIGGER_PARAMTERS) {
		return tf_culture_group_nation_this_nation(tval, ws, primary_slot, from_slot, nullptr);
	}
	bool tf_culture_group_pop_from_nation(TRIGGER_PARAMTERS) {
		return tf_culture_group_pop_this_nation(tval, ws, primary_slot, from_slot, nullptr);
	}
	bool tf_culture_group_nation_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_culture_group_nation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_culture_group_pop_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_culture_group_nation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_state(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_culture_group_pop_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_culture_group_nation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_culture_group_pop_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_religion_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.population_s.rebel_factions[from_slot.rebel].religion, true);
	}
	bool tf_religion_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.nation_s.nations.get<nation::national_religion>(from_slot.nation), true);
	}
	bool tf_religion_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.nation_s.nations.get<nation::national_religion>(this_slot.nation), true);
	}
	bool tf_religion_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.nation_s.nations.get<nation::national_religion>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.nation_s.nations.get<nation::national_religion>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.nation_s.nations.get<nation::national_religion>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_terrain_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::terrain>(primary_slot.prov) == trigger_payload(tval[2]).prov_mod, true);
	}
	bool tf_terrain_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location))
			return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::terrain>(location) == trigger_payload(tval[2]).prov_mod, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_trade_goods(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::rgo_production>(primary_slot.prov) == trigger_payload(tval[2]).small.values.good, true);
	}

	bool tf_is_secondary_power_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overall_rank>(primary_slot.nation) <= int32_t(ws.s.modifiers_m.global_defines.colonial_rank), true);
	}
	bool tf_is_secondary_power_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_is_secondary_power_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_faction_nation(TRIGGER_PARAMTERS) {
		auto nation_factions = get_range(ws.w.population_s.rebel_faction_arrays, ws.w.nation_s.nations.get<nation::active_rebel_factions>(primary_slot.nation));
		auto rebel_type = trigger_payload(tval[2]).small.values.rebel;
		bool has_faction = false;
		for(auto f : nation_factions) {
			if(is_valid_index(f) && ws.w.population_s.rebel_factions[f].type == rebel_type) {
				has_faction = true;
				break;
			}
		}
		return compare_values(tval[0], has_faction, true);
	}
	bool tf_has_faction_pop(TRIGGER_PARAMTERS) {
		auto pop_faction = ws.w.population_s.pops.get<pop::rebel_faction>(primary_slot.pop);
		if(is_valid_index(pop_faction))
			return compare_values(tval[0], ws.w.population_s.rebel_factions[pop_faction].type == trigger_payload(tval[2]).small.values.rebel, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_unclaimed_cores(TRIGGER_PARAMTERS) {
		auto nation_tag = ws.w.nation_s.nations.get<nation::tag>(primary_slot.nation);
		bool has_all_cores = true;
		if(is_valid_index(nation_tag)) {
			auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[nation_tag].core_provinces);
			for(auto p : core_range) {
				if(is_valid_index(p) && provinces::province_owner(ws, p) != primary_slot.nation) {
					has_all_cores = false;
					break;
				}
			}
		}
		return compare_values(tval[0], has_all_cores, true);
	}
	bool tf_is_cultural_union_bool(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::union_holder_of(ws, primary_slot.nation) == primary_slot.nation, true);
	}
	bool tf_is_cultural_union_this_self_pop(TRIGGER_PARAMTERS) {
		auto pculture = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
		auto pop_union = is_valid_index(pculture) ? nations::union_holder_for(ws, pculture) : nations::country_tag();

		return compare_values(tval[0], this_slot.nation == pop_union, true);
	}
	bool tf_is_cultural_union_pop_this_pop(TRIGGER_PARAMTERS) {
		auto nation = population::get_pop_owner(ws, primary_slot.pop);
		if(nation)
			return tf_is_cultural_union_this_self_pop(tval, ws, primary_slot, nation, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_nation(TRIGGER_PARAMTERS) {
		auto main_union = nations::union_holder_of(ws, primary_slot.nation);
		return compare_values(tval[0], this_slot.nation == main_union, true);
	}
	bool tf_is_cultural_union_this_pop(TRIGGER_PARAMTERS) {
		auto pop_owner = population::get_pop_owner(ws, this_slot.pop);
		if(pop_owner)
			return tf_is_cultural_union_this_nation(tval, ws, primary_slot, pop_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_state(TRIGGER_PARAMTERS) {
		auto state_owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(state_owner)
			return tf_is_cultural_union_this_nation(tval, ws, primary_slot, state_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_province(TRIGGER_PARAMTERS) {
		auto prov_owner = provinces::province_owner(ws, this_slot.prov);
		if(prov_owner)
			return tf_is_cultural_union_this_nation(tval, ws, primary_slot, prov_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_rebel(TRIGGER_PARAMTERS) {
		if(auto rc = ws.w.population_s.rebel_factions[from_slot.rebel].culture; is_valid_index(rc))
			return compare_values(tval[0], primary_slot.nation == nations::union_holder_for(ws, rc), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::union_tag_of(ws, primary_slot.nation) == trigger_payload(tval[2]).tag, true);
	}
	bool tf_is_cultural_union_tag_this_pop(TRIGGER_PARAMTERS) {
		auto pop_owner = population::get_pop_owner(ws, this_slot.pop);
		if(pop_owner)
			return tf_is_cultural_union_tag_nation(tval, ws, pop_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_this_state(TRIGGER_PARAMTERS) {
		auto state_owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(state_owner)
			return tf_is_cultural_union_tag_nation(tval, ws, state_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_this_province(TRIGGER_PARAMTERS) {
		auto prov_owner = provinces::province_owner(ws, this_slot.prov);
		if(prov_owner)
			return tf_is_cultural_union_tag_nation(tval, ws, prov_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_this_nation(TRIGGER_PARAMTERS) {
		return tf_is_cultural_union_tag_nation(tval, ws, this_slot, nullptr, nullptr);
	}
	bool tf_can_build_factory_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (ws.w.nation_s.nations.get<nation::current_rules>(primary_slot.nation).rules_value | issues::rules::pop_build_factory), true);
	}
	bool tf_can_build_factory_province(TRIGGER_PARAMTERS) {
		auto p_owner = provinces::province_owner(ws, primary_slot.prov);
		if(p_owner)
			return tf_can_build_factory_nation(tval, ws, p_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_can_build_factory_pop(TRIGGER_PARAMTERS) {
		auto p_owner = population::get_pop_owner(ws, primary_slot.pop);
		if(p_owner)
			return tf_can_build_factory_nation(tval, ws, p_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(primary_slot.nation)) != 0, true);
	}
	bool tf_war_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_war_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_exhaustion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::war_exhaustion>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_war_exhaustion_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_war_exhaustion_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_war_exhaustion_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_war_exhaustion_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_blockade(TRIGGER_PARAMTERS) {
		auto cpc = float(ws.w.nation_s.nations.get<nation::central_province_count>(primary_slot.nation));
		if(cpc != 0.0f)
			return compare_values(tval[0], float(ws.w.nation_s.nations.get<nation::blockaded_count>(primary_slot.nation)) / cpc, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_owns(TRIGGER_PARAMTERS) {
		provinces::province_tag prov(tval[2]);
		return compare_values(tval[0], provinces::province_owner(ws, prov) == primary_slot.nation, true);
	}
	bool tf_controls(TRIGGER_PARAMTERS) {
		provinces::province_tag prov(tval[2]);
		return compare_values(tval[0], provinces::province_controller(ws, prov) == primary_slot.nation, true);
	}
	bool tf_is_core_integer(TRIGGER_PARAMTERS) {
		provinces::province_tag prov(tval[2]);
		return compare_values(tval[0],
			contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(prov),
				ws.w.nation_s.nations.get<nation::tag>(primary_slot.nation)),
			true);
	}
	bool tf_is_core_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(ws.w.province_s.core_arrays,
				ws.w.province_s.province_state_container.get<province_state::cores>(primary_slot.prov),
				ws.w.nation_s.nations.get<nation::tag>(this_slot.nation)),
			true);
	}
	bool tf_is_core_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_is_core_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_core_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_is_core_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_core_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_is_core_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_core_from_nation(TRIGGER_PARAMTERS) {
		return tf_is_core_this_nation(tval, ws, primary_slot, from_slot, nullptr);
	}
	bool tf_is_core_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(primary_slot.prov),
				ws.w.population_s.rebel_factions[from_slot.rebel].independence_tag), true);
	}
	bool tf_is_core_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(primary_slot.prov),
				trigger_payload(tval[2]).tag), true);
	}
	bool tf_num_of_revolts(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::rebel_controlled_provinces>(primary_slot.nation), tval[2]);
	}
	bool tf_revolt_percentage(TRIGGER_PARAMTERS) {
		auto cpc = float(ws.w.nation_s.nations.get<nation::central_province_count>(primary_slot.nation));
		if(cpc != 0.0f)
			return compare_values(tval[0], float(ws.w.nation_s.nations.get<nation::rebel_controlled_provinces>(primary_slot.nation)) / cpc, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_num_of_cities_int(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation)), uint32_t(tval[2]));
	}
	bool tf_num_of_cities_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation)),
			get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(from_slot.nation)));
	}
	bool tf_num_of_cities_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation)),
			get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(this_slot.nation)));
	}
	bool tf_num_of_cities_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_num_of_cities_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation)), 0ui32);
	}
	bool tf_num_of_cities_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_num_of_cities_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation)), 0ui32);
	}
	bool tf_num_of_cities_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_num_of_cities_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation)), 0ui32);
	}
	bool tf_num_of_ports(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::num_connected_ports>(primary_slot.nation), tval[2]);
	}
	bool tf_num_of_allies(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation)), uint32_t(tval[2]));
	}
	bool tf_num_of_vassals(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(primary_slot.nation)), uint32_t(tval[2]));
	}
	bool tf_owned_by_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		return compare_values(tval[0], bool(bool(holder) & (holder == provinces::province_owner(ws, primary_slot.prov))), true);
	}
	bool tf_owned_by_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], bool(bool(this_slot.nation) & (this_slot.nation == provinces::province_owner(ws, primary_slot.prov))), true);
	}
	bool tf_owned_by_from_nation(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, from_slot, nullptr);
	}
	bool tf_owned_by_this_province(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, provinces::province_owner(ws, this_slot.prov), nullptr);
	}
	bool tf_owned_by_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_owned_by_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_owned_by_this_pop(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, population::get_pop_owner(ws,this_slot.pop), nullptr);
	}
	bool tf_exists_bool(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(primary_slot.nation)), true);
	}
	bool tf_exists_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(holder)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_country_flag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.variable_s.national_flags_arrays,
			ws.w.nation_s.nations.get<nation::national_flags>(primary_slot.nation), trigger_payload(tval[2]).nat_flag), true);
	}
	bool tf_has_country_flag_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_has_country_flag(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_country_flag_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_has_country_flag(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_continent_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.s.province_m.province_container.get<province::continent>(primary_slot.prov) == trigger_payload(tval[2]).prov_mod, true);
	}
	bool tf_continent_state(TRIGGER_PARAMTERS) {
		auto region_id = ws.w.nation_s.states.get<state::region_id>(primary_slot.state);
		if(is_valid_index(region_id)) {
			auto prov_id = ws.s.province_m.states_to_province_index.get(region_id, 0);
			return compare_values(tval[0], ws.s.province_m.province_container.get<province::continent>(prov_id) == trigger_payload(tval[2]).prov_mod, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_continent_nation(TRIGGER_PARAMTERS) {
		auto prov_id = ws.w.nation_s.nations.get<nation::current_capital>(primary_slot.nation);
		if(is_valid_index(prov_id))
			return compare_values(tval[0], ws.s.province_m.province_container.get<province::continent>(prov_id) == trigger_payload(tval[2]).prov_mod, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_continent_pop(TRIGGER_PARAMTERS) {
		auto prov_id = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(prov_id))
			return compare_values(tval[0], ws.s.province_m.province_container.get<province::continent>(prov_id) == trigger_payload(tval[2]).prov_mod, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_continent_nation_this(TRIGGER_PARAMTERS) {
		return tf_continent_nation(tval, ws, this_slot, nullptr, nullptr);
	}
	bool tf_continent_state_this(TRIGGER_PARAMTERS) {
		return tf_continent_state(tval, ws, this_slot, nullptr, nullptr);
	}
	bool tf_continent_province_this(TRIGGER_PARAMTERS) {
		return tf_continent_province(tval, ws, this_slot, nullptr, nullptr);
	}
	bool tf_continent_pop_this(TRIGGER_PARAMTERS) {
		return tf_continent_pop(tval, ws, this_slot, nullptr, nullptr);
	}
	bool tf_continent_nation_from(TRIGGER_PARAMTERS) {
		return tf_continent_nation(tval, ws, from_slot, nullptr, nullptr);
	}
	bool tf_continent_state_from(TRIGGER_PARAMTERS) {
		return tf_continent_state(tval, ws, from_slot, nullptr, nullptr);
	}
	bool tf_continent_province_from(TRIGGER_PARAMTERS) {
		return tf_continent_province(tval, ws, from_slot, nullptr, nullptr);
	}
	bool tf_continent_pop_from(TRIGGER_PARAMTERS) {
		return tf_continent_pop(tval, ws, from_slot, nullptr, nullptr);
	}
	bool tf_casus_belli_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], military::can_use_cb_against(ws, primary_slot.nation, tag_holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_casus_belli_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::can_use_cb_against(ws, primary_slot.nation, from_slot.nation), true);
	}
	bool tf_casus_belli_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::can_use_cb_against(ws, primary_slot.nation, this_slot.nation), true);
	}
	bool tf_casus_belli_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], military::can_use_cb_against(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_casus_belli_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], military::can_use_cb_against(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_casus_belli_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], military::can_use_cb_against(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], military::has_military_access_with(ws, primary_slot.nation, holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_military_access_with(ws, primary_slot.nation, from_slot.nation), true);
	}
	bool tf_military_access_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_military_access_with(ws, primary_slot.nation, this_slot.nation), true);
	}
	bool tf_military_access_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], military::has_military_access_with(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], military::has_military_access_with(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], military::has_military_access_with(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_prestige_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_prestige_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), nations::get_prestige(ws, from_slot.nation));
	}
	bool tf_prestige_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), nations::get_prestige(ws, this_slot.nation));
	}
	bool tf_prestige_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), nations::get_prestige(ws, owner));
		else
			return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), 0.0f);
	}
	bool tf_prestige_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), nations::get_prestige(ws, owner));
		else
			return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), 0.0f);
	}
	bool tf_prestige_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), nations::get_prestige(ws, owner));
		else
			return compare_values(tval[0], nations::get_prestige(ws, primary_slot.nation), 0.0f);
	}
	bool tf_badboy(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::infamy>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_has_building_fort(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::fort_level>(primary_slot.prov) != 0ui8, true);
	}
	bool tf_has_building_railroad(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::railroad_level>(primary_slot.prov) != 0ui8, true);
	}
	bool tf_has_building_naval_base(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::naval_base_level>(primary_slot.prov) != 0ui8, true);
	}

	bool tf_has_building_factory(TRIGGER_PARAMTERS) {
		auto si = primary_slot.state;
		auto& factories = ws.w.nation_s.states.get<state::factories>(si);

		auto has_factories = [&factories, si]() {
			for(uint32_t i = 0; i < state::factories_count; ++i) {
				if(factories[i].type)
					return true;
			}
			return false;
		}();
		return compare_values(tval[0], has_factories, true);
	}
	bool tf_has_building_state(TRIGGER_PARAMTERS) {
		auto si = primary_slot.state;
		auto type = trigger_payload(tval[2]).small.values.factory;
		auto& factories = ws.w.nation_s.states.get<state::factories>(si);

		auto has_factory = [si, type, &factories]() {
			for(uint32_t i = 0; i < state::factories_count; ++i) {
				if(factories[i].type && factories[i].type->id == type)
					return true;
			}
			return false;
		}();
		return compare_values(tval[0], has_factory, true);
	}
	bool tf_has_building_state_from_province(TRIGGER_PARAMTERS) {
		auto state = provinces::province_state(ws, primary_slot.prov);
		if(state)
			return tf_has_building_state(tval, ws, state, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_building_factory_from_province(TRIGGER_PARAMTERS) {
		auto state = provinces::province_state(ws, primary_slot.prov);
		if(state)
			return tf_has_building_factory(tval, ws, state, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_empty(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], !is_valid_index(provinces::province_owner(ws, primary_slot.prov)), true);
	}
	bool tf_is_blockaded(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::is_blockaded>(primary_slot.prov), true);
	}
	bool tf_has_country_modifier(TRIGGER_PARAMTERS) {
		const auto mod = trigger_payload(tval[2]).nat_mod;
		return compare_values(tval[0],
			modifiers::has_national_modifier(ws, primary_slot.nation, mod),
			true);
	}
	bool tf_has_country_modifier_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_has_country_modifier(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_province_modifier(TRIGGER_PARAMTERS) {
		const auto mod = trigger_payload(tval[2]).prov_mod;
		return compare_values(tval[0],
			modifiers::has_provincial_modifier(ws, primary_slot.prov, mod),
			true);
	}
	bool tf_region(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.s.province_m.province_container.get<province::state_id>(primary_slot.prov) == trigger_payload(tval[2]).state, true);
	}
	bool tf_tag_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::tag>(primary_slot.nation) == trigger_payload(tval[2]).tag, true);
	}
	bool tf_tag_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot.nation == this_slot.nation, true);
	}
	bool tf_tag_this_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot.nation == provinces::province_owner(ws, this_slot.prov), true);
	}
	bool tf_tag_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot.nation == from_slot.nation, true);
	}
	bool tf_tag_from_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot.nation == provinces::province_owner(ws, from_slot.prov), true);
	}
	bool tf_tag_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_tag_tag(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_neighbour_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0],
				contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(primary_slot.nation), tag_holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_neighbour_this(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(
				ws.w.nation_s.nations_arrays,
				ws.w.nation_s.nations.get<nation::neighboring_nations>(primary_slot.nation),
				this_slot.nation),
			true);
	}
	bool tf_neighbour_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(
				ws.w.nation_s.nations_arrays,
				ws.w.nation_s.nations.get<nation::neighboring_nations>(primary_slot.nation),
				from_slot.nation),
			true);
	}
	bool tf_units_in_province_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::total_units_in_province(ws, primary_slot.prov), uint32_t(tval[2]));
	}
	bool tf_units_in_province_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_units_in_province(ws, from_slot.nation, primary_slot.prov), true);
	}
	bool tf_units_in_province_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_units_in_province(ws, this_slot.nation, primary_slot.prov), true);
	}
	bool tf_units_in_province_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], military::has_units_in_province(ws, owner, primary_slot.prov), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_units_in_province_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], military::has_units_in_province(ws, owner, primary_slot.prov), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_units_in_province_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], military::has_units_in_province(ws, owner, primary_slot.prov), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], military::in_war_against(ws, primary_slot.nation, holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::in_war_against(ws, primary_slot.nation, from_slot.nation), true);
	}
	bool tf_war_with_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::in_war_against(ws, primary_slot.nation, this_slot.nation), true);
	}
	bool tf_war_with_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], military::in_war_against(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], military::in_war_against(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], military::in_war_against(ws, primary_slot.nation, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_unit_in_battle(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::province_is_contested(ws, primary_slot.prov), true);
	}
	bool tf_total_amount_of_divisions(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::total_active_divisions(ws, primary_slot.nation), uint32_t(tval[2]));
	}
	bool tf_money(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], nations::national_treasury(ws, id), economy::goods_qnty_type(read_float_from_payload(tval + 2)));
		else
			return compare_values(tval[0], economy::goods_qnty_type(0.0), economy::goods_qnty_type(read_float_from_payload(tval + 2)));
	}
	bool tf_lost_national(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 1.0f - nations::fraction_of_cores_owned(ws, primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_is_vassal(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation)), true);
	}
	bool tf_ruling_party_ideology_nation(TRIGGER_PARAMTERS) {
		auto ri = ws.w.nation_s.nations.get<nation::ruling_ideology>(primary_slot.nation);
		if(is_valid_index(ri))
			return compare_values(tval[0], ri == trigger_payload(tval[2]).small.values.ideology, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_ruling_party_ideology_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_ruling_party_ideology_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_ruling_party(TRIGGER_PARAMTERS) {
		auto rp = ws.w.nation_s.nations.get<nation::ruling_party>(primary_slot.nation);
		if(is_valid_index(rp))
			return compare_values(tval[0], ws.s.governments_m.parties[rp].name == trigger_payload(tval[2]).text, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_ideology_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ideologies::ideology_enabled(ws, trigger_payload(tval[2]).small.values.ideology), true);
	}
	bool tf_political_reform_want_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::political_interest_fraction>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_political_reform_want_pop(TRIGGER_PARAMTERS) {
		auto pop_id = primary_slot.pop;
		if(ws.w.population_s.pops.is_valid_index(pop_id))
			return compare_values(tval[0], ws.w.population_s.pops.get<pop::political_interest>(pop_id), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_social_reform_want_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::social_interest_fraction>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_social_reform_want_pop(TRIGGER_PARAMTERS) {
		auto pop_id = primary_slot.pop;
		if(ws.w.population_s.pops.is_valid_index(pop_id))
			return compare_values(tval[0], ws.w.population_s.pops.get<pop::social_interest>(pop_id), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_total_amount_of_ships(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::total_active_ships(ws, primary_slot.nation), uint32_t(tval[2]));
	}
	bool tf_plurality(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::plurality>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_corruption(TRIGGER_PARAMTERS) {
		auto cpc = float(ws.w.nation_s.nations.get<nation::central_province_count>(primary_slot.nation));
		if(cpc != 0.0f)
			return compare_values(tval[0], float(ws.w.nation_s.nations.get<nation::crime_count>(primary_slot.nation)) / cpc, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_is_state_religion_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(owner) == ws.w.population_s.pops.get<pop::religion>(primary_slot.pop), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_state_religion_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(owner) == ws.w.province_s.province_state_container.get<province_state::dominant_religion>(primary_slot.prov), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_state_religion_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(owner) == ws.w.nation_s.states.get<state::dominant_religion>(primary_slot.state), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::primary_culture>(owner) == ws.w.population_s.pops.get<pop::culture>(primary_slot.pop), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return compare_values(tval[0],
				ws.w.nation_s.nations.get<nation::primary_culture>(owner) == ws.w.province_s.province_state_container.get<province_state::dominant_culture>(primary_slot.prov), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::primary_culture>(owner) == ws.w.nation_s.states.get<state::dominant_culture>(primary_slot.state), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_nation_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation) == ws.w.population_s.pops.get<pop::culture>(this_slot.pop), true);
	}
	bool tf_is_primary_culture_nation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation) == ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation), true);
	}
	bool tf_is_primary_culture_nation_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = nations::state_owner(ws, this_slot.state);
		if(this_owner)
			return tf_is_primary_culture_nation_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_nation_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = provinces::province_owner(ws, this_slot.prov);
		if(this_owner)
			return tf_is_primary_culture_nation_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_culture>(primary_slot.state) == ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation), true);
	}
	bool tf_is_primary_culture_state_this_pop(TRIGGER_PARAMTERS) {
		auto this_owner = population::get_pop_owner(ws, this_slot.pop);
		if(this_owner)
			return tf_is_primary_culture_state_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = nations::state_owner(ws, this_slot.state);
		if(this_owner)
			return tf_is_primary_culture_state_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = provinces::province_owner(ws, this_slot.prov);
		if(this_owner)
			return tf_is_primary_culture_state_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_culture>(primary_slot.prov) == ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation), true);
	}
	bool tf_is_primary_culture_province_this_pop(TRIGGER_PARAMTERS) {
		auto this_owner = population::get_pop_owner(ws, this_slot.pop);
		if(this_owner)
			return tf_is_primary_culture_province_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = nations::state_owner(ws, this_slot.state);
		if(this_owner)
			return tf_is_primary_culture_province_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = provinces::province_owner(ws, this_slot.prov);
		if(this_owner)
			return tf_is_primary_culture_province_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(primary_slot.pop) == ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation), true);
	}
	bool tf_is_primary_culture_pop_this_pop(TRIGGER_PARAMTERS) {
		auto this_owner = population::get_pop_owner(ws, this_slot.pop);
		if(this_owner)
			return tf_is_primary_culture_pop_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = nations::state_owner(ws, this_slot.state);
		if(this_owner)
			return tf_is_primary_culture_pop_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = provinces::province_owner(ws, this_slot.prov);
		if(this_owner)
			return tf_is_primary_culture_pop_this_nation(tval, ws, primary_slot, this_owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_accepted_culture_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner) {
			auto pc = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
			return compare_values(tval[0],
				ws.w.nation_s.nations.get<nation::primary_culture>(owner) == pc ||
				contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(owner), pc),
				true);
		} else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_accepted_culture_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner) {
			auto prov_culture = ws.w.province_s.province_state_container.get<province_state::dominant_culture>(primary_slot.prov);
			return compare_values(tval[0],
				ws.w.nation_s.nations.get<nation::primary_culture>(owner) == prov_culture ||
				contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(owner), prov_culture),
				true);
		} else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_accepted_culture_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner) {
			auto state_culture = ws.w.nation_s.states.get<state::dominant_culture>(primary_slot.state);
			return compare_values(tval[0],
				ws.w.nation_s.nations.get<nation::primary_culture>(owner) == state_culture ||
				contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(owner), state_culture),
				true);
		} else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_coastal(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.s.province_m.province_container.get<province::is_coastal>(primary_slot.prov),
			true);
	}
	bool tf_in_sphere_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.nation_s.nations.get<nation::sphere_leader>(primary_slot.nation) == ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder,
			true);
	}
	bool tf_in_sphere_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.nation_s.nations.get<nation::sphere_leader>(primary_slot.nation) == from_slot.nation,
			true);
	}
	bool tf_in_sphere_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.nation_s.nations.get<nation::sphere_leader>(primary_slot.nation) == this_slot.nation,
			true);
	}
	bool tf_in_sphere_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_in_sphere_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_sphere_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_in_sphere_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_sphere_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_in_sphere_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_produces_nation(TRIGGER_PARAMTERS) {
		auto good = trigger_payload(tval[2]).small.values.good;

		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation));
		for(auto p : owned_range) {
			auto rgo_production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(p);
			auto artisan_production = ws.w.province_s.province_state_container.get<province_state::artisan_production>(p);
			if((rgo_production == good) | (artisan_production == good))
				return compare_values(tval[0], true, true);
		}
		auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(primary_slot.nation));
		for(auto s = states.first; s != states.second; ++s) {
			auto& factories = ws.w.nation_s.states.get<state::factories>(s->state);
			for(int32_t i = state::factories_count; i--; ) {
				if(factories[i].type && factories[i].type->output_good == good)
					return compare_values(tval[0], true, true);
			}
		}

		return compare_values(tval[0], false, true);
	}
	bool tf_produces_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::rgo_production>(primary_slot.prov) == trigger_payload(tval[2]).small.values.good ||
			ws.w.province_s.province_state_container.get<province_state::artisan_production>(primary_slot.prov) == trigger_payload(tval[2]).small.values.good,
			true);
	}
	bool tf_produces_state(TRIGGER_PARAMTERS) {
		auto state_provinces = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(primary_slot.state));
		auto good = trigger_payload(tval[2]).small.values.good;

		for(auto p : state_provinces) {
			if((provinces::province_state(ws, p) == primary_slot.state)
				&& ((ws.w.province_s.province_state_container.get<province_state::rgo_production>(p) == good)
					|| (ws.w.province_s.province_state_container.get<province_state::artisan_production>(p) == good)))
				return compare_values(tval[0], true, true);
		}
		auto& factories = ws.w.nation_s.states.get<state::factories>(primary_slot.state);
		for(int32_t i = state::factories_count; i--; ) {
			if(factories[i].type && factories[i].type->output_good == good)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	
	bool tf_produces_pop(TRIGGER_PARAMTERS) {
		auto pop_location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(ws.w.population_s.pops.get<pop::type>(primary_slot.pop) == ws.s.population_m.artisan && is_valid_index(pop_location))
			return compare_values(tval[0],
				ws.w.province_s.province_state_container.get<province_state::artisan_production>(pop_location) == trigger_payload(tval[2]).small.values.good,
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_average_militancy_nation(TRIGGER_PARAMTERS) {
		auto nation_id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(nation_id);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(nation_id, population::militancy_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_militancy_state(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.states.is_valid_index(primary_slot.state)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(primary_slot.state);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(primary_slot.state, population::militancy_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_militancy_province(TRIGGER_PARAMTERS) {
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(primary_slot.prov);
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(primary_slot.prov, population::militancy_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_consciousness_nation(TRIGGER_PARAMTERS) {
		auto nation_id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(nation_id);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(nation_id, population::consciousness_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_consciousness_state(TRIGGER_PARAMTERS) {
		auto state_id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(state_id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(state_id);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(state_id, population::consciousness_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_consciousness_province(TRIGGER_PARAMTERS) {
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(primary_slot.prov);
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(primary_slot.prov, population::consciousness_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_is_next_reform_nation(TRIGGER_PARAMTERS) {
		auto reform_id = trigger_payload(tval[2]).small.values.option;
		auto reform_parent = ws.s.issues_m.options[reform_id].parent_issue;
		auto nation_id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto active_option = ws.w.nation_s.active_issue_options.get(nation_id, reform_parent);
			return compare_values(tval[0], (to_index(active_option) + 1) == to_index(reform_id), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_is_next_reform_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_is_next_reform_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rebel_power_fraction(TRIGGER_PARAMTERS) {
		// note: virtually unused
		return compare_values(tval[0], false, true);
	}
	bool tf_recruited_percentage_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::recruited_pop_fraction(ws, primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_recruited_percentage_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_recruited_percentage_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_culture_core(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		auto culture = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto cores = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(location));
			for(auto tag : cores) {
				if(is_valid_index(tag)) {
					if(auto holder = ws.w.culture_s.national_tags_state[tag].holder; bool(holder) && ws.w.nation_s.nations.get<nation::primary_culture>(holder) == culture)
						return compare_values(tval[0], true, true);
				}
			}
			return compare_values(tval[0], false, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_has_culture_core_province_this_pop(TRIGGER_PARAMTERS) {
		auto location = primary_slot.prov;
		auto culture = ws.w.population_s.pops.get<pop::culture>(this_slot.pop);
		if(is_valid_index(location)) {
			auto cores = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(location));
			for(auto tag : cores) {
				if(is_valid_index(tag)) {
					if(auto holder = ws.w.culture_s.national_tags_state[tag].holder; bool(holder) && ws.w.nation_s.nations.get<nation::primary_culture>(holder) == culture)
						return compare_values(tval[0], true, true);
				}
			}
			return compare_values(tval[0], false, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_nationalism(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::nationalism>(primary_slot.prov), float(tval[2]));
	}
	bool tf_is_overseas(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::is_overseas>(primary_slot.prov), true);
	}
	bool tf_controlled_by_rebels(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::rebel_controller>(primary_slot.prov) != population::rebel_faction_tag(), true);
	}
	bool tf_controlled_by_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_controller(ws, primary_slot.prov) == ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder, true);
	}
	bool tf_controlled_by_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_controller(ws, primary_slot.prov) == from_slot.nation, true);
	}
	bool tf_controlled_by_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_controller(ws, primary_slot.prov) == this_slot.nation, true);
	}
	bool tf_controlled_by_this_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_controller(ws, primary_slot.prov) == provinces::province_owner(ws, this_slot.prov), true);
	}
	bool tf_controlled_by_this_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_controller(ws, primary_slot.prov) == nations::state_owner(ws, this_slot.state), true);
	}
	bool tf_controlled_by_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_controller(ws, primary_slot.prov) == population::get_pop_owner(ws, this_slot.pop), true);
	}
	bool tf_controlled_by_owner(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::province_owner(ws, primary_slot.prov) == provinces::province_controller(ws, primary_slot.prov), true);
	}
	bool tf_controlled_by_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.province_state_container.get<province_state::rebel_controller>(primary_slot.prov) == from_slot.rebel, true);
	}
	bool tf_is_canal_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.is_canal_enabled[tval[2]-1] != 0ui8, true);
	}
	bool tf_is_state_capital(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto sid = ws.w.province_s.province_state_container.get<province_state::state_instance>(id);
		
		auto state_range = ws.s.province_m.states_to_province_index.get_row(ws.s.province_m.province_container.get<province::state_id>(id));
		for(auto ip : state_range) {
			if(ws.w.province_s.province_state_container.get<province_state::state_instance>(ip) == sid) {
				if(ip == id)
					return compare_values(tval[0], true, true);
				else
					return compare_values(tval[0], false, true);
			}
		}

		return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(primary_slot.nation), nations::truce{date_tag(), holder}), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(primary_slot.nation), nations::truce { date_tag(), from_slot.nation }), true);
	}
	bool tf_truce_with_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(primary_slot.nation), nations::truce { date_tag(), this_slot.nation }), true);
	}
	bool tf_truce_with_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(primary_slot.nation), nations::truce { date_tag(), owner }), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(primary_slot.nation), nations::truce { date_tag(), owner }), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(primary_slot.nation), nations::truce { date_tag(), owner }), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_total_pops_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.nations.get<nation::total_core_population>(id)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_total_pops_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.states.get<state::total_population>(id)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_total_pops_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		return compare_values(tval[0], float(ws.w.province_s.province_state_container.get<province_state::total_population>(id)), read_float_from_payload(tval + 2));
	}
	bool tf_total_pops_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			if(auto si = provinces::province_state(ws, location); si)
				return tf_total_pops_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_pop_type_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0],
				0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type)),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_type_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0],
				0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type)),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_type_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type)), true);
	}
	bool tf_has_pop_type_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::type>(primary_slot.pop) == trigger_payload(tval[2]).small.values.pop_type, true);
	}
	bool tf_has_empty_adjacent_province(TRIGGER_PARAMTERS) {
		auto adj_range = ws.s.province_m.same_type_adjacency.get_row(primary_slot.prov);
		for(auto p : adj_range) {
			if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_leader(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_named_leader(ws, primary_slot.nation, trigger_payload(tval[2]).text), true);
	}
	bool tf_ai(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_not_ai_controlled>(primary_slot.nation) == false, true);
	}
	bool tf_can_create_vassals(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], true, true); // stub for apparently unused 
	}
	bool tf_is_possible_vassal(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], can_release_as_vassal(ws, primary_slot.nation, trigger_payload(tval[2]).tag), true);
	}
	bool tf_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot.prov == provinces::province_tag(tval[2]), true);
	}
	bool tf_vassal_of_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == tag_holder, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_vassal_of_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == from_slot.nation, true);
	}
	bool tf_vassal_of_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == this_slot.nation, true);
	}
	bool tf_vassal_of_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_vassal_of_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_vassal_of_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(primary_slot.nation) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_substate_of_tag(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation) == false)
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_tag(tval, ws, primary_slot, nullptr, nullptr);
	}
	bool tf_substate_of_from(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation) == false)
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_nation(tval, ws, primary_slot, from_slot, nullptr);
	}
	bool tf_substate_of_this_nation(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation) == false)
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_nation(tval, ws, primary_slot, this_slot, nullptr);
	}
	bool tf_substate_of_this_province(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation) == false)
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_province(tval, ws, primary_slot, this_slot, nullptr);
	}
	bool tf_substate_of_this_state(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation) == false)
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_state(tval, ws, primary_slot, this_slot, nullptr);
	}
	bool tf_substate_of_this_pop(TRIGGER_PARAMTERS) {
		if(ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation) == false)
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_pop(tval, ws, primary_slot, this_slot, nullptr);
	}
	bool tf_alliance_with_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays,
				ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation), holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_alliance_with_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation), from_slot.nation), true);
	}
	bool tf_alliance_with_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation), this_slot.nation), true);
	}
	bool tf_alliance_with_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation), owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_alliance_with_this_state(TRIGGER_PARAMTERS) {
		nations::country_tag owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation), owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_alliance_with_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(primary_slot.nation), owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_recently_lost_war(TRIGGER_PARAMTERS) {
		auto last_lost = ws.w.nation_s.nations.get<nation::last_lost_war>(primary_slot.nation);
		return compare_values(tval[0], is_valid_index(last_lost) && (to_index(ws.w.current_date) - to_index(last_lost)) < (365 * 5 + 1), true);
	}
	bool tf_is_mobilised(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_mobilized>(primary_slot.nation), true);
	}
	bool tf_mobilisation_size(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_size>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_crime_higher_than_education_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.nation_s.nations.get<nation::f_administrative_spending>(primary_slot.nation) >= ws.w.nation_s.nations.get<nation::f_education_spending>(primary_slot.nation), true);
	}
	bool tf_crime_higher_than_education_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_administrative_spending>(owner) >= ws.w.nation_s.nations.get<nation::f_education_spending>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_crime_higher_than_education_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_administrative_spending>(owner) >= ws.w.nation_s.nations.get<nation::f_education_spending>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_crime_higher_than_education_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_administrative_spending>(owner) >= ws.w.nation_s.nations.get<nation::f_education_spending>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_agree_with_ruling_party(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		auto pop_id = primary_slot.pop;
		if(owner && ws.w.population_s.pops.is_valid_index(pop_id)) {
			auto ruling_ideology = ws.w.nation_s.nations.get<nation::ruling_ideology>(owner);
			auto population_size = ws.w.population_s.pops.get<pop::size>(pop_id);
			if(is_valid_index(ruling_ideology) & (population_size != 0)) {
				return compare_values(tval[0],
					float(ws.w.population_s.pop_demographics.get(pop_id, population::to_demo_tag(ws, ruling_ideology))) / float(population_size),
					read_float_from_payload(tval + 2));
			}
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_is_colonial_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			nations::is_colonial_or_protectorate(ws, primary_slot.state),
			true);
	}
	bool tf_is_colonial_province(TRIGGER_PARAMTERS) {
		auto si = provinces::province_state(ws, primary_slot.prov);
		if(si)
			return tf_is_colonial_state(tval, ws, si, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_factories(TRIGGER_PARAMTERS) {
		auto& factories = ws.w.nation_s.states.get<state::factories>(primary_slot.state);
		for(int32_t i = state::factories_count; i--; ) {
			if(factories[i].type)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_in_default_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0],
				economy::is_bankrupt(ws, holder),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_default_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			economy::is_bankrupt(ws, primary_slot.nation),
			true);
	}
	bool tf_in_default_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			economy::is_bankrupt(ws, primary_slot.nation),
			true);
	}
	bool tf_in_default_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_in_default_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_default_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_in_default_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_default_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_in_default_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_total_num_of_ports(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::num_ports>(primary_slot.nation), tval[2]);
	}
	bool tf_always(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], true, true);
	}
	bool tf_election(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_holding_election>(primary_slot.nation), true);
	}
	bool tf_has_global_flag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] != 0.0f, true);
	}
	bool tf_is_capital(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::current_capital>(owner) == primary_slot.prov, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_nationalvalue_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_value>(primary_slot.nation) == trigger_payload(tval[2]).nat_mod, true);
	}
	bool tf_nationalvalue_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_nationalvalue_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_nationalvalue_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_nationalvalue_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_industrial_score_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), trigger_payload(tval[2]).signed_value);
	}
	bool tf_industrial_score_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::industrial_score>(from_slot.nation));
	}
	bool tf_industrial_score_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::industrial_score>(this_slot.nation));
	}
	bool tf_industrial_score_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::industrial_score>(owner));
		else
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), 0i16);
	}
	bool tf_industrial_score_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::industrial_score>(owner));
		else
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), 0i16);
	}
	bool tf_industrial_score_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::industrial_score>(owner));
		else
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::industrial_score>(primary_slot.nation), 0i16);
	}
	bool tf_military_score_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), trigger_payload(tval[2]).signed_value);
	}
	bool tf_military_score_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::military_score>(from_slot.nation));
	}
	bool tf_military_score_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::military_score>(this_slot.nation));
	}
	bool tf_military_score_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::military_score>(owner));
		else
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), 0i16);
	}
	bool tf_military_score_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::military_score>(owner));
		else
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), 0i16);
	}
	bool tf_military_score_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), ws.w.nation_s.nations.get<nation::military_score>(owner));
		else
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::military_score>(primary_slot.nation), 0i16);
	}
	bool tf_civilized_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_civilized>(primary_slot.nation), true);
	}
	bool tf_civilized_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_civilized>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_civilized_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_civilized>(owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_national_provinces_occupied(TRIGGER_PARAMTERS) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(primary_slot.nation));
		if(owned_range.first != owned_range.second) {
			int32_t count_owned_controlled = 0;
			for(auto p : owned_range) {
				if(provinces::province_controller(ws, p) == primary_slot.nation)
					++count_owned_controlled;
			}
			return compare_values(tval[0], float(count_owned_controlled) / float(owned_range.second - owned_range.first), read_float_from_payload(tval + 2));
		} else {
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
		}
	}
	bool tf_is_greater_power_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::is_great_power(ws, primary_slot.nation), true);
	}
	bool tf_is_greater_power_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return compare_values(tval[0], nations::is_great_power(ws, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_greater_power_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return compare_values(tval[0], nations::is_great_power(ws, owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rich_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::rich_tax>(primary_slot.nation), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_middle_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::middle_tax>(primary_slot.nation), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_poor_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::poor_tax>(primary_slot.nation), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_social_spending>(primary_slot.nation), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_social_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_social_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_social_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_military_spending>(primary_slot.nation), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_military_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_administrative_spending>(primary_slot.nation), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_administration_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::f_education_spending>(primary_slot.nation), float(trigger_payload(tval[2]).signed_value) / 100.0f);
	}
	bool tf_education_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_colonial_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_colonial_nation>(primary_slot.nation), true);
	}
	bool tf_pop_majority_religion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_religion>(primary_slot.nation) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_religion_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_religion>(primary_slot.state) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_religion_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_religion>(primary_slot.prov) == trigger_payload(tval[2]).small.values.religion,
			true);
	}
	bool tf_pop_majority_culture_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_culture>(primary_slot.nation) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_culture_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_culture>(primary_slot.state) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_culture_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_culture>(primary_slot.prov) == trigger_payload(tval[2]).culture,
			true);
	}
	bool tf_pop_majority_issue_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_issue>(primary_slot.nation) == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_issue>(primary_slot.state) == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_issue>(primary_slot.prov) == trigger_payload(tval[2]).small.values.option,
			true);
	}
	bool tf_pop_majority_issue_pop(TRIGGER_PARAMTERS) {
		auto id = primary_slot.pop;
		if(ws.w.population_s.pops.is_valid_index(id))
			return compare_values(tval[0], population::is_dominant_issue(ws, id, trigger_payload(tval[2]).small.values.option), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_pop_majority_ideology_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::dominant_ideology>(primary_slot.nation) == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::dominant_ideology>(primary_slot.state) == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			ws.w.province_s.province_state_container.get<province_state::dominant_ideology>(primary_slot.prov) == trigger_payload(tval[2]).small.values.ideology,
			true);
	}
	bool tf_pop_majority_ideology_pop(TRIGGER_PARAMTERS) {
		auto id = primary_slot.pop;
		if(ws.w.population_s.pops.is_valid_index(id))
			return compare_values(tval[0], population::is_dominant_ideology(ws, id, trigger_payload(tval[2]).small.values.ideology), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_poor_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::poor_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::poor_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws)));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::poor_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(primary_slot.pop);
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor)
			return compare_values(tval[0], population::get_militancy_direct(ws, primary_slot.pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::middle_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::middle_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws)));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::middle_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(primary_slot.pop);
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle)
			return compare_values(tval[0], population::get_militancy_direct(ws, primary_slot.pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::rich_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws)));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::rich_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws)));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::rich_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(primary_slot.pop);
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich)
			return compare_values(tval[0], population::get_militancy_direct(ws, primary_slot.pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_tax_above_poor(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::rich_tax>(primary_slot.nation) > ws.w.nation_s.nations.get<nation::poor_tax>(primary_slot.nation), true);
	}
	bool tf_culture_has_union_tag_pop(TRIGGER_PARAMTERS) {
		auto pop_culture = ws.w.population_s.pops.get<pop::culture>(primary_slot.pop);
		if(is_valid_index(pop_culture)) {
			auto group = ws.s.culture_m.culture_container[pop_culture].group;
			return compare_values(tval[0], is_valid_index(ws.s.culture_m.culture_groups[group].union_tag), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_culture_has_union_tag_nation(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], is_valid_index(ws.s.culture_m.culture_groups[group].union_tag), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_tag(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == trigger_payload(tval[2]).tag, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_from(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == ws.w.nation_s.nations.get<nation::tag>(from_slot.nation), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_this_nation(TRIGGER_PARAMTERS) {
		auto primary_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == ws.w.nation_s.nations.get<nation::tag>(this_slot.nation), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_nation(TRIGGER_PARAMTERS) {
		auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		auto this_culture = ws.w.nation_s.nations.get<nation::primary_culture>(this_slot.nation);
		return compare_values(tval[0],
			is_valid_index(prim_culture) &&
			is_valid_index(this_culture) &&
			ws.s.culture_m.culture_container[prim_culture].group == ws.s.culture_m.culture_container[this_culture].group, true);
	}
	bool tf_this_culture_union_this_union_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_minorities_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto accepted_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation)));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(primary_slot.nation));
			for(auto c : accepted_range)
				accepted_pop += ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_minorities_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
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
		auto id = primary_slot.prov;
		auto owner = provinces::province_owner(ws, primary_slot.prov);
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
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::revanchism>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_revanchism_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::revanchism>(owner), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_crime(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.province_s.province_state_container.get<province_state::crime>(primary_slot.prov)), true);
	}
	bool tf_num_of_substates(TRIGGER_PARAMTERS) {
		auto vassal_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(primary_slot.nation));
		uint32_t count_substates = 0ui32;
		for(auto v : vassal_range) {
			if(is_valid_index(v) && ws.w.nation_s.nations.get<nation::is_substate>(v))
				++count_substates;
		}
		return compare_values(tval[0], count_substates, uint32_t(tval[2]));
	}
	bool tf_num_of_vassals_no_substates(TRIGGER_PARAMTERS) {
		auto vassal_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(primary_slot.nation));
		uint32_t count_non_substates = 0ui32;
		for(auto v : vassal_range) {
			if(is_valid_index(v) && ws.w.nation_s.nations.get<nation::is_substate>(v))
				++count_non_substates;
		}
		return compare_values(tval[0], count_non_substates, uint32_t(tval[2]));
	}
	bool tf_brigades_compare_this(TRIGGER_PARAMTERS) {
		auto main_brigades = military::total_active_divisions(ws, primary_slot.nation);
		auto this_brigades = military::total_active_divisions(ws, this_slot.nation);
		return compare_values(tval[0],
			this_brigades != 0 ? float(main_brigades) / float(this_brigades) : 1'000'000.0f,
			read_float_from_payload(tval + 2));
	}
	bool tf_brigades_compare_from(TRIGGER_PARAMTERS) {
		auto main_brigades = military::total_active_divisions(ws, primary_slot.nation);
		auto from_brigades = military::total_active_divisions(ws, from_slot.nation);
		return compare_values(tval[0],
			from_brigades != 0 ? float(main_brigades) / float(from_brigades) : 1'000'000.0f,
			read_float_from_payload(tval + 2));
	}
	bool tf_constructing_cb_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(primary_slot.nation) == tag_holder, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(primary_slot.nation) == from_slot.nation, true);
	}
	bool tf_constructing_cb_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(primary_slot.nation) == this_slot.nation, true);
	}
	bool tf_constructing_cb_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(primary_slot.nation) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(primary_slot.nation) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_target>(primary_slot.nation) == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_discovered(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_discovered>(primary_slot.nation), true);
	}
	bool tf_constructing_cb_progress(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_progress>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_civilization_progress(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.modifier_values.get<modifiers::national_offsets::civilization_progress_modifier>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_constructing_cb_type(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::cb_construction_type>(primary_slot.nation) == trigger_payload(tval[2]).small.values.cb_type, true);
	}
	bool tf_is_our_vassal_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(tag_holder) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(from_slot.nation) == primary_slot.nation, true);
	}
	bool tf_is_our_vassal_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(this_slot.nation) == primary_slot.nation, true);
	}
	bool tf_is_our_vassal_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(owner) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(owner) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::overlord>(owner) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_substate(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::is_substate>(primary_slot.nation), true);
	}
	bool tf_great_wars_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.great_wars_enabled, true);
	}
	bool tf_can_nationalize(TRIGGER_PARAMTERS) {
		auto influencer_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(primary_slot.nation));
		auto this_id = primary_slot.nation;
		for(auto c : influencer_range) {
			if(ws.w.nation_s.nations.is_valid_index(c) && nations::get_foreign_investment(ws, c, this_id) != 0.0f)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_part_of_sphere(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.nation_s.nations.get<nation::sphere_leader>(primary_slot.nation)), true);
	}
	bool tf_is_sphere_leader_of_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(holder) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(from_slot.nation) == primary_slot.nation, true);
	}
	bool tf_is_sphere_leader_of_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(this_slot.nation) == primary_slot.nation, true);
	}
	bool tf_is_sphere_leader_of_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(owner) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(owner) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
		if(owner)
			return compare_values(tval[0], ws.w.nation_s.nations.get<nation::sphere_leader>(owner) == primary_slot.nation, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_number_of_states(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(primary_slot.nation)), uint32_t(tval[2]));
	}
	bool tf_war_score(TRIGGER_PARAMTERS) {
		//stub for apparently unused trigger
		return compare_values(tval[0], true, true);
	}
	bool tf_is_releasable_vassal_from(TRIGGER_PARAMTERS) {
		auto tag = ws.w.nation_s.nations.get<nation::tag>(from_slot.nation);
		if(is_valid_index(tag))
			return compare_values(tval[0], !(ws.w.culture_s.national_tags_state[tag].is_not_releasable), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_releasable_vassal_other(TRIGGER_PARAMTERS) {
		auto tag = ws.w.nation_s.nations.get<nation::tag>(primary_slot.nation);
		if(is_valid_index(tag))
			return compare_values(tval[0], !(ws.w.culture_s.national_tags_state[tag].is_not_releasable), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_recent_imigration(TRIGGER_PARAMTERS) {
		auto last_immigration = ws.w.province_s.province_state_container.get<province_state::last_immigration>(primary_slot.prov);
		if(!is_valid_index(last_immigration))
			return compare_values(tval[0], std::numeric_limits<uint16_t>::max(), tval[2]);
		else
			return compare_values(tval[0], to_index(ws.w.current_date) - to_index(last_immigration), int32_t(tval[2]));
	}
	bool tf_province_control_days(TRIGGER_PARAMTERS) {
		auto last_control_change = ws.w.province_s.province_state_container.get<province_state::last_controller_change>(primary_slot.prov);
		if(!is_valid_index(last_control_change))
			return compare_values(tval[0], std::numeric_limits<uint16_t>::max(), tval[2]);
		else
			return compare_values(tval[0], to_index(ws.w.current_date) - to_index(last_control_change), int32_t(tval[2]));
	}
	bool tf_is_disarmed(TRIGGER_PARAMTERS) {
		auto disarmed_date = ws.w.nation_s.nations.get<nation::disarmed_until>(primary_slot.nation);
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
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::social_movement_support>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_political_movement_strength(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::political_movement_support>(primary_slot.nation), read_float_from_payload(tval + 2));
	}
	bool tf_can_build_factory_in_capital_state(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_social_movement(TRIGGER_PARAMTERS) {
		auto mt = ws.w.population_s.pops.get<pop::movement>(primary_slot.pop);
		if(is_valid_index(mt))
			return compare_values(tval[0], ws.w.population_s.pop_movements[mt].type == population::movement_type::social, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_political_movement(TRIGGER_PARAMTERS) {
		auto mt = ws.w.population_s.pops.get<pop::movement>(primary_slot.pop);
		if(is_valid_index(mt))
			return compare_values(tval[0], ws.w.population_s.pop_movements[mt].type == population::movement_type::political, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_cultural_sphere(TRIGGER_PARAMTERS) {
		auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(primary_slot.nation);
		auto culture_group = is_valid_index(prim_culture) ? ws.s.culture_m.culture_container[prim_culture].group : cultures::culture_group_tag();
		auto sphere_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(primary_slot.nation));
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
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(primary_slot.pop) == ws.w.population_s.pops.get<pop::culture>(this_slot.pop), true);
	}
	bool tf_has_pop_culture_state_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ws.w.population_s.pops.get<pop::culture>(this_slot.pop);
		auto id = primary_slot.state;
		if(is_valid_index(culture) && ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_province_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ws.w.population_s.pops.get<pop::culture>(this_slot.pop);
		auto id = primary_slot.prov;
		if(is_valid_index(culture))
			return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_nation_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ws.w.population_s.pops.get<pop::culture>(this_slot.pop);
		auto id = primary_slot.nation;
		if(is_valid_index(culture) && ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::culture>(primary_slot.pop) == trigger_payload(tval[2]).culture, true);
	}
	bool tf_has_pop_culture_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
	}
	bool tf_has_pop_culture_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_pop_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == ws.w.population_s.pops.get<pop::religion>(this_slot.pop), true);
	}
	bool tf_has_pop_religion_state_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ws.w.population_s.pops.get<pop::religion>(this_slot.pop);
		auto id = primary_slot.state;
		if(is_valid_index(religion) && ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_province_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ws.w.population_s.pops.get<pop::religion>(this_slot.pop);
		auto id = primary_slot.prov;
		if(is_valid_index(religion))
			return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_nation_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ws.w.population_s.pops.get<pop::religion>(this_slot.pop);
		auto id = primary_slot.nation;
		if(is_valid_index(religion) && ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::religion>(primary_slot.pop) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_has_pop_religion_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
	}
	bool tf_has_pop_religion_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_life_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::needs_satisfaction>(primary_slot.pop), read_float_from_payload(tval + 2));
	}
	bool tf_everyday_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::needs_satisfaction>(primary_slot.pop), read_float_from_payload(tval + 2) + 1.0f);
	}
	bool tf_luxury_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.population_s.pops.get<pop::needs_satisfaction>(primary_slot.pop), read_float_from_payload(tval + 2) + 2.0f);
	}
	bool tf_consciousness_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], population::get_consciousness_direct(ws, primary_slot.pop), read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		auto con = ws.w.province_s.province_demographics.get(id, population::consciousness_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) * 10.0f / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto con = ws.w.nation_s.state_demographics.get(id, population::consciousness_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::consciousness_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], population::get_literacy_direct(ws, primary_slot.pop), read_float_from_payload(tval + 2));
	}
	bool tf_literacy_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		auto con = ws.w.province_s.province_demographics.get(id, population::literacy_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto con = ws.w.nation_s.state_demographics.get(id, population::literacy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::literacy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], population::get_militancy_direct(ws, primary_slot.pop), read_float_from_payload(tval + 2));
	}
	bool tf_militancy_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		auto con = ws.w.province_s.province_demographics.get(id, population::militancy_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) * 10.0f / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.states.get<state::total_population>(id);
			auto con = ws.w.nation_s.state_demographics.get(id, population::militancy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nations.get<nation::total_core_population>(id);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::militancy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_trade_goods_in_state_state(TRIGGER_PARAMTERS) {
		auto state_region = ws.w.nation_s.states.get<state::region_id>(primary_slot.state);
		if(is_valid_index(state_region)) {
			auto prov_range = ws.s.province_m.states_to_province_index.get_row(state_region);
			for(auto p : prov_range) {
				if(provinces::province_state(ws, p) == primary_slot.state
					&& ws.w.province_s.province_state_container.get<province_state::rgo_production>(p) == trigger_payload(tval[2]).small.values.good) {

					return compare_values(tval[0], true, true);
				}
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_trade_goods_in_state_province(TRIGGER_PARAMTERS) {
		auto si = provinces::province_state(ws, primary_slot.prov);
		if(si)
			return tf_trade_goods_in_state_state(tval, ws, si, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_flashpoint(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(ws.w.nation_s.states.get<state::crisis_tag>(primary_slot.state)), true);
	}
	bool tf_flashpoint_tension(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.states.get<state::current_tension>(primary_slot.state), read_float_from_payload(tval + 2));
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
		auto id = primary_slot.nation;
		return compare_values(tval[0],
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, id) ||
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, id) ||
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, id),
			true);
	}
	bool tf_involved_in_crisis_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_involved_in_crisis_nation(nullptr, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rich_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_rich_strata_life_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_rich_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_rich_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_middle_strata_life_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_middle_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_middle_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_poor_strata_life_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
		if(is_valid_index(location)) {
			auto si = provinces::province_state(ws, location);
			if(si)
				return tf_poor_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
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
			return compare_values(tval[0], nations::get_influence_value(ws, primary_slot.nation, holder), int32_t(tval[2]));
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_diplomatic_influence_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_influence_value(ws, primary_slot.nation, this_slot.nation), int32_t(tval[2]));
	}
	bool tf_diplomatic_influence_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_diplomatic_influence_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_diplomatic_influence_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_influence_value(ws, primary_slot.nation, from_slot.nation), int32_t(tval[2]));
	}
	bool tf_diplomatic_influence_from_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, from_slot.prov);
		if(owner)
			return tf_diplomatic_influence_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_pop_unemployment_nation(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_state(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_province(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type));
		if(total_pop != 0)
			return compare_values(tval[0], 1.0f - float(ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_pop(TRIGGER_PARAMTERS) {
		auto id = primary_slot.pop;
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_size = ws.w.population_s.pops.get<pop::size>(id);
			if(total_size != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.population_s.pop_demographics.get(id, population::total_employment_tag)) / float(total_size), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_nation_this_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(this_slot.pop);
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_state_this_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(this_slot.pop);
		auto id = primary_slot.state;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_province_this_pop(TRIGGER_PARAMTERS) {
		auto type = ws.w.population_s.pops.get<pop::type>(this_slot.pop);
		auto id = primary_slot.prov;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type));
		if(total_pop != 0)
			return compare_values(tval[0], 1.0f - float(ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_relation_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[3]).tag].holder;
		if(holder)
			return compare_values(tval[0], nations::get_relationship(ws, primary_slot.nation, holder), int32_t(trigger_payload(tval[2]).signed_value));
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_relation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_relationship(ws, primary_slot.nation, this_slot.nation), int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_relation_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_relation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_relation_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_relationship(ws, primary_slot.nation, from_slot.nation), int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_relation_from_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, from_slot.prov);
		if(owner)
			return tf_relation_this_nation(tval, ws, primary_slot, owner, nullptr);
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_check_variable(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], ws.w.nation_s.national_variables.get(id, triggers::trigger_payload(tval[3]).nat_var), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_upper_house(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
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
		auto location = ws.w.population_s.pops.get<pop::location>(primary_slot.pop);
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
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(from_slot.prov, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_generic(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(primary_slot.prov, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_from_nation_scope_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(primary_slot.prov, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_from_province_scope_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(primary_slot.prov, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_can_build_in_province_railroad_no_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(primary_slot.prov)) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(primary_slot.prov) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(from_slot.nation),
			true);
	}
	bool tf_can_build_in_province_railroad_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(primary_slot.prov)) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(primary_slot.prov) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(from_slot.nation),
			true);
	}
	bool tf_can_build_in_province_railroad_no_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(primary_slot.prov)) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(primary_slot.prov) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(this_slot.nation),
			true);
	}
	bool tf_can_build_in_province_railroad_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::railroad_level>(primary_slot.prov)) +
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(primary_slot.prov) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(this_slot.nation),
			true);
	}
	bool tf_can_build_in_province_fort_no_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(primary_slot.prov)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(from_slot.nation),
			true);
	}
	bool tf_can_build_in_province_fort_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(primary_slot.prov)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(from_slot.nation),
			true);
	}
	bool tf_can_build_in_province_fort_no_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(primary_slot.prov)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(this_slot.nation),
			true);
	}
	bool tf_can_build_in_province_fort_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(ws.w.province_s.province_state_container.get<province_state::fort_level>(primary_slot.prov)) <
			ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(this_slot.nation),
			true);
	}
	bool tf_can_build_in_province_naval_base_no_limit_from_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(primary_slot.prov);
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(from_slot.nation),
				true);
		} else if(auto si = provinces::province_state(ws, primary_slot.prov); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(primary_slot.prov) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(from_slot.nation) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(primary_slot.prov);
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(from_slot.nation),
				true);
		} else if(auto si = provinces::province_state(ws, primary_slot.prov); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(primary_slot.prov) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(from_slot.nation) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_no_limit_this_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(primary_slot.prov);
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(this_slot.nation),
				true);
		} else if(auto si = provinces::province_state(ws, primary_slot.prov); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(primary_slot.prov) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(this_slot.nation) >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		auto nb_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(primary_slot.prov);
		if(nb_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(nb_level) <
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(this_slot.nation),
				true);
		} else if(auto si = provinces::province_state(ws, primary_slot.prov); si) {
			return compare_values(tval[0],
				!is_valid_index(nations::state_port_province(ws, si)) &&
				ws.s.province_m.province_container.get<province::is_coastal>(primary_slot.prov) &&
				ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(this_slot.nation) >= technologies::tech_attribute_type(1),
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
		auto id = primary_slot.nation;

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
		auto id = primary_slot.state;

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
		auto id = primary_slot.prov;

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
		auto id = primary_slot.nation;
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
		auto id = primary_slot.state;
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
		auto id = primary_slot.prov;

		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_pop(TRIGGER_PARAMTERS) {
		auto id = primary_slot.pop;
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
		auto id = primary_slot.nation;
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
		auto id = primary_slot.state;
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
		auto id = primary_slot.prov;

		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_pop(TRIGGER_PARAMTERS) {
		auto id = primary_slot.pop;
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

		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], ws.w.nation_s.active_issue_options.get(id, issue) == option, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(primary_slot.state);
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, primary_slot.prov);
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, primary_slot.pop);
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_pop_type_name_nation(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
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
		auto id = primary_slot.state;
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
		auto id = primary_slot.prov;

		auto total_pop = ws.w.province_s.province_state_container.get<province_state::total_population>(id);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_pop(TRIGGER_PARAMTERS) {
		if(ws.w.population_s.pops.get<pop::type>(primary_slot.pop) == trigger_payload(tval[2]).small.values.pop_type)
			return compare_values(tval[0], 1.0f, read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_good_name(TRIGGER_PARAMTERS) {
		auto id = primary_slot.nation;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.national_stockpiles.get(id, trigger_payload(tval[2]).small.values.good)), read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_religion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(primary_slot.nation) == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_religion_nation_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(primary_slot.nation) == ws.w.population_s.rebel_factions[from_slot.rebel].religion, true);
	}
	bool tf_religion_nation_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(primary_slot.nation) == ws.w.nation_s.nations.get<nation::national_religion>(from_slot.nation), true);
	}
	bool tf_religion_nation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.nation_s.nations.get<nation::national_religion>(primary_slot.nation) == ws.w.nation_s.nations.get<nation::national_religion>(this_slot.nation), true);
	}
	bool tf_religion_nation_this_state(TRIGGER_PARAMTERS) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_slot.state);
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_nation_this_province(TRIGGER_PARAMTERS) {
		auto owner = provinces::province_owner(ws, this_slot.prov);
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_nation_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, this_slot.pop);
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
