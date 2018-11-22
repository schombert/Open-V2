#pragma once
#include "common\\common.h"

#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"

namespace scenario {
	class scenario_manager;
}

namespace economy {
	constexpr goods_tag money_good(0);
	constexpr money_qnty_type daily_debt_payment = money_qnty_type(0.1);

	struct good_definition {
		static constexpr uint8_t not_available_from_start = 0x01;
		static constexpr uint8_t overseas_penalty         = 0x02;
		static constexpr uint8_t money                    = 0x04;
		static constexpr uint8_t mined                    = 0x08;
		static constexpr uint8_t has_rgo				  = 0x10;

		economy::money_qnty_type base_price = economy::money_qnty_type(1);
		economy::goods_qnty_type base_rgo_value = economy::goods_qnty_type(1);

		graphics::color_rgb color = {0, 0, 0};

		text_data::text_tag name;
		text_data::text_tag rgo_name;

		goods_tag id;

		goods_type_tag type;
		factory_type_tag factory_id;
		artisan_type_tag artisan_id;

		uint8_t flags = 0;
		uint8_t icon = 0;
	};

	enum class contribution_type : uint8_t {
		not_specified, input, output, throughput
	};

	struct owner_data {
		float effect_multiplier = 1.0f;
		population::pop_type_tag type;
		contribution_type contribution = contribution_type::not_specified;
	};

	struct employee_data {
		float effect_multiplier = 1.0f;
		float amount = 1.0f;
		population::pop_type_tag type;
		contribution_type contribution = contribution_type::not_specified;
	};

	struct bonus {
		float value = 0.0f;
		triggers::trigger_tag condition;
	};

	constexpr uint32_t max_worker_types = 3;

	struct workers_information {
		owner_data owner;
		employee_data workers[max_worker_types] = {};
		int32_t workforce = 40000;
	};

	struct factory_type {
		workers_information factory_workers;

		bonus bonuses[4] = {};

		economy::goods_qnty_type output_amount = economy::goods_qnty_type(1);

		uint32_t building_time = 730;

		text_data::text_tag name;

		goods_tag output_good;
		
		factory_type_tag id;
		bool coastal = false;
		bool default_enabled = false;
	};

	constexpr size_t fsize = sizeof(factory_type);

	struct artisan_type {
		economy::goods_qnty_type output_amount = economy::goods_qnty_type(1);

		int32_t workforce = 10000;

		text_data::text_tag name;
		goods_tag output_good;

		contribution_type artisan_contribution = contribution_type::output;
		bool coastal = false;
		artisan_type_tag id;
	};

	using rgo_information = workers_information;

	struct fort_information {
		uint32_t max_level = 6;
		uint32_t time = 1080;
		factory_type_tag cost_tag;
	};
	struct railroad_information : public fort_information {
		float infrastructure = 0.16f;
	};
	struct naval_base_information : public fort_information {
		uint32_t extra_cost = 15000;
		uint32_t naval_capacity = 1;
		uint32_t colonial_range = 50;
		uint32_t colonial_points[8] = {30,50,70,90,110,130,150,170};
	};

	struct worked_instance {
		float worker_populations[max_worker_types] = { 0 };
		float production_scale = 1.0f;
	};

	struct production_modifiers {
		float input_modifier = 0.0f;
		float output_modifier = 0.0f;
		float throughput_modifier = 0.0f;
	};

	struct factory_instance {
		factory_type* type = nullptr;
		float factory_bank = 0.0f;
		float factory_progress = 0.0f; // for upgrades and construction; level 0 & non zero progress = under construction
		worked_instance worker_data;
		uint16_t level = 0;
		uint8_t flags = 0;

		constexpr static uint8_t is_subsidized = 0x01;
		constexpr static uint8_t owner_is_upgrading = 0x02;
	};

#ifdef _DEBUG
//#define DEBUG_ECONOMY
#endif

	class economic_state {
	public:
		stable_variable_vector_storage_mk_2<money_qnty_type, 64, 30'000'000, true> purchasing_arrays;

#ifdef DEBUG_ECONOMY
		tagged_vector<economy::goods_qnty_type, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> world_rgo_production;
		tagged_vector<economy::goods_qnty_type, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> world_other_production;
		tagged_vector<economy::goods_qnty_type, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> world_pop_consumption;
		tagged_vector<economy::goods_qnty_type, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> world_other_consumption;

		std::mutex rgo_production_mutex;
		std::mutex other_production_mutex;
		std::mutex pop_consumption_mutex;
		std::mutex other_consumption_mutex;
#endif
		goods_tag coal;
	};

	class economic_scenario {
	public:
		tagged_vector<text_data::text_tag, goods_type_tag> good_type_names;
		tagged_vector<good_definition, goods_tag> goods; // note: last = money

		tagged_vector<factory_type, factory_type_tag> factory_types;
		tagged_vector<artisan_type, artisan_type_tag> artisan_types;

		rgo_information rgo_mine;
		rgo_information rgo_farm;

		fort_information fort;
		railroad_information railroad;
		naval_base_information naval_base;

		modifiers::provincial_modifier_tag fort_modifier;
		modifiers::provincial_modifier_tag railroad_modifier;
		modifiers::provincial_modifier_tag naval_base_modifier;

		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, factory_type_tag, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> factory_input_goods;
		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, factory_type_tag, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> factory_efficiency_goods;
		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, artisan_type_tag, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> artisan_input_goods;
		
		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, factory_type_tag, goods_tag, aligned_allocator_32<economy::goods_qnty_type>> building_costs;

		boost::container::flat_map<text_data::text_tag, goods_tag> named_goods_index;
		boost::container::flat_map<text_data::text_tag, factory_type_tag> named_factory_types_index;

		uint32_t goods_count = 0;
		uint32_t aligned_32_goods_count = 0;
	};
}
