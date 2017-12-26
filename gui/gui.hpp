#pragma once
#include "gui.h"
#include "graphics\\v2_window.h"
#undef max
#undef min

namespace ui {
	namespace detail {
		template<typename A, typename B, typename ... C>
		struct _has_update : std::false_type {};
		template<typename A, typename ... C>
		struct _has_update<A, decltype(void(std::declval<A>().update_data(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_update = _has_update<A, void, C ...>::value;
	}

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

	class unmanaged_region_scollbar {
	private:
		gui_object& contents_frame;
	public:
		unmanaged_region_scollbar(gui_object& g) : contents_frame(g) {}
		void on_position(int32_t pos);
	};

	class unmanaged_scrollable_region : public visible_region {
	public:
		scrollbar<unmanaged_region_scollbar> sb;

		unmanaged_scrollable_region(gui_object& g) : sb(g) {}
		virtual bool on_scroll(tagged_gui_object o, gui_manager& m, const scroll& s) override { return sb.on_scroll(o, m, s); };
	};

	namespace detail {
		template<typename LM, typename BH = null_behavior_creation>
		std::pair<ui::xy_pair, uint32_t> text_chunk_to_single_instance(ui::gui_manager& container, vector_backed_string<char16_t> text_source, uint32_t offset_in_chunk, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM& lm, const BH& behavior_creator = null_behavior_creation()) {
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

			return std::make_pair(ui::xy_pair{ position.x + new_gobj.object.size.x, position.y }, offset_in_chunk + new_text_instance.object.length);
		}
	}
}

template<typename LM, typename BH>
ui::xy_pair ui::text_chunk_to_instances(ui::gui_manager& manager, vector_backed_string<char16_t> text_source, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM& lm, const BH& behavior_creator) {
	uint32_t position_in_chunk = 0;

	const auto chunk_size = text_source.length();
	while (position_in_chunk < chunk_size) {
		std::tie(position, position_in_chunk) = detail::text_chunk_to_single_instance(manager, text_source, position_in_chunk, parent_object, position, fmt, lm, behavior_creator);
	}
	return position;
}


template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::detail::dispatch_message(const gui_manager& manager, const MESSAGE_FUNCTION &member_f, ui::tagged_gui_object obj, const MESSAGE_TYPE& message) {
	auto relocated_message = adjust_message_location(message, -obj.object.position.x, -obj.object.position.y);
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible) == 0)
		return false;
	
	if (message_within_bounds(relocated_message, obj.object.size.x, obj.object.size.y)) {
	
		if ((object_flags & ui::gui_object::enabled) != 0) {
			const gui_object_tag child = obj.object.first_child;
			if (is_valid_index(child)) {
				gui_object_tag current_child = detail::last_sibling_of(manager, tagged_gui_object{ manager.gui_objects.at(child), child }).id;
				do {
					auto& current_obj = manager.gui_objects.at(current_child);
					const gui_object_tag next_index = current_obj.left_sibling;

					if (dispatch_message(manager, member_f, tagged_gui_object{ current_obj, current_child }, relocated_message))
						return true;

					current_child = next_index;
				} while (is_valid_index(current_child));
			}
		}

		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			if constexpr(std::is_base_of_v<message_with_location, MESSAGE_TYPE>) {
				if ((obj.object.flags.load(std::memory_order_acquire) & ui::gui_object::type_mask) == ui::gui_object::type_graphics_object) {
					const auto& gobj = manager.graphics_instances.at(graphics_instance_tag(obj.object.type_dependant_handle.load(std::memory_order_acquire)));
					if ((gobj.graphics_object->flags & graphics::object::do_transparency_check) != 0) {
						graphics::color_rgba pixel;

						if (gobj.graphics_object->number_of_frames != 0) {
							pixel = gobj.t->get_pixel(
								(static_cast<float>(relocated_message.x + obj.object.size.x * gobj.frame)) / (static_cast<float>(obj.object.size.x * gobj.graphics_object->number_of_frames)),
								static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y)));
						} else {
							pixel = gobj.t->get_pixel(
								static_cast<float>(relocated_message.x) / (static_cast<float>(obj.object.size.x)),
								static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y)));
						}

						if (pixel.a < 127)
							return false;
					}
				}
			}

			if (member_f(obj, relocated_message))
				return true;
		}
	}
	return false;
}

template<typename T>
void ui::for_each_child(gui_manager& manager, tagged_gui_object parent, const T& f) {
	gui_object_tag child = parent.object.first_child;
	while (is_valid_index(child)) {
		auto& current_obj = manager.gui_objects.at(child);
		const gui_object_tag next_index = current_obj.right_sibling;
		f(tagged_gui_object{ current_obj, child });
		child = next_index;
	}
}

namespace ui {
	namespace detail {
		template<typename A, typename B>
		struct _has_shortcut : public std::false_type {};
		template<typename A>
		struct _has_shortcut<A, decltype(void(std::declval<A>().shortcut))> : public std::true_type {};
		template<typename A>
		constexpr bool has_shortcut = _has_shortcut<A, void>::value;
	}
}

template<typename T, typename B>
ui::tagged_gui_object ui::create_static_button(gui_manager& manager, T handle, tagged_gui_object parent, B& b) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);
	new_obj.object.flags.fetch_or(gui_object::static_behavior, std::memory_order_acq_rel);
	new_obj.object.associated_behavior = &b;

	if constexpr(std::is_same_v<T, button_tag> && detail::has_shortcut<B>) {
		auto& bdef = manager.ui_definitions.buttons[handle];
		b.shortcut = bdef.shortcut;
	}

	ui::add_to_back(manager, parent, new_obj);

	return new_obj;
}

