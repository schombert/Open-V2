#pragma once
#include "gui.h"

template<typename BASE>
bool ui::piechart<BASE>::on_lclick(gui_object_tag, world_state&, const lbutton_down & m) {
	if (portion_used == 0)
		return false;

	const auto xmod = m.x - int32_t(associated_object->size.x) / 2;
	const auto ymod = m.y - int32_t(associated_object->size.y) / 2;
	const float radius_sq = static_cast<float>(xmod * xmod + ymod * ymod) / static_cast<float>(int32_t(associated_object->size.x) * int32_t(associated_object->size.x) / 4);

	return radius_sq <= 1.0f;
}
template<typename BASE>
bool ui::piechart<BASE>::on_rclick(gui_object_tag, world_state &, const rbutton_down &m) {
	if (portion_used == 0)
		return false;

	const auto xmod = m.x - int32_t(associated_object->size.x) / 2;
	const auto ymod = m.y - int32_t(associated_object->size.y) / 2;
	const float radius_sq = static_cast<float>(xmod * xmod + ymod * ymod) / static_cast<float>(int32_t(associated_object->size.x) * int32_t(associated_object->size.x) / 4);

	return radius_sq <= 1.0f;
}
template<typename BASE>
ui::tooltip_behavior ui::piechart<BASE>::has_tooltip(gui_object_tag, world_state &, const mouse_move& m) {
	constexpr double M_PI = 3.1415926535897932384626433832795;

	if (portion_used == 0)
		return tooltip_behavior::transparent;

	const auto xmod = m.x - int32_t(associated_object->size.x) / 2;
	const auto ymod = m.y - int32_t(associated_object->size.y) / 2;
	const float radius_sq = static_cast<float>(xmod * xmod + ymod * ymod) / static_cast<float>(int32_t(associated_object->size.x) * int32_t(associated_object->size.x) / 4);

	if (radius_sq > 1.0f)
		return tooltip_behavior::transparent;
	else {
		const double fraction =
			(std::atan2(
				static_cast<double>(m.y - int32_t(associated_object->size.y) / 2),
				static_cast<double>(m.x - int32_t(associated_object->size.x) / 2)) + M_PI) / (2.0 * M_PI);

		const int32_t data_index = std::min(ui::piechart_resolution - 1, std::max(0, static_cast<int32_t>(fraction * static_cast<double>(ui::piechart_resolution))));
		const auto label = labels[data_index];

		if (label.length() == 0)
			return tooltip_behavior::no_tooltip;
		else
			return tooltip_behavior::variable_tooltip;
	}
}
template<typename BASE>
void ui::piechart<BASE>::create_tooltip(gui_object_tag, world_state& ws, const mouse_move& mm, tagged_gui_object tw) {
	constexpr double M_PI = 3.1415926535897932384626433832795;

	const double fraction =
		(std::atan2(
			static_cast<double>(mm.y - int32_t(associated_object->size.y) / 2),
			static_cast<double>(mm.x - int32_t(associated_object->size.x) / 2)) + M_PI) / (2.0 * M_PI);

	const int32_t data_index = std::min(ui::piechart_resolution - 1, std::max(0, static_cast<int32_t>(fraction * static_cast<double>(ui::piechart_resolution))));
	const float amount = fractions[data_index];
	const auto label = labels[data_index];


	ui::xy_pair cursor{ 0,0 };
	
	char16_t lbuffer[8] = { 0,0,0,0,0,0,0,0 };
	put_value_in_buffer(lbuffer, display_type::percent, amount);
	cursor = ui::text_chunk_to_instances(
		ws.s.gui_m,
		ws.w.gui_m,
		vector_backed_string<char16_t>(lbuffer),
		tw,
		ui::xy_pair{ 0,0 },
		ui::tooltip_text_format);
	cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
	ui::text_chunk_to_instances(
		ws.s.gui_m,
		ws.w.gui_m,
		label,
		tw,
		cursor,
		ui::tooltip_text_format);
}
template<typename BASE>
void ui::piechart<BASE>::clear_entries(gui_manager& manager) {
	for (int32_t i = ui::piechart_resolution - 1; i >= 0; --i) {
		labels[i] = vector_backed_string<char16_t>(); // to ensure atomic assignment
	}
	memset(labels, 0, ui::piechart_resolution * sizeof(vector_backed_string<char16_t>));
	memset(fractions, 0, ui::piechart_resolution * sizeof(float));
	if (const auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt)
		memset(dt->data(), 255, ui::piechart_resolution * 3);
	portion_used = 0;
	remainder = 0.0f;
}

template<typename BASE>
void ui::piechart<BASE>::add_entry(gui_manager& manager, vector_backed_string<char16_t> label, float fraction, graphics::color_rgb color) {
	const int32_t last_entry = portion_used;

	float adjusted_amount = 0.0f;
	remainder = std::modf(float(portion_used) + remainder + fraction * static_cast<float>(ui::piechart_resolution), &adjusted_amount);
	portion_used = std::min(ui::piechart_resolution, static_cast<int32_t>(adjusted_amount));

	if (const auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt) {
		const auto data = dt->data();
		for (int32_t i = last_entry; i < portion_used; ++i) {
			fractions[i] = fraction;
			labels[i] = label;
			data[i * 3 + 0] = color.r;
			data[i * 3 + 1] = color.g;
			data[i * 3 + 2] = color.b;
		}
	}
}

template<typename BASE>
void ui::piechart<BASE>::fill_remainder(gui_manager& manager, vector_backed_string<char16_t> label, graphics::color_rgb color) {
	const int32_t last_entry = portion_used;
	const float unused = (float(ui::piechart_resolution - portion_used) - remainder) / float(ui::piechart_resolution);

	portion_used = ui::piechart_resolution;
	remainder = 0.0f;
	
	if(const auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt) {
		const auto data = dt->data();
		for(int32_t i = last_entry; i < portion_used; ++i) {
			fractions[i] = unused;
			labels[i] = label;
			data[i * 3 + 0] = color.r;
			data[i * 3 + 1] = color.g;
			data[i * 3 + 2] = color.b;
		}
	}
}

template<typename BASE>
void ui::piechart<BASE>::update_display(gui_manager& manager) const {
	if (const auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt)
		dt->data_ready();
}

template<typename BASE>
void ui::piechart<BASE>::update_data(gui_object_tag, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, ui::piechart<BASE>&, world_state&>) {
		clear_entries(w.w.gui_m);
		BASE::update(*this, w);
		update_display(w.w.gui_m);
	}
}

template<typename BASE>
template<typename window_type>
std::enable_if_t<ui::detail::has_windowed_update<BASE, ui::piechart<BASE>&, window_type&, world_state&>, void> ui::piechart<BASE>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, ui::piechart<BASE>&, window_type&, world_state&>) {
		clear_entries(s.w.gui_m);
		BASE::windowed_update(*this, w, s);
		update_display(s.w.gui_m);
	}
}

template<typename BASE>
ui::tagged_gui_object ui::create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, piechart<BASE>& b) {
	const auto res = ui::detail::create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);

	res.object.associated_behavior = &b;
	b.associated_object = &res.object;

	b.clear_entries(ws.w.gui_m);
	b.update_display(ws.w.gui_m);

	ui::add_to_back(ws.w.gui_m, parent, res);
	ws.w.gui_m.flag_minimal_update();
	return res;
}

#include "gui.hpp"
