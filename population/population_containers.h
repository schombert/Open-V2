#pragma once
#include "common\common.h"
#include "population.h"
#include "population_io.h"
#include "pop.h"

namespace population {
	class population_state {
	public:

		tagged_vector<float, cultures::national_tag, padded_aligned_allocator_64<float>, true> independence_rebel_support;
		tagged_vector<float, cultures::national_tag, padded_aligned_allocator_64<float>, true> independence_movement_support;

		pop::container pops;
		//rebel_faction::container rebel_factions;
		//pop_movement::container pop_movements;
		stable_2d_vector<float, pop_tag, demo_tag, 2048, 256> pop_demographics;

		stable_variable_vector_storage_mk_2<pop_tag, 16, 131'072> pop_arrays;
		//stable_variable_vector_storage_mk_2<rebel_faction_tag, 8, 65536> rebel_faction_arrays;
		//stable_variable_vector_storage_mk_2<movement_tag, 8, 65536> pop_movement_arrays;
	};

	class population_manager {
	public:
		boost::container::flat_map<text_data::text_tag, pop_type_tag> named_pop_type_index;
		boost::container::flat_map<text_data::text_tag, rebel_type_tag> named_rebel_type_index;

		tagged_vector<pop_type, pop_type_tag> pop_types;
		tagged_vector<rebel_type, rebel_type_tag> rebel_types;

		tagged_fixed_blocked_2dvector<
			economy::goods_qnty_type,
			pop_type_tag,
			economy::goods_tag,
			aligned_allocator_64<economy::goods_qnty_type>> life_needs;
		tagged_fixed_blocked_2dvector<
			economy::goods_qnty_type,
			pop_type_tag,
			economy::goods_tag,
			aligned_allocator_64<economy::goods_qnty_type>> everyday_needs;
		tagged_fixed_blocked_2dvector<
			economy::goods_qnty_type,
			pop_type_tag,
			economy::goods_tag,
			aligned_allocator_64<economy::goods_qnty_type>> luxury_needs;
		//tagged_fixed_blocked_2dvector<
		//	float,
		//	pop_type_tag,
		//	military::unit_type_tag,
		//	aligned_allocator_32<float>> rebel_units;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, issues::option_tag> issue_inclination;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, ideologies::ideology_tag> ideological_inclination;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, pop_type_tag> promote_to;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, pop_type_tag> demote_to;

		tagged_fixed_2dvector<governments::government_tag, rebel_type_tag, governments::government_tag> rebel_change_government_to;

		modifiers::factor_tag promotion_chance;
		modifiers::factor_tag demotion_chance;
		modifiers::factor_tag migration_chance;
		modifiers::factor_tag colonial_migration_chance;
		modifiers::factor_tag emigration_chance;
		modifiers::factor_tag assimilation_chance;
		modifiers::factor_tag conversion_chance;

		government_employment clergy_pay;
		government_employment bureaucrat_pay;
		government_employment soldier_pay;
		government_employment officer_pay;

		int32_t officer_leadership = 2;

		std::vector<pop_type_tag> factory_workers;

		pop_type_tag artisan;
		pop_type_tag farmer;
		pop_type_tag laborer;
		pop_type_tag capitalist;
		pop_type_tag clergy;
		pop_type_tag bureaucrat;
		pop_type_tag slave;
		pop_type_tag soldier;
		pop_type_tag officer;

		rebel_type_tag nationalist_rebels;

		uint32_t count_poptypes = 0;

		template<typename F>
		void for_each_pop_type(F const& f) const {
			int32_t const cmax = int32_t(count_poptypes);
			for(int32_t i = 0; i < cmax; ++i) {
				f(pop_type_tag(pop_type_tag::value_base_t(i)));
			}
		}
	};
}

template<>
class serialization::serializer<population::population_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, population::population_state const& obj, world_state const&);
	static void deserialize_object(std::byte const* &input, population::population_state& obj, world_state& ws);
	static size_t size(population::population_state const& obj, world_state const&);
};

template<>
class serialization::serializer<population::population_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(population::population_manager& obj);

	static void serialize_object(std::byte* &output, population::population_manager const& obj);
	static void deserialize_object(std::byte const* &input, population::population_manager& obj);
	static void deserialize_object(std::byte const* &input, population::population_manager& obj, concurrency::task_group& tg);

	static size_t size(population::population_manager const& obj);
	template<typename T>
	static size_t size(population::population_manager const& obj, T const&) {
		return size(obj);
	}
};
