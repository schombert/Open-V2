#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"
#include <algorithm>

ui::gui_manager::~gui_manager() {};

void ui::gui_manager::destroy(gui_object & g) {
	if (&(gui_objects.at(tooltip)) == &g) {
		hide_tooltip();
		tooltip = 0;
	}
	if (&(gui_objects.at(tooltip)) == &g)
		focus = 0;

	if (g.associated_behavior) {
		if ((g.flags.load(std::memory_order_relaxed) & g.static_behavior) == 0) {
			g.associated_behavior->~gui_behavior();
			concurrent_allocator<gui_behavior>().deallocate(g.associated_behavior, 0);
		}
		g.associated_behavior = nullptr;
	}
	const auto type_handle = g.type_dependant_handle.load(std::memory_order_relaxed);
	if (type_handle != 0) {
		const auto flags = g.flags.load(std::memory_order_relaxed);
		if ((flags & gui_object::type_mask) == gui_object::type_text_instance) {
			text_instances.free(type_handle - 1);
		} else if ((flags & gui_object::type_mask) == gui_object::type_graphics_object) {
			graphics_instances.free(type_handle - 1);
		} else if ((flags & gui_object::type_mask) == gui_object::type_masked_flag) {
			flag_instances.free(type_handle - 1);
		}
	}

	auto child = g.first_child.load(std::memory_order_relaxed);
	g.first_child.store(0, std::memory_order_release);

	while (child != 0) {
		auto next = gui_objects.at(child).right_sibling.load(std::memory_order_relaxed);
		gui_objects.free(child - 1, *this);
		child = next;
	}

	g.parent.store(0, std::memory_order_release);
	g.left_sibling.store(0, std::memory_order_release);
	g.right_sibling.store(0, std::memory_order_release);
	g.flags.store(0, std::memory_order_release);
	g.type_dependant_handle.store(0, std::memory_order_release);
}

