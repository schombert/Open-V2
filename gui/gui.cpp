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

void ui::gui_manager::destroy(tagged_gui_object g) {
	if (tooltip == g.id) {
		hide_tooltip();
		tooltip = gui_object_tag();
	}
	if (focus == g.id)
		focus = gui_object_tag();

	if (g.object.associated_behavior) {
		if ((g.object.flags.load(std::memory_order_relaxed) & gui_object::dynamic_behavior) != 0) {
			g.object.associated_behavior->~gui_behavior();
			concurrent_allocator<gui_behavior>().deallocate(g.object.associated_behavior, 1);
		}
		g.object.associated_behavior = nullptr;
	}
	const auto type_handle = g.object.type_dependant_handle.load(std::memory_order_relaxed);
	if (type_handle != 0) {
		const auto flags = g.object.flags.load(std::memory_order_relaxed);
		if ((flags & gui_object::type_mask) == gui_object::type_text_instance) {
			text_instances.free(text_instance_tag(type_handle));
		} else if ((flags & gui_object::type_mask) == gui_object::type_graphics_object) {
			graphics_instances.free(graphics_instance_tag(type_handle));
		} else if (((flags & gui_object::type_mask) == gui_object::type_masked_flag) |
			((flags & gui_object::type_mask) == gui_object::type_progress_bar)) {
			multi_texture_instances.free(multi_texture_instance_tag(type_handle));
		} else if((flags & gui_object::type_mask) == gui_object::type_tinted_icon) {
			tinted_icon_instances.free(tinted_icon_instance_tag(type_handle));
		}
	}

	gui_object_tag child = g.object.first_child;
	g.object.first_child = gui_object_tag();

	while (child != gui_object_tag()) {
		gui_object_tag next = gui_objects.at(child).right_sibling;
		destroy(tagged_gui_object{ gui_objects.at(child), child });
		child = next;
	}

	g.object.parent = gui_object_tag();
	g.object.left_sibling = gui_object_tag();
	g.object.right_sibling = gui_object_tag();
	g.object.flags.store(0, std::memory_order_release);
	g.object.type_dependant_handle.store(0, std::memory_order_release);

	gui_objects.free(g.id);
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


	namespace {
		std::pair<ui::xy_pair, char16_t const*> text_chunk_to_single_instance(
			world_state& ws, char16_t const* text_begin, char16_t const* text_end,
			tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, line_manager& lm, behavior_manager& b_manager) {

			graphics::font& this_font = ws.s.gui_m.fonts.at(fmt.font_handle);

			const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
			const auto new_text_instance = ui::create_text_instance(ws.w.gui_m, new_gobj, fmt);

			new_text_instance.object.length = (uint8_t)std::min(ui::text_instance::max_instance_length, uint32_t(text_end - text_begin));
			memcpy(new_text_instance.object.text, text_begin, (new_text_instance.object.length) * sizeof(char16_t));

			if(new_text_instance.object.length == ui::text_instance::max_instance_length
				&& new_text_instance.object.text[ui::text_instance::max_instance_length - 1] != u' ')
				shorten_text_instance_to_space(new_text_instance.object);


			const auto original_length = new_text_instance.object.length;
			auto previous_length = new_text_instance.object.length;
			float new_size = this_font.metrics_text_extent(
				new_text_instance.object.text,
				new_text_instance.object.length,
				ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)),
				is_outlined_color(fmt.color));

			while(lm.exceeds_extent(position.x + int32_t(new_size + 0.5f))) {
				shorten_text_instance_to_space(new_text_instance.object);

				if(new_text_instance.object.length == previous_length) {
					if(position.x != 0) {
						lm.finish_current_line();
						position.x = 0;
						position.y = int16_t(float(position.y) + this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f);
						new_text_instance.object.length = original_length;
					} else {
						break;
					}
				}

				new_size = this_font.metrics_text_extent(
					new_text_instance.object.text,
					new_text_instance.object.length,
					ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)),
					is_outlined_color(fmt.color));
				previous_length = new_text_instance.object.length;
			}

			new_gobj.object.size = ui::xy_pair{
				int16_t(new_size + 0.5f),
				int16_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f) };
			new_gobj.object.position = position;

			b_manager(ws.w.gui_m, new_gobj, new_text_instance);

			add_to_back(ws.w.gui_m, parent_object, new_gobj);
			lm.add_object(&(new_gobj.object));

			return std::make_pair(ui::xy_pair{ static_cast<int16_t>(position.x + new_gobj.object.size.x), position.y }, text_begin + new_text_instance.object.length);
		}

		xy_pair make_raw_text(world_state& ws, char16_t const* text_begin, char16_t const* text_end,
			tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, line_manager& lm, behavior_manager& b_manager) {

			while(text_begin < text_end) {
				std::tie(position, text_begin) = text_chunk_to_single_instance(ws, text_begin, text_end, parent_object, position, fmt, lm, b_manager);
			}

			return position;
		}
	}

	namespace detail {
		xy_pair impl_add_text(xy_pair cursor, std::monostate, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			return cursor;
		}

		xy_pair impl_add_text(xy_pair position, text_data::text_tag text_handle, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			if(!is_valid_index(text_handle))
				return position;

			auto& components = ws.s.gui_m.text_data_sequences.all_sequences[text_handle];
			graphics::font& this_font = ws.s.gui_m.fonts.at(fmt.font_handle);

			const auto components_start = ws.s.gui_m.text_data_sequences.all_components.data() + components.starting_component;
			const auto components_end = components_start + components.component_count;

			ui::text_color current_color = fmt.color;

			for(auto component_i = components_start; component_i != components_end; ++component_i) {
				if(std::holds_alternative<text_data::color_change>(*component_i)) {
					if(std::get<text_data::color_change>(*component_i).color == text_data::text_color::unspecified)
						current_color = fmt.color;
					else
						current_color = text_color_to_ui_text_color(std::get<text_data::color_change>(*component_i).color);
				} else if(std::holds_alternative<text_data::value_placeholder>(*component_i)) {
					const auto rep = text_data::find_replacement(std::get<text_data::value_placeholder>(*component_i), candidates, count);
					const text_format format{ current_color, fmt.font_handle, fmt.font_size };

					if(rep) {
						auto new_bm = bool(b_manager) ? b_manager : behavior_manager{ rep->new_color, rep->click_function };
						position = std::visit([position, format, &ws, &lm, candidates, count, new_bm, parent_object](auto v) mutable {
							return add_text(position, v, format, ws, parent_object, lm, candidates, count, new_bm);
						}, rep->data);
					} else {
						auto const text = text_data::name_from_value_type(std::get<text_data::value_placeholder>(*component_i).value);
						position = add_text(position, text, format, ws, parent_object, lm, candidates, count, b_manager);
					}
				} else if(std::holds_alternative<text_data::text_chunk>(*component_i)) {
					const auto chunk = std::get<text_data::text_chunk>(*component_i);
					const text_format format{ current_color, fmt.font_handle, fmt.font_size };

					const auto text = ws.s.gui_m.text_data_sequences.text_data.data() + chunk.offset;
					const auto text_end = text + chunk.length;

					position = make_raw_text(ws, text, text_end, parent_object, position, format, lm, b_manager);
				} else if(std::holds_alternative<text_data::line_break>(*component_i)) {
					lm.finish_current_line();
					position.x = 0;
					position.y += int16_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f);
				}
			}

			return position;
		}

		xy_pair impl_add_text(xy_pair cursor, char16_t const* txt, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t const* txt_end = txt + std::char_traits<char16_t>::length(txt);
			return make_raw_text(ws, txt, txt_end, parent_object, cursor, fmt, lm, b_manager);
		}

		xy_pair impl_add_text(xy_pair cursor, text_data::percent v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::percent, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::integer v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::integer, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::exact_integer v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::exact_integer, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::fp_three_places v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::fp_three_places, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::fp_two_places v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::fp_two_places, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::fp_one_place v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::fp_one_place, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::percent_fp_one_place v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::percent_fp_one_place, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}
		xy_pair impl_add_text(xy_pair cursor, text_data::currency v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			char16_t local_buffer[32];
			auto end = put_value_in_buffer(local_buffer, display_type::currency, v.value);

			return make_raw_text(ws, local_buffer, end, parent_object, cursor, fmt, lm, b_manager);
		}

		xy_pair impl_add_text(xy_pair cursor, date_tag v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			auto ymd = tag_to_date(v).year_month_day();

			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::month_1 + ymd.month - 1], fmt, ws, parent_object, lm, candidates, count, b_manager);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);

			cursor = ui::add_text(cursor, text_data::integer{ int32_t(ymd.day) }, fmt, ws, parent_object, lm, candidates, count, b_manager);
			cursor = ui::add_text(cursor, u", ", fmt, ws, parent_object, lm, candidates, count, b_manager);
			cursor = ui::add_text(cursor, text_data::integer{ int32_t(ymd.year) }, fmt, ws, parent_object, lm, candidates, count, b_manager);

			return cursor;
		}

		xy_pair impl_add_text(xy_pair cursor, uint32_t v, text_format const& fmt, world_state& ws, tagged_gui_object parent_object,
			line_manager& lm, const text_data::text_replacement* candidates, uint32_t count,
			behavior_manager& b_manager) {

			return impl_add_text(cursor, ws.s.fixed_ui_text[v], fmt, ws, parent_object, lm, candidates, count, b_manager);
		}
	}
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
			case text_data::text_color::light_blue:
				return ui::text_color::blue;
			case text_data::text_color::dark_blue:
				return ui::text_color::dark_blue;
			default:
				return ui::text_color::black;
		}
	}

	void unmanaged_region_scrollbar::on_position(int32_t pos) {
		contents_frame.position.y = static_cast<int16_t>(-pos);
	}


	bool line_manager::exceeds_extent(int32_t w) const {
		return !no_auto_newline && (w + indent) > max_line_extent;
	}

	void line_manager::add_object(gui_object * o) {
		current_line.push_back(o);
	}

	void line_manager::finish_current_line() {
		if(align == text_data::alignment::left) {
			for(auto p : current_line)
				p->position.x += int16_t(indent);
			current_line.clear();
		} else if(align == text_data::alignment::right) {
			int32_t total_element_width = 0;
			for(auto p : current_line)
				total_element_width = std::max(total_element_width, p->position.x + p->size.x);
			const int32_t adjustment = (max_line_extent - total_element_width);
			for(auto p : current_line) {
				p->position.x += int16_t(adjustment);
			}
			current_line.clear();
		} else { // center
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
	new_gobj.object.type_dependant_handle.store(uint16_t(to_index(new_text_instance.id)), std::memory_order_release);

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

ui::xy_pair ui::advance_cursor_to_newline(ui::xy_pair cursor, gui_static& manager, text_format const& fmt, ui::line_manager& lm) {
	lm.finish_current_line();
	return advance_cursor_to_newline(cursor, manager, fmt);
}

ui::xy_pair ui::advance_cursor_by_space(ui::xy_pair cursor, gui_static& manager, text_format const& fmt, int32_t count) {
	graphics::font& this_font = manager.fonts.at(fmt.font_handle);

	static char16_t space = u' ';

	int32_t new_size = static_cast<int32_t>(0.5f + count * this_font.metrics_text_extent(
		&space, 1, ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)), is_outlined_color(fmt.color)));
	return ui::xy_pair{ int16_t(cursor.x + new_size), cursor.y };
}

