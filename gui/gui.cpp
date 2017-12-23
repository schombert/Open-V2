#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"
#include <algorithm>
#include "boost\\container\\small_vector.hpp"

#ifdef _DEBUG
#include "windows.h"
#undef min
#undef max
#endif

ui::gui_manager::~gui_manager() {};

void ui::gui_manager::destroy(gui_object & g) {
	if (&(gui_objects.at(tooltip)) == &g) {
		hide_tooltip();
		tooltip = gui_object_tag();
	}
	if (&(gui_objects.at(tooltip)) == &g)
		focus = gui_object_tag();

	if (g.associated_behavior) {
		if ((g.flags.load(std::memory_order_relaxed) & g.static_behavior) == 0) {
			g.associated_behavior->~gui_behavior();
			concurrent_allocator<gui_behavior>().deallocate(g.associated_behavior, 1);
		}
		g.associated_behavior = nullptr;
	}
	const auto type_handle = g.type_dependant_handle.load(std::memory_order_relaxed);
	if (type_handle != 0) {
		const auto flags = g.flags.load(std::memory_order_relaxed);
		if ((flags & gui_object::type_mask) == gui_object::type_text_instance) {
			text_instances.free(text_instance_tag(type_handle));
		} else if ((flags & gui_object::type_mask) == gui_object::type_graphics_object) {
			graphics_instances.free(graphics_instance_tag(type_handle));
		} else if (((flags & gui_object::type_mask) == gui_object::type_masked_flag) |
			((flags & gui_object::type_mask) == gui_object::type_progress_bar)) {
			multi_texture_instances.free(multi_texture_instance_tag(type_handle));
		}
	}

	gui_object_tag child = g.first_child;
	g.first_child = gui_object_tag();

	while (child != gui_object_tag()) {
		gui_object_tag next = gui_objects.at(child).right_sibling;
		gui_objects.free(child, *this);
		child = next;
	}

	g.parent = gui_object_tag();
	g.left_sibling = gui_object_tag();
	g.right_sibling = gui_object_tag();
	g.flags.store(0, std::memory_order_release);
	g.type_dependant_handle.store(0, std::memory_order_release);
}

namespace ui {
	text_data::alignment text_aligment_from_button_definition(const button_def& def) {
		switch (def.flags & button_def::format_mask) {
			case button_def::format_center:
				return text_data::alignment::center;
			case button_def::format_left:
				return text_data::alignment::left;
			default:
				return text_data::alignment::center;
		}
	}
	text_data::alignment text_aligment_from_text_definition(const text_def& def) {
		switch (def.flags & text_def::format_mask) {
			case text_def::format_center:
				return text_data::alignment::top_center;
			case text_def::format_left:
				return text_data::alignment::top_left;
			case text_def::format_right:
				return text_data::alignment::top_right;
			case text_def::format_justified:
				return text_data::alignment::top_left;
			default:
				return text_data::alignment::top_left;
		}
	}
	ui::text_color text_color_to_ui_text_color(text_data::text_color c) {
		switch (c) {
			case text_data::text_color::black:
				return ui::text_color::black;
			case text_data::text_color::green:
				return ui::text_color::green;
			case text_data::text_color::red:
				return ui::text_color::red;
			case text_data::text_color::unspecified:
				return ui::text_color::black;
			case text_data::text_color::white:
				return ui::text_color::white;
			case text_data::text_color::yellow:
				return ui::text_color::yellow;
		}
	}
}



namespace ui {
	tagged_object<ui::text_instance, ui::text_instance_tag> create_text_instance(ui::gui_manager &container, tagged_gui_object new_gobj, const text_format& fmt) {
		const auto new_text_instance = container.text_instances.emplace();

		new_gobj.object.flags.store(gui_object::type_text_instance | gui_object::visible | gui_object::enabled, std::memory_order_release);
		new_gobj.object.type_dependant_handle.store(to_index(new_text_instance.id), std::memory_order_release);

		new_text_instance.object.color = fmt.color;
		new_text_instance.object.font_handle = fmt.font_handle;
		new_text_instance.object.size = fmt.font_size / 2;
		new_text_instance.object.length = 0;

		return new_text_instance;
	}
}

namespace ui {
	float text_component_width(text_data::text_component& c, const std::vector<char16_t>& text_data, graphics::font& this_font, uint32_t font_size) {
		if (std::holds_alternative<text_data::text_chunk>(c)) {
			const auto chunk = std::get<text_data::text_chunk>(c);
			return this_font.metrics_text_extent(text_data.data() + chunk.offset, chunk.length, font_size, false);
		} else {
			return 0.0f;
		}
	}

	void shorten_text_instance_to_space(ui::text_instance& txt) {
		int32_t i = int32_t(txt.length) - 2;
		for (; i >= 0; --i) {
			if (txt.text[i] == u' ')
				break;
		}
		if(i >= 0)
			txt.length = i + 1;
	}

	class line_manager {
	private:
		boost::container::small_vector<gui_object*, 16, concurrent_allocator<gui_object*>> current_line;
		const text_data::alignment align;
		const int32_t max_line_extent;
	public:
		line_manager(text_data::alignment a, int32_t m) : align(a), max_line_extent(m) {}