namespace ui {
	text_data::alignment aligment_from_button_definition(const button_def& def) {
		switch (def.flags & button_def::orientation_mask) {
			case button_def::orientation_center:
				return text_data::alignment::center;
			case button_def::orientation_lower_left:
				return text_data::alignment::bottom_left;
			case button_def::orientation_lower_right:
				return text_data::alignment::bottom_right;
			case button_def::orientation_upper_left:
				return text_data::alignment::top_left;
			case button_def::orientation_upper_right:
				return text_data::alignment::top_right;
			default:
				return text_data::alignment::center;
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

void ui::gui_manager::create_simple_single_line_text(tagged_gui_object container, uint16_t text_handle, ui::text_color default_color, text_data::alignment align, uint32_t font_handle, uint32_t font_size) {
	const auto& components = text_data_sequences.all_sequences[text_handle];
	font& this_font = fonts.at(font_handle);

	const auto components_start = text_data_sequences.all_components.data() + components.starting_component;
	const auto components_end = components_start + components.component_count;

	float x_extent = 0.0f;
	const float y_extent = this_font.line_height(font_size);

	for (auto component_i = components_start; component_i != components_end; ++component_i) {
		if (std::holds_alternative<text_data::text_chunk>(*component_i)) {
			const auto chunk = std::get<text_data::text_chunk>(*component_i);
			x_extent += this_font.metrics_text_extent(text_data_sequences.text_data.data() + chunk.offset, chunk.length, font_size, false);
		}
	}

	int32_t y_offset = 0;
	if ((align == text_data::alignment::bottom_center) | (align == text_data::alignment::bottom_right) | (align == text_data::alignment::bottom_left)) {
		y_offset = container.object.size.y - int32_t(y_extent + 0.5f);
	} else if ((align == text_data::alignment::center) | (align == text_data::alignment::left) | (align == text_data::alignment::right)) {
		y_offset = (container.object.size.y - int32_t(y_extent + 0.5f)) / 2;
	}
	int32_t x_offset = 0;
	if ((align == text_data::alignment::right) | (align == text_data::alignment::bottom_right) | (align == text_data::alignment::top_right)) {
		x_offset = container.object.size.x - int32_t(x_extent + 0.5f);
	} else if ((align == text_data::alignment::center) | (align == text_data::alignment::bottom_center) | (align == text_data::alignment::top_center)) {
		x_offset = (container.object.size.x - int32_t(x_extent + 0.5f)) / 2;
	}

	ui::text_color current_color = default_color;
	for (auto component_i = components_start; component_i != components_end; ++component_i) {
		if (std::holds_alternative<text_data::color_change>(*component_i)) {
			current_color = text_color_to_ui_text_color(std::get<text_data::color_change>(*component_i).color);
		} else if (std::holds_alternative<text_data::text_chunk>(*component_i)) {
			const auto chunk = std::get<text_data::text_chunk>(*component_i);

			int32_t position_in_chunk = 0;

			while (position_in_chunk < chunk.length) {
				const auto new_gobj = gui_objects.emplace();

				const auto new_text_instance = text_instances.emplace();

				new_gobj.object.flags.store(gui_object::type_text_instance | gui_object::visible | gui_object::enabled, std::memory_order_release);
				new_gobj.object.type_dependant_handle.store(new_text_instance.id, std::memory_order_release);

				auto last_in_chunk = (int32_t)std::min(position_in_chunk + ui::text_instance::max_instance_length, (uint32_t)chunk.length);
				if (last_in_chunk != chunk.length) {
					for (; last_in_chunk > position_in_chunk; --last_in_chunk) {
						if (text_data_sequences.text_data[chunk.offset + last_in_chunk] == u' ')
							break;
					}
					if (last_in_chunk == position_in_chunk)
						last_in_chunk = (int32_t)std::min(position_in_chunk + ui::text_instance::max_instance_length, (uint32_t)chunk.length);
				}
				new_text_instance.object.color = current_color;
				new_text_instance.object.font_handle = font_handle;
				new_text_instance.object.size = font_size / 2;
				new_text_instance.object.length = last_in_chunk - position_in_chunk;
				memcpy(new_text_instance.object.text, text_data_sequences.text_data.data() + chunk.offset + position_in_chunk, (new_text_instance.object.length) * sizeof(char16_t));

				const int32_t new_size = 0.5f + this_font.metrics_text_extent(new_text_instance.object.text, new_text_instance.object.length, font_size, false);
				new_gobj.object.size = ui::xy_pair{ int16_t(new_size) , int16_t(y_extent + 0.5f) };
				new_gobj.object.position = ui::xy_pair{ x_offset, y_offset };

				add_to_back(container, new_gobj);

				x_offset += new_size;
				position_in_chunk += new_text_instance.object.length;
			}
			//...
			//x_extent += this_font.metrics_text_extent(text_data_sequences.text_data.data() + chunk.offset, chunk.length, font_size, false);
		}
	}
}

ui::tagged_gui_object ui::gui_manager::create_element_instance(uint16_t packed_handle) {
	const auto[element_t, handle] = ui::unpack_ui_definition_handle(packed_handle);
	switch (element_t) {
		case element_type::button:
		{
			const button_def& def = ui_definitions.buttons[handle];
			const auto new_gobj = gui_objects.emplace();

			const uint16_t rotation =
				(def.flags & button_def::rotation_90_left) ?
				gui_object::rotation_left :
				gui_object::rotation_upright;

			auto& button_graphic_def = graphics_object_definitions.definitions[def.graphical_object_handle - 1];

			new_gobj.object.flags.store(gui_object::type_graphics_object | gui_object::visible | gui_object::enabled | rotation, std::memory_order_release);
			new_gobj.object.position = def.position;
			new_gobj.object.size = def.size;

			if (button_graphic_def.primary_texture_handle != 0) {
				const auto button_graphic = graphics_instances.emplace();

				button_graphic.object.frame = 0;
				button_graphic.object.graphics_object = &button_graphic_def;
				button_graphic.object.t = &textures.retrieve_by_key(button_graphic_def.primary_texture_handle);

				new_gobj.object.type_dependant_handle.store(button_graphic.id, std::memory_order_release);
			}

			const auto [font_h, int_font_size] = unpack_font_handle(def.font_handle);
			create_simple_single_line_text(new_gobj, def.text_handle, ui::text_color::white, aligment_from_button_definition(def), font_h, int_font_size);

			return new_gobj;
		}
		default:
			return tagged_gui_object{root, 0};
	}
}

void ui::gui_manager::render_object_type(open_gl_wrapper& ogl, const gui_object& root_obj, ui::xy_pair position, uint32_t type, bool currently_enabled) {
	const float effective_position_x = static_cast<float>(position.x) + static_cast<float>(root_obj.position.x) * scale;
	const float effective_position_y = static_cast<float>(position.y) + static_cast<float>(root_obj.position.y) * scale;
	const float effective_width = static_cast<float>(root_obj.size.x) * scale;
	const float effective_height = static_cast<float>(root_obj.size.y) * scale;
	const auto current_rotation = root_obj.get_rotation();

	switch (type) {
		case ui::gui_object::type_barchart:
		{
			auto& dt = data_textures.at(root_obj.type_dependant_handle.load(std::memory_order_acquire));
			ogl.render_barchart(
				currently_enabled,
				effective_position_x,
				effective_position_y,
				effective_width,
				effective_height,
				dt,
				current_rotation);
			break;
		}
		case ui::gui_object::type_graphics_object:
		{
			auto& gi = graphics_instances.at(root_obj.type_dependant_handle.load(std::memory_order_acquire));
			if (gi.graphics_object->number_of_frames != 0) {
				ogl.render_subsprite(
					currently_enabled,
					gi.frame,
					gi.graphics_object->number_of_frames,
					effective_position_x,
					effective_position_y,
					effective_width,
					effective_height,
					*gi.t,
					current_rotation);
			} else {
				ogl.render_textured_rect(
					currently_enabled,
					effective_position_x,
					effective_position_y,
					effective_width,
					effective_height,
					*gi.t,
					current_rotation);
			}
			break;
		}
		case ui::gui_object::type_linegraph:
		{
			auto& l = lines_set.at(root_obj.type_dependant_handle.load(std::memory_order_acquire));
			ogl.render_linegraph(
				currently_enabled,
				effective_position_x,
				effective_position_y,
				effective_width,
				effective_height,
				l);
			break;
		}
		case ui::gui_object::type_masked_flag:
		{
			auto& m = flag_instances.at(root_obj.type_dependant_handle.load(std::memory_order_acquire));
			ogl.render_masked_rect(
				currently_enabled,
				effective_position_x,
				effective_position_y,
				effective_width,
				effective_height,
				*m.flag,
				*m.mask,
				current_rotation);
			ogl.render_textured_rect(
				currently_enabled,
				effective_position_x,
				effective_position_y,
				effective_width,
				effective_height,
				*m.overlay,
				current_rotation);
			break;
		}
		case ui::gui_object::type_piechart:
		{
			auto& dt = data_textures.at(root_obj.type_dependant_handle.load(std::memory_order_acquire));
			ogl.render_piechart(
				currently_enabled,
				effective_position_x,
				effective_position_y,
				effective_width,
				dt);
			break;
		}
		case ui::gui_object::type_text_instance:
		{
			auto& ti = text_instances.at(root_obj.type_dependant_handle.load(std::memory_order_acquire));
			auto& fnt = fonts.at(ti.font_handle);

			switch (ti.color) {
				case ui::text_color::black:
					ogl.render_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 0.0f, 0.0f, 0.0f }, fnt);
					break;
				case ui::text_color::green:
					ogl.render_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 0.0f, 0.623f, 0.01f }, fnt);
					break;
				case ui::text_color::outlined_black:
					ogl.render_outlined_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 0.0f, 0.0f, 0.0f }, fnt);
					break;
				case ui::text_color::outlined_white:
					ogl.render_outlined_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 1.0f, 1.0f, 1.0f }, fnt);
					break;
				case ui::text_color::red:
					ogl.render_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 1.0f, 0.2f, 0.2f }, fnt);
					break;
				case ui::text_color::white:
					ogl.render_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 1.0f, 1.0f, 1.0f }, fnt);
					break;
				case ui::text_color::yellow:
					ogl.render_text(ti.text, ti.size, currently_enabled, effective_position_x, effective_position_y, ti.size * 2 * scale, color{ 1.0f, 0.75f, 1.0f }, fnt);
					break;
			}
			break;
		}
	}
}

