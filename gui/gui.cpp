#include "gui.hpp"
#include <algorithm>
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"
#include "boost\\container\\small_vector.hpp"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "world_state\\world_state.h"

#ifdef _DEBUG
#include "Windows.h"
#undef min
#undef max
#endif

ui::gui_manager::~gui_manager() {};

void ui::gui_manager::destroy(gui_object & g) {
	if (gui_objects.safe_at(tooltip) == &g) {
		hide_tooltip();
		tooltip = gui_object_tag();
	}
	if (gui_objects.safe_at(focus) == &g)
		focus = gui_object_tag();

	if (g.associated_behavior) {
		if ((g.flags.load(std::memory_order_relaxed) & gui_object::dynamic_behavior) != 0) {
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
	namespace detail {
		enum class sub_alignment {
			base, center, extreme
		};

		inline int32_t position_from_subalignment(int32_t container, int32_t base_position, sub_alignment align) {
			switch (align) {
				case sub_alignment::base:
					return base_position;
				case sub_alignment::center:
					return container / 2 + base_position;
				case sub_alignment::extreme:
					return container + base_position;
				default:
					return base_position;
			}
		}

		inline std::pair<int32_t, int32_t> bounds_from_subalignment(int32_t container, sub_alignment align) {
			switch(align) {
				case sub_alignment::base:
					return std::pair<int32_t, int32_t>(0, container);
				case sub_alignment::center:
					return std::pair<int32_t, int32_t>(-container / 2, container / 2);
				case sub_alignment::extreme:
					return std::pair<int32_t, int32_t>(-container, 0);
				default:
					return std::pair<int32_t, int32_t>(0, container);
			}
		}

		inline sub_alignment vertical_subalign(ui::alignment align) {
			switch (align) {
				case alignment::top_left:
				case alignment::top_center:
				case alignment::top_right:
					return sub_alignment::base;
				case alignment::left:
				case alignment::center:
				case alignment::right:
					return sub_alignment::center;
				case alignment::bottom_left:
				case alignment::bottom_center:
				case alignment::bottom_right:
					return sub_alignment::extreme;
				default:
					return sub_alignment::base;
			}
		}

		inline sub_alignment horizontal_subalign(ui::alignment align) {
			switch (align) {
				case alignment::top_left:
				case alignment::left:
				case alignment::bottom_left:
					return sub_alignment::base;
				case alignment::top_center:
				case alignment::center:
				case alignment::bottom_center:
					return sub_alignment::center;
				case alignment::top_right:
				case alignment::right:
				case alignment::bottom_right:
					return sub_alignment::extreme;
				default:
					return sub_alignment::base;
			}
		}
	}

	gui_behavior::gui_behavior(gui_behavior && o) noexcept {
		associated_object = o.associated_object;
		o.associated_object = nullptr;
		if (associated_object)
			associated_object->associated_behavior = this;
	}
	gui_behavior::~gui_behavior() {
		if (associated_object)
			associated_object->associated_behavior = nullptr;
		associated_object = nullptr;
	}
	unmanaged_scrollable_region::unmanaged_scrollable_region(gui_object & g) : sb(g) {}
	bool unmanaged_scrollable_region::on_scroll(gui_object_tag o, world_state & m, const scroll & s) { return sb.on_scroll(o, m, s); }
}


ui::xy_pair ui::detail::position_with_alignment(ui::xy_pair container_size, ui::xy_pair raw_position, ui::alignment align) {
	return ui::xy_pair{
		(int16_t)position_from_subalignment(static_cast<int32_t>(container_size.x), static_cast<int32_t>(raw_position.x), ui::detail::horizontal_subalign(align)),
		(int16_t)position_from_subalignment(static_cast<int32_t>(container_size.y), static_cast<int32_t>(raw_position.y), ui::detail::vertical_subalign(align))
	};
}

std::pair<ui::xy_pair, ui::xy_pair> ui::detail::position_bounds_with_alignment(ui::xy_pair container_size, ui::xy_pair size, ui::alignment align) {
	auto h_bound = bounds_from_subalignment(container_size.x, ui::detail::horizontal_subalign(align));
	auto v_bound = bounds_from_subalignment(container_size.y, ui::detail::vertical_subalign(align));
	
	return std::pair<ui::xy_pair, ui::xy_pair>(ui::xy_pair{ int16_t(h_bound.first), int16_t(v_bound.first) },
		ui::xy_pair{ int16_t(std::max(h_bound.first, h_bound.second - size.x)), int16_t(std::max(v_bound.first, v_bound.second - size.y))});
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
				return text_data::alignment::center;
			case text_def::format_left:
				return text_data::alignment::left;
			case text_def::format_right:
				return text_data::alignment::right;
			case text_def::format_justified:
				return text_data::alignment::left;
			default:
				return text_data::alignment::left;
		}
	}
	text_data::alignment text_aligment_from_overlapping_definition(const overlapping_region_def& def) {
		switch (def.flags & overlapping_region_def::format_mask) {
			case overlapping_region_def::format_center:
				return text_data::alignment::center;
			case overlapping_region_def::format_left:
				return text_data::alignment::left;
			case overlapping_region_def::format_right:
				return text_data::alignment::right;
			default:
				return text_data::alignment::left;
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
			default:
				return ui::text_color::black;
		}
	}

	void unmanaged_region_scollbar::on_position(int32_t pos) {
		contents_frame.position.y = static_cast<int16_t>(-pos);
	}

	line_manager::line_manager(text_data::alignment a, int32_t m) : align(a), max_line_extent(m) {}

	line_manager::line_manager() : align(text_data::alignment::left), max_line_extent(-1) {}

	bool line_manager::exceeds_extent(int32_t w) const {
		if(max_line_extent < 0)
			return false;
		else
			return (w + indent) > max_line_extent;
	}

	void line_manager::add_object(gui_object * o) {
		current_line.push_back(o);
	}

	void line_manager::finish_current_line() {
		if(align == text_data::alignment::left) {
			for(auto p : current_line)
				p->position.x += int16_t(indent);
			current_line.clear();
		} else {
			int32_t max_right = 0;
			for(auto p : current_line)
				max_right = std::max(max_right, p->position.x + p->size.x);
			int32_t total_element_width = indent + max_right;
			const int32_t adjustment = (max_line_extent - total_element_width) /
				(align == text_data::alignment::right ? 1 : 2);
			for(auto p : current_line)
				p->position.x += int16_t(adjustment);

			current_line.clear();
		}
	}

	text_box_line_manager::text_box_line_manager(text_data::alignment a, int32_t m, int32_t bx, int32_t by) : align(a), max_line_extent(m), border_x(bx), border_y(by) {}

	void text_box_line_manager::add_object(gui_object * o) {
		current_line.push_back(o);
	}

	void text_box_line_manager::finish_current_line() {
		if(align == text_data::alignment::left) {
			for(auto p : current_line) {
				p->position.x += int16_t(border_x);
				p->position.y += int16_t(border_y);
			}
			current_line.clear();
		} else if(align == text_data::alignment::right) {
			int32_t total_element_width = 0;
			for(auto p : current_line)
				total_element_width = std::max(total_element_width, p->position.x + p->size.x);
			const int32_t adjustment = (max_line_extent - total_element_width - border_x);
			for(auto p : current_line) {
				p->position.x += int16_t(adjustment);
				p->position.y += int16_t(border_y);
			}
			current_line.clear();
		} else { // center
			int32_t total_element_width = 0;
			for(auto p : current_line)
				total_element_width = std::max(total_element_width, p->position.x + p->size.x);
			const int32_t adjustment = (max_line_extent - total_element_width) / 2;
			for(auto p : current_line) {
				p->position.x += int16_t(adjustment);
				p->position.y += int16_t(border_y);
			}
			current_line.clear();
		}
	}

	void line_manager::increase_indent(int32_t n) {
		indent += indent_size * n;
	}
	void line_manager::decrease_indent(int32_t n) {
		indent -= indent_size * n;
	}

	bool draggable_region::on_drag(gui_object_tag t, world_state& ws, const mouse_drag &m) {
		auto& obj = ws.w.gui_m.gui_objects.at(t);
		if(is_valid_index(obj.parent)) {
			auto& parent = ws.w.gui_m.gui_objects.at(obj.parent);
			auto bounds = ui::detail::position_bounds_with_alignment(parent.size, obj.size, obj.align);

			associated_object->position.x = std::clamp(int16_t(base_position.x + m.x), bounds.first.x, bounds.second.x);
			associated_object->position.y = std::clamp(int16_t(base_position.y + m.y), bounds.first.y, bounds.second.y);
		} else {
			associated_object->position.x = static_cast<int16_t>(base_position.x + m.x);
			associated_object->position.y = static_cast<int16_t>(base_position.y + m.y);
		}
		return true;
	}

	bool draggable_region::on_get_focus(gui_object_tag t, world_state& m) {
		base_position = associated_object->position;
		ui::move_to_front(m.w.gui_m, ui::tagged_gui_object{ *associated_object, t });
		return true;
	}

	bool window_pane::on_get_focus(gui_object_tag t, world_state& ws) {
		gui_object_tag parent_tag = ws.w.gui_m.gui_objects.at(t).parent;
		if(is_valid_index(parent_tag)) {
			if(auto b = ws.w.gui_m.gui_objects.at(parent_tag).associated_behavior; b) {
				b->on_get_focus(parent_tag, ws);
			}
		}
		return true;
	}
	bool window_pane::on_scroll(gui_object_tag t, world_state& ws, const scroll& e) {
		gui_object_tag parent_tag = ws.w.gui_m.gui_objects.at(t).parent;
		if(is_valid_index(parent_tag)) {
			if(auto b = ws.w.gui_m.gui_objects.at(parent_tag).associated_behavior; b) {
				b->on_scroll(parent_tag, ws, e);
			}
		}
		return true;
	}
	bool window_pane::on_drag(gui_object_tag t, world_state& ws, const mouse_drag& e) {
		gui_object_tag parent_tag = ws.w.gui_m.gui_objects.at(t).parent;
		if(is_valid_index(parent_tag)) {
			if(auto b = ws.w.gui_m.gui_objects.at(parent_tag).associated_behavior; b) {
				b->on_drag(parent_tag, ws, e);
			}
		}
		return true;
	}
	bool window_pane::on_text(gui_object_tag t, world_state& ws, const text_event& e) {
		gui_object_tag parent_tag = ws.w.gui_m.gui_objects.at(t).parent;
		if(is_valid_index(parent_tag)) {
			if(auto b = ws.w.gui_m.gui_objects.at(parent_tag).associated_behavior; b) {
				b->on_text(parent_tag, ws, e);
			}
		}
		return true;
	}
}