		bool exceeds_extent(int32_t w) const { return w > max_line_extent; }
		void add_object(gui_object* o) {
			current_line.push_back(o);
		}
		void finish_current_line() {
			if ((align == text_data::alignment::bottom_left) | (align == text_data::alignment::top_left) | (align == text_data::alignment::left))
				return;

			int32_t total_element_width = 0;
			for (auto p : current_line)
				total_element_width += p->size.x;

			const int32_t adjustment = (max_line_extent - total_element_width) /
				((align == text_data::alignment::bottom_right) | (align == text_data::alignment::top_right) | (align == text_data::alignment::right) ? 1 : 2);
			for (auto p : current_line)
				p->position.x += adjustment;

			current_line.clear();
		}
	};

	class single_line_manager {
	public:
		bool exceeds_extent(int32_t) const { return false; };
		void add_object(gui_object*) const {}
		void finish_current_line() const {}
	};

	class null_behavior_creation {
	public:
		void operator()(tagged_gui_object) const {};
	};

	template<typename LM, typename BH = null_behavior_creation>
	std::pair<ui::xy_pair, uint32_t> text_chunk_to_instances(ui::gui_manager& container, vector_backed_string<char16_t> text_source, uint32_t offset_in_chunk, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM& lm, const BH& behavior_creator = null_behavior_creation()) {
		graphics::font& this_font = container.fonts.at(fmt.font_handle);

		const auto new_gobj = container.gui_objects.emplace();
		const auto new_text_instance = create_text_instance(container, new_gobj, fmt);

		new_text_instance.object.length = (uint8_t)std::min(ui::text_instance::max_instance_length, (uint32_t)(text_source.length()) - offset_in_chunk);
		memcpy(new_text_instance.object.text, text_source.get_str(container.text_data_sequences.text_data) + offset_in_chunk, (new_text_instance.object.length) * sizeof(char16_t));

		if (new_text_instance.object.length == ui::text_instance::max_instance_length
			&& new_text_instance.object.text[ui::text_instance::max_instance_length - 1] != u' ')
			shorten_text_instance_to_space(new_text_instance.object);


		const auto original_length = new_text_instance.object.length;
		auto previous_length = new_text_instance.object.length;
		float new_size = this_font.metrics_text_extent(new_text_instance.object.text, new_text_instance.object.length, fmt.font_size, false);

		while (lm.exceeds_extent(position.x + int32_t(new_size + 0.5f))) {
			shorten_text_instance_to_space(new_text_instance.object);

			if (new_text_instance.object.length == previous_length) {
				if (position.x != 0) {
					lm.finish_current_line();
					position.x = 0;
					position.y += this_font.line_height(fmt.font_size) + 0.5f;
					new_text_instance.object.length = original_length;
				} else {
					break;
				}
			}

			new_size = this_font.metrics_text_extent(new_text_instance.object.text, new_text_instance.object.length, fmt.font_size, false);
			previous_length = new_text_instance.object.length;
		}

		new_gobj.object.size = ui::xy_pair{
			int16_t(new_size + 0.5f),
			int16_t(this_font.line_height(fmt.font_size) + 0.5f) };
		new_gobj.object.position = position;

		behavior_creator(new_gobj);

		add_to_back(container, parent_object, new_gobj);
		lm.add_object(&(new_gobj.object));

		return std::make_pair( ui::xy_pair{ position.x + new_gobj.object.size.x, position.y}, offset_in_chunk + new_text_instance.object.length ) ;
	}
}

void ui::detail::create_multiline_text(gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format& fmt, const text_data::replacement* candidates, uint32_t count) {
	graphics::font& this_font = manager.fonts.at(fmt.font_handle);
	const auto& components = manager.text_data_sequences.all_sequences[text_handle];

	const auto components_start = manager.text_data_sequences.all_components.data() + components.starting_component;
	const auto components_end = components_start + components.component_count;

	ui::xy_pair position{ 0, 0 };
	ui::text_color current_color = fmt.color;

	ui::line_manager lm(align, container.object.size.x);
	
	for (auto component_i = components_start; component_i != components_end; ++component_i) {
		if (std::holds_alternative<text_data::color_change>(*component_i)) {
			current_color = text_color_to_ui_text_color(std::get<text_data::color_change>(*component_i).color);
		} else if (std::holds_alternative<text_data::line_break>(*component_i)) {
			lm.finish_current_line();
			position.x = 0;
			position.y += this_font.line_height(fmt.font_size) + 0.5f;
		} else if (std::holds_alternative<text_data::value_placeholder>(*component_i)) {
			const auto rep = text_data::find_replacement(std::get<text_data::value_placeholder>(*component_i), candidates, count);

			const auto replacement_text = rep ? std::get<1>(*rep) : vector_backed_string<char16_t>(text_data::name_from_value_type(std::get<text_data::value_placeholder>(*component_i).value));
			const auto replacement_text_length = replacement_text.length();

			const text_format format{ current_color, fmt.font_handle, fmt.font_size };
			uint32_t position_in_chunk = 0;

			if (rep) {
				while (position_in_chunk < replacement_text_length) {
					std::tie(position, position_in_chunk) = text_chunk_to_instances(manager, replacement_text, position_in_chunk, container, position, format, lm, std::get<2>(*rep));
				}
			} else {
				while (position_in_chunk < replacement_text_length) {
					std::tie(position, position_in_chunk) = text_chunk_to_instances(manager, replacement_text, position_in_chunk, container, position, format, lm);
				}
			}

		} else if (std::holds_alternative<text_data::text_chunk>(*component_i)) {
			const auto chunk = std::get<text_data::text_chunk>(*component_i);
			const text_format format{ current_color, fmt.font_handle, fmt.font_size };
			uint32_t position_in_chunk = 0;

			while (position_in_chunk < chunk.length) {
				std::tie(position, position_in_chunk) = text_chunk_to_instances(manager, chunk, position_in_chunk, container, position, format, lm);
			}
		}
	}

	lm.finish_current_line();
	container.object.size.y = position.y + this_font.line_height(fmt.font_size) + 0.5f;;
}