ui::xy_pair ui::display_colored_percentage(ui::xy_pair cursor_in, float value, ui::text_format const & fmt, world_state & ws, ui::tagged_gui_object container, ui::unlimited_line_manager & lm) {
	if(value < 1.0f) {
		return ui::add_text(cursor_in, text_data::percent{ value },
			ui::text_format{ ui::text_color::red, fmt.font_handle, fmt.font_size },
			ws, container, lm);
	} else {
		return ui::add_text(cursor_in, text_data::percent{ value },
			ui::text_format{ ui::text_color::green, fmt.font_handle, fmt.font_size },
			ws, container, lm);
	}
}

ui::xy_pair ui::display_colored_factor(ui::xy_pair cursor_in, float value, ui::text_format const & fmt, world_state & ws, ui::tagged_gui_object container, ui::unlimited_line_manager & lm) {
	if(value < 1.0f) {
		if(value < 0.01)
			return ui::add_text(cursor_in, text_data::fp_three_places{ value },
				ui::text_format{ ui::text_color::red, fmt.font_handle, fmt.font_size },
				ws, container, lm);
		else
			return ui::add_text(cursor_in, text_data::fp_two_places{ value },
				ui::text_format{ ui::text_color::red, fmt.font_handle, fmt.font_size },
				ws, container, lm);
	} else {
		return ui::add_text(cursor_in, text_data::fp_two_places{ value },
			ui::text_format{ ui::text_color::green, fmt.font_handle, fmt.font_size },
			ws, container, lm);
	}
}