void ui::gui_manager::render_internal(open_gl_wrapper &ogl, const gui_object &root_obj, ui::xy_pair position, bool parent_enabled) {
	const auto type = root_obj.flags & ui::gui_object::type_mask;
	
	const float effective_position_x = static_cast<float>(position.x) + static_cast<float>(root_obj.position.x) * scale;
	const float effective_position_y = static_cast<float>(position.y) + static_cast<float>(root_obj.position.y) * scale;
	const bool currently_enabled = parent_enabled && ((root_obj.flags.load(std::memory_order_acquire) & ui::gui_object::enabled) != 0);

	render_object_type(ogl, root_obj, position, type, currently_enabled);
	
	const ui::xy_pair new_base_position{ int16_t(effective_position_x), int16_t(effective_position_y) };

	uint32_t current_child = root_obj.first_child.load(std::memory_order_acquire);
	while (current_child != 0) {
		const auto& child_object = gui_objects.at(current_child);
		render_internal(ogl, child_object, new_base_position, currently_enabled);
		current_child = child_object.right_sibling.load(std::memory_order_acquire);
	}
}

void ui::gui_manager::set_focus(tagged_gui_object g) {
	if (focus != g.id) {
		if (g.object.associated_behavior && g.object.associated_behavior->on_get_focus(g, *this)) {
			auto& with_focus = gui_objects.at(focus);
			if (with_focus.associated_behavior) {
				with_focus.associated_behavior->on_lose_focus(tagged_gui_object{ with_focus, focus }, *this);
			}
			focus = g.id;
		}
	}
}