void ui::detail::create_linear_text(gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format& fmt, const text_data::replacement* candidates, uint32_t count) {
	const auto& components = manager.text_data_sequences.all_sequences[text_handle];
	graphics::font& this_font = manager.fonts.at(fmt.font_handle);

	const auto components_start = manager.text_data_sequences.all_components.data() + components.starting_component;
	const auto components_end = components_start + components.component_count;

	ui::xy_pair position{ 0, 0 };
	ui::text_color current_color = fmt.color;

	ui::single_line_manager lm;

	float x_extent = 0.0f;
	for (auto component_i = components_start; component_i != components_end; ++component_i) {
		x_extent += ui::text_component_width(*component_i, manager.text_data_sequences.text_data, this_font, fmt.font_size);
	}

	std::tie(position.x, position.y) = align_in_bounds(align, int32_t(x_extent + 0.5f), int32_t(this_font.line_height(fmt.font_size) + 0.5f), container.object.size.x, container.object.size.y);

	for (auto component_i = components_start; component_i != components_end; ++component_i) {
		if (std::holds_alternative<text_data::color_change>(*component_i)) {
			current_color = text_color_to_ui_text_color(std::get<text_data::color_change>(*component_i).color);
		} else if (std::holds_alternative<text_data::value_placeholder>(*component_i)) {
			const auto rep = text_data::find_replacement(std::get<text_data::value_placeholder>(*component_i), candidates, count);

			const auto replacement_text = rep ? std::get<1>(*rep) : vector_backed_string<char16_t>(text_data::name_from_value_type(std::get<text_data::value_placeholder>(*component_i).value));
			const auto replacement_text_length = replacement_text.length();

			const text_format format{ current_color, fmt.font_handle, fmt.font_size };
			uint32_t position_in_chunk = 0;

			if (rep) {
				while (position_in_chunk < replacement_text_length) {
					std::tie(position, position_in_chunk) = text_chunk_to_instances(manager, replacement_text, position_in_chunk, container, position, format, lm, std::get<2>(*rep));
				}
			} else {
				while (position_in_chunk < replacement_text_length) {
					std::tie(position, position_in_chunk) = text_chunk_to_instances(manager, replacement_text, position_in_chunk, container, position, format, lm);
				}
			}
			
		} else if (std::holds_alternative<text_data::text_chunk>(*component_i)) {
			const auto chunk = std::get<text_data::text_chunk>(*component_i);
			const text_format format{ current_color, fmt.font_handle, fmt.font_size };
			uint32_t position_in_chunk = 0;

			while (position_in_chunk < chunk.length) {
				std::tie(position, position_in_chunk) = text_chunk_to_instances(manager, chunk, position_in_chunk, container, position, format, lm);
			}
		}
	}
}

ui::tagged_gui_object ui::detail::create_element_instance(gui_manager& manager, button_tag handle) {
	const button_def& def = manager.ui_definitions.buttons[handle];
	const auto new_gobj = manager.gui_objects.emplace();

	const uint16_t rotation =
		(def.flags & button_def::rotation_mask) == button_def::rotation_90_left ?
		gui_object::rotation_left :
		gui_object::rotation_upright;

	auto& button_graphic_def = manager.graphics_object_definitions.definitions[def.graphical_object_handle];
	new_gobj.object.flags.store(gui_object::type_graphics_object | gui_object::visible | gui_object::enabled | rotation, std::memory_order_release);
	new_gobj.object.position = def.position;
	new_gobj.object.size = def.size;
	if (new_gobj.object.size == ui::xy_pair{ 0,0 })
		new_gobj.object.size = ui::xy_pair{ button_graphic_def.size.x, button_graphic_def.size.y };

	if (is_valid_index(button_graphic_def.primary_texture_handle)) {
		const auto button_graphic = manager.graphics_instances.emplace();

		button_graphic.object.frame = 0;
		button_graphic.object.graphics_object = &button_graphic_def;
		button_graphic.object.t = &(manager.textures.retrieve_by_key(button_graphic_def.primary_texture_handle));

		if (((int32_t)new_gobj.object.size.y | (int32_t)new_gobj.object.size.x) == 0) {
			button_graphic.object.t->load_filedata();
			new_gobj.object.size.y = button_graphic.object.t->get_height();
			new_gobj.object.size.x = button_graphic.object.t->get_width() / ((button_graphic_def.number_of_frames != 0) ? button_graphic_def.number_of_frames : 1);
		}

		new_gobj.object.type_dependant_handle.store(to_index(button_graphic.id), std::memory_order_release);
	}

	if (is_valid_index(def.text_handle)) {
		const auto[font_h, int_font_size] = graphics::unpack_font_handle(def.font_handle);
		detail::create_linear_text(manager, new_gobj, def.text_handle, text_aligment_from_button_definition(def), text_format{ ui::text_color::black, font_h, int_font_size });
	}

	return new_gobj;
}