ui::xy_pair ui::display_colored_additive_factor(ui::xy_pair cursor_in, float v, ui::text_format const& fmt, world_state& ws, ui::tagged_gui_object container, ui::unlimited_line_manager& lm, bool invert_color) {
	auto const new_fmt = ui::text_format{ (v < 0) == (invert_color == false) ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size };
	if(v >= 0) {
		char16_t local_buffer[16] = { u'+', 0 };
		put_pos_value_in_buffer(local_buffer + 1, display_type::fp_two_places, v);
		return ui::add_text(cursor_in, local_buffer,
			new_fmt, ws, container, lm);
	} else {
		return ui::add_text(cursor_in, text_data::fp_two_places{ v },
			new_fmt, ws, container, lm);
	}
}

void ui::detail::create_linear_text(world_state& ws, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format& fmt, const text_data::text_replacement* candidates, uint32_t count) {
	graphics::font& this_font = ws.s.gui_m.fonts.at(fmt.font_handle);
	const auto res = align_in_bounds(align, 0, int32_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)))), 0, container.object.size.y);
	ui::xy_pair position{ 0, int16_t(res.second) };
	
	add_text(position, text_handle, fmt, ws, container, ui::line_manager(align, container.object.size.x, line_manager::textbox{}), candidates, count);
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

		overlay_gobj.object.type_dependant_handle.store(uint16_t(to_index(overlay_graphic.id)), std::memory_order_release);
		overlay_gobj.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);

		const auto flag_graphics = manager.multi_texture_instances.emplace();
		flag_graphics.object.mask_or_primary = mask_texture_ptr;
		flag_graphics.object.flag_or_secondary = nullptr;

		flag_gobj.object.type_dependant_handle.store(uint16_t(to_index(flag_graphics.id)), std::memory_order_release);
		flag_gobj.object.flags.fetch_or(ui::gui_object::type_masked_flag, std::memory_order_acq_rel);
	}
}

