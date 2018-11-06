#pragma once
#include "gui.h"
#include <algorithm>

#undef max
#undef min

namespace ui {
	template<typename BASE>
	class scrollbar_track : public gui_behavior {
	private:
		scrollbar<BASE>& parent;
	public:
		scrollbar_track(scrollbar<BASE>& p) : parent(p) {}
		virtual bool on_lclick(gui_object_tag, world_state&, const lbutton_down& ld) final override;
	};

	template<typename BASE>
	class scrollbar_slider : public gui_behavior {
	private:
		scrollbar<BASE>& parent;
		int32_t base_position;
	public:
		scrollbar_slider(scrollbar<BASE>& p) : parent(p), base_position(0) {}
		virtual bool mouse_consumer(ui::xy_pair) final override { return true; }
		virtual bool on_get_focus(gui_object_tag, world_state&) final override { return true; }
		virtual bool on_lclick(gui_object_tag, world_state&, const lbutton_down&) final override { base_position = parent.position(); return true; }
		virtual bool on_drag(gui_object_tag, world_state&, const mouse_drag& md) final override;
		virtual void on_drag_finish(gui_object_tag, world_state&) final override { parent.is_being_dragged = false; };
	};

	template<typename BASE>
	class scrollbar_left_button {
	private:
		scrollbar<BASE>& parent;
	public:
		scrollbar_left_button(scrollbar<BASE>& p) : parent(p) {}
		void button_function(gui_object_tag, world_state& ws) {
			parent.adjust_position(ws, parent.position() - parent.step_size());
		}
	};

	template<typename BASE>
	class scrollbar_right_button {
	private:
		scrollbar<BASE>& parent;
	public:
		scrollbar_right_button(scrollbar<BASE>& p) : parent(p) {}
		void button_function(gui_object_tag, world_state& ws) {
			parent.adjust_position(ws, parent.position() + parent.step_size());
		}
	};
}

template<typename BASE>
template<typename ... PARAMS>
ui::scrollbar<BASE>::scrollbar(bool vert, int32_t mini, int32_t maxi, int32_t step, PARAMS&& ... params) : BASE(std::forward<PARAMS>(params)...), _position(mini), maximum(maxi), minimum(mini), limt_maximum(maxi), limit_minimum(mini), _step_size(step), vertical(vert) {}

template<typename BASE>
void ui::scrollbar<BASE>::initialize(bool vert, int32_t mini, int32_t maxi, int32_t step) {
	_position = 0;
	maximum = maxi;
	minimum = mini;
	limt_maximum = maxi;
	limit_minimum = mini;
	_step_size = step;
	vertical = vert;
}

template<typename BASE>
void ui::scrollbar<BASE>::adjust_position(world_state& ws, int32_t position) {
	const auto old_position = _position;
	update_position(position);
	if(old_position != _position) {
		if constexpr (ui::detail::has_on_position<BASE, int32_t>)
			BASE::on_position(_position);
		else
			BASE::on_position(ws, *this, _position);
	}
}

template<typename BASE>
void ui::scrollbar<BASE>::update_data(gui_object_tag o, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, ui::scrollbar<BASE>&, world_state&>) {
		if(is_being_dragged == false)
			BASE::update(*this, w);
	}
}

template<typename BASE>
template<typename window_type>
std::enable_if_t<ui::detail::has_windowed_update<BASE, ui::scrollbar<BASE>&, window_type&, world_state&>, void> ui::scrollbar<BASE>::windowed_update(window_type& w, world_state& s) {
	BASE::windowed_update(*this, w, s);
}

template<typename BASE>
void ui::scrollbar<BASE>::update_position(int32_t position) {
	_position = std::min(limt_maximum, std::max(limit_minimum, position));
	if (vertical) {
		slider->position.y = static_cast<int16_t>(valid_start - slider->size.y / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (_position - minimum)) / static_cast<double>(maximum - minimum)));
	} else {
		slider->position.x = static_cast<int16_t>(valid_start - slider->size.x / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (_position - minimum)) / static_cast<double>(maximum - minimum)));
	}
}

