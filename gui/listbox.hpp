#pragma once
#include "gui.hpp"

template<typename BASE, typename ELEMENT>
bool ui::display_listbox<BASE, ELEMENT>::on_scroll(gui_object_tag, gui_manager&, const scroll&) {

}

template<typename BASE, typename ELEMENT>
void ui::display_listbox<BASE, ELEMENT>::update_data(gui_object_tag, gui_manager&, world_state&) {

}

template<typename B, typename ELEMENT>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, listbox_tag handle, tagged_gui_object parent, ui::display_listbox<B, ELEMENT>& b) {

}