void ui::detail::instantiate_graphical_object(gui_static& static_manager, ui::gui_manager& manager, ui::tagged_gui_object container, graphics::obj_definition_tag gtag, int32_t frame, bool force_tinted) {
	auto& graphic_object_def = static_manager.graphics_object_definitions.definitions[gtag];

	if (container.object.size == ui::xy_pair{ 0,0 })
		container.object.size = ui::xy_pair{ graphic_object_def.size.x, graphic_object_def.size.y };

	if((graphic_object_def.flags & graphics::object::flip_v) != 0)
		container.object.flags.fetch_or(uint16_t(ui::gui_object::rotation_upright_vertical_flipped), std::memory_order_acq_rel);

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

			container.object.type_dependant_handle.store(uint16_t(to_index(icon_graphic.id)), std::memory_order_release);
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

				container.object.type_dependant_handle.store(uint16_t(to_index(icon_graphic.id)), std::memory_order_release);
			}
			break;
		case graphics::object_type::horizontal_progress_bar:
		{
			const graphics::texture_tag primary_texture = graphic_object_def.primary_texture_handle;
			const graphics::texture_tag secondary_texture(graphic_object_def.type_dependant);

			const auto bar_graphics = manager.multi_texture_instances.emplace();
			bar_graphics.object.mask_or_primary = &(static_manager.textures.retrieve_by_key(primary_texture));
			bar_graphics.object.flag_or_secondary = &(static_manager.textures.retrieve_by_key(secondary_texture));

			container.object.type_dependant_handle.store(uint16_t(to_index(bar_graphics.id)), std::memory_order_release);
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

			container.object.type_dependant_handle.store(uint16_t(to_index(bar_graphics.id)), std::memory_order_release);
			container.object.flags.fetch_or(ui::gui_object::type_progress_bar | ui::gui_object::rotation_left, std::memory_order_acq_rel);
		}
			break;
		case graphics::object_type::flag_mask:
			make_masked_flag(static_manager, manager, container, graphic_object_def);
			break;
		case graphics::object_type::barchart:
		{
			container.object.flags.fetch_or(ui::gui_object::type_barchart, std::memory_order_acq_rel);
			const auto new_dt = manager.data_textures.emplace(uint16_t(frame), 4ui16);
			container.object.type_dependant_handle.store(uint16_t(to_index(new_dt.id)), std::memory_order_release);
		}
			break;
		case graphics::object_type::piechart:
		{
			container.object.flags.fetch_or(ui::gui_object::type_piechart, std::memory_order_acq_rel);

			container.object.size.x = graphic_object_def.size.x * 2;
			container.object.size.y = graphic_object_def.size.y * 2;

			container.object.position.x -= graphic_object_def.size.x;

			const auto new_dt = manager.data_textures.emplace(uint16_t(ui::piechart_resolution), 3ui16);

			container.object.type_dependant_handle.store(uint16_t(to_index(new_dt.id)), std::memory_order_release);
		}
			break;
		case graphics::object_type::linegraph:
			container.object.flags.fetch_or(ui::gui_object::type_linegraph, std::memory_order_acq_rel);

			const auto new_lines = manager.lines_set.emplace(uint32_t(frame));
			container.object.type_dependant_handle.store(uint16_t(to_index(new_lines.id)), std::memory_order_release);

			break;
	}
}

ui::tagged_gui_object ui::detail::create_element_instance(world_state& ws, button_tag handle) {
	const button_def& def = ws.s.gui_m.ui_definitions.buttons[handle];
	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

	uint16_t rotation =
		(def.flags & button_def::rotation_mask) == button_def::rotation_90_right ?
		gui_object::rotation_right :
		gui_object::rotation_upright;

	new_gobj.object.flags.store(gui_object::visible_after_update | gui_object::enabled | rotation, std::memory_order_release);
	new_gobj.object.position = def.position;
	new_gobj.object.size = def.size;
	new_gobj.object.align = alignment_from_definition(def);

	instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, def.graphical_object_handle);

	if (is_valid_index(def.text_handle)) {
		const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(def.font_handle);
		create_linear_text(ws, new_gobj, def.text_handle, text_aligment_from_button_definition(def), text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size });
	}

	return new_gobj;
}


ui::tagged_gui_object ui::detail::create_element_instance(world_state& ws, icon_tag handle) {
	const ui::icon_def& icon_def = ws.s.gui_m.ui_definitions.icons[handle];
	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

	uint16_t rotation =
		(icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright?
		ui::gui_object::rotation_upright :
		((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);

	new_gobj.object.position = icon_def.position;
	new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
	new_gobj.object.align = alignment_from_definition(icon_def);

	instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);

	if(rotation == ui::gui_object::rotation_right) {
		new_gobj.object.size = ui::xy_pair{new_gobj.object.size.y, new_gobj.object.size.x};
	} else if(rotation == ui::gui_object::rotation_left) {
		new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
	}


	if(uint16_t const final_rotation = new_gobj.object.flags.load(std::memory_order_acquire) & ui::gui_object::rotation_mask;
		final_rotation != ui::gui_object::rotation_upright) {

		ui::for_each_child(ws.w.gui_m, new_gobj, [final_rotation](ui::tagged_gui_object child) {
			child.object.flags.fetch_or(final_rotation, std::memory_order_acq_rel);
			child.object.size = ui::xy_pair{ child.object.size.y, child.object.size.x };
		});
	}
	
	new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
	new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);

	return new_gobj;
}