void ui::gui_manager::clear_focus() {
	auto& with_focus = gui_objects.at(focus);
	if (with_focus.associated_behavior) {
		with_focus.associated_behavior->on_lose_focus(tagged_gui_object{ with_focus, focus }, *this);
		focus = 0;
	}
}

void ui::gui_manager::set_visible(tagged_gui_object g, bool visible) {
	if (visible)
		g.object.flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
	else
		g.object.flags.fetch_and(~ui::gui_object::visible, std::memory_order_acq_rel);
}

void ui::gui_manager::set_enabled(tagged_gui_object g, bool enabled) {
	if (enabled)
		g.object.flags.fetch_or(ui::gui_object::enabled, std::memory_order_acq_rel);
	else
		g.object.flags.fetch_and(~ui::gui_object::enabled, std::memory_order_acq_rel);
}

void ui::gui_manager::hide_tooltip() {
	set_visible(tagged_gui_object{ tooltip_window,3 }, false);
}

void ui::gui_manager::clear_children(tagged_gui_object g) {
	auto current_child = g.object.first_child.load(std::memory_order_acquire);
	g.object.first_child.store(0, std::memory_order_release);

	while (current_child != 0) {
		auto& child_object = gui_objects.at(current_child);
		const auto next_child = child_object.right_sibling.load(std::memory_order_acquire);

		gui_objects.free(current_child, *this);
		current_child = next_child;
	}
}

void ui::gui_manager::remove_object(tagged_gui_object g) {
	const auto parent_id = g.object.parent.load(std::memory_order_acquire);
	auto& parent_object = gui_objects.at(parent_id);

	const auto left_sibling = g.object.left_sibling.load(std::memory_order_acquire);
	const auto right_sibling = g.object.right_sibling.load(std::memory_order_acquire);

	if (left_sibling == 0)
		parent_object.first_child.store(right_sibling, std::memory_order_release);
	else
		gui_objects.at(left_sibling).right_sibling.store(right_sibling, std::memory_order_release);
	
	if (right_sibling != 0)
		gui_objects.at(right_sibling).left_sibling.store(left_sibling, std::memory_order_release);

	gui_objects.free(g.id, *this);
}

