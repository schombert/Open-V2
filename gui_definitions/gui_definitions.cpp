#include "gui_definitions.h"

namespace ui {
	ui::xy_pair definitions::get_size(element_tag t) const {
		return visit(t, [](const auto& def) {
			if constexpr(detail::has_size<decltype(def)>)
				return def.size;
			else
				return ui::xy_pair{ 0,0 };
		});
	}

	const char* format_error(ui::errors  e) {
		switch (e) {
			case ui::errors::expected_tooltip_empty_for_button:
				return "expected empty tooltip for button";
			case ui::errors::unexpected_button_clicksound:
				return "unexpected clicksound for button";
			case ui::errors::expected_tooltiptext_empty_for_button:
				return "expected empty tooltip text for button";
			case ui::errors::expected_delayedtooltiptext_empty_for_button:
				return "expected empty delayed tooltip text for button";
			case ui::errors::expected_button_format_to_be_left:
				return "expected button to have left format";
			case ui::errors::unknown_button_orientation:
				return "unknown button orientation";
			case ui::errors::unknown_button_shortcut:
				return "unknown shortcut for button";
			case ui::errors::unexpected_button_rotation:
				return "invalid rotation value for button";
			case ui::errors::unexpected_button_attribute:
				return "unexpected attribute for button";
			case ui::errors::unknown_icon_orientation:
				return "unknown icon orientation";
			case ui::errors::unexpected_icon_rotation:
				return "invalid rotation for icon";
			case ui::errors::unexpected_icon_attribute:
				return "unexpected attribute for icon";
			case ui::errors::unknown_text_orientation:
				return "unknown text orientation";
			case ui::errors::unknown_text_format:
				return "unknown text format";
			case ui::errors::unexpected_text_attribute:
				return "unexpected attribute for text type element";
			case ui::errors::unexpected_text_background:
				return "unknown background for text type element";
			case ui::errors::unexpected_position_attribute:
				return "unexpected attribute for position type element";
			case ui::errors::unknown_overlapping_region_format:
				return "unknown format for overlapping region";
			case ui::errors::unknown_overlapping_region_orientation:
				return "unknown orientation for overlapping region";
			case ui::errors::unexpected_overlapping_region_attribute:
				return "unexpected attribute for overlapping region";
			case ui::errors::unsupported_listbox_scrollbar_type:
				return "only the standard scrollbar is supported for listboxes";
			case ui::errors::unexpected_listbox_step_value:
				return "nonzero step value specified for listbox";
			case ui::errors::unexpected_listbox_spacing_value:
				return "spacing for listbox out of range";
			case ui::errors::unexpected_listbox_priority:
				return "priority other than 100 specified for listbox";
			case ui::errors::unknown_listbox_orientation:
				return "unknown orientation for listbox";
			case ui::errors::horizontal_listboxes_not_supported:
				return "horizontal listboxes are not supported";
			case ui::errors::unexpected_listbox_attribute:
				return "unexpected attribute for listbox";
			case ui::errors::unexpected_scrollbar_priority:
				return "priority other than 100 specified for scrollbar";
			case ui::errors::unexpected_scrollbar_minimum_value:
				return "minimum value other than 0 specified for scrollbar";
			case ui::errors::unexpected_scrollbar_horizontal_value:
				return "horizontal value other than 0 or 1 specified for scrollbar";
			case ui::errors::unexpected_scrollbar_step_size:
				return "step size for scrollbar not 2, 1, 0.1, 0.01, or 0.001";
			case ui::errors::scrollbar_component_not_found:
				return "scrollbar referenced a component it did not define";
			case ui::errors::missing_necessary_scrollbar_component:
				return "one of the necessary components for a scrollbar was undefined";
			case ui::errors::unexpected_scrollbar_attribute:
				return "unexpected attribute for scrollbar";
			case ui::errors::unexpected_window_attribute:
				return "unexpected attribute for window";
			case ui::errors::unknown_window_orientation:
				return "unknown orientation for window";
			case ui::errors::window_background_not_found:
				return "window referenced a background it did not define";
			case ui::errors::unexpected_window_moveable_value:
				return "moveable value other than 0 or 1 specified for window";
			case ui::errors::unknown_definition_type:
				return "unknown definition type found at top scope";
			case ui::errors::missing_standardlistbox_slider:
				return "no definition found for standardlistbox_slider";
		}
	}
}