ui::tagged_gui_object ui::detail::create_element_instance(world_state& ws, ui::text_tag handle, const text_data::text_replacement* candidates, uint32_t count) {
	const ui::text_def& text_def = ws.s.gui_m.ui_definitions.text[handle];
	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
	
	new_gobj.object.position = text_def.position;
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width), static_cast<int16_t>(text_def.max_height) };
	new_gobj.object.align = alignment_from_definition(text_def);

	if (is_valid_index(text_def.text_handle)) {
		const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
		add_text(xy_pair{ 0,0 }, text_def.text_handle, text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size }, ws,
			new_gobj, line_manager(text_aligment_from_text_definition(text_def), new_gobj.object.size.x - text_def.border_size.x * 2, line_manager::textbox{}), candidates, count);
	}

	for_each_child(ws.w.gui_m, new_gobj, [adjust = text_def.border_size](tagged_gui_object c) {
		c.object.position += adjust;
	});

	return new_gobj;
}

namespace ui {
	inline graphics::rotation reverse_rotation(graphics::rotation r) {
		switch(r) {
			case graphics::rotation::upright: return graphics::rotation::upright;
			case graphics::rotation::right: return graphics::rotation::left;
			case graphics::rotation::left: return graphics::rotation::right;
			case graphics::rotation::upright_vertical_flipped: return graphics::rotation::upright_vertical_flipped;
			case graphics::rotation::right_vertical_flipped: return graphics::rotation::left_vertical_flipped;
			case graphics::rotation::left_vertical_flipped: return graphics::rotation::right_vertical_flipped;
			default: return graphics::rotation::upright;
		}
	}
}

void ui::detail::render_object_type(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper& ogl, const gui_object& root_obj, const screen_position& position, uint32_t type, graphics::color_modification cmod) {
	const auto current_rotation = root_obj.get_rotation();

	switch (type) {
		case ui::gui_object::type_barchart:
		{
			auto dt = manager.data_textures.safe_at(data_texture_tag(root_obj.type_dependant_handle.load(std::memory_order_acquire)));
			if (dt) {
				ogl.render_barchart(
					cmod,
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
							cmod,
							gi->graphics_object->type_dependant,
							position.effective_position_x,
							position.effective_position_y,
							position.effective_width,
							position.effective_height,
							*t,
							current_rotation);
					} else if (gi->graphics_object->number_of_frames > 1) {
						ogl.render_subsprite(
							cmod,
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
							cmod,
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
					cmod,
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
						cmod,
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
						cmod,
						m->progress,
						position.effective_position_x,
						position.effective_position_y,
						position.effective_width,
						position.effective_height,
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
					cmod,
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
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.0f, 0.0f, 0.0f }, fnt);
						break;
					case ui::text_color::green:
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.2f, 0.823f, 0.2f }, fnt);
						break;
					case ui::text_color::outlined_black:
						ogl.render_outlined_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.0f, 0.0f, 0.0f }, fnt);
						break;
					case ui::text_color::outlined_white:
						ogl.render_outlined_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 1.0f, 1.0f }, fnt);
						break;
					case ui::text_color::red:
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 0.2f, 0.2f }, fnt);
						break;
					case ui::text_color::white:
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 1.0f, 1.0f }, fnt);
						break;
					case ui::text_color::yellow:
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 1.0f, 0.75f, 0.2f }, fnt);
						break;
					case ui::text_color::blue:
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.6f, 0.6f, 1.0f }, fnt);
						break;
					case ui::text_color::dark_blue:
						ogl.render_text(ti->text, ti->length, cmod, position.effective_position_x, position.effective_position_y, ui::detail::font_size_to_render_size(fnt, ti->size * 2) * manager.scale(), graphics::color{ 0.05f, 0.05f, 0.7f }, fnt);
						break;
				}
			}
			break;
		}
	}
}

inline graphics::color_modification determine_color_modification(const ui::gui_manager& manager, const ui::gui_object &root_obj, graphics::color_modification parent_color) {
	const auto flags = root_obj.flags.load(std::memory_order_acquire);

	if(parent_color == graphics::color_modification::interactable_disabled)
		return graphics::color_modification::interactable_disabled;
	if(parent_color == graphics::color_modification::disabled)
		return graphics::color_modification::disabled;
	if(parent_color == graphics::color_modification::interactable) {
		if((flags & ui::gui_object::enabled) != 0)
			return graphics::color_modification::interactable;
		else
			return graphics::color_modification::disabled;
	}
	//else parent_color == graphics::color_modification::none
	if((flags & ui::gui_object::enabled) == 0)
		return graphics::color_modification::disabled;

	if((flags & ui::gui_object::display_as_disabled) != 0) {
		if(&root_obj == manager.gui_objects.safe_at(manager.selected_interactable))
			return graphics::color_modification::interactable_disabled;
		else
			return graphics::color_modification::disabled;
	}

	if(&root_obj == manager.gui_objects.safe_at(manager.selected_interactable))
		return graphics::color_modification::interactable;
	else
		return graphics::color_modification::none;
}