tagged_object<ui::text_instance, ui::text_instance_tag> ui::create_text_instance(ui::gui_manager &container, tagged_gui_object new_gobj, const text_format& fmt) {
	const auto new_text_instance = container.text_instances.emplace();

	new_gobj.object.flags.store(gui_object::type_text_instance | gui_object::enabled | gui_object::visible, std::memory_order_release);
	new_gobj.object.type_dependant_handle.store(to_index(new_text_instance.id), std::memory_order_release);

	new_text_instance.object.color = fmt.color;
	new_text_instance.object.font_handle = fmt.font_handle;
	new_text_instance.object.size = static_cast<uint8_t>(fmt.font_size / 2);
	new_text_instance.object.length = 0;

	return new_text_instance;
}


float ui::text_component_width(const text_data::text_component& c, const std::vector<char16_t>& text_data, graphics::font& this_font, uint32_t font_size) {
	if (std::holds_alternative<text_data::text_chunk>(c)) {
		const auto chunk = std::get<text_data::text_chunk>(c);
		return this_font.metrics_text_extent(text_data.data() + chunk.offset, chunk.length, ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(font_size)), false);
	} else {
		return 0.0f;
	}
}

void ui::shorten_text_instance_to_space(ui::text_instance& txt) {
	int32_t i = int32_t(txt.length) - 2;
	for (; i >= 0; --i) {
		if (txt.text[i] == u' ')
			break;
	}
	if (i >= 0)
		txt.length = static_cast<uint8_t>(i + 1);
}

ui::xy_pair ui::advance_cursor_to_newline(ui::xy_pair cursor, gui_static& manager, text_format const& fmt) {
	graphics::font& this_font = manager.fonts.at(fmt.font_handle);

	return ui::xy_pair{ 0i16,
		int16_t(cursor.y + static_cast<int32_t>(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f)) };
}

ui::xy_pair ui::advance_cursor_by_space(ui::xy_pair cursor, gui_static& manager, text_format const& fmt, int32_t count) {
	graphics::font& this_font = manager.fonts.at(fmt.font_handle);

	static char16_t space = u' ';

	int32_t new_size = static_cast<int32_t>(0.5f + count * this_font.metrics_text_extent(
		&space, 1, ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)), is_outlined_color(fmt.color)));
	return ui::xy_pair{ int16_t(cursor.x + new_size), cursor.y };
}


void ui::detail::create_multiline_text(gui_static& static_manager, gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format& fmt, const text_data::replacement* candidates, uint32_t count) {
	graphics::font& this_font = static_manager.fonts.at(fmt.font_handle);
	const auto& components = static_manager.text_data_sequences.all_sequences[text_handle];

	const auto components_start = static_manager.text_data_sequences.all_components.data() + components.starting_component;
	const auto components_end = components_start + components.component_count;

	ui::xy_pair position{ 0, 0 };
	ui::text_color current_color = fmt.color;

	ui::line_manager lm(align, container.object.size.x);
	
	for (auto component_i = components_start; component_i != components_end; ++component_i) {
		if (std::holds_alternative<text_data::color_change>(*component_i)) {
			if(std::get<text_data::color_change>(*component_i).color == text_data::text_color::unspecified)
				current_color = fmt.color;
			else
				current_color = text_color_to_ui_text_color(std::get<text_data::color_change>(*component_i).color);
		} else if (std::holds_alternative<text_data::line_break>(*component_i)) {
			lm.finish_current_line();
			position.x = 0;
			position.y += int16_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f);
		} else if (std::holds_alternative<text_data::value_placeholder>(*component_i)) {
			const auto rep = text_data::find_replacement(std::get<text_data::value_placeholder>(*component_i), candidates, count);

			const auto replacement_text = rep ? std::get<1>(*rep) : vector_backed_string<char16_t>(text_data::name_from_value_type(std::get<text_data::value_placeholder>(*component_i).value));

			const text_format format{ current_color, fmt.font_handle, fmt.font_size };
			if (rep) 
				position = text_chunk_to_instances(static_manager, manager, replacement_text, container, position, format, lm, std::get<2>(*rep));
			else
				position = text_chunk_to_instances(static_manager, manager, replacement_text, container, position, format, lm);
		} else if (std::holds_alternative<text_data::text_chunk>(*component_i)) {
			const auto chunk = std::get<text_data::text_chunk>(*component_i);
			const text_format format{ current_color, fmt.font_handle, fmt.font_size };

			position = text_chunk_to_instances(static_manager, manager, chunk, container, position, format, lm);
		}
	}

	lm.finish_current_line();
	container.object.size.y = position.y + static_cast<int16_t>(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f);
}

