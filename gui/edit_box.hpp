#pragma once
#include "gui.hpp"

namespace ui {

	template<typename A, typename B, typename ... C>
	struct _has_on_edit : std::false_type {};
	template<typename A, typename ... C>
	struct _has_on_edit<A, decltype(void(std::declval<A>().on_edit(std::declval<C>() ...))), C...> : std::true_type {};
	template<typename A, typename ... C>
	constexpr bool has_on_edit = _has_on_edit<A, void, C ...>::value;

	template<typename A, typename B, typename ... C>
	struct _has_filter : std::false_type {};
	template<typename A, typename ... C>
	struct _has_filter<A, decltype(void(std::declval<A>().filter(std::declval<C>() ...))), C...> : std::true_type {};
	template<typename A, typename ... C>
	constexpr bool has_filter = _has_filter<A, void, C ...>::value;

	template<typename BASE>
	bool edit_box<BASE>::on_lclick(gui_object_tag, world_state& ws, const lbutton_down& m) {
		graphics::font& this_font = ws.s.gui_m.fonts.at(format.font_handle);

		float previous = float(border_size);
		for(int32_t i = 1; i <= size; ++i) {
			float const text_extent = float(border_size) + this_font.metrics_text_extent(contents, i,
				ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(format.font_size)),
				is_outlined_color(format.color));
			
			if(float(m.x) < text_extent) {
				if(float(m.x) < previous + (text_extent - previous) / 2.0f) {
					cursor_position = i - 1;
				} else {
					cursor_position = i;
				}
				adjust_cursor_position(ws);
				return true;
			}

			previous = text_extent;
		}

