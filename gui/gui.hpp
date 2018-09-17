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
		struct _has_update<A, decltype(void(std::declval<A>().update(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_update = _has_update<A, void, C ...>::value;

		template<typename A, typename B, typename ... C>
		struct _has_create_tooltip : std::false_type {};
		template<typename A, typename ... C>
		struct _has_create_tooltip<A, decltype(void(std::declval<A>().create_tooltip(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_create_tooltip = _has_create_tooltip<A, void, C ...>::value;

		template<typename A, typename B, typename ... C>
		struct _has_button_function : std::false_type {};
		template<typename A, typename ... C>
		struct _has_button_function<A, decltype(void(std::declval<A>().button_function(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_button_function = _has_button_function<A, void, C ...>::value;

		template<typename A, typename B, typename ... C>
		struct _has_on_position : std::false_type {};
		template<typename A, typename ... C>
		struct _has_on_position<A, decltype(void(std::declval<A>().on_position(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_on_position = _has_on_position<A, void, C ...>::value;

		template<typename A, typename B, typename ... C>
		struct _has_has_tooltip : std::false_type {};
		template<typename A, typename ... C>
		struct _has_has_tooltip<A, decltype(void(std::declval<A>().has_tooltip(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_has_tooltip = _has_has_tooltip<A, void, C ...>::value;
	}

	class unmanaged_scrollable_region : public visible_region {
	public:
		scrollbar<unmanaged_region_scollbar> sb;

		unmanaged_scrollable_region(gui_object& g);
		virtual bool on_scroll(gui_object_tag o, world_state& m, const scroll& s) override;
	};

	namespace detail {
		template<typename LM, typename BH = null_behavior_creation>
		std::pair<ui::xy_pair, uint32_t> text_chunk_to_single_instance(gui_static& static_manager, ui::gui_manager& container, vector_backed_string<char16_t> text_source, uint32_t offset_in_chunk, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM&& lm, const BH& behavior_creator = null_behavior_creation()) {
			graphics::font& this_font = static_manager.fonts.at(fmt.font_handle);

			const auto new_gobj = container.gui_objects.emplace();
			const auto new_text_instance = ui::create_text_instance(container, new_gobj, fmt);

			new_text_instance.object.length = (uint8_t)std::min(ui::text_instance::max_instance_length, (uint32_t)(text_source.length()) - offset_in_chunk);
			memcpy(new_text_instance.object.text, text_source.get_str(static_manager.text_data_sequences.text_data) + offset_in_chunk, (new_text_instance.object.length) * sizeof(char16_t));

			if (new_text_instance.object.length == ui::text_instance::max_instance_length
				&& new_text_instance.object.text[ui::text_instance::max_instance_length - 1] != u' ')
				shorten_text_instance_to_space(new_text_instance.object);


			const auto original_length = new_text_instance.object.length;
			auto previous_length = new_text_instance.object.length;
			float new_size = this_font.metrics_text_extent(
				new_text_instance.object.text,
				new_text_instance.object.length,
				ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size)),
				is_outlined_color(fmt.color));

			while (lm.exceeds_extent(position.x + int32_t(new_size + 0.5f))) {
				shorten_text_instance_to_space(new_text_instance.object);

				if (new_text_instance.object.length == previous_length) {
					if (position.x != 0) {
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

			behavior_creator(new_gobj);

			add_to_back(container, parent_object, new_gobj);
			lm.add_object(&(new_gobj.object));

			return std::make_pair(ui::xy_pair{ static_cast<int16_t>(position.x + new_gobj.object.size.x), position.y }, offset_in_chunk + new_text_instance.object.length);
		}
	}
}

template<typename LM, typename BH>
ui::xy_pair ui::text_chunk_to_instances(gui_static& static_manager, ui::gui_manager& manager, vector_backed_string<char16_t> text_source, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM&& lm, const BH& behavior_creator) {
	int32_t position_in_chunk = 0;

	const auto chunk_size = text_source.length();
	while (position_in_chunk < chunk_size) {
		std::tie(position, position_in_chunk) = detail::text_chunk_to_single_instance(static_manager, manager, text_source, static_cast<uint32_t>(position_in_chunk), parent_object, position, fmt, lm, behavior_creator);
	}
	return position;
}

template<typename LM>
ui::xy_pair ui::add_linear_text(ui::xy_pair position, text_data::text_tag text_handle, text_format const& fmt, gui_static& static_manager, gui_manager& manager, tagged_gui_object container, LM&& lm, const text_data::replacement* candidates, uint32_t count) {
	auto& components = static_manager.text_data_sequences.all_sequences[text_handle];
	graphics::font& this_font = static_manager.fonts.at(fmt.font_handle);

	const auto components_start = static_manager.text_data_sequences.all_components.data() + components.starting_component;
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

			const auto replacement_text = rep ? std::get<1>(*rep) : vector_backed_string<char16_t>(text_data::name_from_value_type(std::get<text_data::value_placeholder>(*component_i).value));

			const text_format format{ current_color, fmt.font_handle, fmt.font_size };

			if(rep)
				position = text_chunk_to_instances(static_manager, manager, replacement_text, container, position, format, lm, std::get<2>(*rep));
			else
				position = text_chunk_to_instances(static_manager, manager, replacement_text, container, position, format, lm);

		} else if(std::holds_alternative<text_data::text_chunk>(*component_i)) {
			const auto chunk = std::get<text_data::text_chunk>(*component_i);
			const text_format format{ current_color, fmt.font_handle, fmt.font_size };

			position = text_chunk_to_instances(static_manager, manager, chunk, container, position, format, lm);
		} else if(std::holds_alternative<text_data::line_break>(*component_i)) {
			lm.finish_current_line();
			position.x = 0;
			position.y += int16_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(fmt.font_size))) + 0.5f);
		}
	}

	return position;
}

template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::detail::dispatch_message(const gui_manager& manager, const MESSAGE_FUNCTION &member_f, ui::tagged_gui_object obj, ui::xy_pair container_size, const MESSAGE_TYPE& message) {
	const auto actual_position = ui::detail::position_with_alignment(container_size, obj.object.position, obj.object.align);

	auto relocated_message = adjust_message_location(message, -actual_position.x, -actual_position.y);
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible) == 0)
		return false;
	
	if (message_within_bounds(relocated_message, obj.object.size.x, obj.object.size.y)) {
	
		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			const gui_object_tag child = obj.object.first_child;
			if (is_valid_index(child)) {
				gui_object_tag current_child = detail::last_sibling_of(manager, tagged_gui_object{ manager.gui_objects.at(child), child }).id;
				do {
					auto& current_obj = manager.gui_objects.at(current_child);
					const gui_object_tag next_index = current_obj.left_sibling;

					if (dispatch_message(manager, member_f, tagged_gui_object{ current_obj, current_child }, obj.object.size, relocated_message))
						return true;

					current_child = next_index;
				} while (is_valid_index(current_child));
			}
		}

		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			if constexpr(std::is_base_of_v<message_with_location, MESSAGE_TYPE>) {
				if ((object_flags & ui::gui_object::type_mask) == ui::gui_object::type_graphics_object) {
					const auto& gobj = manager.graphics_instances.at(graphics_instance_tag(obj.object.type_dependant_handle.load(std::memory_order_acquire)));
					if (((object_flags & ui::gui_object::force_transparency_check) != 0) | ((gobj.graphics_object->flags & graphics::object::do_transparency_check) != 0)) {
						graphics::color_rgba pixel;

						if (gobj.graphics_object->number_of_frames > 1) {
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

		template<typename A, typename B, typename ... C>
		struct _has_initialize_in_window : std::false_type {};
		template<typename A, typename ... C>
		struct _has_initialize_in_window<A, decltype(void(std::declval<A>().initialize_in_window(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_initialize_in_window = _has_initialize_in_window<A, void, C ...>::value;

		template<typename OBJ, typename RET, typename ... PARAMS>
		struct has_windowed_update_s : public std::false_type {};
		template<typename OBJ, typename ... PARAMS>
		struct has_windowed_update_s<OBJ, decltype(void(std::declval<OBJ>().windowed_update(std::declval<PARAMS>() ...))), PARAMS ...> : public std::true_type {};
		template<typename OBJ, typename ... PARAMS>
		constexpr bool has_windowed_update = has_windowed_update_s<OBJ, void, PARAMS ...>::value;

		template<typename OBJ, typename RET, typename ... PARAMS>
		struct has_on_create_s : public std::false_type {};
		template<typename OBJ, typename ... PARAMS>
		struct has_on_create_s<OBJ, decltype(void(std::declval<OBJ>().on_create(std::declval<PARAMS>() ...))), PARAMS ...> : public std::true_type {};
		template<typename OBJ, typename ... PARAMS>
		constexpr bool has_on_create = has_on_create_s<OBJ, void, PARAMS ...>::value;

		template<typename RES, typename HANDLE>
		struct can_create_instance_s : public std::false_type {};
		template<typename HANDLE>
		struct can_create_instance_s<decltype(create_element_instance(std::declval<gui_static&>(), std::declval<gui_manager&>(), std::declval<HANDLE>())), HANDLE> : public std::true_type {};
		template<typename HANDLE>
		constexpr bool can_create_instance = can_create_instance_s<tagged_gui_object, HANDLE>::value;

		template<typename T>
		ui::tagged_gui_object safe_create_element_instance(gui_static& static_manager, gui_manager& manager, T handle) {
			if constexpr(can_create_instance<T>)
				return ui::detail::create_element_instance(static_manager, manager, handle);
			else
				return manager.gui_objects.emplace();
		}

		template<typename RES, typename HANDLE, typename TYPE>
		struct can_create_static_s : public std::false_type {};
		template<typename HANDLE, typename TYPE>
		struct can_create_static_s<decltype(create_static_element(std::declval<gui_static&>(), std::declval<gui_manager&>(), std::declval<HANDLE>(), std::declval<tagged_gui_object>(), std::declval<TYPE&>())), HANDLE, TYPE> : public std::true_type {};
		template<typename HANDLE, typename TYPE>
		constexpr bool can_create_static = can_create_static_s<tagged_gui_object, HANDLE, TYPE>::value;
	}
}

template<typename BEHAVIOR, typename T, typename ... PARAMS>
ui::tagged_gui_object ui::create_dynamic_element(world_state& ws, T handle, tagged_gui_object parent, PARAMS&& ... params) {
	if constexpr(!std::is_same_v<BEHAVIOR, gui_behavior>) {
		BEHAVIOR* b = concurrent_allocator<BEHAVIOR>().allocate(1);
		new (b)BEHAVIOR(std::forward<PARAMS>(params) ...);

		if constexpr(detail::can_create_static<T, BEHAVIOR>) {
			const auto new_obj = create_static_element(ws, handle, parent, *b);
			new_obj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
			return new_obj;
		} else {
			auto new_obj = ui::detail::safe_create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);
			new_obj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);

			new_obj.object.associated_behavior = b;
			b->associated_object = &new_obj.object;

			ui::add_to_back(ws.w.gui_m, parent, new_obj);
			return new_obj;
		}
	} else {
		auto new_obj = ui::detail::safe_create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);
		ui::add_to_back(ws.w.gui_m, parent, new_obj);
		return new_obj;
	}
}

template<typename FILL_FUNCTION>
ui::tagged_gui_object ui::create_scrollable_region(world_state& ws, tagged_gui_object parent, ui::xy_pair position, int32_t height, int32_t step_size, graphics::obj_definition_tag , const FILL_FUNCTION& f) {
	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
	new_gobj.object.position = position;

	const auto inner_area = f(ws.s.gui_m, ws.w.gui_m);
	inner_area.object.position = ui::xy_pair{ 0,0 };
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(inner_area.object.size.x + 16), static_cast<int16_t>(height) };

	ui::add_to_back(ws.w.gui_m, new_gobj, inner_area);

	if (inner_area.object.size.y > height) {
		unmanaged_scrollable_region* new_sr = concurrent_allocator<unmanaged_scrollable_region>().allocate(1);
		new (new_sr)unmanaged_scrollable_region(inner_area.object);
		

		int32_t size_difference = inner_area.object.size.y - height;
		ui::create_static_fixed_sz_scrollbar(ws, ws.s.gui_m.ui_definitions.standardlistbox_slider, new_gobj, ui::xy_pair{ inner_area.object.size.x, 0 }, height, new_sr->sb);
		new_sr->sb.set_range(ws.w.gui_m, 0, size_difference);
		new_sr->sb.set_step(step_size);

		new_gobj.object.associated_behavior = new_sr;
		new_sr->associated_object = &new_gobj.object;
		new_gobj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
	} else {
		ui::visible_region* new_vr = concurrent_allocator<ui::visible_region>().allocate(1);
		new (new_vr)ui::visible_region();

		new_gobj.object.associated_behavior = new_vr;
		new_vr->associated_object = &new_gobj.object;
		new_gobj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
	}

	ui::add_to_back(ws.w.gui_m, parent, new_gobj);
	return new_gobj;
}

namespace ui {
	namespace detail {
		template<typename RESULT, typename tag_type, typename object_type>
		struct can_create_s : public std::false_type {};
		template<typename tag_type, typename object_type>
		struct can_create_s<decltype(void(ui::create_static_element(std::declval<world_state&>(), tag_type(), std::declval<tagged_gui_object>(), std::declval<object_type&>()))), tag_type, object_type> : public std::true_type {};
		
		template<typename tag_type, typename object_type>
		constexpr bool can_create = can_create_s<void, tag_type, object_type>::value;
	}
}

#include "buttons.hpp"
#include "scrollbar.hpp"
#include "piechart.hpp"
#include "gui_window.hpp"
#include "text_box.hpp"
#include "listbox.hpp"
#include "icons.hpp"
#include "progress_bar.hpp"
#include "linechart.hpp"
#include "concurrency_tools\\concurrency_tools.hpp"