ui::tagged_gui_object ui::detail::create_element_instance(gui_manager& manager, icon_tag handle) {
	const ui::icon_def& icon_def = manager.ui_definitions.icons[handle];
	const auto new_gobj = manager.gui_objects.emplace();

	const uint16_t rotation =
		(icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright?
		ui::gui_object::rotation_upright :
		((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);

	auto& graphic_object_def = manager.graphics_object_definitions.definitions[icon_def.graphical_object_handle];

	new_gobj.object.position = icon_def.position;
	if (new_gobj.object.size == ui::xy_pair{ 0,0 })
		new_gobj.object.size = ui::xy_pair{ graphic_object_def.size.x, graphic_object_def.size.y };

	if ((icon_def.flags & ui::icon_def::is_shield) == 0) {
		new_gobj.object.flags.store(ui::gui_object::type_graphics_object | ui::gui_object::visible | ui::gui_object::enabled | rotation, std::memory_order_release);

		if (is_valid_index(graphic_object_def.primary_texture_handle)) {
			const auto icon_graphic = manager.graphics_instances.emplace();

			icon_graphic.object.frame = icon_def.frame;
			icon_graphic.object.graphics_object = &graphic_object_def;
			icon_graphic.object.t = &(manager.textures.retrieve_by_key(graphic_object_def.primary_texture_handle));

			if (((int32_t)new_gobj.object.size.y | (int32_t)new_gobj.object.size.x) == 0) {
				icon_graphic.object.t->load_filedata();
				new_gobj.object.size.y = icon_graphic.object.t->get_height();
				new_gobj.object.size.x = icon_graphic.object.t->get_width() / ((graphic_object_def.number_of_frames != 0) ? graphic_object_def.number_of_frames : 1);
			}

			new_gobj.object.type_dependant_handle.store(to_index(icon_graphic.id), std::memory_order_release);
		}
	} else {
		new_gobj.object.flags.store(ui::gui_object::type_masked_flag | ui::gui_object::visible | ui::gui_object::enabled | rotation, std::memory_order_release);

		if (is_valid_index(graphic_object_def.primary_texture_handle) & is_valid_index(graphics::texture_tag(graphic_object_def.type_dependant))) {
			const auto flag_graphic = manager.multi_texture_instances.emplace();

			flag_graphic.object.flag = nullptr;
			flag_graphic.object.mask_or_primary = &(manager.textures.retrieve_by_key(graphic_object_def.primary_texture_handle));
			flag_graphic.object.overlay_or_secondary = &(manager.textures.retrieve_by_key(graphics::texture_tag(graphic_object_def.type_dependant)));

			if (((int32_t)new_gobj.object.size.y | (int32_t)new_gobj.object.size.x) == 0) {
				flag_graphic.object.overlay_or_secondary->load_filedata();
				new_gobj.object.size.y = flag_graphic.object.overlay_or_secondary->get_height();
				new_gobj.object.size.x = flag_graphic.object.overlay_or_secondary->get_width();
			}

			new_gobj.object.type_dependant_handle.store(to_index(flag_graphic.id), std::memory_order_release);
		}
	}

	new_gobj.object.size.x *= icon_def.scale;
	new_gobj.object.size.y *= icon_def.scale;

	return new_gobj;
}

ui::tagged_gui_object ui::detail::create_element_instance(gui_manager& manager, ui::text_tag handle, const text_data::replacement* candidates, uint32_t count) {
	const ui::text_def& text_def = manager.ui_definitions.text[handle];
	const auto new_gobj = manager.gui_objects.emplace();
	
	new_gobj.object.position = text_def.position;
	new_gobj.object.size = ui::xy_pair{ text_def.max_width, 0 };

	const auto background = text_def.flags & text_def.background_mask;
	if ((background != ui::text_def::background_none_specified) & (background != ui::text_def::background_transparency_tga)) {
		new_gobj.object.flags.store(ui::gui_object::type_graphics_object | ui::gui_object::visible | ui::gui_object::enabled, std::memory_order_release);
		new_gobj.object.type_dependant_handle = to_index(manager.graphics_object_definitions.standard_text_background);

		const auto bg_graphic = manager.graphics_instances.emplace();

		const auto texture =
			(background == ui::text_def::background_small_tiles_dialog_tga) ?
			manager.textures.standard_small_tiles_dialog :
			manager.textures.standard_tiles_dialog;

		bg_graphic.object.frame = 0;
		bg_graphic.object.graphics_object = &(manager.graphics_object_definitions.definitions[manager.graphics_object_definitions.standard_text_background]);
		bg_graphic.object.t = &(manager.textures.retrieve_by_key(texture));

		new_gobj.object.type_dependant_handle.store(to_index(bg_graphic.id), std::memory_order_release);
	}

	new_gobj.object.size.x -= text_def.border_size.x * 2;

	if (is_valid_index(text_def.text_handle)) {
		const auto[font_h, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
		detail::create_multiline_text(manager, new_gobj, text_def.text_handle, text_aligment_from_text_definition(text_def), text_format{ ui::text_color::black, font_h, int_font_size }, candidates, count);
	}

	for_each_child(manager, new_gobj, [adjust = text_def.border_size](tagged_gui_object c) {
		c.object.position += adjust;
	});

	new_gobj.object.size.x += text_def.border_size.x * 2;
	new_gobj.object.size.y += text_def.border_size.y * 2;

	return new_gobj;
}

void ui::detail::render_object_type(const gui_manager& manager, graphics::open_gl_wrapper& ogl, const gui_object& root_obj, ui::xy_pair position, uint32_t type, bool currently_enabled) {
	const float effective_position_x = static_cast<float>(position.x) + static_cast<float>(root_obj.position.x) * manager.scale();
	const float effective_position_y = static_cast<float>(position.y) + static_cast<float>(root_obj.position.y) * manager.scale();
	const float effective_width = static_cast<float>(root_obj.size.x) * manager.scale();
	const float effective_height = static_cast<float>(root_obj.size.y) * manager.scale();
	const auto current_rotation = root_obj.get_rotation();

	switch (type) {
		case ui::gui_object::type_barchart:
		{
			auto dt = manager.data_textures.safe_at(data_texture_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (dt) {
				ogl.render_barchart(
					currently_enabled,
					effective_position_x,
					effective_position_y,
					effective_width,
					effective_height,
					*dt,
					current_rotation);
			}
			break;
		}
		case ui::gui_object::type_graphics_object:
		{
			auto gi = manager.graphics_instances.safe_at(graphics_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (gi) {
				const auto t = gi->t;
				if (t) {
					if ((gi->graphics_object->flags & graphics::object::type_mask) == (uint8_t)graphics::object_type::bordered_rect) {
						ogl.render_bordered_rect(
							currently_enabled,
							gi->graphics_object->type_dependant,
							effective_position_x,
							effective_position_y,
							effective_width,
							effective_height,
							*t,
							current_rotation);
					} else if (gi->graphics_object->number_of_frames > 1) {
						ogl.render_subsprite(
							currently_enabled,
							gi->frame,
							gi->graphics_object->number_of_frames,
							effective_position_x,
							effective_position_y,
							effective_width,
							effective_height,
							*t,
							current_rotation);
					} else {
						ogl.render_textured_rect(
							currently_enabled,
							effective_position_x,
							effective_position_y,
							effective_width,
							effective_height,
							*t,
							current_rotation);
					}

				}
			}
			break;
		}
		case ui::gui_object::type_linegraph:
		{
			auto l = manager.lines_set.safe_at(lines_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (l) {
				ogl.render_linegraph(
					currently_enabled,
					effective_position_x,
					effective_position_y,
					effective_width,
					effective_height,
					*l);
			}
			break;
		}
		case ui::gui_object::type_masked_flag:
		{
			auto m = manager.multi_texture_instances.safe_at(multi_texture_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (m) {
				const auto flag = m->flag;
				const auto mask = m->mask_or_primary;
				const auto overlay = m->overlay_or_secondary;
				if (flag && mask) {
					ogl.render_masked_rect(
						currently_enabled,
						effective_position_x,
						effective_position_y,
						effective_width,
						effective_height,
						*flag,
						*mask,
						current_rotation);
				}
				if (overlay) {
					ogl.render_textured_rect(
						currently_enabled,
						effective_position_x,
						effective_position_y,
						effective_width,
						effective_height,
						*overlay,
						current_rotation);
				}
			}
			break;
		}
		case ui::gui_object::type_progress_bar:
		{
			auto m = manager.multi_texture_instances.safe_at(multi_texture_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (m) {
				const auto primary = m->mask_or_primary;
				const auto secondary = m->overlay_or_secondary;
				if (primary && secondary) {
					ogl.render_progress_bar(
						currently_enabled,
						m->progress,
						effective_position_x,
						effective_position_y,
						effective_width,
						effective_height,
						*primary,
						*secondary,
						current_rotation);
				}
			}
			break;
		}
		case ui::gui_object::type_piechart:
		{
			auto dt = manager.data_textures.safe_at(data_texture_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (dt) {
				ogl.render_piechart(
					currently_enabled,
					effective_position_x,
					effective_position_y,
					effective_width,
					*dt);
			}
			break;
		}
		case ui::gui_object::type_text_instance:
		{
			auto ti = manager.text_instances.safe_at(text_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (ti) {
				auto& fnt = manager.fonts.at(ti->font_handle);

				switch (ti->color) {
					case ui::text_color::black:
						ogl.render_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 0.0f, 0.0f, 0.0f }, fnt);
						break;
					case ui::text_color::green:
						ogl.render_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 0.0f, 0.623f, 0.01f }, fnt);
						break;
					case ui::text_color::outlined_black:
						ogl.render_outlined_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 0.0f, 0.0f, 0.0f }, fnt);
						break;
					case ui::text_color::outlined_white:
						ogl.render_outlined_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 1.0f, 1.0f, 1.0f }, fnt);
						break;
					case ui::text_color::red:
						ogl.render_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 1.0f, 0.2f, 0.2f }, fnt);
						break;
					case ui::text_color::white:
						ogl.render_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 1.0f, 1.0f, 1.0f }, fnt);
						break;
					case ui::text_color::yellow:
						ogl.render_text(ti->text, ti->length, currently_enabled, effective_position_x, effective_position_y, ti->size * 2 * manager.scale(), graphics::color{ 1.0f, 0.75f, 1.0f }, fnt);
						break;
				}
			}
			break;
		}
	}
}