ui::tagged_gui_object ui::gui_manager::last_sibling_of(tagged_gui_object g) {
	uint16_t sib_child_id = g.id;
	gui_object* current = &g.object;
	while (true) {
		const auto next_id = current->right_sibling.load(std::memory_order_acquire);
		if (next_id == 0)
			return tagged_gui_object{*current, sib_child_id };
		sib_child_id = next_id;
		current = &(gui_objects.at(next_id));
	}
}

void ui::gui_manager::add_to_front(tagged_gui_object parent, tagged_gui_object child) {

	child.object.parent.store(parent.id, std::memory_order_release);
	const auto first_child_id = parent.object.first_child.load(std::memory_order_acquire);
	if (first_child_id == 0) {
		parent.object.first_child.store(child.id, std::memory_order_release);
	} else {
		const auto last_in_list = last_sibling_of(tagged_gui_object{ gui_objects.at(first_child_id), first_child_id });

		child.object.left_sibling.store(last_in_list.id, std::memory_order_release);
		last_in_list.object.right_sibling.store(child.id, std::memory_order_release);
	}
	
}

void ui::gui_manager::add_to_back(tagged_gui_object parent, tagged_gui_object child) {

	child.object.parent.store(parent.id, std::memory_order_release);
	const auto first_child_id = parent.object.first_child.load(std::memory_order_acquire);
	if (first_child_id != 0) {
		child.object.right_sibling.store(first_child_id, std::memory_order_release);
		auto& first_child_object = gui_objects.at(first_child_id);
		first_child_object.left_sibling.store(child.id, std::memory_order_release);
	}
	parent.object.first_child.store(child.id, std::memory_order_release);
}

void ui::gui_manager::move_to_front(tagged_gui_object g) {
	const auto left_sibling = g.object.left_sibling.load(std::memory_order_acquire);
	const auto right_sibling = g.object.right_sibling.load(std::memory_order_acquire);

	if (right_sibling == 0)
		return;

	const auto parent_id = g.object.parent.load(std::memory_order_acquire);
	auto& parent_object = gui_objects.at(parent_id);

	const auto last_in_list = last_sibling_of(g);

	if (left_sibling == 0)
		parent_object.first_child.store(right_sibling, std::memory_order_release);
	else
		gui_objects.at(left_sibling).right_sibling.store(right_sibling, std::memory_order_release);

	gui_objects.at(right_sibling).left_sibling.store(left_sibling, std::memory_order_release);

	g.object.left_sibling.store(last_in_list.id, std::memory_order_release);
	g.object.right_sibling.store(0, std::memory_order_release);
	last_in_list.object.right_sibling.store(g.id, std::memory_order_release);
}

void ui::gui_manager::move_to_back(tagged_gui_object g) {
	const auto left_sibling = g.object.left_sibling.load(std::memory_order_acquire);
	const auto right_sibling = g.object.right_sibling.load(std::memory_order_acquire);

	if (left_sibling == 0)
		return;

	const auto parent_id = g.object.parent.load(std::memory_order_acquire);
	auto& parent_object = gui_objects.at(parent_id);

	gui_objects.at(left_sibling).right_sibling.store(right_sibling, std::memory_order_release);
	if (right_sibling != 0)
		gui_objects.at(right_sibling).left_sibling.store(left_sibling, std::memory_order_release);

	g.object.left_sibling.store(0, std::memory_order_release);
	g.object.right_sibling.store(parent_object.first_child.load(std::memory_order_acquire), std::memory_order_release);
	parent_object.first_child.store(g.id, std::memory_order_release);
}

bool ui::gui_manager::on_lbutton_down(const lbutton_down& ld) {
	return dispatch_message_internal([_this = this](ui::tagged_gui_object obj, const lbutton_down& l) {
		_this->set_focus(obj);
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_lclick(obj, *_this, l);
		return false;
	}, tagged_gui_object{ root,0 }, ld);
}

