#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"

ui::gui_manager::~gui_manager() {};

void ui::gui_manager::destroy(gui_object & g) {
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

void ui::gui_manager::render_object_type(open_gl_wrapper& ogl, const gui_object& root_obj, ui::xy_pair position, uint32_t type) {
	const float effective_position_x = static_cast<float>(position.x) + static_cast<float>(root_obj.position.x) * scale;
	const float effective_position_y = static_cast<float>(position.y) + static_cast<float>(root_obj.position.y) * scale;
	const float effective_width = static_cast<float>(root_obj.size.x) * scale;
	const float effective_height = static_cast<float>(root_obj.size.y) * scale;
	const auto current_rotation = root_obj.get_rotation();
	const bool currently_enabled = (root_obj.flags.load(std::memory_order_acquire) & ui::gui_object::enabled) != 0;

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

void ui::gui_manager::render_internal(open_gl_wrapper &ogl, const gui_object &root_obj, ui::xy_pair position) {
	const auto type = root_obj.flags & ui::gui_object::type_mask;
	
	const float effective_position_x = static_cast<float>(position.x) + static_cast<float>(root_obj.position.x) * scale;
	const float effective_position_y = static_cast<float>(position.y) + static_cast<float>(root_obj.position.y) * scale;
	render_object_type(ogl, root_obj, position, type);
	
	const ui::xy_pair new_base_position{ int16_t(effective_position_x), int16_t(effective_position_y) };

	uint32_t current_child = root_obj.first_child.load(std::memory_order_acquire);
	while (current_child != 0) {
		const auto& child_object = gui_objects.at(current_child);
		render_internal(ogl, child_object, new_base_position);
		current_child = child_object.right_sibling.load(std::memory_order_acquire);
	}
}


bool ui::gui_manager::on_lbutton_down(const lbutton_down& ld) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const lbutton_down& l) {
		if (obj.associated_behavior)
			return obj.associated_behavior->on_lclick(obj, *_this, l);
		return false;
	}, root, ld);
}

bool ui::gui_manager::on_rbutton_down(const rbutton_down& rd) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const rbutton_down& r) {
		if (obj.associated_behavior)
			return obj.associated_behavior->on_rclick(obj, *_this, r);
		return false;
	}, root, rd);
}

bool ui::gui_manager::on_mouse_move(const mouse_move& mm) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const mouse_move& m) {
		//if (obj.associated_behavior)
		//	return obj.associated_behavior->on_mouse_move(obj, *_this, m);
		return false;
	}, root, mm);
}

bool ui::gui_manager::on_mouse_drag(const mouse_drag& md) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const mouse_drag& m) {
		if (obj.associated_behavior)
			return obj.associated_behavior->on_drag(obj, *_this, m);
		return false;
	}, root, md);
}

bool ui::gui_manager::on_keydown(const key_down& kd) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const key_down& k) {
		if (obj.associated_behavior)
			return obj.associated_behavior->on_keydown(obj, *_this, k);
		return false;
	}, root, kd);
}

bool ui::gui_manager::on_scroll(const scroll& se) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const scroll& s) {
		if (obj.associated_behavior)
			return obj.associated_behavior->on_scroll(obj, *_this, s);
		return false;
	}, root, se);
}

bool ui::gui_manager::on_text(const text_event &te) {
	return dispatch_message_internal([_this = this](ui::gui_object& obj, const text_event& t) {
		if (obj.associated_behavior)
			return obj.associated_behavior->on_text(obj, *_this, t);
		return false;
	}, root, te);
}

void ui::gui_manager::render(open_gl_wrapper& ogl) {
	render_internal(ogl, background, ui::xy_pair{ 0, 0 });
	render_internal(ogl, root, ui::xy_pair{ 0, 0 });
	render_internal(ogl, foreground, ui::xy_pair{ 0, 0 });
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