void ui::detail::render(const gui_manager& manager, graphics::open_gl_wrapper &ogl, const gui_object &root_obj, ui::xy_pair position, bool parent_enabled) {
	const auto type = root_obj.flags & ui::gui_object::type_mask;
	
	const float effective_position_x = static_cast<float>(position.x) + static_cast<float>(root_obj.position.x) * manager.scale();
	const float effective_position_y = static_cast<float>(position.y) + static_cast<float>(root_obj.position.y) * manager.scale();
	const float effective_width = static_cast<float>(root_obj.size.x) * manager.scale();
	const float effective_height = static_cast<float>(root_obj.size.y) * manager.scale();
	const bool currently_enabled = parent_enabled && ((root_obj.flags.load(std::memory_order_acquire) & ui::gui_object::enabled) != 0);

	graphics::scissor_rect clip(std::lround(effective_position_x), manager.height() - std::lround(effective_position_y + effective_height), std::lround(effective_width), std::lround(effective_height));

	detail::render_object_type(manager, ogl, root_obj, position, type, currently_enabled);
	
	const ui::xy_pair new_base_position{ int16_t(effective_position_x), int16_t(effective_position_y) };

	gui_object_tag current_child = root_obj.first_child;
	while (is_valid_index(current_child)) {
		const auto& child_object = manager.gui_objects.at(current_child);
		detail::render(manager, ogl, child_object, new_base_position, currently_enabled);
		current_child = child_object.right_sibling;
	}
}

