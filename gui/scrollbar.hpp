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
		virtual bool on_lclick(tagged_gui_object, gui_manager&, const lbutton_down& ld) override;
	};

	template<typename BASE>
	class scrollbar_slider : public gui_behavior {
	private:
		scrollbar<BASE>& parent;
		int32_t base_position;
	public:
		scrollbar_slider(scrollbar<BASE>& p) : parent(p), base_position(0) {}
		virtual bool on_get_focus(tagged_gui_object, gui_manager&) override { return true; };
		virtual bool on_lclick(tagged_gui_object, gui_manager&, const lbutton_down&) override { base_position = parent.position(); return true; };
		virtual bool on_drag(tagged_gui_object, gui_manager&, const mouse_drag& md) override;
	};

	template<typename BASE>
	class scrollbar_left_button {
	private:
		scrollbar<BASE>& parent;
	public:
		scrollbar_left_button(scrollbar<BASE>& p) : parent(p) {}
		void button_function(ui::tagged_gui_object, ui::gui_manager&) {
			parent.adjust_position(parent.position() - parent.step_size());
		}
	};

	template<typename BASE>
	class scrollbar_right_button {
	private:
		scrollbar<BASE>& parent;
	public:
		scrollbar_right_button(scrollbar<BASE>& p) : parent(p) {}
		void button_function(ui::tagged_gui_object, ui::gui_manager&) {
			parent.adjust_position(parent.position() + parent.step_size());
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
void ui::scrollbar<BASE>::adjust_position(int32_t position) {
	const auto old_position = _position;
	update_position(position);
	if (old_position != _position)
		BASE::on_position(_position);
}

template<typename BASE>
void ui::scrollbar<BASE>::update_data(tagged_gui_object o, gui_manager& m) {
	if constexpr(ui::detail::has_update<BASE, ui::scrollbar<BASE>&, tagged_gui_object, gui_manager&>) {
		BASE::update(*this, o, m);
	}
}

template<typename BASE>
void ui::scrollbar<BASE>::update_position(int32_t position) {
	_position = std::min(limt_maximum, std::max(limit_minimum, position));
	if (vertical) {
		slider->position.y = valid_start - slider->size.y / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (_position - minimum)) / static_cast<double>(maximum - minimum));
	} else {
		slider->position.x = valid_start - slider->size.x / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (_position - minimum)) / static_cast<double>(maximum - minimum));
	}
}

namespace detail {
	template<typename T>
	T round_away(T i) {
		return (i < T(0)) ? std::floor(i) : std::ceil(i);
	}
}

template<typename BASE>
bool ui::scrollbar<BASE>::on_scroll(tagged_gui_object, gui_manager&, const scroll& s) {
	adjust_position(_position + (int32_t)::detail::round_away(static_cast<float>(_step_size) * -s.amount));
	return true;
}

template<typename BASE>
void ui::scrollbar<BASE>::update_limit_icons(gui_manager& m) {
	if (minimum_limit_icon) {
		if (limit_minimum != minimum) {
			if (vertical) {
				minimum_limit_icon->position.y = valid_start - slider->size.y / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limit_minimum - minimum)) / static_cast<double>(maximum - minimum));
			} else {
				minimum_limit_icon->position.x = valid_start - slider->size.x / 2 + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limit_minimum - minimum)) / static_cast<double>(maximum - minimum));
			}
			ui::make_visible(m, ui::tagged_gui_object{ *minimum_limit_icon, lmin_tag });
		} else
			ui::hide(ui::tagged_gui_object{ *minimum_limit_icon, lmin_tag });
	}
	if (maximum_limit_icon) {
		if (limt_maximum != maximum) {
			if (vertical) {
				maximum_limit_icon->position.y = valid_start + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limt_maximum - minimum)) / static_cast<double>(maximum - minimum));
			} else {
				maximum_limit_icon->position.x = valid_start + static_cast<int32_t>(static_cast<double>((valid_end - valid_start) * (limt_maximum - minimum)) / static_cast<double>(maximum - minimum));
			}
			ui::make_visible(m, ui::tagged_gui_object{ *maximum_limit_icon, lmax_tag });
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
bool ui::scrollbar_track<BASE>::on_lclick(tagged_gui_object track, gui_manager&, const lbutton_down& ld) {
	const auto[valid_start, valid_end] = parent.track_range();
	const auto[minimum, maximum] = parent.range();
	parent.adjust_position(static_cast<int32_t>(static_cast<double>(((parent.vertical ? (ld.y + track.object.position.y) : (ld.x + track.object.position.x)) - valid_start) * (maximum - minimum)) / static_cast<double>(valid_end - valid_start)) + minimum);
	return true;
}