void ui::detail::render(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper &ogl, const gui_object &root_obj, ui::xy_pair position, ui::xy_pair container_size, graphics::color_modification parent_enabled) {
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

	graphics::color_modification cmod = determine_color_modification(manager, root_obj, parent_enabled);

	if ((flags & ui::gui_object::dont_clip_children) == 0) {
		graphics::scissor_rect clip(ogl,
			std::lround(screen_pos.effective_position_x) - 1,
			manager.height() - std::lround(screen_pos.effective_position_y + screen_pos.effective_height) - 1,
			std::lround(screen_pos.effective_width) + 2,
			std::lround(screen_pos.effective_height) + 2
		);
		/*auto const int_start_x = int32_t(std::floor(screen_pos.effective_position_x));
		auto const int_start_y = manager.height() - int32_t(std::ceil(screen_pos.effective_position_y + screen_pos.effective_height));
		graphics::scissor_rect clip(ogl,
			int_start_x - 1,
			int_start_y - 1,
			int32_t(std::ceil(screen_pos.effective_width + screen_pos.effective_position_x)) - int_start_x + 2,
			manager.height() - int32_t(std::floor(screen_pos.effective_position_y)) - int_start_y + 2
		);*/

		detail::render_object_type(static_manager, manager, ogl, root_obj, screen_pos, type, cmod);

		gui_object_tag current_child = root_obj.first_child;
		while (is_valid_index(current_child)) {
			const auto& child_object = manager.gui_objects.at(current_child);
			detail::render(static_manager, manager, ogl, child_object, root_position, root_obj.size, cmod);
			current_child = child_object.right_sibling;
		}
	} else {
		detail::render_object_type(static_manager, manager, ogl, root_obj, screen_pos, type, cmod);

		gui_object_tag current_child = root_obj.first_child;
		while (is_valid_index(current_child)) {
			const auto& child_object = manager.gui_objects.at(current_child);
			detail::render(static_manager, manager, ogl, child_object, root_position, root_obj.size, cmod);
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
			with_focus.associated_behavior->on_drag_finish(ws.w.gui_m.focus, ws);
			with_focus.associated_behavior->on_lose_focus(ws.w.gui_m.focus, ws);
			ws.w.gui_m.focus = gui_object_tag();
		}
	}
}

void ui::make_visible_and_update(gui_manager& manager, gui_object& g) {
	g.flags.fetch_or(ui::gui_object::visible_after_update, std::memory_order_acq_rel);
	manager.flag_minimal_update();
}