namespace detail {
	template<typename T>
	T round_away(T i) {
		return (i < T(0)) ? std::floor(i) : std::ceil(i);
	}
}

template<typename BASE>
bool ui::scrollbar<BASE>::on_scroll(gui_object_tag, world_state& ws, const scroll& s) {
	adjust_position(ws, _position + (int32_t)::detail::round_away(static_cast<float>(_step_size) * -s.amount));
	return true;
}

template<typename BASE>
void ui::scrollbar<BASE>::update_limit_icons(gui_manager& m) {
	if (minimum_limit_icon) {
		if (limit_minimum != minimum) {
			if (vertical) {
				minimum_limit_icon->position.y = static_cast<int16_t>(valid_start - slider->size.y / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limit_minimum - minimum)) / static_cast<double>(maximum - minimum)));
			} else {
				minimum_limit_icon->position.x = static_cast<int16_t>(valid_start - slider->size.x / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limit_minimum - minimum)) / static_cast<double>(maximum - minimum)));
			}
			minimum_limit_icon->flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
		} else
			ui::hide(ui::tagged_gui_object{ *minimum_limit_icon, lmin_tag });
	}
	if (maximum_limit_icon) {
		if (limt_maximum != maximum) {
			if (vertical) {
				maximum_limit_icon->position.y = static_cast<int16_t>(valid_start + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limt_maximum - minimum)) / static_cast<double>(maximum - minimum)));
			} else {
				maximum_limit_icon->position.x = static_cast<int16_t>(valid_start + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limt_maximum - minimum)) / static_cast<double>(maximum - minimum)));
			}
			maximum_limit_icon->flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
		} else
			ui::hide(ui::tagged_gui_object{ *maximum_limit_icon, lmax_tag });
	}
}

template<typename BASE>
void ui::scrollbar<BASE>::set_limits(gui_manager& m, int32_t lmin, int32_t lmax) {
	limt_maximum = std::min(lmax, maximum);
	limit_minimum = std::max(lmin, minimum);
	update_limit_icons(m);
}

template<typename BASE>
void ui::scrollbar<BASE>::set_range(gui_manager& m, int32_t rmin, int32_t rmax) {
	if (minimum == limit_minimum)
		limit_minimum = rmin;
	minimum = rmin;
	if (maximum == limt_maximum)
		limt_maximum = rmax;
	maximum = rmax;
	update_limit_icons(m);
}

template<typename BASE>
bool ui::scrollbar_track<BASE>::on_lclick(gui_object_tag track, world_state& ws, const lbutton_down& ld) {
	const auto[valid_start, valid_end] = parent.track_range();
	const auto[minimum, maximum] = parent.range();
	parent.adjust_position(ws, static_cast<int32_t>(static_cast<double>(((parent.vertical ? (ld.y + associated_object->position.y) : (ld.x + associated_object->position.x)) - valid_start) * (maximum - minimum)) / static_cast<double>(valid_end - valid_start)) + minimum);
	return true;
}

template<typename BASE>
bool ui::scrollbar_slider<BASE>::on_drag(gui_object_tag, world_state& ws, const mouse_drag& md) {
	parent.is_being_dragged = true;
	const auto[valid_start, valid_end] = parent.track_range();
	const auto[minimum, maximum] = parent.range();
	parent.adjust_position(ws, static_cast<int32_t>(static_cast<double>((parent.vertical ? md.y : md.x) * (maximum - minimum)) / static_cast<double>(valid_end - valid_start)) + base_position);
	return true;
}