void ui::detail::create_linear_text(gui_static& static_manager, gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format& fmt, const text_data::replacement* candidates, uint32_t count) {
	ui::xy_pair position{ 0, 0 };
	
	const auto& components = static_manager.text_data_sequences.all_sequences[text_handle];
	graphics::font& this_font = static_manager.fonts.at(fmt.font_handle);
	const auto components_start = static_manager.text_data_sequences.all_components.data() + components.starting_component;
	const auto components_end = components_start + components.component_count;

	float x_extent = 0.0f;

	for(auto component_i = components_start; component_i != components_end; ++component_i) {
		x_extent += ui::text_component_width(*component_i, static_manager.text_data_sequences.text_data, this_font, fmt.font_size);
	}
	const auto res = align_in_bounds(align, int32_t(x_extent + 0.5f), int32_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)))), container.object.size.x, container.object.size.y);
	position.x = int16_t(res.first);
	position.y = int16_t(res.second);
	add_linear_text(position, text_handle, fmt, static_manager, manager, container, ui::single_line_manager(), candidates, count);
}

namespace {
	inline void make_masked_flag(ui::gui_static& static_manager, ui::gui_manager& manager, ui::tagged_gui_object container, graphics::object const& graphic_object_def) {
		const graphics::texture_tag main_texture = graphic_object_def.primary_texture_handle;
		const graphics::texture_tag mask_texture(graphic_object_def.type_dependant);

		auto main_texture_ptr = &(static_manager.textures.retrieve_by_key(main_texture)); //overlay
		auto mask_texture_ptr = &(static_manager.textures.retrieve_by_key(mask_texture));

		main_texture_ptr->load_filedata();
		auto main_height = main_texture_ptr->get_height();
		auto main_width = main_texture_ptr->get_width();

		mask_texture_ptr->load_filedata();
		auto mask_height = mask_texture_ptr->get_height();
		auto mask_width = mask_texture_ptr->get_width();

		container.object.size.x = int16_t(std::max(main_width, mask_width));
		container.object.size.y = int16_t(std::max(main_height, mask_height));

		const auto flag_gobj = manager.gui_objects.emplace();
		const auto overlay_gobj = manager.gui_objects.emplace();

		ui::add_to_back(manager, container, overlay_gobj);
		ui::add_to_back(manager, container, flag_gobj);

		flag_gobj.object.size = ui::xy_pair{ int16_t(mask_width), int16_t(mask_height) };
		flag_gobj.object.position = ui::xy_pair{int16_t((container.object.size.x - mask_width) / 2), int16_t((container.object.size.y - mask_height) / 2) };
		overlay_gobj.object.size = ui::xy_pair{ int16_t(main_width), int16_t(main_height) };
		overlay_gobj.object.position = ui::xy_pair{ int16_t((container.object.size.x - main_width) / 2), int16_t((container.object.size.y - main_height) / 2) };

		const auto overlay_graphic = manager.graphics_instances.emplace();

		overlay_graphic.object.frame = 0;
		overlay_graphic.object.graphics_object = &graphic_object_def;
		overlay_graphic.object.t = main_texture_ptr;

		overlay_gobj.object.type_dependant_handle.store(to_index(overlay_graphic.id), std::memory_order_release);
		overlay_gobj.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);

		const auto flag_graphics = manager.multi_texture_instances.emplace();
		flag_graphics.object.mask_or_primary = mask_texture_ptr;
		flag_graphics.object.flag_or_secondary = nullptr;

		flag_gobj.object.type_dependant_handle.store(to_index(flag_graphics.id), std::memory_order_release);
		flag_gobj.object.flags.fetch_or(ui::gui_object::type_masked_flag, std::memory_order_acq_rel);
	}
}