void ui::make_visible(gui_manager& manager, gui_object& g) {
	if((g.flags.load(std::memory_order_acquire) & ui::gui_object::visible) == 0) {
		g.flags.fetch_or(ui::gui_object::visible_after_update, std::memory_order_acq_rel);
		manager.flag_minimal_update();
	}
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

void ui::gui_manager::flag_minimal_update() {
	pending_minimal_update.store(true, std::memory_order_release);
}

void ui::replace_children(gui_manager& manager, tagged_gui_object g, tagged_gui_object replacement) {
	auto temp = g.object.first_child;

	gui_object_tag current_child = replacement.object.first_child;
	while(is_valid_index(current_child)) {
		auto& child_object = manager.gui_objects.at(current_child);
		child_object.parent = g.id;
		current_child = child_object.right_sibling;
	}

	g.object.first_child = replacement.object.first_child;
	replacement.object.first_child = temp;

	ui::clear_children(manager, replacement);
}

void ui::clear_children(gui_manager& manager, tagged_gui_object g) {
	gui_object_tag current_child = g.object.first_child;
	g.object.first_child = gui_object_tag();

	while (is_valid_index(current_child)) {
		auto& child_object = manager.gui_objects.at(current_child);
		const gui_object_tag next_child = child_object.right_sibling;

		manager.destroy(tagged_gui_object{child_object, current_child});
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

	manager.destroy(g);
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

ui::tagged_gui_object ui::find_in_parent(const ui::gui_manager& manager, ui::gui_object& o) {
	ui::gui_object_tag parent = o.parent;
	if(parent) {
		auto& parent_object = manager.gui_objects.at(parent);

		ui::gui_object_tag child = parent_object.first_child;
		while(is_valid_index(child)) {
			auto& current_obj = manager.gui_objects.at(child);
			if(&current_obj == &o)
				return ui::tagged_gui_object{ o, child };
			const ui::gui_object_tag next_index = current_obj.right_sibling;
			child = next_index;
		}
	}

	return ui::tagged_gui_object{ o, ui::gui_object_tag() };
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

void ui::gui_manager::refresh_tooltip(world_state& ws) {
	tooltip = ui::gui_object_tag();
	on_mouse_move(ws, mouse_move{ last_mouse_move.x, last_mouse_move.y, key_modifiers::modifiers_none });
	if(!tooltip)
		hide(ui::tagged_gui_object{ tooltip_window, gui_object_tag(3) });
}

bool ui::gui_manager::on_mouse_move(world_state& static_manager, const mouse_move& mm) {
	last_mouse_move = ui::xy_pair{ int16_t(mm.x), int16_t(mm.y) };

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

				auto temp_holder = static_manager.w.gui_m.gui_objects.emplace();

				obj.object.associated_behavior->create_tooltip(obj.id, static_manager, m, temp_holder);
				
				ui::shrink_to_children(static_manager.w.gui_m, temp_holder, 16);

				if(temp_holder.object.size.y > static_manager.w.gui_m.root.size.y) {
					auto const new_y_size = temp_holder.object.size.y / 2;
					auto const new_x_column = temp_holder.object.size.x;
					ui::for_each_child(static_manager.w.gui_m, temp_holder, [new_y_size, new_x_column](tagged_gui_object o) {
						if(o.object.position.y > new_y_size) {
							o.object.position.y -= int16_t(new_y_size);
							o.object.position.x += int16_t(new_x_column);
						}
					});
				}

				ui::replace_children(static_manager.w.gui_m, ui::tagged_gui_object{ static_manager.w.gui_m.tooltip_window, gui_object_tag(3) }, temp_holder);
				static_manager.w.gui_m.destroy(temp_holder);
				
				ui::shrink_to_children(static_manager.w.gui_m, ui::tagged_gui_object{ static_manager.w.gui_m.tooltip_window, gui_object_tag(3) }, 16);

				static_manager.w.gui_m.tooltip_window.position = ui::absolute_position(static_manager.w.gui_m, obj);
				if(static_manager.w.gui_m.tooltip_window.position.y + obj.object.size.y + static_manager.w.gui_m.tooltip_window.size.y <= static_manager.w.gui_m.root.size.y)
					static_manager.w.gui_m.tooltip_window.position.y += obj.object.size.y;
				else
					static_manager.w.gui_m.tooltip_window.position.y -= static_manager.w.gui_m.tooltip_window.size.y;

				static_manager.w.gui_m.tooltip_window.position.y = std::min(static_manager.w.gui_m.tooltip_window.position.y, int16_t(static_manager.w.gui_m.root.size.y - static_manager.w.gui_m.tooltip_window.size.y));
				static_manager.w.gui_m.tooltip_window.position.y = std::max(static_manager.w.gui_m.tooltip_window.position.y, 0i16);

				static_manager.w.gui_m.tooltip_window.position.x += obj.object.size.x / 2;
				static_manager.w.gui_m.tooltip_window.position.x -= static_manager.w.gui_m.tooltip_window.size.x / 2;
				static_manager.w.gui_m.tooltip_window.position.x = std::max(static_manager.w.gui_m.tooltip_window.position.x, 0i16);
				static_manager.w.gui_m.tooltip_window.position.x = std::min(static_manager.w.gui_m.tooltip_window.position.x, int16_t(static_manager.w.gui_m.root.size.x - static_manager.w.gui_m.tooltip_window.size.x));

				static_manager.w.gui_m.tooltip_window.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
			}
			return true;
		}
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, root.size, ui::rescale_message(mm, _scale));

	const bool found_interactable = detail::dispatch_message(*this, [_this = this](ui::tagged_gui_object obj, const ui::message_with_location& m) {
		if(obj.object.associated_behavior) {
			const auto consumes_mouse = obj.object.associated_behavior->mouse_consumer(ui::xy_pair{ int16_t(m.x), int16_t(m.y) });
			if(consumes_mouse) {
				if((obj.object.flags.load(std::memory_order_acquire) & ui::gui_object::interactable) != 0)
					_this->selected_interactable = obj.id;
				else
					_this->selected_interactable = gui_object_tag();
				return true;
			} else {
				return false;
			}
		}
		return false;
	}, tagged_gui_object{ root,gui_object_tag(0) }, root.size, ui::rescale_message(ui::message_with_location{mm.x, mm.y}, _scale));
	if(!found_interactable) {
		selected_interactable = gui_object_tag();
	}
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

void ui::gui_manager::on_lbutton_up(world_state& ws, const lbutton_up&) {
	if(is_valid_index(focus)) {
		if(gui_objects.at(focus).associated_behavior) {
			gui_objects.at(focus).associated_behavior->on_drag_finish(focus, ws);
		}
	}
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

	w.w.gui_m.tooltip = gui_object_tag();
	w.w.gui_m.on_mouse_move(w, ui::mouse_move{ w.w.gui_m.last_mouse_move.x, w.w.gui_m.last_mouse_move.y, key_modifiers::modifiers_none });
}

ui::gui_manager::gui_manager() : gui_manager(1080, 640) {}

ui::gui_manager::gui_manager(int32_t width, int32_t height) :
	_width(width), _height(height),
	root(gui_objects.emplace_at(gui_object_tag(0))),
	background(gui_objects.emplace_at(gui_object_tag(1))),
	foreground(gui_objects.emplace_at(gui_object_tag(2))),
	tooltip_window(gui_objects.emplace_at(gui_object_tag(3))),
	edit_cursor(gui_objects.emplace_at(gui_object_tag(4))) {

	on_resize(resize{ static_cast<uint32_t>(width > 0 ? width : 0) , static_cast<uint32_t>(height > 0 ? height : 0) });
	
	hide(tagged_gui_object{ tooltip_window, gui_object_tag(3) });
	hide(tagged_gui_object{ edit_cursor, gui_object_tag(4) });
	add_to_back(*this, tagged_gui_object{ foreground, gui_object_tag(2) }, tagged_gui_object{ tooltip_window, gui_object_tag(3) });
	add_to_back(*this, tagged_gui_object{ foreground, gui_object_tag(2) }, tagged_gui_object{ edit_cursor, gui_object_tag(4) });
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
	const resize new_size{ static_cast<uint32_t>(_width), static_cast<uint32_t>(_height) };
	_scale = new_scale;
	on_resize(new_size);
}

void ui::render(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper& ogl) {
	ogl.use_default_program();
	auto& cursor_graphic = manager.graphics_instances.at(graphics_instance_tag(manager.edit_cursor.type_dependant_handle));

	auto const time = std::chrono::system_clock::now();
	auto const ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch());
	cursor_graphic.frame = std::clamp(int32_t((sin(float(ms.count() % 1000) * 3.14159f * 2.0f / 1000.0f) + 1.0f) * 24.0f),0,47);
	
	detail::render(static_manager, manager, ogl, manager.background, ui::xy_pair{ 0, 0 }, manager.background.size, graphics::color_modification::none);
	detail::render(static_manager, manager, ogl, manager.root, ui::xy_pair{ 0, 0 }, manager.root.size, graphics::color_modification::none);
	detail::render(static_manager, manager, ogl, manager.foreground, ui::xy_pair{ 0, 0 }, manager.foreground.size, graphics::color_modification::none);
}

graphics::rotation ui::gui_object::get_rotation() const {
	const auto rotation_bits = flags.load(std::memory_order_acquire) & ui::gui_object::rotation_mask;
	if (rotation_bits == ui::gui_object::rotation_left)
		return graphics::rotation::left;
	else if (rotation_bits == ui::gui_object::rotation_right)
		return graphics::rotation::right;
	else if(rotation_bits == ui::gui_object::rotation_right_vertical_flipped)
		return graphics::rotation::right_vertical_flipped;
	else if(rotation_bits == ui::gui_object::rotation_left_vertical_flipped)
		return graphics::rotation::left_vertical_flipped;
	else if(rotation_bits == ui::gui_object::rotation_upright_vertical_flipped)
		return graphics::rotation::upright_vertical_flipped;
	else
		return graphics::rotation::upright;
}


void ui::init_tooltip_window(gui_static& static_manager, gui_manager& manager) {
	manager.tooltip_window.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);
	const auto bg_graphic = manager.graphics_instances.emplace();
	bg_graphic.object.frame = 0;
	bg_graphic.object.graphics_object = &(static_manager.graphics_object_definitions.definitions[static_manager.graphics_object_definitions.standard_text_background]);
	bg_graphic.object.t = &(static_manager.textures.retrieve_by_key(static_manager.textures.standard_tiles_dialog));
	manager.tooltip_window.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);

	manager.edit_cursor.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);
	const auto cursor_graphics = manager.graphics_instances.emplace();
	cursor_graphics.object.frame = 0;
	cursor_graphics.object.graphics_object = &(static_manager.graphics_object_definitions.definitions[static_manager.graphics_object_definitions.edit_cursor]);
	cursor_graphics.object.t = &(static_manager.textures.retrieve_by_key(static_manager.textures.edit_cursor));
	manager.edit_cursor.type_dependant_handle.store(uint16_t(to_index(cursor_graphics.id)), std::memory_order_release);
	manager.edit_cursor.size = ui::xy_pair{ 2,16 };
}