namespace ui {
	namespace detail {
		template<typename BEHAVIOR>
		ui::tagged_gui_object create_scrollbar_internal(world_state& ws, ui::scrollbar<BEHAVIOR>& b, const scrollbar_def& scrollbar_definition, tagged_gui_object parent, ui::xy_pair position, int32_t extent) {

			auto scrollbar_obj = ws.w.gui_m.gui_objects.emplace();

			b.initialize((scrollbar_definition.flags & ui::scrollbar_def::is_horizontal) == 0,
				0,
				scrollbar_definition.max_value,
				(scrollbar_definition.flags & scrollbar_def::step_mask) == scrollbar_def::step_two ? 2 : 1);

			scrollbar_obj.object.associated_behavior = &b;
			b.associated_object = &scrollbar_obj.object;

			scrollbar_obj.object.align = alignment_from_definition(scrollbar_definition);

			if ((scrollbar_definition.flags & scrollbar_def::has_range_limit) != 0) {
				const auto minimum_range = ui::create_dynamic_element(ws, scrollbar_definition.minimum_limit_icon, scrollbar_obj);
				const auto maximum_range = ui::create_dynamic_element(ws, scrollbar_definition.maximum_limit_icon, scrollbar_obj);

				ui::hide(minimum_range);
				ui::hide(maximum_range);
				b.minimum_limit_icon = &minimum_range.object;
				b.maximum_limit_icon = &maximum_range.object;
			}

			const auto slider = create_dynamic_element<scrollbar_slider<BEHAVIOR>>(ws, scrollbar_definition.slider, scrollbar_obj, b);
			b.slider = &slider.object;
			slider.object.flags.fetch_or(ui::gui_object::interactable, std::memory_order_acq_rel);

			const auto left_button = create_dynamic_element<simple_button<scrollbar_left_button<BEHAVIOR>>>(ws, b.vertical ? scrollbar_definition.maximum_button : scrollbar_definition.minimum_button, scrollbar_obj, b);
			const auto right_button = create_dynamic_element<simple_button<scrollbar_right_button<BEHAVIOR>>>(ws, b.vertical ? scrollbar_definition.minimum_button : scrollbar_definition.maximum_button, scrollbar_obj, b);
			const auto track = create_dynamic_element<scrollbar_track<BEHAVIOR>>(ws, scrollbar_definition.track, scrollbar_obj, b);
			left_button.object.flags.fetch_or(ui::gui_object::interactable, std::memory_order_acq_rel);
			right_button.object.flags.fetch_or(ui::gui_object::interactable, std::memory_order_acq_rel);

			scrollbar_obj.object.size = b.vertical ? ui::xy_pair{ std::max(left_button.object.size.x, scrollbar_definition.size.x) , static_cast<int16_t>(extent) } : ui::xy_pair{ static_cast<int16_t>(extent), std::max(left_button.object.size.y, scrollbar_definition.size.y) };
			scrollbar_obj.object.position = position;

			if (track.object.position.x != 0)
				--track.object.position.x;
			if (track.object.position.y != 0)
				--track.object.position.y;
			if (b.vertical) {
				left_button.object.position.y = 0;
				left_button.object.position.x = static_cast<int16_t>(scrollbar_obj.object.size.x / 2 - left_button.object.size.x / 2);

				right_button.object.position.y = static_cast<int16_t>(scrollbar_obj.object.size.y - right_button.object.size.y);
				right_button.object.position.x = static_cast<int16_t>(scrollbar_obj.object.size.x / 2 - right_button.object.size.x / 2);

				slider.object.position.x = static_cast<int16_t>(scrollbar_obj.object.size.x / 2 - slider.object.size.x / 2);
				slider.object.position.y = left_button.object.size.y;

				b.valid_start = left_button.object.size.y + slider.object.size.y / 2;
				b.valid_end = scrollbar_obj.object.size.y - right_button.object.size.y - slider.object.size.y / 2;

				track.object.size.y = static_cast<int16_t>(scrollbar_obj.object.size.y - left_button.object.size.y / 2 - right_button.object.size.y / 2);
				track.object.position.y = left_button.object.size.y / 2;

				if (b.minimum_limit_icon)
					b.minimum_limit_icon->position = ui::xy_pair{ static_cast<int16_t>(scrollbar_obj.object.size.x / 2 - b.minimum_limit_icon->size.x / 2), left_button.object.size.y };
				if (b.maximum_limit_icon)
					b.maximum_limit_icon->position = ui::xy_pair{ static_cast<int16_t>(scrollbar_obj.object.size.x / 2 - b.maximum_limit_icon->size.x / 2), static_cast<int16_t>(scrollbar_obj.object.size.y - right_button.object.size.y - slider.object.size.y / 2) };
			} else {
				left_button.object.position.x = 0;
				left_button.object.position.y = static_cast<int16_t>(scrollbar_obj.object.size.y / 2 - left_button.object.size.y / 2);

				right_button.object.position.x = static_cast<int16_t>(scrollbar_obj.object.size.x - right_button.object.size.x);
				right_button.object.position.y = static_cast<int16_t>(scrollbar_obj.object.size.y / 2 - right_button.object.size.y / 2);

				slider.object.position.y = static_cast<int16_t>(scrollbar_obj.object.size.y / 2 - slider.object.size.y / 2);
				slider.object.position.x = left_button.object.size.x;

				b.valid_start = left_button.object.size.x + slider.object.size.x / 2;
				b.valid_end = scrollbar_obj.object.size.x - right_button.object.size.x - slider.object.size.x / 2;

				track.object.size.x = static_cast<int16_t>(scrollbar_obj.object.size.x - left_button.object.size.x / 2 - right_button.object.size.x / 2);
				track.object.position.x = static_cast<int16_t>(left_button.object.size.x / 2);

				if (b.minimum_limit_icon)
					b.minimum_limit_icon->position = ui::xy_pair{ left_button.object.size.x, static_cast<int16_t>(scrollbar_obj.object.size.y / 2 - b.minimum_limit_icon->size.y / 2) };
				if (b.maximum_limit_icon)
					b.maximum_limit_icon->position = ui::xy_pair{ static_cast<int16_t>(scrollbar_obj.object.size.x - right_button.object.size.x - slider.object.size.x / 2), static_cast<int16_t>(scrollbar_obj.object.size.y / 2 - b.maximum_limit_icon->size.y / 2) };
			}

			ui::add_to_back(ws.w.gui_m, parent, scrollbar_obj);
			return scrollbar_obj;
		}
	}
}

