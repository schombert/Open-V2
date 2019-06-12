#pragma once
#include "gui.h"
#include "simple_fs\\simple_fs.h"
#include "graphics\\texture.h"
#include "gui_definitions\\gui_definitions_io.h"
#include "text_data\\text_data_io.h"
#include "graphics_objects\\graphics_objects_io.h"

template<>
class serialization::serializer<ui::gui_static> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, ui::gui_static const& obj) {
		serialize(output, obj.textures);
		serialize(output, obj.nmaps);
		serialize(output, obj.ui_definitions);
		serialize(output, obj.graphics_object_definitions);
		serialize(output, obj.text_data_sequences);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, ui::gui_static& obj, CONTEXT&& ...) {
		deserialize(input, obj.textures);
		deserialize(input, obj.nmaps);
		deserialize(input, obj.ui_definitions);
		deserialize(input, obj.graphics_object_definitions);
		deserialize(input, obj.text_data_sequences);
	}
	static size_t size(ui::gui_static const& obj) {
		return serialize_size(obj.textures) +
			serialize_size(obj.nmaps) +
			serialize_size(obj.ui_definitions) +
			serialize_size(obj.graphics_object_definitions) +
			serialize_size(obj.text_data_sequences);
	}
	template<typename T>
	static size_t size(ui::gui_static const& obj, T const&) {
		return size(obj);
	}
};

namespace ui {
	void load_gui_from_directory(const directory& source_directory, gui_static& static_manager, graphics::name_maps& gobj_nmaps);
}