ui::tagged_gui_object ui::create_scrollable_text_block(world_state& ws, ui::text_tag handle, tagged_gui_object parent, const text_data::text_replacement* candidates, uint32_t count) {
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
		[handle, candidates, count](world_state& m) {
		return detail::create_element_instance(m, handle, candidates, count);
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

		res.object.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);
	}

	return res;
}

ui::tagged_gui_object ui::create_scrollable_text_block(world_state& ws, ui::text_tag handle, text_data::text_tag contents, tagged_gui_object parent, const text_data::text_replacement* candidates, uint32_t count) {
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
		[handle, contents, candidates, count, &text_def](world_state& ws) {
			const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

			new_gobj.object.position = ui::xy_pair{ 0, 0 };
			new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width), 0 };

			new_gobj.object.size.x -= text_def.border_size.x * 2;

			if (is_valid_index(contents)) {
				const auto tple_res /*[font_h, is_black, int_font_size]*/ = graphics::unpack_font_handle(text_def.font_handle);
				add_text(xy_pair{ 0,0 }, contents, 
					text_format{ std::get<1>(tple_res) ? ui::text_color::black : ui::text_color::white, std::get<0>(tple_res), std::get<2>(tple_res) },
					ws, new_gobj, line_manager{ text_aligment_from_text_definition(text_def), new_gobj.object.size.x }, candidates, count);
			}

			for_each_child(ws.w.gui_m, new_gobj, [adjust = text_def.border_size](tagged_gui_object c) {
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

		res.object.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);
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

		window.object.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);

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