void ui::detail::instantiate_graphical_object(gui_static& static_manager, ui::gui_manager& manager, ui::tagged_gui_object container, graphics::obj_definition_tag gtag, int32_t frame, bool force_tinted) {
	auto& graphic_object_def = static_manager.graphics_object_definitions.definitions[gtag];

	if (container.object.size == ui::xy_pair{ 0,0 })
		container.object.size = ui::xy_pair{ graphic_object_def.size.x, graphic_object_def.size.y };

	if(force_tinted) {
		if(is_valid_index(graphic_object_def.primary_texture_handle)) {
			container.object.flags.fetch_or(ui::gui_object::type_tinted_icon, std::memory_order_acq_rel);

			const auto icon_graphic = manager.tinted_icon_instances.emplace();

			icon_graphic.object.graphics_object = &graphic_object_def;
			icon_graphic.object.t = &(static_manager.textures.retrieve_by_key(graphic_object_def.primary_texture_handle));

			if(((int32_t)container.object.size.y | (int32_t)container.object.size.x) == 0) {
				icon_graphic.object.t->load_filedata();
				container.object.size.y = static_cast<int16_t>(icon_graphic.object.t->get_height());
				container.object.size.x = static_cast<int16_t>(icon_graphic.object.t->get_width() / ((graphic_object_def.number_of_frames != 0) ? graphic_object_def.number_of_frames : 1));
			}

			container.object.type_dependant_handle.store(to_index(icon_graphic.id), std::memory_order_release);
		}

		return;
	}

	switch (graphics::object_type(graphic_object_def.flags & graphics::object::type_mask)) {
		case graphics::object_type::bordered_rect:
		case graphics::object_type::text_sprite:
		case graphics::object_type::tile_sprite:
		case graphics::object_type::generic_sprite:
			if (is_valid_index(graphic_object_def.primary_texture_handle)) {
				container.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);

				const auto icon_graphic = manager.graphics_instances.emplace();

				icon_graphic.object.frame = frame;
				icon_graphic.object.graphics_object = &graphic_object_def;
				icon_graphic.object.t = &(static_manager.textures.retrieve_by_key(graphic_object_def.primary_texture_handle));

				if (((int32_t)container.object.size.y | (int32_t)container.object.size.x) == 0) {
					icon_graphic.object.t->load_filedata();
					container.object.size.y = static_cast<int16_t>(icon_graphic.object.t->get_height());
					container.object.size.x = static_cast<int16_t>(icon_graphic.object.t->get_width() / ((graphic_object_def.number_of_frames != 0) ? graphic_object_def.number_of_frames : 1));
				}

				container.object.type_dependant_handle.store(to_index(icon_graphic.id), std::memory_order_release);
			}
			break;
		case graphics::object_type::horizontal_progress_bar:
		{
			const graphics::texture_tag primary_texture = graphic_object_def.primary_texture_handle;
			const graphics::texture_tag secondary_texture(graphic_object_def.type_dependant);

			const auto bar_graphics = manager.multi_texture_instances.emplace();
			bar_graphics.object.mask_or_primary = &(static_manager.textures.retrieve_by_key(primary_texture));
			bar_graphics.object.flag_or_secondary = &(static_manager.textures.retrieve_by_key(secondary_texture));

			container.object.type_dependant_handle.store(to_index(bar_graphics.id), std::memory_order_release);
			container.object.flags.fetch_or(ui::gui_object::type_progress_bar, std::memory_order_acq_rel);
		}
			break;
		case graphics::object_type::vertical_progress_bar:
		{
			const graphics::texture_tag primary_texture = graphic_object_def.primary_texture_handle;
			const graphics::texture_tag secondary_texture(graphic_object_def.type_dependant);

			const auto bar_graphics = manager.multi_texture_instances.emplace();
			bar_graphics.object.mask_or_primary = &(static_manager.textures.retrieve_by_key(primary_texture));
			bar_graphics.object.flag_or_secondary = &(static_manager.textures.retrieve_by_key(secondary_texture));

			container.object.type_dependant_handle.store(to_index(bar_graphics.id), std::memory_order_release);
			container.object.flags.fetch_or(ui::gui_object::type_progress_bar | ui::gui_object::rotation_left, std::memory_order_acq_rel);
		}
			break;
		case graphics::object_type::flag_mask:
			make_masked_flag(static_manager, manager, container, graphic_object_def);
			break;
		case graphics::object_type::barchart:
			if (is_valid_index(graphic_object_def.primary_texture_handle) & is_valid_index(graphics::texture_tag(graphic_object_def.type_dependant))) {
				container.object.flags.fetch_or(ui::gui_object::type_barchart, std::memory_order_acq_rel);

				const auto flag_graphic = manager.multi_texture_instances.emplace();

				flag_graphic.object.mask_or_primary = &(static_manager.textures.retrieve_by_key(graphic_object_def.primary_texture_handle));
				flag_graphic.object.flag_or_secondary = &(static_manager.textures.retrieve_by_key(graphics::texture_tag(graphic_object_def.type_dependant)));

				if (((int32_t)container.object.size.y | (int32_t)container.object.size.x) == 0) {
					flag_graphic.object.flag_or_secondary->load_filedata();
					container.object.size.y = static_cast<int16_t>(flag_graphic.object.flag_or_secondary->get_height());
					container.object.size.x = static_cast<int16_t>(flag_graphic.object.flag_or_secondary->get_width());
				}

				container.object.type_dependant_handle.store(to_index(flag_graphic.id), std::memory_order_release);
			}
			break;
		case graphics::object_type::piechart:
		{
			container.object.flags.fetch_or(ui::gui_object::type_piechart, std::memory_order_acq_rel);

			container.object.size.x = graphic_object_def.size.x * 2;
			container.object.size.y = graphic_object_def.size.y * 2;

			container.object.position.x -= graphic_object_def.size.x;

			const auto new_dt = manager.data_textures.emplace(uint16_t(ui::piechart_resolution), 3ui16);

			container.object.type_dependant_handle.store(to_index(new_dt.id), std::memory_order_release);
		}
			break;
		case graphics::object_type::linegraph:
			container.object.flags.fetch_or(ui::gui_object::type_linegraph, std::memory_order_acq_rel);

			const auto new_lines = manager.lines_set.emplace(uint32_t(frame));
			container.object.type_dependant_handle.store(to_index(new_lines.id), std::memory_order_release);

			break;
	}
}

ui::tagged_gui_object ui::detail::create_element_instance(gui_static& static_manager, gui_manager& manager, button_tag handle) {
	const button_def& def = static_manager.ui_definitions.buttons[handle];
	const auto new_gobj = manager.gui_objects.emplace();

	const uint16_t rotation =
		(def.flags & button_def::rotation_mask) == button_def::rotation_90_left ?
		gui_object::rotation_left :
		gui_object::rotation_upright;

	new_gobj.object.flags.store(gui_object::visible_after_update | gui_object::enabled | rotation, std::memory_order_release);
	new_gobj.object.position = def.position;
	new_gobj.object.size = def.size;
	new_gobj.object.align = alignment_from_definition(def);

	instantiate_graphical_object(static_manager, manager, new_gobj, def.graphical_object_handle);

	if (is_valid_index(def.text_handle)) {
		const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(def.font_handle);
		detail::create_linear_text(static_manager, manager, new_gobj, def.text_handle, text_aligment_from_button_definition(def), text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size });
	}

	return new_gobj;
}


ui::tagged_gui_object ui::detail::create_element_instance(gui_static& static_manager, gui_manager& manager, icon_tag handle) {
	const ui::icon_def& icon_def = static_manager.ui_definitions.icons[handle];
	const auto new_gobj = manager.gui_objects.emplace();

	const uint16_t rotation =
		(icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright?
		ui::gui_object::rotation_upright :
		((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);

	new_gobj.object.position = icon_def.position;
	new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
	new_gobj.object.align = alignment_from_definition(icon_def);

	instantiate_graphical_object(static_manager, manager, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);

	if(rotation == ui::gui_object::rotation_right) {
		new_gobj.object.position = ui::xy_pair{
			int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
			int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
		new_gobj.object.size = ui::xy_pair{new_gobj.object.size.y, new_gobj.object.size.x};
	}

	new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
	new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);

	return new_gobj;
}

ui::tagged_gui_object ui::detail::create_element_instance(gui_static& static_manager, gui_manager& manager, ui::text_tag handle, const text_data::replacement* candidates, uint32_t count) {
	const ui::text_def& text_def = static_manager.ui_definitions.text[handle];
	const auto new_gobj = manager.gui_objects.emplace();
	
	new_gobj.object.position = text_def.position;
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width), 0 };
	new_gobj.object.align = alignment_from_definition(text_def);

	new_gobj.object.size.x -= text_def.border_size.x * 2;

	if (is_valid_index(text_def.text_handle)) {
		const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
		detail::create_multiline_text(static_manager, manager, new_gobj, text_def.text_handle, text_aligment_from_text_definition(text_def), text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size }, candidates, count);
	}

	for_each_child(manager, new_gobj, [adjust = text_def.border_size](tagged_gui_object c) {
		c.object.position += adjust;
	});

	new_gobj.object.size.x += text_def.border_size.x * 2;
	new_gobj.object.size.y += text_def.border_size.y * 2;

	return new_gobj;
}

namespace ui {
	inline graphics::rotation reverse_rotation(graphics::rotation r) {
		switch(r) {
			case graphics::rotation::upright: return graphics::rotation::upright;
			case graphics::rotation::right: return graphics::rotation::left;
			case graphics::rotation::left: return graphics::rotation::right;
			default: return graphics::rotation::upright;
		}
	}
}