void ui::gui_manager::set_focus(tagged_gui_object g) {
	if (focus != g.id) {
		if (g.object.associated_behavior && g.object.associated_behavior->on_get_focus(g, *this)) {
			clear_focus();
			focus = g.id;
		}
	}
}

void ui::gui_manager::clear_focus() {
	if (is_valid_index(focus)) {
		auto& with_focus = gui_objects.at(focus);
		if (with_focus.associated_behavior) {
			with_focus.associated_behavior->on_lose_focus(tagged_gui_object{ with_focus, focus }, *this);
			focus = gui_object_tag();
		}
	}
}

void ui::make_visible(gui_manager& manager, tagged_gui_object g) {
	g.object.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
	detail::update(manager, g);
}

void ui::hide(tagged_gui_object g) {
	g.object.flags.fetch_and(~ui::gui_object::visible, std::memory_order_acq_rel);
}

void ui::set_enabled(tagged_gui_object g, bool enabled) {
	if (enabled)
		g.object.flags.fetch_or(ui::gui_object::enabled, std::memory_order_acq_rel);
	else
		g.object.flags.fetch_and(~ui::gui_object::enabled, std::memory_order_acq_rel);
}

void ui::gui_manager::hide_tooltip() {
	hide(tagged_gui_object{ tooltip_window, gui_object_tag(3) });
}

void ui::clear_children(gui_manager& manager, tagged_gui_object g) {
	gui_object_tag current_child = g.object.first_child;
	g.object.first_child = gui_object_tag();

	while (is_valid_index(current_child)) {
		auto& child_object = manager.gui_objects.at(current_child);
		const gui_object_tag next_child = child_object.right_sibling;

		manager.gui_objects.free(current_child, manager);
		current_child = next_child;
	}
}

void ui::remove_object(gui_manager& manager, tagged_gui_object g) {
	gui_object_tag parent_id = g.object.parent;
	auto& parent_object = manager.gui_objects.at(parent_id);

	const gui_object_tag left_sibling = g.object.left_sibling;
	const gui_object_tag right_sibling = g.object.right_sibling;

	if (!is_valid_index(left_sibling))
		parent_object.first_child = right_sibling;
	else
		manager.gui_objects.at(left_sibling).right_sibling = right_sibling;
	
	if (is_valid_index(right_sibling))
		manager.gui_objects.at(right_sibling).left_sibling = left_sibling;

	manager.gui_objects.free(g.id, manager);
}

ui::tagged_gui_object ui::detail::last_sibling_of(const gui_manager& manager, tagged_gui_object g) {
	gui_object_tag sib_child_id = g.id;
	gui_object* current = &g.object;
	while (true) {
		const gui_object_tag next_id = current->right_sibling;
		if (!is_valid_index(next_id))
			return tagged_gui_object{*current, sib_child_id };
		sib_child_id = next_id;
		current = &(manager.gui_objects.at(next_id));
	}
}