		cursor_position = size;
		adjust_cursor_position(ws);
		return true;
	}
	template<typename BASE>
	void edit_box<BASE>::adjust_cursor_position(world_state& ws) const {
		graphics::font& this_font = ws.s.gui_m.fonts.at(format.font_handle);
		int32_t const text_extent = int32_t(this_font.metrics_text_extent(contents, cursor_position,
			ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(format.font_size)),
			is_outlined_color(format.color)) + 0.5f);

		ws.w.gui_m.edit_cursor.position =
			ui::absolute_position(ws.w.gui_m, tagged_gui_object{ *associated_object, gui_object_tag() })
			+ ui::xy_pair{ int16_t(border_size + text_extent - 1), int16_t(border_size) };
	}
	template<typename BASE>
	bool edit_box<BASE>::on_keydown(gui_object_tag, world_state& ws, const key_down& m) {
		if(m.keycode == virtual_key::DELETE_KEY || m.keycode == virtual_key::BACK) {
			if(cursor_position > 0) {
				cursor_position--;
				for(int32_t i = cursor_position; i < size; ++i) {
					contents[i] = contents[i + 1];
				}
				size = std::max(size - 1, 0);
				contents[size] = 0;

				adjust_cursor_position(ws);

				if constexpr(ui::has_on_edit<edit_box<BASE>, edit_box<BASE>&, world_state&>)
					BASE::on_edit(*this, ws);
			}
			ui::make_visible_and_update(ws.w.gui_m, *associated_object);
		} else if(m.keycode == virtual_key::LEFT || m.keycode == virtual_key::NAVIGATION_LEFT) {
			cursor_position = std::max(cursor_position - 1, 0);
			adjust_cursor_position(ws);
		} else if(m.keycode == virtual_key::RIGHT || m.keycode == virtual_key::NAVIGATION_RIGHT) {
			cursor_position = std::min(cursor_position + 1, size);
			adjust_cursor_position(ws);
		}
		return true;
	}

	template<typename BASE>
	bool edit_box<BASE>::_filter(world_state& ws, char16_t t) const {
		if constexpr(ui::has_filter<edit_box<BASE>, edit_box<BASE>&, world_state&, char16_t>)
			return filter(*this, ws, t);
		else
			return true;
	}

	template<typename BASE>
	bool edit_box<BASE>::on_text(gui_object_tag, world_state& ws, const text_event& m) {
		if(size < int32_t(std::extent_v<decltype(contents)>) - 1 && m.text > 31 && _filter(ws, m.text)) {
			for(int32_t i = cursor_position; i < size; ++i) {
				contents[i + 1] = contents[i];
			}
			contents[cursor_position] = m.text;
			cursor_position++;
			size++;
			contents[size] = 0;

			adjust_cursor_position(ws);

			if constexpr(ui::has_on_edit<edit_box<BASE>, edit_box<BASE>&, world_state&>)
				BASE::on_edit(*this, ws);

			ui::make_visible_and_update(ws.w.gui_m, *associated_object);
		}
		return true;
	}
	template<typename BASE>
	bool edit_box<BASE>::on_get_focus(gui_object_tag, world_state& ws) {
		adjust_cursor_position(ws);
		ws.w.gui_m.edit_cursor.size.y = int16_t(associated_object->size.y - border_size * 2);

		ui::make_visible_immediate(ws.w.gui_m.edit_cursor);
		return true;
	}
	template<typename BASE>
	void edit_box<BASE>::on_lose_focus(gui_object_tag, world_state& ws) {
		ui::hide(ws.w.gui_m.edit_cursor);
	}
	template<typename BASE>
	void edit_box<BASE>::update_data(gui_object_tag self, world_state& ws) {
		ui::unlimited_line_manager lm;

		auto temp_holder = ws.w.gui_m.gui_objects.emplace();
		temp_holder.object.size = associated_object->size;
		temp_holder.object.position = associated_object->position;

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(contents),
			temp_holder,
			xy_pair{int16_t(border_size), int16_t(border_size)},
			format,
			lm);

		ui::replace_children(ws.w.gui_m, tagged_gui_object{ *associated_object, self }, temp_holder);
		ws.w.gui_m.gui_objects.free(temp_holder.id);
	}

	template<typename B>
	tagged_gui_object create_static_element(world_state& ws, text_tag handle, tagged_gui_object parent, edit_box<B>& b) {
		auto const& t_def = ws.s.gui_m.ui_definitions.text[handle];
		b.border_size = std::min(t_def.border_size.x, t_def.border_size.y);

		const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(t_def.font_handle);
		b.format = text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size };

		const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

		new_gobj.object.position = t_def.position;
		new_gobj.object.align = alignment_from_definition(t_def);
		new_gobj.object.size = ui::xy_pair{ int16_t(t_def.max_width), int16_t(t_def.max_height) };
		

		if((t_def.flags & text_def::background_mask) == text_def::background_small_tiles_dialog_tga) {
			const auto bg_graphic = ws.w.gui_m.graphics_instances.emplace();
			bg_graphic.object.frame = 0;
			bg_graphic.object.graphics_object = &(ws.s.gui_m.graphics_object_definitions.definitions[ws.s.gui_m.graphics_object_definitions.small_text_background]);
			bg_graphic.object.t = &(ws.s.gui_m.textures.retrieve_by_key(ws.s.gui_m.textures.standard_small_tiles_dialog));
			new_gobj.object.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);
			new_gobj.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);
		} else if((t_def.flags & text_def::background_mask) == text_def::background_tiles_dialog_tga) {
			const auto bg_graphic = ws.w.gui_m.graphics_instances.emplace();
			bg_graphic.object.frame = 0;
			bg_graphic.object.graphics_object = &(ws.s.gui_m.graphics_object_definitions.definitions[ws.s.gui_m.graphics_object_definitions.standard_text_background]);
			bg_graphic.object.t = &(ws.s.gui_m.textures.retrieve_by_key(ws.s.gui_m.textures.standard_tiles_dialog));
			new_gobj.object.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);
			new_gobj.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);
		} else if((t_def.flags & text_def::background_mask) == text_def::background_transparency_tga) {
			const auto bg_graphic = ws.w.gui_m.graphics_instances.emplace();
			bg_graphic.object.frame = 0;
			bg_graphic.object.graphics_object = &(ws.s.gui_m.graphics_object_definitions.definitions[ws.s.gui_m.graphics_object_definitions.small_text_background]);
			bg_graphic.object.t = &(ws.s.gui_m.textures.retrieve_by_key(ws.s.gui_m.textures.standard_transparency));
			new_gobj.object.type_dependant_handle.store(uint16_t(to_index(bg_graphic.id)), std::memory_order_release);
			new_gobj.object.flags.fetch_or(ui::gui_object::type_graphics_object, std::memory_order_acq_rel);
		}

		new_gobj.object.associated_behavior = &b;
		b.associated_object = &new_gobj.object;

		ui::add_to_back(ws.w.gui_m, parent, new_gobj);

		if constexpr(ui::detail::has_on_create<edit_box<B>, edit_box<B>&, world_state&>)
			b.on_create(b, ws);

		ws.w.gui_m.flag_minimal_update();
		return new_gobj;
	}
}