void ui::detail::render_object_type(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper& ogl, const gui_object& root_obj, const screen_position& position, uint32_t type, bool currently_enabled) {
	const auto current_rotation = root_obj.get_rotation();

	switch (type) {
		case ui::gui_object::type_barchart:
		{
			auto dt = manager.data_textures.safe_at(data_texture_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (dt) {
				ogl.render_barchart(
					currently_enabled,
					position.effective_position_x,
					position.effective_position_y,
					position.effective_width,
					position.effective_height,
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
							position.effective_position_x,
							position.effective_position_y,
							position.effective_width,
							position.effective_height,
							*t,
							current_rotation);
					} else if (gi->graphics_object->number_of_frames > 1) {
						ogl.render_subsprite(
							currently_enabled,
							gi->frame,
							gi->graphics_object->number_of_frames,
							position.effective_position_x,
							position.effective_position_y,
							position.effective_width,
							position.effective_height,
							*t,
							current_rotation);
					} else {
						ogl.render_textured_rect(
							currently_enabled,
							position.effective_position_x,
							position.effective_position_y,
							position.effective_width,
							position.effective_height,
							*t,
							current_rotation);
					}

				}
			}
			break;
		}
		case ui::gui_object::type_tinted_icon:
		{
			auto ti = manager.tinted_icon_instances.safe_at(tinted_icon_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if(ti) {
				ogl.render_tinted_textured_rect(
					position.effective_position_x,
					position.effective_position_y,
					position.effective_width,
					position.effective_height,
					ti->r,
					ti->g,
					ti->b,
					*(ti->t),
					current_rotation
				);
			}
			break;
		}
		case ui::gui_object::type_linegraph:
		{
			auto l = manager.lines_set.safe_at(lines_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (l) {
				ogl.render_linegraph(
					currently_enabled,
					position.effective_position_x,
					position.effective_position_y,
					position.effective_width,
					position.effective_height,
					*l);
			}
			break;
		}
		case ui::gui_object::type_masked_flag:
		{
			auto m = manager.multi_texture_instances.safe_at(multi_texture_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (m) {
				const auto mask = m->mask_or_primary;
				const auto flag = m->flag_or_secondary;
				if (flag && mask) {
					ogl.render_masked_rect(
						currently_enabled,
						position.effective_position_x,
						position.effective_position_y,
						position.effective_width,
						position.effective_height,
						*flag,
						*mask,
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
				const auto secondary = m->flag_or_secondary;
				if (primary && secondary) {
					ogl.render_progress_bar(
						currently_enabled,
						m->progress,
						position.effective_position_x,
						position.effective_position_y,
						position.effective_width,
						position.effective_height,
						*primary,
						*secondary,
						reverse_rotation(current_rotation));
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
					position.effective_position_x,
					position.effective_position_y,
					position.effective_width,
					*dt);
			}
			break;
		}
		case ui::gui_object::type_text_instance:
		{
			auto ti = manager.text_instances.safe_at(text_instance_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (ti) {
				auto& fnt = static_manager.fonts.at(ti->font_handle);

				switch (ti->color) {
					case ui::text_color::black:
						ogl.render_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.0f, 0.0f, 0.0f }, fnt);
						break;
					case ui::text_color::green:
						ogl.render_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.2f, 0.823f, 0.2f }, fnt);
						break;
					case ui::text_color::outlined_black:
						ogl.render_outlined_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.0f, 0.0f, 0.0f }, fnt);
						break;
					case ui::text_color::outlined_white:
						ogl.render_outlined_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 1.0f, 1.0f }, fnt);
						break;
					case ui::text_color::red:
						ogl.render_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 0.2f, 0.2f }, fnt);
						break;
					case ui::text_color::white:
						ogl.render_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 1.0f, 1.0f }, fnt);
						break;
					case ui::text_color::yellow:
						ogl.render_text(ti->text, ti->length, currently_enabled, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 0.75f, 0.2f }, fnt);
						break;
				}
			}
			break;
		}
	}
}

void ui::detail::render(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper &ogl, const gui_object &root_obj, ui::xy_pair position, ui::xy_pair container_size, bool parent_enabled) {
	const auto flags = root_obj.flags.load(std::memory_order_acquire);
	if ((flags & ui::gui_object::visible) == 0)
		return;

	const uint16_t type = flags & ui::gui_object::type_mask;

	const auto root_position = position + ui::detail::position_with_alignment(container_size, root_obj.position, root_obj.align);

	screen_position screen_pos{
		static_cast<float>(root_position.x) * manager.scale(),
		static_cast<float>(root_position.y) * manager.scale(),
		static_cast<float>(root_obj.size.x) * manager.scale(),
		static_cast<float>(root_obj.size.y) * manager.scale()
	};

	const bool currently_enabled = parent_enabled && ((flags & ui::gui_object::enabled) != 0);

	if ((flags & ui::gui_object::dont_clip_children) == 0) {
		graphics::scissor_rect clip(ogl, std::lround(screen_pos.effective_position_x), manager.height() - std::lround(screen_pos.effective_position_y + screen_pos.effective_height), std::lround(screen_pos.effective_width), std::lround(screen_pos.effective_height));

		detail::render_object_type(static_manager, manager, ogl, root_obj, screen_pos, type, currently_enabled && ((flags & ui::gui_object::display_as_disabled) == 0));

		gui_object_tag current_child = root_obj.first_child;
		while (is_valid_index(current_child)) {
			const auto& child_object = manager.gui_objects.at(current_child);
			detail::render(static_manager, manager, ogl, child_object, root_position, root_obj.size, currently_enabled);
			current_child = child_object.right_sibling;
		}
	} else {
		detail::render_object_type(static_manager, manager, ogl, root_obj, screen_pos, type, currently_enabled && ((flags & ui::gui_object::display_as_disabled) == 0));

		gui_object_tag current_child = root_obj.first_child;
		while (is_valid_index(current_child)) {
			const auto& child_object = manager.gui_objects.at(current_child);
			detail::render(static_manager, manager, ogl, child_object, root_position, root_obj.size, currently_enabled);
			current_child = child_object.right_sibling;
		}
	}
}

bool ui::set_focus(world_state& ws, tagged_gui_object g) {
	
	if (g.object.associated_behavior && g.object.associated_behavior->on_get_focus(g.id, ws)) {
		if (ws.w.gui_m.focus != g.id) {
			clear_focus(ws);
			ws.w.gui_m.focus = g.id;
		}
		return true;
	}
	return false;
}

void ui::clear_focus(world_state& ws) {
	if (is_valid_index(ws.w.gui_m.focus)) {
		auto& with_focus = ws.w.gui_m.gui_objects.at(ws.w.gui_m.focus);
		if (with_focus.associated_behavior) {
			with_focus.associated_behavior->on_lose_focus(ws.w.gui_m.focus, ws);
			ws.w.gui_m.focus = gui_object_tag();
		}
	}
}

void ui::make_visible_and_update(gui_manager& manager, gui_object& g) {
	g.flags.fetch_or(ui::gui_object::visible_after_update, std::memory_order_acq_rel);
	manager.flag_minimal_update();
}

void ui::hide(gui_object& g) {
	g.flags.fetch_and((uint16_t)(~ui::gui_object::visible & ~ui::gui_object::visible_after_update), std::memory_order_acq_rel);
}

void ui::make_visible_immediate(gui_object& g) {
	g.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
}

