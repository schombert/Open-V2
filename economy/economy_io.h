#pragma once
#include "common\\common.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "economy.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"
#include <ppl.h>

class world_state;

template<>
class serialization::serializer<economy::artisan_type> : public serialization::memcpy_serializer<economy::artisan_type> {};
template<>
class serialization::serializer<economy::factory_type> : public serialization::memcpy_serializer<economy::factory_type> {};
template<>
class serialization::serializer<economy::good_definition> : public serialization::memcpy_serializer<economy::good_definition> {};
template<>
class serialization::serializer<economy::rgo_information> : public serialization::memcpy_serializer<economy::rgo_information> {};
template<>
class serialization::serializer<economy::fort_information> : public serialization::memcpy_serializer<economy::fort_information> {};
template<>
class serialization::serializer<economy::railroad_information> : public serialization::memcpy_serializer<economy::railroad_information> {};
template<>
class serialization::serializer<economy::naval_base_information> : public serialization::memcpy_serializer<economy::naval_base_information> {};

template<>
class serialization::serializer<economy::economic_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, economy::economic_state const& obj, world_state const&) {
		//serialize(output, obj.current_prices);
	}
	static void deserialize_object(std::byte const* &input, economy::economic_state& obj, world_state& ws);
	static size_t size(economy::economic_state const& obj, world_state const&) {
		return 0; //serialize_size(obj.current_prices);
	}
};

template<>
class serialization::serializer<economy::economic_scenario> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(economy::economic_scenario& obj) {
		for(auto const& i_good : obj.goods)
			obj.named_goods_index.emplace(i_good.name, i_good.id);
		for(auto const& i_factory : obj.factory_types)
			obj.named_factory_types_index.emplace(i_factory.name, i_factory.id);
		obj.goods_count = static_cast<uint32_t>(obj.goods.size());
		obj.aligned_32_goods_count = ((static_cast<uint32_t>(sizeof(economy::goods_qnty_type)) * obj.goods_count + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::goods_qnty_type));
	}

	static void serialize_object(std::byte* &output, economy::economic_scenario const& obj) {
		serialize(output, obj.good_type_names);
		serialize(output, obj.goods);
		serialize(output, obj.factory_types);
		serialize(output, obj.artisan_types);
		serialize(output, obj.rgo_mine);
		serialize(output, obj.rgo_farm);
		serialize(output, obj.fort);
		serialize(output, obj.railroad);
		serialize(output, obj.naval_base);
		serialize(output, obj.fort_modifier);
		serialize(output, obj.railroad_modifier);
		serialize(output, obj.naval_base_modifier);
		serialize(output, obj.factory_input_goods);
		serialize(output, obj.factory_efficiency_goods);
		serialize(output, obj.artisan_input_goods);
		serialize(output, obj.building_costs);
	}
	static void deserialize_object(std::byte const* &input, economy::economic_scenario& obj) {
		deserialize(input, obj.good_type_names);
		deserialize(input, obj.goods);
		deserialize(input, obj.factory_types);
		deserialize(input, obj.artisan_types);
		deserialize(input, obj.rgo_mine);
		deserialize(input, obj.rgo_farm);
		deserialize(input, obj.fort);
		deserialize(input, obj.railroad);
		deserialize(input, obj.naval_base);
		deserialize(input, obj.fort_modifier);
		deserialize(input, obj.railroad_modifier);
		deserialize(input, obj.naval_base_modifier);
		deserialize(input, obj.factory_input_goods);
		deserialize(input, obj.factory_efficiency_goods);
		deserialize(input, obj.artisan_input_goods);
		deserialize(input, obj.building_costs);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, economy::economic_scenario& obj, concurrency::task_group& tg) {
		deserialize(input, obj.good_type_names);
		deserialize(input, obj.goods);
		deserialize(input, obj.factory_types);
		deserialize(input, obj.artisan_types);
		deserialize(input, obj.rgo_mine);
		deserialize(input, obj.rgo_farm);
		deserialize(input, obj.fort);
		deserialize(input, obj.railroad);
		deserialize(input, obj.naval_base);
		deserialize(input, obj.fort_modifier);
		deserialize(input, obj.railroad_modifier);
		deserialize(input, obj.naval_base_modifier);
		deserialize(input, obj.factory_input_goods);
		deserialize(input, obj.factory_efficiency_goods);
		deserialize(input, obj.artisan_input_goods);
		deserialize(input, obj.building_costs);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(economy::economic_scenario const& obj) {
		return serialize_size(obj.good_type_names) +
			serialize_size(obj.goods) +
			serialize_size(obj.factory_types) +
			serialize_size(obj.artisan_types) +
			serialize_size(obj.rgo_mine) +
			serialize_size(obj.rgo_farm) +
			serialize_size(obj.fort) +
			serialize_size(obj.railroad) +
			serialize_size(obj.naval_base) +
			serialize_size(obj.fort_modifier) +
			serialize_size(obj.railroad_modifier) +
			serialize_size(obj.naval_base_modifier) +
			serialize_size(obj.factory_input_goods) +
			serialize_size(obj.factory_efficiency_goods) +
			serialize_size(obj.artisan_input_goods) +
			serialize_size(obj.building_costs);
	}
	template<typename T>
	static size_t size(economy::economic_scenario const& obj, T const&) {
		return size(obj);
	}
};

namespace modifiers {
	class modifiers_manager;
}

namespace economy {
	void read_goods(
		economic_scenario& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function);
	boost::container::flat_map<text_data::text_tag, factory_type_tag> read_buildings(
		economic_scenario& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function,
		modifiers::modifiers_manager& mod_manager
	); //invoke after reading goods, returns map of production type name -> factory building type
	void read_production_types(
		scenario::scenario_manager& s,
		boost::container::flat_map<text_data::text_tag, factory_type_tag>& map,
		const directory& source_directory);
}
