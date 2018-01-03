#pragma once
#include "gui.h"

template<typename BASE>
bool ui::piechart<BASE>::on_lclick(gui_object_tag, gui_manager &, const lbutton_down & m) {
	if (fraction_used == 0.0f)
		return false;

	const auto xmod = m.x - int32_t(associated_object->size.x) / 2;
	const auto ymod = m.y - int32_t(associated_object->size.y) / 2;
	const float radius_sq = static_cast<float>(xmod * xmod + ymod * ymod) / static_cast<float>(int32_t(associated_object->size.x) * int32_t(associated_object->size.x) / 4);

	return radius_sq <= 1.0f;
}
template<typename BASE>
bool ui::piechart<BASE>::on_rclick(gui_object_tag, gui_manager &, const rbutton_down &m) {
	if (fraction_used == 0.0f)
		return false;

	const auto xmod = m.x - int32_t(associated_object->size.x) / 2;
	const auto ymod = m.y - int32_t(associated_object->size.y) / 2;
	const float radius_sq = static_cast<float>(xmod * xmod + ymod * ymod) / static_cast<float>(int32_t(associated_object->size.x) * int32_t(associated_object->size.x) / 4);

	return radius_sq <= 1.0f;
}
template<typename BASE>
ui::tooltip_behavior ui::piechart<BASE>::has_tooltip(gui_object_tag, gui_manager &, const mouse_move& m) {
	constexpr double M_PI = 3.1415926535897932384626433832795;

	if (fraction_used == 0.0f)
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
void ui::piechart<BASE>::create_tooltip(gui_object_tag, gui_manager &m, const mouse_move& mm, tagged_gui_object tw) {
	constexpr double M_PI = 3.1415926535897932384626433832795;

	const double fraction =
		(std::atan2(
			static_cast<double>(mm.y - int32_t(associated_object->size.y) / 2),
			static_cast<double>(mm.x - int32_t(associated_object->size.x) / 2)) + M_PI) / (2.0 * M_PI);

	const int32_t data_index = std::min(ui::piechart_resolution - 1, std::max(0, static_cast<int32_t>(fraction * static_cast<double>(ui::piechart_resolution))));
	const float amount = fractions[data_index];
	const auto label = labels[data_index];


	ui::xy_pair cursor{ 0,0 };
	int32_t int_amount = static_cast<int32_t>(amount * 100.0f);
	if (int_amount <= 0) {
		cursor = ui::text_chunk_to_instances(
			m,
			vector_backed_string<char16_t>(u"<1% "),
			tw,
			ui::xy_pair{ 0,0 },
			ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 });
	} else {
		char16_t lbuffer[8] = { 0,0,0,0,0,0,0,0 };
		u16itoa(std::min(int_amount, 100), lbuffer);
		for (int32_t i = 0; i < 8; ++i) {
			if (lbuffer[i] == 0) {
				lbuffer[i] = u'%';
				lbuffer[i + 1] = u' ';
				lbuffer[i + 2] = 0;
				break;
			}
		}
		cursor = ui::text_chunk_to_instances(
			m,
			vector_backed_string<char16_t>(lbuffer),
			tw,
			ui::xy_pair{ 0,0 },
			ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 });
	}

	ui::text_chunk_to_instances(
		m,
		label,
		tw,
		cursor,
		ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 });
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
	fraction_used = 0.0f;
}
template<typename BASE>
void ui::piechart<BASE>::add_entry(gui_manager& manager, vector_backed_string<char16_t> label, float fraction, graphics::color_rgb color) {
	const int32_t last_entry = std::max(0, (int32_t)std::lround(fraction_used * static_cast<float>(ui::piechart_resolution)));
	fraction_used += fraction;
	const int32_t new_last_entry = std::min(ui::piechart_resolution - 1, static_cast<int32_t>(fraction_used * static_cast<float>(ui::piechart_resolution)));

	if (const auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt) {
		const auto data = dt->data();
		for (int32_t i = last_entry; i <= new_last_entry; ++i) {
			fractions[i] = fraction;
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
void ui::piechart<BASE>::update_data(gui_object_tag o, gui_manager& m, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, ui::piechart<BASE>&, tagged_gui_object, gui_manager&, world_state&>) {
		BASE::update(*this, o, m, w);
	}
}

template<typename BASE>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, icon_tag handle, tagged_gui_object parent, piechart<BASE>& b) {
	const auto res = ui::detail::create_element_instance(manager, handle);

	res.object.associated_behavior = &b;
	b.associated_object = &res.object;

	b.clear_entries(manager);
	b.update_display(manager);

	ui::add_to_back(manager, parent, res);
	return res;
}

#include "gui.hpp"