void ui::set_enabled(gui_object& g, bool enabled) {
	if (enabled)
		g.flags.fetch_or(ui::gui_object::enabled, std::memory_order_acq_rel);
	else
		g.flags.fetch_and((uint16_t)~ui::gui_object::enabled, std::memory_order_acq_rel);
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

ui::xy_pair ui::absolute_position(gui_manager& manager, tagged_gui_object g) {
	ui::xy_pair sum{ 0, 0 };
	ui::xy_pair child_position = g.object.position;
	auto child_alignment = g.object.align;
	gui_object_tag parent = g.object.parent;

	while (is_valid_index(parent)) {
		const auto& parent_object = manager.gui_objects.at(parent);

		sum += ui::detail::position_with_alignment(parent_object.size, child_position, child_alignment);
		child_position = parent_object.position;
		child_alignment = parent_object.align;

		parent = parent_object.parent;
	}
	sum += child_position;

	return sum;
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

bool ui::gui_manager::on_lbutton_down(world_state& ws, const lbutton_down& ld) {
	if (false == detail::dispatch_message(*this, [&ws](ui::tagged_gui_object obj, const lbutton_down& ) {
		return set_focus(ws, obj);
	}, tagged_gui_object{ root, gui_object_tag(0) }, root.size, ui::rescale_message(ld, _scale))) {
		focus = gui_object_tag();
	}

	return detail::dispatch_message(*this, [&ws](ui::tagged_gui_object obj, const lbutton_down& l) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_lclick(obj.id, ws, l);
		return false;
	}, tagged_gui_object{ root, gui_object_tag(0) }, root.size, ui::rescale_message(ld, _scale));
}

bool ui::gui_manager::on_rbutton_down(world_state& ws, const rbutton_down& rd) {
	return detail::dispatch_message(*this, [&ws](ui::tagged_gui_object obj, const rbutton_down& r) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_rclick(obj.id, ws, r);
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, root.size, ui::rescale_message(rd, _scale));
}

bool ui::gui_manager::on_mouse_move(world_state& static_manager, const mouse_move& mm) {
	const bool found_tooltip = detail::dispatch_message(*this, [&static_manager](ui::tagged_gui_object obj, const mouse_move& m) {
		if (obj.object.associated_behavior) {
			const auto tt_behavior = obj.object.associated_behavior->has_tooltip(obj.id, static_manager, m);
			if (tt_behavior == tooltip_behavior::transparent)
				return false;
			if (tt_behavior == tooltip_behavior::no_tooltip) {
				ui::hide(ui::tagged_gui_object{ static_manager.w.gui_m.tooltip_window, gui_object_tag(3) });
				static_manager.w.gui_m.tooltip = obj.id;
				return true;
			}
			if ((static_manager.w.gui_m.tooltip != obj.id) | (tt_behavior == tooltip_behavior::variable_tooltip)) {
				static_manager.w.gui_m.tooltip = obj.id;
				clear_children(static_manager.w.gui_m, ui::tagged_gui_object{ static_manager.w.gui_m.tooltip_window, gui_object_tag(3) });
				obj.object.associated_behavior->create_tooltip(obj.id, static_manager, m, ui::tagged_gui_object{ static_manager.w.gui_m.tooltip_window, gui_object_tag(3) });
				ui::shrink_to_children(static_manager.w.gui_m, ui::tagged_gui_object{ static_manager.w.gui_m.tooltip_window, gui_object_tag(3) }, 16);

				static_manager.w.gui_m.tooltip_window.position = ui::absolute_position(static_manager.w.gui_m, obj);
				if(static_manager.w.gui_m.tooltip_window.position.y + obj.object.size.y + static_manager.w.gui_m.tooltip_window.size.y <= static_manager.w.gui_m.height())
					static_manager.w.gui_m.tooltip_window.position.y += obj.object.size.y;
				else
					static_manager.w.gui_m.tooltip_window.position.y -= static_manager.w.gui_m.tooltip_window.size.y;
				static_manager.w.gui_m.tooltip_window.position.x += obj.object.size.x / 2;
				static_manager.w.gui_m.tooltip_window.position.x -= static_manager.w.gui_m.tooltip_window.size.x / 2;
				static_manager.w.gui_m.tooltip_window.position.x = std::max(static_manager.w.gui_m.tooltip_window.position.x, 0i16);

				static_manager.w.gui_m.tooltip_window.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
			}
			return true;
		}
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, root.size, ui::rescale_message(mm, _scale));

	if (!found_tooltip && is_valid_index(tooltip)) {
		tooltip = gui_object_tag();
		hide(ui::tagged_gui_object{ tooltip_window, gui_object_tag(3) });
	}

	return found_tooltip;
}

bool ui::gui_manager::on_mouse_drag(world_state& ws, const mouse_drag& md) {
	if (is_valid_index(focus)) {
		if (gui_objects.at(focus).associated_behavior) {
			return gui_objects.at(focus).associated_behavior->on_drag(focus, ws, ui::rescale_message(md, _scale));
		} else
			return false;
	}
	return false;
}

bool ui::gui_manager::on_keydown(world_state& ws, const key_down& kd) {
	return detail::dispatch_message(*this, [&ws](ui::tagged_gui_object obj, const key_down& k) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_keydown(obj.id, ws, k);
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, root.size, ui::rescale_message(kd, _scale));
}

bool ui::gui_manager::on_scroll(world_state& ws, const scroll& se) {
	return detail::dispatch_message(*this, [&ws](ui::tagged_gui_object obj, const scroll& s) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_scroll(obj.id, ws, s);
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, root.size, ui::rescale_message(se, _scale));
}

bool ui::gui_manager::on_text(world_state& ws, const text_event &te) {
	if (is_valid_index(focus)) {
		if (gui_objects.at(focus).associated_behavior)
			return gui_objects.at(focus).associated_behavior->on_text(focus, ws, te);
		else
			return false;
	}
	return false;
}

void ui::detail::update(tagged_gui_object obj, world_state& w) {
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible_after_update) != 0) {
		obj.object.flags.fetch_and((uint16_t)~ui::gui_object::visible_after_update, std::memory_order_acq_rel);
		if ((object_flags & ui::gui_object::visible) == 0) {
			if (obj.object.associated_behavior)
				obj.object.associated_behavior->on_visible(obj.id, w);
			obj.object.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
		}
	} else if ((object_flags & ui::gui_object::visible) == 0) {
		return;
	}

	if (obj.object.associated_behavior)
		obj.object.associated_behavior->update_data(obj.id, w);

	ui::for_each_child(w.w.gui_m, obj, [&w](ui::tagged_gui_object child) {
		update(child, w);
	});
}

void ui::detail::minimal_update(tagged_gui_object obj, world_state& w) {
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible_after_update) != 0) {
		obj.object.flags.fetch_and((uint16_t)~ui::gui_object::visible_after_update, std::memory_order_acq_rel);
		
		if ((object_flags & ui::gui_object::visible) == 0) {
			if (obj.object.associated_behavior) 
				obj.object.associated_behavior->on_visible(obj.id, w);
			obj.object.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
		}

		if (obj.object.associated_behavior)
			obj.object.associated_behavior->update_data(obj.id, w);

		ui::for_each_child(w.w.gui_m, obj, [&w](ui::tagged_gui_object child) {
			update(child, w);
		});
	} else if ((object_flags & ui::gui_object::visible) != 0) {
		ui::for_each_child(w.w.gui_m, obj, [&w](ui::tagged_gui_object child) {
			minimal_update(child, w);
		});
	}
}