template<typename BASE>
bool ui::scrollbar_slider<BASE>::on_drag(tagged_gui_object, gui_manager&, const mouse_drag& md) {
	const auto[valid_start, valid_end] = parent.track_range();
	const auto[minimum, maximum] = parent.range();
	parent.adjust_position(static_cast<int32_t>(static_cast<double>((parent.vertical ? md.y : md.x) * (maximum - minimum)) / static_cast<double>(valid_end - valid_start)) + base_position);
	return true;
}

namespace ui {
	namespace detail {
		template<typename BEHAVIOR>
		ui::tagged_gui_object create_scrollbar_internal(gui_manager& manager, ui::scrollbar<BEHAVIOR>& b, const scrollbar_def& scrollbar_definition, tagged_gui_object parent, ui::xy_pair position, int32_t extent) {

			auto scrollbar_obj = manager.gui_objects.emplace();

			b.initialize((scrollbar_definition.flags & ui::scrollbar_def::is_horizontal) == 0,
				0,
				scrollbar_definition.max_value,
				(scrollbar_definition.flags & scrollbar_def::step_mask) == scrollbar_def::step_two ? 2 : 1);

			scrollbar_obj.object.associated_behavior = &b;
			scrollbar_obj.object.align = alignment_from_definition(scrollbar_definition);

			if ((scrollbar_definition.flags & scrollbar_def::has_range_limit) != 0) {
				const auto minimum_range = ui::create_dynamic_element(manager, scrollbar_definition.minimum_limit_icon, scrollbar_obj);
				const auto maximum_range = ui::create_dynamic_element(manager, scrollbar_definition.maximum_limit_icon, scrollbar_obj);

				ui::hide(minimum_range);
				ui::hide(maximum_range);
				b.minimum_limit_icon = &minimum_range.object;
				b.maximum_limit_icon = &maximum_range.object;
			}

			const auto slider = create_dynamic_element<scrollbar_slider<BEHAVIOR>>(manager, scrollbar_definition.slider, scrollbar_obj, b);
			b.slider = &slider.object;

			const auto left_button = create_dynamic_element<simple_button<scrollbar_left_button<BEHAVIOR>>>(manager, b.vertical ? scrollbar_definition.maximum_button : scrollbar_definition.minimum_button, scrollbar_obj, b);
			const auto right_button = create_dynamic_element<simple_button<scrollbar_right_button<BEHAVIOR>>>(manager, b.vertical ? scrollbar_definition.minimum_button : scrollbar_definition.maximum_button, scrollbar_obj, b);
			const auto track = create_dynamic_element<scrollbar_track<BEHAVIOR>>(manager, scrollbar_definition.track, scrollbar_obj, b);

			scrollbar_obj.object.size = b.vertical ? ui::xy_pair{ std::max(left_button.object.size.x, scrollbar_definition.size.x) , extent } : ui::xy_pair{ extent, std::max(left_button.object.size.y, scrollbar_definition.size.y) };
			scrollbar_obj.object.position = position;

			if (track.object.position.x != 0)
				--track.object.position.x;
			if (track.object.position.y != 0)
				--track.object.position.y;
			if (b.vertical) {
				left_button.object.position.y = 0;
				left_button.object.position.x = scrollbar_obj.object.size.x / 2 - left_button.object.size.x / 2;

				right_button.object.position.y = scrollbar_obj.object.size.y - right_button.object.size.y;
				right_button.object.position.x = scrollbar_obj.object.size.x / 2 - right_button.object.size.x / 2;

				slider.object.position.x = scrollbar_obj.object.size.x / 2 - slider.object.size.x / 2;
				slider.object.position.y = left_button.object.size.y;

				b.valid_start = left_button.object.size.y + slider.object.size.y / 2;
				b.valid_end = scrollbar_obj.object.size.y - right_button.object.size.y - slider.object.size.y / 2;

				track.object.size.y = scrollbar_obj.object.size.y - left_button.object.size.y / 2 - right_button.object.size.y / 2;
				track.object.position.y = left_button.object.size.y / 2;

				if (b.minimum_limit_icon)
					b.minimum_limit_icon->position = ui::xy_pair{ scrollbar_obj.object.size.x / 2 - b.minimum_limit_icon->size.x / 2, left_button.object.size.y };
				if (b.maximum_limit_icon)
					b.maximum_limit_icon->position = ui::xy_pair{ scrollbar_obj.object.size.x / 2 - b.maximum_limit_icon->size.x / 2, scrollbar_obj.object.size.y - right_button.object.size.y - slider.object.size.y / 2 };
			} else {
				left_button.object.position.x = 0;
				left_button.object.position.y = scrollbar_obj.object.size.y / 2 - left_button.object.size.y / 2;

				right_button.object.position.x = scrollbar_obj.object.size.x - right_button.object.size.x;
				right_button.object.position.y = scrollbar_obj.object.size.y / 2 - right_button.object.size.y / 2;

				slider.object.position.y = scrollbar_obj.object.size.y / 2 - slider.object.size.y / 2;
				slider.object.position.x = left_button.object.size.x;

				b.valid_start = left_button.object.size.x + slider.object.size.x / 2;
				b.valid_end = scrollbar_obj.object.size.x - right_button.object.size.x - slider.object.size.x / 2;

				track.object.size.x = scrollbar_obj.object.size.x - left_button.object.size.x / 2 - right_button.object.size.x / 2;
				track.object.position.x = left_button.object.size.x / 2;

				if (b.minimum_limit_icon)
					b.minimum_limit_icon->position = ui::xy_pair{ left_button.object.size.x, scrollbar_obj.object.size.y / 2 - b.minimum_limit_icon->size.y / 2 };
				if (b.maximum_limit_icon)
					b.maximum_limit_icon->position = ui::xy_pair{ scrollbar_obj.object.size.x - right_button.object.size.x - slider.object.size.x / 2, scrollbar_obj.object.size.y / 2 - b.maximum_limit_icon->size.y / 2 };
			}

			ui::add_to_back(manager, parent, scrollbar_obj);
			return scrollbar_obj;
		}
	}
}