void ui::add_to_front(const gui_manager& manager, tagged_gui_object parent, tagged_gui_object child) {
	child.object.parent = parent.id;
	const gui_object_tag first_child_id = parent.object.first_child;

	if (!is_valid_index(first_child_id)) {
		parent.object.first_child = child.id;
	} else {
		const auto last_in_list = detail::last_sibling_of(manager, tagged_gui_object{ manager.gui_objects.at(first_child_id), first_child_id });

		child.object.left_sibling = last_in_list.id;
		last_in_list.object.right_sibling = child.id;
	}
	
}

void ui::add_to_back(const gui_manager& manager, tagged_gui_object parent, tagged_gui_object child) {
	child.object.parent = parent.id;
	const gui_object_tag first_child_id = parent.object.first_child;
	if (is_valid_index(first_child_id)) {
		child.object.right_sibling = first_child_id;
		auto& first_child_object = manager.gui_objects.at(first_child_id);
		first_child_object.left_sibling = child.id;
	}
	parent.object.first_child = child.id;
}

void ui::move_to_front(const gui_manager& manager, tagged_gui_object g) {
	const gui_object_tag left_sibling = g.object.left_sibling;
	const gui_object_tag right_sibling = g.object.right_sibling;

	if (!is_valid_index(right_sibling))
		return;

	const gui_object_tag parent_id = g.object.parent;
	auto& parent_object = manager.gui_objects.at(parent_id);

	const auto last_in_list = ui::detail::last_sibling_of(manager, g);

	if (!is_valid_index(left_sibling))
		parent_object.first_child = right_sibling;
	else
		manager.gui_objects.at(left_sibling).right_sibling = right_sibling;

	manager.gui_objects.at(right_sibling).left_sibling = left_sibling;

	g.object.left_sibling = last_in_list.id;
	g.object.right_sibling = gui_object_tag();
	last_in_list.object.right_sibling = g.id;
}

void ui::move_to_back(const gui_manager& manager, tagged_gui_object g) {
	const gui_object_tag left_sibling = g.object.left_sibling;
	const gui_object_tag right_sibling = g.object.right_sibling;

	if (!is_valid_index(left_sibling))
		return;

	const gui_object_tag parent_id = g.object.parent;
	auto& parent_object = manager.gui_objects.at(parent_id);

	manager.gui_objects.at(left_sibling).right_sibling = right_sibling;
	if (is_valid_index(right_sibling))
		manager.gui_objects.at(right_sibling).left_sibling = left_sibling;

	g.object.left_sibling = gui_object_tag();
	g.object.right_sibling = parent_object.first_child;
	parent_object.first_child = g.id;
}

bool ui::gui_manager::on_lbutton_down(const lbutton_down& ld) {
	return detail::dispatch_message(*this, [_this = this](ui::tagged_gui_object obj, const lbutton_down& l) {
		_this->set_focus(obj);
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_lclick(obj, *_this, l);
		return false;
	}, tagged_gui_object{ root, gui_object_tag(0) }, ui::rescale_message(ld, _scale));
}

bool ui::gui_manager::on_rbutton_down(const rbutton_down& rd) {
	return detail::dispatch_message(*this, [_this = this](ui::tagged_gui_object obj, const rbutton_down& r) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_rclick(obj, *_this, r);
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, ui::rescale_message(rd, _scale));
}

bool ui::gui_manager::on_mouse_move(const mouse_move& mm) {
	const bool found_tooltip = detail::dispatch_message(*this, [_this = this](ui::tagged_gui_object obj, const mouse_move& m) {
		if (obj.object.associated_behavior && obj.object.associated_behavior->has_tooltip(obj, *_this)) {
			if (_this->tooltip != obj.id) {
				_this->tooltip = obj.id;
				clear_children(*_this, ui::tagged_gui_object{ _this->tooltip_window, gui_object_tag(3) });
				obj.object.associated_behavior->create_tooltip(obj, *_this, ui::tagged_gui_object{ _this->tooltip_window, gui_object_tag(3) });
				make_visible(*_this, ui::tagged_gui_object{ _this->tooltip_window, gui_object_tag(3) });
			}
			return true;
		}
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, ui::rescale_message(mm, _scale));

	if (!found_tooltip && is_valid_index(tooltip)) {
		tooltip = gui_object_tag();
		hide(ui::tagged_gui_object{ tooltip_window, gui_object_tag(3) });
	}

	return found_tooltip;
}

bool ui::gui_manager::on_mouse_drag(const mouse_drag& md) {
	if (is_valid_index(focus)) {
		if (gui_objects.at(focus).associated_behavior) {
			return gui_objects.at(focus).associated_behavior->on_drag(ui::tagged_gui_object{ gui_objects.at(focus), focus }, *this, ui::rescale_message(md, _scale));
		} else
			return false;
	}
	return false;
}

bool ui::gui_manager::on_keydown(const key_down& kd) {
	return detail::dispatch_message(*this, [_this = this](ui::tagged_gui_object obj, const key_down& k) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_keydown(obj, *_this, k);
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, ui::rescale_message(kd, _scale));
}

bool ui::gui_manager::on_scroll(const scroll& se) {
	return detail::dispatch_message(*this, [_this = this](ui::tagged_gui_object obj, const scroll& s) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_scroll(obj, *_this, s);
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, ui::rescale_message(se, _scale));
}

bool ui::gui_manager::on_text(const text_event &te) {
	if (is_valid_index(focus)) {
		if (gui_objects.at(focus).associated_behavior)
			return gui_objects.at(focus).associated_behavior->on_text(ui::tagged_gui_object{ gui_objects.at(focus), focus }, *this, te);
		else
			return false;
	}
	return false;
}

void ui::detail::update(gui_manager& manager, tagged_gui_object obj) {
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible) == 0)
		return;

	if (obj.object.associated_behavior)
		obj.object.associated_behavior->update_data(obj, manager);

	gui_object_tag child = obj.object.first_child;
	while (is_valid_index(child)) {
		auto& child_object = manager.gui_objects.at(child);
		const gui_object_tag next_index = child_object.right_sibling;

		update(manager, tagged_gui_object{ child_object, child });
		child = next_index;
	}
}