void ui::minimal_update(world_state& w) {
	detail::minimal_update(tagged_gui_object{ w.w.gui_m.root, gui_object_tag(0) }, w);
	detail::minimal_update(tagged_gui_object{ w.w.gui_m.background, gui_object_tag(1) }, w);
	detail::minimal_update(tagged_gui_object{ w.w.gui_m.foreground, gui_object_tag(2) }, w);
}

void ui::update(world_state& w) {
	w.w.gui_m.check_and_clear_minimal_update();
	detail::update(tagged_gui_object{ w.w.gui_m.root, gui_object_tag(0) }, w);
	detail::update(tagged_gui_object{ w.w.gui_m.background, gui_object_tag(1) }, w);
	detail::update(tagged_gui_object{ w.w.gui_m.foreground, gui_object_tag(2) }, w);
}

ui::gui_manager::gui_manager() : gui_manager(1080, 640) {}

ui::gui_manager::gui_manager(int32_t width, int32_t height) :
	_width(width), _height(height),
	root(gui_objects.emplace_at(gui_object_tag(0))),
	background(gui_objects.emplace_at(gui_object_tag(1))),
	foreground(gui_objects.emplace_at(gui_object_tag(2))),
	tooltip_window(gui_objects.emplace_at(gui_object_tag(3))) {

	on_resize(resize{ static_cast<uint32_t>(width > 0 ? width : 0) , static_cast<uint32_t>(height > 0 ? height : 0) });
	
	hide(tagged_gui_object{ tooltip_window, gui_object_tag(3) });
	add_to_back(*this, tagged_gui_object{ foreground, gui_object_tag(2) }, tagged_gui_object{ tooltip_window, gui_object_tag(3) });
}

void ui::gui_manager::on_resize(const resize& r) {
	ui::xy_pair new_size{
		static_cast<int16_t>(static_cast<float>(r.width) / scale()),
		static_cast<int16_t>(static_cast<float>(r.height) / scale()) };

	_width = static_cast<int32_t>(r.width);
	_height = static_cast<int32_t>(r.height);
	root.size = new_size;
	foreground.size = new_size;
	background.size = new_size;
}

void ui::gui_manager::rescale(float new_scale) {
	const resize new_size{ static_cast<uint32_t>(static_cast<float>(root.size.x) * scale()), static_cast<uint32_t>(static_cast<float>(root.size.y) * scale()) };
	_scale = new_scale;
	on_resize(new_size);
}

void ui::render(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper& ogl) {
	ogl.use_default_program();
	detail::render(static_manager, manager, ogl, manager.background, ui::xy_pair{ 0, 0 }, manager.background.size, true);
	detail::render(static_manager, manager, ogl, manager.root, ui::xy_pair{ 0, 0 }, manager.root.size, true);
	detail::render(static_manager, manager, ogl, manager.foreground, ui::xy_pair{ 0, 0 }, manager.foreground.size, true);
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


void ui::init_tooltip_window(gui_static& static_manager, gui_manager& manager) {
	manager.tooltip_window.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);
	const auto bg_graphic = manager.graphics_instances.emplace();
	bg_graphic.object.frame = 0;
	bg_graphic.object.graphics_object = &(static_manager.graphics_object_definitions.definitions[static_manager.graphics_object_definitions.standard_text_background]);
	bg_graphic.object.t = &(static_manager.textures.retrieve_by_key(static_manager.textures.standard_tiles_dialog));
	manager.tooltip_window.type_dependant_handle.store(to_index(bg_graphic.id), std::memory_order_release);
}

ui::tagged_gui_object ui::create_scrollable_text_block(world_state& ws, ui::text_tag handle, tagged_gui_object parent, const text_data::replacement* candidates, uint32_t count) {
	const ui::text_def& text_def = ws.s.gui_m.ui_definitions.text[handle];
	const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
	const auto& this_font = ws.s.gui_m.fonts.at(font_h);

	auto res = create_scrollable_region(
		ws,
		parent,
		text_def.position,
		text_def.max_height,
		(int32_t)this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(int_font_size))),
		graphics::obj_definition_tag(),
		[handle, candidates, count](gui_static& sm, gui_manager& m) {
		return detail::create_element_instance(sm, m, handle, candidates, count);
	});
	res.object.align = alignment_from_definition(text_def);

	const auto background = text_def.flags & text_def.background_mask;
	if ((background != ui::text_def::background_none_specified) & (background != ui::text_def::background_transparency_tga)) {

		res.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);

		const auto bg_graphic = ws.w.gui_m.graphics_instances.emplace();
		const auto texture =
			(background == ui::text_def::background_small_tiles_dialog_tga) ?
			ws.s.gui_m.textures.standard_small_tiles_dialog :
			ws.s.gui_m.textures.standard_tiles_dialog;
		bg_graphic.object.frame = 0;
		bg_graphic.object.graphics_object = &(ws.s.gui_m.graphics_object_definitions.definitions[ws.s.gui_m.graphics_object_definitions.standard_text_background]);
		bg_graphic.object.t = &(ws.s.gui_m.textures.retrieve_by_key(texture));

		res.object.type_dependant_handle.store(to_index(bg_graphic.id), std::memory_order_release);
	}

	return res;
}

ui::tagged_gui_object ui::create_scrollable_text_block(world_state& ws, ui::text_tag handle, text_data::text_tag contents, tagged_gui_object parent, const text_data::replacement* candidates, uint32_t count) {
	const ui::text_def& text_def = ws.s.gui_m.ui_definitions.text[handle];
	const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
	const auto& this_font = ws.s.gui_m.fonts.at(font_h);

	auto res = create_scrollable_region(
		ws,
		parent,
		text_def.position,
		text_def.max_height,
		(int32_t)this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(int_font_size))),
		graphics::obj_definition_tag(),
		[handle, contents, candidates, count, &text_def](gui_static& sm, gui_manager& m) {
		const auto new_gobj = m.gui_objects.emplace();

		new_gobj.object.position = ui::xy_pair{ 0, 0 };
		new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width), 0 };

		new_gobj.object.size.x -= text_def.border_size.x * 2;

		if (is_valid_index(contents)) {
			const auto tple_res /*[font_h, is_black, int_font_size]*/ = graphics::unpack_font_handle(text_def.font_handle);
			detail::create_multiline_text(sm, m, new_gobj, contents, text_aligment_from_text_definition(text_def), text_format{ std::get<1>(tple_res) ? ui::text_color::black : ui::text_color::white, std::get<0>(tple_res), std::get<2>(tple_res) }, candidates, count);
		}

		for_each_child(m, new_gobj, [adjust = text_def.border_size](tagged_gui_object c) {
			c.object.position += adjust;
		});

		new_gobj.object.size.x += text_def.border_size.x * 2;
		new_gobj.object.size.y += text_def.border_size.y * 2;

		return new_gobj;
	});
	res.object.align = alignment_from_definition(text_def);

	const auto background = text_def.flags & text_def.background_mask;
	if ((background != ui::text_def::background_none_specified) & (background != ui::text_def::background_transparency_tga)) {

		res.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);

		const auto bg_graphic = ws.w.gui_m.graphics_instances.emplace();
		const auto texture =
			(background == ui::text_def::background_small_tiles_dialog_tga) ?
			ws.s.gui_m.textures.standard_small_tiles_dialog :
			ws.s.gui_m.textures.standard_tiles_dialog;
		bg_graphic.object.frame = 0;
		bg_graphic.object.graphics_object = &(ws.s.gui_m.graphics_object_definitions.definitions[ws.s.gui_m.graphics_object_definitions.standard_text_background]);
		bg_graphic.object.t = &(ws.s.gui_m.textures.retrieve_by_key(texture));

		res.object.type_dependant_handle.store(to_index(bg_graphic.id), std::memory_order_release);
	}

	return res;
}