template<typename BEHAVIOR, typename ... PARAMS>
ui::tagged_gui_object ui::create_fixed_sz_scrollbar(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, PARAMS&& ... params) {
	const auto& scrollbar_definition = ws.s.gui_m.ui_definitions.scrollbars[handle];
	ui::scrollbar<BEHAVIOR>* b = concurrent_allocator<ui::scrollbar<BEHAVIOR>>().allocate(1);
	new (b) ui::scrollbar<BEHAVIOR>(std::forward<PARAMS>(params)...);

	const auto res = ui::detail::create_scrollbar_internal<BEHAVIOR>(ws, *b, scrollbar_definition, parent, position, extent);
	res.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
	return res;
}

template<typename BEHAVIOR, typename ... PARAMS>
ui::tagged_gui_object ui::create_scrollbar(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, PARAMS&& ... params) {
	const auto& scrollbar_definition = ws.s.gui_m.ui_definitions.scrollbars[handle];
	ui::scrollbar<BEHAVIOR>* b = concurrent_allocator<ui::scrollbar<BEHAVIOR>>().allocate(1);
	new (b) ui::scrollbar<BEHAVIOR>(std::forward<PARAMS>(params)...);

	const auto res = ui::detail::create_scrollbar_internal<BEHAVIOR>(ws, *b, scrollbar_definition, parent, scrollbar_definition.position, std::max(scrollbar_definition.size.x, scrollbar_definition.size.y));
	res.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
	return res;
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, scrollbar<B>& b) {
	const auto& scrollbar_definition = ws.s.gui_m.ui_definitions.scrollbars[handle];

	const auto res = ui::detail::create_scrollbar_internal<B>(ws, b, scrollbar_definition, parent, scrollbar_definition.position, std::max(scrollbar_definition.size.x, scrollbar_definition.size.y));
	ws.w.gui_m.flag_minimal_update();
	return res;
}

template<typename BEHAVIOR>
ui::tagged_gui_object ui::create_static_fixed_sz_scrollbar(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, scrollbar<BEHAVIOR>& b) {
	const auto& scrollbar_definition = ws.s.gui_m.ui_definitions.scrollbars[handle];

	return ui::detail::create_scrollbar_internal<BEHAVIOR>(ws, b, scrollbar_definition, parent, position, extent);
}

#include "buttons.hpp"
#include "gui.hpp"
