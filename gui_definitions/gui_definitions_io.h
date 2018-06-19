#pragma once
#include "common\\common.h"
#include "gui_definitions.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"

namespace ui {
	void load_ui_definitions_from_directory(
		const directory& source_directory,
		ui::name_maps& nmaps,
		ui::definitions& defs,
		std::vector<std::pair<std::string, ui::errors>>& errors_generated,
		const text_handle_lookup& th_f,
		const font_handle_lookup& fh_f,
		const gobj_lookup& qt_f);
}

template<>
class serialization::serializer<ui::name_maps> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, ui::name_maps const& obj) {
		serialize(output, obj.button_names);
		serialize(output, obj.icon_names);
		serialize(output, obj.text_names);
		serialize(output, obj.position_names);
		serialize(output, obj.overlapping_region_names);
		serialize(output, obj.listbox_names);
		serialize(output, obj.scrollbar_names);
		serialize(output, obj.window_names);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, ui::name_maps& obj, CONTEXT&& ... c) {
		deserialize(input, obj.button_names);
		deserialize(input, obj.icon_names);
		deserialize(input, obj.text_names);
		deserialize(input, obj.position_names);
		deserialize(input, obj.overlapping_region_names);
		deserialize(input, obj.listbox_names);
		deserialize(input, obj.scrollbar_names);
		deserialize(input, obj.window_names);
	}
	static size_t size(ui::name_maps const& obj) {
		return serialize_size(obj.button_names) +
			serialize_size(obj.icon_names) +
			serialize_size(obj.text_names) +
			serialize_size(obj.position_names) +
			serialize_size(obj.overlapping_region_names) +
			serialize_size(obj.listbox_names) +
			serialize_size(obj.scrollbar_names) +
			serialize_size(obj.window_names);
	}
};

template<>
class serialization::serializer<ui::definitions> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, ui::definitions const& obj) {
		serialize(output, obj.buttons);
		serialize(output, obj.icons);
		serialize(output, obj.text);
		serialize(output, obj.positions);
		serialize(output, obj.overlapping_regions);
		serialize(output, obj.listboxes);
		serialize(output, obj.scrollbars);
		serialize(output, obj.windows);
		serialize(output, obj.standardlistbox_slider);
		serialize(output, obj.name_data);
		serialize(output, obj.name_to_element_map);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, ui::definitions& obj, CONTEXT&& ... c) {
		deserialize(input, obj.buttons);
		deserialize(input, obj.icons);
		deserialize(input, obj.text);
		deserialize(input, obj.positions);
		deserialize(input, obj.overlapping_regions);
		deserialize(input, obj.listboxes);
		deserialize(input, obj.scrollbars);
		deserialize(input, obj.windows);
		deserialize(input, obj.standardlistbox_slider);
		deserialize(input, obj.name_data);
		deserialize(input, obj.name_to_element_map);
	}
	static size_t size(ui::definitions const& obj) {
		return serialize_size(obj.buttons) +
			serialize_size(obj.icons) +
			serialize_size(obj.text) +
			serialize_size(obj.positions) +
			serialize_size(obj.overlapping_regions) +
			serialize_size(obj.listboxes) +
			serialize_size(obj.scrollbars) +
			serialize_size(obj.windows) +
			serialize_size(obj.standardlistbox_slider) +
			serialize_size(obj.name_data) +
			serialize_size(obj.name_to_element_map);
	}
};