void ui::shrink_to_children(gui_manager& manager, tagged_gui_object g) {
	ui::shrink_to_children(manager, g, 0);
}

void ui::shrink_to_children(gui_manager& manager, tagged_gui_object g, int32_t border) {
	g.object.size = ui::xy_pair{ 0,0 };
	ui::xy_pair minimum_child{ std::numeric_limits<decltype(g.object.size.x)>::max(), std::numeric_limits<decltype(g.object.size.y)>::max() };

	for_each_child(manager, g, [g, &minimum_child](tagged_gui_object child) {
		g.object.size.x = static_cast<int16_t>(std::max(int32_t(g.object.size.x), child.object.size.x + child.object.position.x));
		g.object.size.y = static_cast<int16_t>(std::max(int32_t(g.object.size.y), child.object.size.y + child.object.position.y));
		minimum_child.x = std::min(minimum_child.x, child.object.position.x);
		minimum_child.y = std::min(minimum_child.y, child.object.position.y);
	});

	for_each_child(manager, g, [g, minimum_child, border](tagged_gui_object child) {
		child.object.position.x += int16_t(border - minimum_child.x);
		child.object.position.y += int16_t(border - minimum_child.y);
	});

	g.object.size.x = static_cast<int16_t>(border * 2 + std::max(g.object.size.x - minimum_child.x, 0));
	g.object.size.y = static_cast<int16_t>(border * 2 + std::max(g.object.size.y - minimum_child.y, 0));
}

ui::tagged_gui_object ui::create_dynamic_window(world_state& ws, window_tag t, tagged_gui_object parent) {
	const auto& definition = ws.s.gui_m.ui_definitions.windows[t];

	if (is_valid_index(definition.background_handle)) {
		const auto window = ((definition.flags & (window_def::is_moveable | window_def::is_dialog)) != 0) ?
			create_dynamic_element<draggable_region>(ws, definition.background_handle, parent) :
			create_dynamic_element<fixed_region>(ws, definition.background_handle, parent);

		window.object.size = definition.size;
		window.object.position = definition.position;
		window.object.align = alignment_from_definition(definition);

		return window;
	} else {
		const auto window = ws.w.gui_m.gui_objects.emplace();

		window.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);

		const auto bg_graphic = ws.w.gui_m.graphics_instances.emplace();
		bg_graphic.object.frame = 0;
		bg_graphic.object.graphics_object = &(ws.s.gui_m.graphics_object_definitions.definitions[ws.s.gui_m.graphics_object_definitions.standard_text_background]);
		bg_graphic.object.t = &(ws.s.gui_m.textures.retrieve_by_key(ws.s.gui_m.textures.standard_tiles_dialog));

		window.object.type_dependant_handle.store(to_index(bg_graphic.id), std::memory_order_release);

		if ((definition.flags & (window_def::is_moveable | window_def::is_dialog)) != 0) {
			window.object.associated_behavior = concurrent_allocator<draggable_region>().allocate(1);
			new (window.object.associated_behavior)draggable_region();
		} else {
			window.object.associated_behavior = concurrent_allocator<fixed_region>().allocate(1);
			new (window.object.associated_behavior)fixed_region();
		}

		window.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
		window.object.associated_behavior->associated_object = &window.object;

		window.object.size = definition.size;
		window.object.position = definition.position;
		window.object.align = alignment_from_definition(definition);

		add_to_back(ws.w.gui_m, parent, window);
		return window;
	}
}

ui::alignment ui::alignment_from_definition(const button_def& d) {
	switch (d.flags & button_def::orientation_mask) {
		case button_def::orientation_lower_left:
			return alignment::bottom_left;
		case button_def::orientation_lower_right:
			return alignment::bottom_right;
		case button_def::orientation_center:
			return alignment::center;
		case button_def::orientation_upper_right:
			return alignment::top_right;
	}
	return alignment::top_left;
}

ui::alignment ui::alignment_from_definition(const icon_def& d) {
	switch (d.flags & icon_def::orientation_mask) {
		case icon_def::orientation_lower_left:
			return alignment::bottom_left;
		case icon_def::orientation_lower_right:
			return alignment::bottom_right;
		case icon_def::orientation_center:
			return alignment::center;
		case icon_def::orientation_upper_right:
			return alignment::top_right;
		case icon_def::orientation_center_down:
			return alignment::bottom_center;
		case icon_def::orientation_center_up:
			return alignment::top_center;
	}
	return alignment::top_left;
}

ui::alignment ui::alignment_from_definition(const text_def& d) {
	switch (d.flags & text_def::orientation_mask) {
		case text_def::orientation_lower_left:
			return alignment::bottom_left;
		case text_def::orientation_lower_right:
			return alignment::bottom_right;
		case text_def::orientation_center:
			return alignment::center;
		case text_def::orientation_upper_right:
			return alignment::top_right;
		case text_def::orientation_center_down:
			return alignment::bottom_center;
		case text_def::orientation_center_up:
			return alignment::top_center;
	}
	return alignment::top_left;
}

ui::alignment ui::alignment_from_definition(const overlapping_region_def& d) {
	switch (d.flags & overlapping_region_def::orientation_mask) {
		case overlapping_region_def::orientation_center:
			return alignment::center;
		case overlapping_region_def::orientation_upper_right:
			return alignment::top_right;
	}
	return alignment::top_left;
}

ui::alignment ui::alignment_from_definition(const listbox_def& d) {
	switch (d.flags & listbox_def::orientation_mask) {
		case listbox_def::orientation_center:
			return alignment::center;
		case listbox_def::orientation_upper_right:
			return alignment::top_right;
		case listbox_def::orientation_center_down:
			return alignment::bottom_center;
	}
	return alignment::top_left;
}

ui::alignment ui::alignment_from_definition(const scrollbar_def&) {
	return alignment::top_left;
}

ui::alignment ui::alignment_from_definition(const window_def& d) {
	switch (d.flags & window_def::orientation_mask) {
		case window_def::orientation_center:
			return alignment::center;
		case window_def::orientation_upper_right:
			return alignment::top_right;
		case window_def::orientation_lower_left:
			return alignment::bottom_left;
		case window_def::orientation_lower_right:
			return alignment::bottom_right;
	}
	return alignment::top_left;
}

float ui::detail::font_size_to_render_size(const graphics::font& f, int32_t sz) {
	const auto ft64_sz = f.line_height(64.0f);
	return static_cast<float>(sz) * 64.0f / ft64_sz;
}
