#pragma once
#include "gui\\gui.h"

namespace ui {
	template<typename BASE>
	void progress_bar<BASE>::set_visibility(gui_manager& m, bool visible) {
		if(visible)
			make_visible_and_update(m, *associated_object);
		else
			hide(*associated_object);
	}
	template<typename BASE>
	void progress_bar<BASE>::set_fraction(float fraction) {
		underlying_obj->progress = fraction;
	}
	template<typename BASE>
	void progress_bar<BASE>::get_fraction() {
		return underlying_obj->progress;
	}
	template<typename BASE>
	void progress_bar<BASE>::set_underlying_obj(multi_texture_instance* o) {
		underlying_obj = o;
	}
	template<typename BASE>
	void progress_bar<BASE>::update_data(gui_object_tag, world_state& w) {
		if constexpr(ui::detail::has_update<BASE, progress_bar<BASE>&, world_state&>) {
			BASE::update(*this, w);
		}
	}
	template<typename BASE>
	template<typename window_type>
	void progress_bar<BASE>::windowed_update(window_type& w, world_state& s) {
		if constexpr(ui::detail::has_windowed_update<BASE, progress_bar<BASE>&, window_type&, world_state&>) {
			BASE::windowed_update(*this, w, s);
		}
	}
	template<typename BASE>
	tooltip_behavior progress_bar<BASE>::has_tooltip(gui_object_tag, world_state& ws, const mouse_move&) {
		if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
			return BASE::has_tooltip(ws) ? tooltip_behavior::tooltip : tooltip_behavior::no_tooltip;
		else
			return tooltip_behavior::no_tooltip;
	}
	template<typename BASE>
	void progress_bar<BASE>::create_tooltip(gui_object_tag, world_state& ws, const mouse_move&, tagged_gui_object tw) {
		if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
			BASE::create_tooltip(*this, ws, tw);
	}
	template<typename B>
	tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, progress_bar<B>& b) {
		auto new_obj = ui::detail::create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);

		new_obj.object.associated_behavior = &b;
		b.associated_object = &new_obj.object;

		b.set_underlying_obj(ws.w.gui_m.multi_texture_instances.safe_at(ui::multi_texture_instance_tag(new_obj.object.type_dependant_handle.load(std::memory_order_acquire))));
	
		ui::add_to_back(ws.w.gui_m, parent, new_obj);
		ws.w.gui_m.flag_minimal_update();
		return new_obj;
	}
}