template<typename BEHAVIOR, typename T, typename ... PARAMS>
ui::tagged_gui_object ui::create_dynamic_button(gui_manager& manager, T handle, tagged_gui_object parent, PARAMS&& ... params) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);

	BEHAVIOR* b = concurrent_allocator<BEHAVIOR>().allocate(1);
	new (b)BEHAVIOR(std::forward<PARAMS>(params) ...);

	new_obj.object.associated_behavior = b;

	if constexpr(std::is_same_v<T, button_tag> && detail::has_shortcut<BEHAVIOR>) {
		auto& bdef = manager.ui_definitions.buttons[handle];
		b->shortcut = bdef.shortcut;
	}

	ui::add_to_back(manager, parent, new_obj);

	return new_obj;
}

template<typename BASE>
bool ui::simple_button<BASE>::on_lclick(tagged_gui_object o, gui_manager & m, const lbutton_down &) {
	BASE::button_function(o, m);
	return true;
}

template<typename BASE>
bool ui::simple_button<BASE>::on_keydown(tagged_gui_object o, gui_manager & m, const key_down & k) {
	if (k.keycode == shortcut) {
		BASE::button_function(o, m);
		return true;
	} else {
		return false;
	}
}

template<typename BASE>
void ui::simple_button<BASE>::update_data(tagged_gui_object o, gui_manager& m) {
	if constexpr(ui::detail::has_update<BASE, tagged_gui_object, gui_manager&>) {
		BASE::update_data(o, m);
	}
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
	if(old_position != _position)
		BASE::on_position(_position);
}

template<typename BASE>
void ui::scrollbar<BASE>::update_data(tagged_gui_object o, gui_manager& m) {
	if constexpr(ui::detail::has_update<BASE, tagged_gui_object, gui_manager&>) {
		BASE::update_data(o, m);
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

			if ((scrollbar_definition.flags & scrollbar_def::has_range_limit) != 0) {
				const auto minimum_range = ui::detail::create_element_instance(manager, scrollbar_definition.minimum_limit_icon);
				ui::add_to_back(manager, scrollbar_obj, minimum_range);
				const auto maximum_range = ui::detail::create_element_instance(manager, scrollbar_definition.maximum_limit_icon);
				ui::add_to_back(manager, scrollbar_obj, maximum_range);

				ui::hide(minimum_range);
				ui::hide(maximum_range);
				b.minimum_limit_icon = &minimum_range.object;
				b.maximum_limit_icon = &maximum_range.object;
			}

			const auto slider = create_dynamic_button<scrollbar_slider<BEHAVIOR>>(manager, scrollbar_definition.slider, scrollbar_obj, b);
			b.slider = &slider.object;

			const auto left_button = create_dynamic_button<simple_button<scrollbar_left_button<BEHAVIOR>>>(manager, b.vertical ? scrollbar_definition.maximum_button : scrollbar_definition.minimum_button, scrollbar_obj, b);
			const auto right_button = create_dynamic_button<simple_button<scrollbar_right_button<BEHAVIOR>>>(manager, b.vertical ? scrollbar_definition.minimum_button : scrollbar_definition.maximum_button, scrollbar_obj, b);
			const auto track = create_dynamic_button<scrollbar_track<BEHAVIOR>>(manager, scrollbar_definition.track, scrollbar_obj, b);

			scrollbar_obj.object.size = b.vertical ? ui::xy_pair{ std::max(left_button.object.size.x, scrollbar_definition.size.x) , extent } : ui::xy_pair{ extent, std::max(left_button.object.size.y, scrollbar_definition.size.y)};
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

template<typename BEHAVIOR>
ui::tagged_gui_object ui::create_static_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, scrollbar<BEHAVIOR>& b) {
	const auto& scrollbar_definition = manager.ui_definitions.scrollbars[handle];
	
	const auto res = ui::detail::create_scrollbar_internal<BEHAVIOR>(manager, b, scrollbar_definition, parent, scrollbar_definition.position, std::max(scrollbar_definition.size.x, scrollbar_definition.size.y));
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

template<typename FILL_FUNCTION>
ui::tagged_gui_object ui::create_scrollable_region(gui_manager& manager, tagged_gui_object parent, ui::xy_pair position, int32_t height, int32_t step_size, graphics::obj_definition_tag bg, const FILL_FUNCTION& f) {
	const auto new_gobj = manager.gui_objects.emplace();
	new_gobj.object.position = position;

	const auto inner_area = f(manager);
	inner_area.object.position = ui::xy_pair{ 0,0 };
	new_gobj.object.size = ui::xy_pair{ inner_area.object.size.x + 16, height };

	ui::add_to_back(manager, new_gobj, inner_area);

	if (inner_area.object.size.y > height) {
		unmanaged_scrollable_region* new_sr = concurrent_allocator<unmanaged_scrollable_region>().allocate(1);
		new (new_sr)unmanaged_scrollable_region(inner_area.object);

		int32_t size_difference = inner_area.object.size.y - height;
		ui::create_static_fixed_sz_scrollbar(manager, manager.ui_definitions.standardlistbox_slider, new_gobj, ui::xy_pair{ inner_area.object.size.x, 0 }, height, new_sr->sb);
		new_sr->sb.set_range(manager, 0, size_difference);
		new_sr->sb.set_step(step_size);

		new_gobj.object.associated_behavior = new_sr;
	} else {
		new_gobj.object.flags.fetch_or(ui::gui_object::static_behavior, std::memory_order_acq_rel);
		new_gobj.object.associated_behavior = &global_visible_region;
	}

	ui::add_to_back(manager, parent, new_gobj);
	return new_gobj;
}

#include "concurrency_tools\\concurrency_tools.hpp"