template<typename BEHAVIOR, typename ... PARAMS>
ui::tagged_gui_object ui::create_fixed_sz_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, PARAMS&& ... params) {
	const auto& scrollbar_definition = manager.ui_definitions.scrollbars[handle];
	ui::scrollbar<BEHAVIOR>* b = concurrent_allocator<ui::scrollbar<BEHAVIOR>>().allocate(1);
	new (b) ui::scrollbar<BEHAVIOR>(std::forward<PARAMS>(params)...);

	return ui::detail::create_scrollbar_internal<BEHAVIOR>(manager, *b, scrollbar_definition, parent, position, extent);
}

template<typename BEHAVIOR, typename ... PARAMS>
ui::tagged_gui_object ui::create_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, PARAMS&& ... params) {
	const auto& scrollbar_definition = manager.ui_definitions.scrollbars[handle];
	ui::scrollbar<BEHAVIOR>* b = concurrent_allocator<ui::scrollbar<BEHAVIOR>>().allocate(1);
	new (b) ui::scrollbar<BEHAVIOR>(std::forward<PARAMS>(params)...);

	return ui::detail::create_scrollbar_internal<BEHAVIOR>(manager, *b, scrollbar_definition, parent, scrollbar_definition.position, std::max(scrollbar_definition.size.x, scrollbar_definition.size.y));
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, scrollbar<B>& b) {
	const auto& scrollbar_definition = manager.ui_definitions.scrollbars[handle];

	const auto res = ui::detail::create_scrollbar_internal<B>(manager, b, scrollbar_definition, parent, scrollbar_definition.position, std::max(scrollbar_definition.size.x, scrollbar_definition.size.y));
	res.object.flags.fetch_or(ui::gui_object::static_behavior, std::memory_order_acq_rel);

	return res;
}

template<typename BEHAVIOR>
ui::tagged_gui_object ui::create_static_fixed_sz_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, scrollbar<BEHAVIOR>& b) {
	const auto& scrollbar_definition = manager.ui_definitions.scrollbars[handle];

	const auto res = ui::detail::create_scrollbar_internal<BEHAVIOR>(manager, b, scrollbar_definition, parent, position, extent);
	res.object.flags.fetch_or(ui::gui_object::static_behavior, std::memory_order_acq_rel);

	return res;
}

#include "gui.hpp"