bool ui::gui_manager::on_rbutton_down(const rbutton_down& rd) {
	return dispatch_message_internal([_this = this](ui::tagged_gui_object obj, const rbutton_down& r) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_rclick(obj, *_this, r);
		return false;
	}, tagged_gui_object{ root,0 }, rd);
}

bool ui::gui_manager::on_mouse_move(const mouse_move& mm) {
	const bool found_tooltip = dispatch_message_internal([_this = this](ui::tagged_gui_object obj, const mouse_move& m) {
		if (_this->tooltip != obj.id) {
			if (obj.object.associated_behavior && obj.object.associated_behavior->has_tooltip(obj, *_this)) {
				_this->tooltip = obj.id;
				_this->clear_children(ui::tagged_gui_object{ _this->tooltip_window, 3 });
				obj.object.associated_behavior->create_tooltip(obj, *_this, ui::tagged_gui_object{ _this->tooltip_window, 3 });
				_this->set_visible(ui::tagged_gui_object{ _this->tooltip_window, 3 }, true);
				return true;
			}
		}
		return false;
	}, tagged_gui_object{ root,0 }, mm);

	if (!found_tooltip && tooltip) {
		tooltip = 0;
		set_visible(ui::tagged_gui_object{ tooltip_window, 3 }, false);
	}

	return found_tooltip;
}

bool ui::gui_manager::on_mouse_drag(const mouse_drag& md) {
	if (gui_objects.at(focus).associated_behavior) {
		auto scaled_drag(md);
		scaled_drag.x /= scale;
		scaled_drag.y /= scale;
		return gui_objects.at(focus).associated_behavior->on_drag(ui::tagged_gui_object{ gui_objects.at(focus), focus }, *this, scaled_drag);
	} else
		return false;
}

bool ui::gui_manager::on_keydown(const key_down& kd) {
	return dispatch_message_internal([_this = this](ui::tagged_gui_object obj, const key_down& k) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_keydown(obj, *_this, k);
		return false;
	}, tagged_gui_object{ root,0 }, kd);
}

bool ui::gui_manager::on_scroll(const scroll& se) {
	return dispatch_message_internal([_this = this](ui::tagged_gui_object obj, const scroll& s) {
		if (obj.object.associated_behavior)
			return obj.object.associated_behavior->on_scroll(obj, *_this, s);
		return false;
	}, tagged_gui_object{ root,0 }, se);
}

bool ui::gui_manager::on_text(const text_event &te) {
	if (gui_objects.at(focus).associated_behavior)
		return gui_objects.at(focus).associated_behavior->on_text(ui::tagged_gui_object{ gui_objects.at(focus), focus }, *this, te);
	else
		return false;
}

ui::gui_manager::gui_manager(int32_t width, int32_t height) :
	root(gui_objects.emplace_at(0)),
	background(gui_objects.emplace_at(1)),
	foreground(gui_objects.emplace_at(2)),
	tooltip_window(gui_objects.emplace_at(3)) {

	on_resize(resize{ width , height });
}

void ui::gui_manager::on_resize(const resize& r) {
	ui::xy_pair new_size{ static_cast<float>(r.width) / scale, static_cast<float>(r.height) / scale };

	root.size = new_size;
	foreground.size = new_size;
	background.size = new_size;
}

void ui::gui_manager::rescale(float new_scale) {
	const resize new_size{ static_cast<float>(root.size.x) * scale, static_cast<float>(root.size.y) * scale };
	scale = new_scale;
	on_resize(new_size);
}

void ui::gui_manager::render(open_gl_wrapper& ogl) {
	render_internal(ogl, background, ui::xy_pair{ 0, 0 }, true);
	render_internal(ogl, root, ui::xy_pair{ 0, 0 }, true);
	render_internal(ogl, foreground, ui::xy_pair{ 0, 0 }, true);
}

const rotation ui::gui_object::get_rotation() const {
	const auto rotation_bits = flags.load(std::memory_order_acquire) & ui::gui_object::rotation_mask;
	if (rotation_bits == ui::gui_object::rotation_left)
		return rotation::left;
	else if (rotation_bits == ui::gui_object::rotation_right)
		return rotation::right;
	else
		return rotation::upright;
}