void ui::update(gui_manager& manager) {
	detail::update(manager, tagged_gui_object{ manager.root, gui_object_tag(0) });
	detail::update(manager, tagged_gui_object{ manager.background, gui_object_tag(1) });
	detail::update(manager, tagged_gui_object{ manager.foreground, gui_object_tag(2) });
}

ui::gui_manager::gui_manager(int32_t width, int32_t height) :
	_width(width), _height(height),
	root(gui_objects.emplace_at(gui_object_tag(0))),
	background(gui_objects.emplace_at(gui_object_tag(1))),
	foreground(gui_objects.emplace_at(gui_object_tag(2))),
	tooltip_window(gui_objects.emplace_at(gui_object_tag(3))) {

	on_resize(resize{ width , height });

	hide(tagged_gui_object{ tooltip_window, gui_object_tag(3) });
	add_to_back(*this, tagged_gui_object{ foreground, gui_object_tag(2) }, tagged_gui_object{ tooltip_window, gui_object_tag(3) });
}

void ui::gui_manager::on_resize(const resize& r) {
	ui::xy_pair new_size{ static_cast<float>(r.width) / scale(), static_cast<float>(r.height) / scale() };

	_width = r.width;
	_height = r.height;
	root.size = new_size;
	foreground.size = new_size;
	background.size = new_size;
}

void ui::gui_manager::rescale(float new_scale) {
	const resize new_size{ static_cast<float>(root.size.x) * scale(), static_cast<float>(root.size.y) * scale() };
	_scale = new_scale;
	on_resize(new_size);
}

void ui::render(const gui_manager& manager, graphics::open_gl_wrapper& ogl) {
	detail::render(manager, ogl, manager.background, ui::xy_pair{ 0, 0 }, true);
	detail::render(manager, ogl, manager.root, ui::xy_pair{ 0, 0 }, true);
	detail::render(manager, ogl, manager.foreground, ui::xy_pair{ 0, 0 }, true);
}

graphics::rotation ui::gui_object::get_rotation() const {
	const auto rotation_bits = flags.load(std::memory_order_acquire) & ui::gui_object::rotation_mask;
	if (rotation_bits == ui::gui_object::rotation_left)
		return graphics::rotation::left;
	else if (rotation_bits == ui::gui_object::rotation_right)
		return graphics::rotation::right;
	else
		return graphics::rotation::upright;
}


void ui::load_gui_from_directory(const directory& source_directory, gui_manager& manager) {
	auto fonts_directory = source_directory.get_directory(u"\\gfx\\fonts");
	manager.fonts.load_standard_fonts(fonts_directory);

	manager.fonts.load_metrics_fonts();

	manager.textures.load_standard_textures(source_directory);

	auto localisation_directory = source_directory.get_directory(u"\\localisation");
	load_text_sequences_from_directory(localisation_directory, manager.text_data_sequences);

	auto interface_directory = source_directory.get_directory(u"\\interface");

	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;

	graphics::name_maps gobj_nmaps;
	std::vector<std::pair<std::string, graphics::errors>> gobj_errors_generated;

	ui::load_ui_definitions_from_directory(
		interface_directory, nmaps, manager.ui_definitions, errors_generated,
		[&manager](const char* a, const char* b) { return text_data::get_text_handle(manager.text_data_sequences, a, b); },
		[&manager](const char* a, const char* b) { return graphics::pack_font_handle(manager.fonts.find_font(a, b), manager.fonts.find_font_size(a, b)); },
		[&gobj_nmaps](const char* a, const char* b) { return graphics::reserve_graphics_object(gobj_nmaps, a, b); });

#ifdef _DEBUG
	for (auto& e : errors_generated) {
		OutputDebugStringA(e.first.c_str());
		OutputDebugStringA(": ");
		OutputDebugStringA(ui::format_error(e.second));
		OutputDebugStringA("\n");
	}
#endif

	graphics::load_graphics_object_definitions_from_directory(
		interface_directory,
		gobj_nmaps,
		manager.graphics_object_definitions,
		gobj_errors_generated,
		[&manager, &source_directory](const char* a, const char* b) { return manager.textures.retrieve_by_name(source_directory, a, b); });

#ifdef _DEBUG
	for (auto& e : gobj_errors_generated) {
		OutputDebugStringA(e.first.c_str());
		OutputDebugStringA(": ");
		OutputDebugStringA(graphics::format_error(e.second));
		OutputDebugStringA("\n");
	}
#endif
}