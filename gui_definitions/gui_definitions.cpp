#include "gui_definitions.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include <string>
#include <vector>
#include <set>
#include <optional>

struct empty_type {

};

struct parsing_environment {
	ui::name_maps& nmaps;
	ui::definitions& defs;
	std::vector<std::pair<std::string, ui::errors>>& errors_generated;
	const text_handle_lookup th_f;
	const font_handle_lookup fh_f;
	const gobj_lookup gl_f;
	const sound_lookup sl_f;
	std::string file;

	parsing_environment(
		ui::name_maps& a,
		ui::definitions& b,
		std::vector<std::pair<std::string, ui::errors>>& c,
		const text_handle_lookup& d,
		const font_handle_lookup& e,
		const gobj_lookup& f,
		const sound_lookup& h) : 
	nmaps(a), defs(b), errors_generated(c), th_f(d), fh_f(e), gl_f(f), sl_f(h) {}
};

int discard_empty_type(const token_and_type& a, association_type, empty_type&) {
	return 0;
}

MEMBER_DEF(ui::xy_pair, x, "x");
MEMBER_DEF(ui::xy_pair, y, "y");

token_and_type token_from_rh(association_type, const token_and_type& t) {
	return t;
}

std::optional<virtual_key> virtual_key_from_rh(association_type, const token_and_type& t) {
	if (t.start == t.end)
		return virtual_key::NONE;
	if (t.start + 1 == t.end) {
		if ((*t.start >= 'a') & (*t.start <= 'z')) {
			return virtual_key((uint8_t)virtual_key::A + (*t.start - 'a'));
		} else if ((*t.start >= 'A') & (*t.start <= 'Z')) {
			return virtual_key((uint8_t)virtual_key::A + (*t.start - 'A'));
		} else if ((*t.start >= '0') & (*t.start <= '9')) {
			return virtual_key((uint8_t)virtual_key::NUM_0 + (*t.start - '0'));
		} else if (*t.start == ':') {
			return virtual_key::SEMICOLON;
		} else if (*t.start == ';') {
			return virtual_key::SEMICOLON;
		} else if (*t.start == '+') {
			return virtual_key::PLUS;
		} else if (*t.start == '=') {
			return virtual_key::PLUS;
		} else if (*t.start == '<') {
			return virtual_key::COMMA;
		} else if (*t.start == ',') {
			return virtual_key::COMMA;
		} else if (*t.start == '-') {
			return virtual_key::MINUS;
		} else if (*t.start == '_') {
			return virtual_key::MINUS;
		} else if (*t.start == '.') {
			return virtual_key::PERIOD;
		} else if (*t.start == '>') {
			return virtual_key::PERIOD;
		} else if (*t.start == '?') {
			return virtual_key::FORWARD_SLASH;
		} else if (*t.start == '/') {
			return virtual_key::FORWARD_SLASH;
		} else if (*t.start == '~') {
			return virtual_key::TILDA;
		} else if (*t.start == '`') {
			return virtual_key::TILDA;
		} else if (*t.start == '{') {
			return virtual_key::OPEN_BRACKET;
		} else if (*t.start == '[') {
			return virtual_key::OPEN_BRACKET;
		} else if (*t.start == '|') {
			return virtual_key::BACK_SLASH;
		} else if (*t.start == '\\') {
			return virtual_key::BACK_SLASH;
		} else if (*t.start == '}') {
			return virtual_key::CLOSED_BRACKET;
		} else if (*t.start == ']') {
			return virtual_key::CLOSED_BRACKET;
		} else if (*t.start == '\'') {
			return virtual_key::QUOTE;
		} else if (*t.start == '\"') {
			return virtual_key::QUOTE;
		}
	}

	if (is_fixed_token_ci(t, "none")) {
		return virtual_key::NONE;
	} else if (is_fixed_token_ci(t, "backspace")) {
		return virtual_key::BACK;
	} else if (is_fixed_token_ci(t, "back")) {
		return virtual_key::BACK;
	} else if (is_fixed_token_ci(t, "tab")) {
		return virtual_key::TAB;
	} else if (is_fixed_token_ci(t, "return")) {
		return virtual_key::RETURN;
	} else if (is_fixed_token_ci(t, "enter")) {
		return virtual_key::RETURN;
	} else if (is_fixed_token_ci(t, "esc")) {
		return virtual_key::ESCAPE;
	} else if (is_fixed_token_ci(t, "escape")) {
		return virtual_key::ESCAPE;
	} else if (is_fixed_token_ci(t, "space")) {
		return virtual_key::SPACE;
	} else if (is_fixed_token_ci(t, "page_up")) {
		return virtual_key::PRIOR;
	} else if (is_fixed_token_ci(t, "page_down")) {
		return virtual_key::NEXT;
	} else if (is_fixed_token_ci(t, "end")) {
		return virtual_key::END;
	} else if (is_fixed_token_ci(t, "home")) {
		return virtual_key::HOME;
	} else if (is_fixed_token_ci(t, "left")) {
		return virtual_key::LEFT;
	} else if (is_fixed_token_ci(t, "up")) {
		return virtual_key::UP;
	} else if (is_fixed_token_ci(t, "right")) {
		return virtual_key::RIGHT;
	} else if (is_fixed_token_ci(t, "down")) {
		return virtual_key::DOWN;
	} else if (is_fixed_token_ci(t, "select")) {
		return virtual_key::SELECT;
	} else if (is_fixed_token_ci(t, "insert")) {
		return virtual_key::INSERT;
	} else if (is_fixed_token_ci(t, "delete")) {
		return virtual_key::DELETE_KEY;
	} else if (is_fixed_token_ci(t, "del")) {
		return virtual_key::DELETE_KEY;
	} else if (is_fixed_token_ci(t, "f1")) {
		return virtual_key::F1;
	} else if (is_fixed_token_ci(t, "f2")) {
		return virtual_key::F2;
	} else if (is_fixed_token_ci(t, "f3")) {
		return virtual_key::F3;
	} else if (is_fixed_token_ci(t, "f4")) {
		return virtual_key::F4;
	} else if (is_fixed_token_ci(t, "f5")) {
		return virtual_key::F5;
	} else if (is_fixed_token_ci(t, "f6")) {
		return virtual_key::F6;
	} else if (is_fixed_token_ci(t, "f7")) {
		return virtual_key::F7;
	} else if (is_fixed_token_ci(t, "f8")) {
		return virtual_key::F8;
	} else if (is_fixed_token_ci(t, "f9")) {
		return virtual_key::F9;
	} else if (is_fixed_token_ci(t, "f10")) {
		return virtual_key::F10;
	} else if (is_fixed_token_ci(t, "f11")) {
		return virtual_key::F11;
	} else if (is_fixed_token_ci(t, "f12")) {
		return virtual_key::F12;
	} else if (is_fixed_token_ci(t, "f13")) {
		return virtual_key::F13;
	} else if (is_fixed_token_ci(t, "f14")) {
		return virtual_key::F14;
	} else if (is_fixed_token_ci(t, "f15")) {
		return virtual_key::F15;
	} else if (is_fixed_token_ci(t, "f16")) {
		return virtual_key::F16;
	} else if (is_fixed_token_ci(t, "f17")) {
		return virtual_key::F17;
	} else if (is_fixed_token_ci(t, "f18")) {
		return virtual_key::F18;
	} else if (is_fixed_token_ci(t, "f19")) {
		return virtual_key::F19;
	} else if (is_fixed_token_ci(t, "f20")) {
		return virtual_key::F20;
	} else if (is_fixed_token_ci(t, "f21")) {
		return virtual_key::F21;
	} else if (is_fixed_token_ci(t, "f22")) {
		return virtual_key::F22;
	} else if (is_fixed_token_ci(t, "f23")) {
		return virtual_key::F23;
	} else if (is_fixed_token_ci(t, "f24")) {
		return virtual_key::F24;
	} else if (is_fixed_token_ci(t, "numpad0")) {
		return virtual_key::NUMPAD0;
	} else if (is_fixed_token_ci(t, "numpad1")) {
		return virtual_key::NUMPAD1;
	} else if (is_fixed_token_ci(t, "numpad2")) {
		return virtual_key::NUMPAD2;
	} else if (is_fixed_token_ci(t, "numpad3")) {
		return virtual_key::NUMPAD3;
	} else if (is_fixed_token_ci(t, "numpad4")) {
		return virtual_key::NUMPAD4;
	} else if (is_fixed_token_ci(t, "numpad5")) {
		return virtual_key::NUMPAD5;
	} else if (is_fixed_token_ci(t, "numpad6")) {
		return virtual_key::NUMPAD6;
	} else if (is_fixed_token_ci(t, "numpad7")) {
		return virtual_key::NUMPAD7;
	} else if (is_fixed_token_ci(t, "numpad8")) {
		return virtual_key::NUMPAD8;
	} else if (is_fixed_token_ci(t, "numpad9")) {
		return virtual_key::NUMPAD9;
	} else if (is_fixed_token_ci(t, "multiply")) {
		return virtual_key::MULTIPLY;
	} else if (is_fixed_token_ci(t, "add")) {
		return virtual_key::ADD;
	} else if (is_fixed_token_ci(t, "separator")) {
		return virtual_key::SEPARATOR;
	} else if (is_fixed_token_ci(t, "subtract")) {
		return virtual_key::SUBTRACT;
	} else if (is_fixed_token_ci(t, "decimal")) {
		return virtual_key::DECIMAL;
	} else if (is_fixed_token_ci(t, "divide")) {
		return virtual_key::DIVIDE;
	}

	return std::optional<virtual_key>();
}

std::optional<uint8_t> button_orientation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "center")) {
		return ui::button_def::orientation_center;
	} else if (is_fixed_token_ci(t, "lower_left")) {
		return ui::button_def::orientation_lower_left;
	} else if (is_fixed_token_ci(t, "lower_right")) {
		return ui::button_def::orientation_lower_right;
	} else if (is_fixed_token_ci(t, "upper_left")) {
		return ui::button_def::orientation_upper_left;
	} else if (is_fixed_token_ci(t, "upper_right")) {
		return ui::button_def::orientation_upper_right;
	}
	return std::optional<uint8_t>();
}

std::optional<uint8_t> button_format_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "left")) {
		return ui::button_def::format_left;
	} 
	return std::optional<uint8_t>();
}

std::optional<uint8_t> button_rotation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "-1.5708")) {
		return ui::button_def::rotation_90_left;
	} else if (is_fixed_token_ci(t, "1.5708")) {
		return ui::button_def::rotation_90_right;
	} else if (token_to<double>(t) == 0.0) {
		return ui::button_def::rotation_upright;
	}
	return std::optional<uint8_t>();
}

struct guiButtonType {
	ui::button_def internal_definition;
	const parsing_environment& env;

	guiButtonType(const parsing_environment& e) : env(e) {};

	std::string name;

	void buttonText(const token_and_type& t) {
		internal_definition.text_handle = env.th_f(t.start, t.end);
	}
	void clicksound(const token_and_type& t) {
		internal_definition.clicksound_handle = env.sl_f(t.start, t.end);
	}
	void quadTextureSprite(const token_and_type& t) {
		internal_definition.graphical_object_handle = env.gl_f(t.start, t.end);
	}
	void spriteType(const token_and_type& t) {
		internal_definition.flags |= ui::button_def::graphical_obj_sprite_type;
		internal_definition.graphical_object_handle = env.gl_f(t.start, t.end);
	}
	void buttonFont(const token_and_type& t) {
		internal_definition.font_handle = env.fh_f(t.start, t.end);
	}
	void tooltip(const token_and_type& t) {
		if (t.start != t.end) 
			env.errors_generated.emplace_back(env.file, ui::errors::expected_tooltip_empty_for_button);
	}
	void tooltipText(const token_and_type& t) {
		if (t.start != t.end)
			env.errors_generated.emplace_back(env.file,  ui::errors::expected_tooltiptext_empty_for_button);
	}
	void delayedtooltipText(const token_and_type& t) {
		if (t.start != t.end)
			env.errors_generated.emplace_back(env.file,  ui::errors::expected_delayedtooltiptext_empty_for_button);
	}
	void orientation_bits(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_button_orientation);
	}
	void format_bits(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file,  ui::errors::expected_button_format_to_be_left);
	}
	void rotation_bits(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_button_rotation);
	}
	void shortcut(std::optional<virtual_key> f) {
		if (f)
			internal_definition.shortcut = *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_button_shortcut);
	}
	void discard_member(int) {
	}
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_button_attribute);
	};
};


MEMBER_DEF(guiButtonType, name, "name");
MEMBER_FDEF(guiButtonType, quadTextureSprite, "quadTextureSprite");
MEMBER_FDEF(guiButtonType, buttonText, "buttonText");
MEMBER_FDEF(guiButtonType, buttonFont, "buttonFont");
MEMBER_FDEF(guiButtonType, orientation_bits, "Orientation");
MEMBER_FDEF(guiButtonType, clicksound, "clicksound");
MEMBER_FDEF(guiButtonType, delayedtooltipText, "delayedtooltipText");
MEMBER_FDEF(guiButtonType, format_bits, "format");
MEMBER_DEF(guiButtonType, internal_definition.position, "position");
MEMBER_FDEF(guiButtonType, rotation_bits, "rotation");
MEMBER_FDEF(guiButtonType, shortcut, "shortcut");
MEMBER_DEF(guiButtonType, internal_definition.size, "size");
MEMBER_FDEF(guiButtonType, spriteType, "spriteType");
MEMBER_FDEF(guiButtonType, tooltip, "tooltip");
MEMBER_FDEF(guiButtonType, tooltipText, "tooltipText");
MEMBER_FDEF(guiButtonType, discard_member, "parent");
MEMBER_FDEF(guiButtonType, add_unknown_key, "unknown_key");

std::optional<uint8_t> icon_orientation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "center")) {
		return ui::icon_def::orientation_center;
	} else if (is_fixed_token_ci(t, "lower_left")) {
		return ui::icon_def::orientation_lower_left;
	} else if (is_fixed_token_ci(t, "lower_right")) {
		return ui::icon_def::orientation_lower_right;
	} else if (is_fixed_token_ci(t, "upper_left") || is_fixed_token_ci(t, "upperl_left")) {
		return ui::icon_def::orientation_upper_left;
	} else if (is_fixed_token_ci(t, "upper_right")) {
		return ui::icon_def::orientation_upper_right;
	} else if (is_fixed_token_ci(t, "center_up")) {
		return ui::icon_def::orientation_center_up;
	} else if (is_fixed_token_ci(t, "center_down")) {
		return ui::icon_def::orientation_center_down;
	}
	return std::optional<uint8_t>();
}

std::optional<uint8_t> icon_rotation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "-1.5708")) {
		return ui::icon_def::rotation_90_left;
	} else if (is_fixed_token_ci(t, "1.5708")) {
		return ui::icon_def::rotation_90_right;
	} else if (token_to<double>(t) == 0.0) {
		return ui::icon_def::rotation_upright;
	}
	return std::optional<uint8_t>();
}

struct iconType {
	ui::icon_def internal_definition;
	const parsing_environment& env;

	iconType(const parsing_environment& e) : env(e) {};

	std::string name;

	void orientation_bits(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_icon_orientation);
	}
	void rotation_bits(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_icon_rotation);
	}
	void gobj_handle(const token_and_type& t) {
		internal_definition.graphical_object_handle = env.gl_f(t.start, t.end);
	}
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_icon_attribute);
	};
};

MEMBER_FDEF(iconType, orientation_bits, "orientation");
MEMBER_FDEF(iconType, gobj_handle, "buttonMesh");
MEMBER_DEF(iconType, internal_definition.frame, "frame");
MEMBER_DEF(iconType, name, "name");
MEMBER_DEF(iconType, internal_definition.position, "position");
MEMBER_FDEF(iconType, rotation_bits, "rotation");
MEMBER_DEF(iconType, internal_definition.scale, "scale");
MEMBER_FDEF(iconType, gobj_handle, "spriteType");
MEMBER_FDEF(iconType, add_unknown_key, "unknown_key");

std::optional<uint16_t> textbox_orientation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "center")) {
		return ui::text_def::orientation_center;
	} else if (is_fixed_token_ci(t, "lower_left")) {
		return ui::text_def::orientation_lower_left;
	} else if (is_fixed_token_ci(t, "lower_right")) {
		return ui::text_def::orientation_lower_right;
	} else if (is_fixed_token_ci(t, "upper_left")) {
		return ui::text_def::orientation_upper_left;
	} else if (is_fixed_token_ci(t, "upper_right")) {
		return ui::text_def::orientation_upper_right;
	} else if (is_fixed_token_ci(t, "center_up")) {
		return ui::text_def::orientation_center_up;
	} else if (is_fixed_token_ci(t, "center_down")) {
		return ui::text_def::orientation_center_down;
	}
	return std::optional<uint16_t>();
}

std::optional<uint16_t> textbox_format_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "centre") || is_fixed_token_ci(t, "center")) {
		return ui::text_def::format_center;
	} else if (is_fixed_token_ci(t, "left")) {
		return ui::text_def::format_left;
	} else if (is_fixed_token_ci(t, "right")) {
		return ui::text_def::format_right;
	} else if (is_fixed_token_ci(t, "justified")) {
		return ui::text_def::format_justified;
	}
	return std::optional<uint16_t>();
}

std::optional<uint16_t> textbox_background_from_rh(association_type, const token_and_type& t) {
	if (t.start == t.end) {
		return ui::text_def::background_none_specified;
	} else if (is_fixed_token_ci(t, "gfx\\\\interface\\\\tiles_dialog.tga")) {
		return ui::text_def::background_tiles_dialog_tga;
	} else if (is_fixed_token_ci(t, "gfx\\\\interface\\\\transparency.tga")) {
		return ui::text_def::background_transparency_tga;
	} else if (is_fixed_token_ci(t, "gfx\\\\interface\\\\small_tiles_dialog.dds")) {
		return ui::text_def::background_small_tiles_dialog_tga;
	}
	return std::optional<uint16_t>();
}

struct allTextBoxType {
	ui::text_def internal_definition;
	const parsing_environment& env;

	allTextBoxType(const parsing_environment& e) : env(e) {};

	std::string name;

	void orientation(std::optional<uint16_t> i) {
		if (i)
			internal_definition.flags |= *i;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_text_orientation);
	}
	void size(const ui::xy_pair& s) {
		internal_definition.max_width = s.x;
		internal_definition.max_height = s.y;
	}
	void format(std::optional<uint16_t> i) {
		if (i)
			internal_definition.flags |= *i;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_text_format);
	}
	void background(std::optional<uint16_t> i) {
		if (i)
			internal_definition.flags |= *i;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_text_background);
	}
	void text(const token_and_type& t) {
		internal_definition.text_handle = env.th_f(t.start, t.end);
	}
	void font(const token_and_type& t) {
		internal_definition.font_handle = env.fh_f(t.start, t.end);
	}
	void fixed_size(bool value) {
		if (value)
			internal_definition.flags |= ui::text_def::fixed_size;
	}
	void always_transparent(bool value) {
		if (value)
			internal_definition.flags |= ui::text_def::always_transparent;
	}
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_text_attribute);
	};
};

MEMBER_FDEF(allTextBoxType, orientation, "orientation");
MEMBER_FDEF(allTextBoxType, always_transparent, "alwaysTransparent");
MEMBER_DEF(allTextBoxType, internal_definition.border_size, "borderSize");
MEMBER_FDEF(allTextBoxType, fixed_size, "fixedSize");
MEMBER_FDEF(allTextBoxType, font, "font");
MEMBER_FDEF(allTextBoxType, format, "format");
MEMBER_FDEF(allTextBoxType, size, "size");
MEMBER_DEF(allTextBoxType, internal_definition.max_height, "maxHeight");
MEMBER_DEF(allTextBoxType, internal_definition.max_width, "maxWidth");
MEMBER_DEF(allTextBoxType, name, "name");
MEMBER_DEF(allTextBoxType, internal_definition.position, "position");
MEMBER_FDEF(allTextBoxType, text, "text");
MEMBER_FDEF(allTextBoxType, background, "textureFile");
MEMBER_FDEF(allTextBoxType, add_unknown_key, "unknown_key");


std::optional<uint8_t> listbox_orientation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "center")) {
		return ui::listbox_def::orientation_center;
	} else if (is_fixed_token_ci(t, "upper_left")) {
		return ui::listbox_def::orientation_upper_left;
	} else if (is_fixed_token_ci(t, "upper_right")) {
		return ui::listbox_def::orientation_upper_right;
	} else if (is_fixed_token_ci(t, "center_down")) {
		return ui::listbox_def::orientation_center_down;
	}
	return std::optional<uint8_t>();
}

struct listBoxType {
	ui::listbox_def internal_definition;
	const parsing_environment& env;

	listBoxType(const parsing_environment& e) : env(e) {};

	std::string name;

	void orientation(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_listbox_orientation);
	}
	void background(const token_and_type& t) {
		internal_definition.background_handle = env.gl_f(t.start, t.end);
	}
	void step(int v) {
		if(v != 0)
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_listbox_step_value);
	}
	void scrollbar(const token_and_type& t) {
		if(!is_fixed_token_ci(t, "standardlistbox_slider"))
			env.errors_generated.emplace_back(env.file, ui::errors::unsupported_listbox_scrollbar_type);
	}
	void priority(int v) {
		if (v != 100)
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_listbox_priority);
	}
	void spacing(int v) {
		if (v < 0 || v > 0x0F)
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_listbox_spacing_value);
		else
			internal_definition.flags |= (uint8_t)(v & 0x0F);
	}
	void horizontal(bool value) {
		if (value)
			env.errors_generated.emplace_back(env.file, ui::errors::horizontal_listboxes_not_supported);
	}
	void always_transparent(bool value) {
		if (value)
			internal_definition.flags |= ui::listbox_def::always_transparent;
	}
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_listbox_attribute);
	};
};

MEMBER_FDEF(listBoxType, orientation, "orientation");
MEMBER_FDEF(listBoxType, always_transparent, "alwaystransparent");
MEMBER_FDEF(listBoxType, background, "background");
MEMBER_FDEF(listBoxType, horizontal, "horizontal");
MEMBER_DEF(listBoxType, name, "name");
MEMBER_FDEF(listBoxType, priority, "priority");
MEMBER_FDEF(listBoxType, scrollbar, "scrollbartype");
MEMBER_FDEF(listBoxType, spacing, "spacing");
MEMBER_FDEF(listBoxType, step, "step");
MEMBER_DEF(listBoxType, internal_definition.size, "size");
MEMBER_DEF(listBoxType, internal_definition.border_size, "bordersize");
MEMBER_DEF(listBoxType, internal_definition.position, "position");
MEMBER_DEF(listBoxType, internal_definition.offset, "offset");
MEMBER_FDEF(listBoxType, add_unknown_key, "unknown_key");

struct positionType {
	ui::position_def internal_definition;
	const parsing_environment& env;

	positionType(const parsing_environment& e) : env(e) {};

	std::string name;
	
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_position_attribute);
	};
};

MEMBER_DEF(positionType, name, "name");
MEMBER_DEF(positionType, internal_definition.position, "position");
MEMBER_FDEF(positionType, add_unknown_key, "unknown_key");

struct scrollbarType {
	ui::scrollbar_def internal_definition;
	const parsing_environment& env;

	scrollbarType(const parsing_environment& e) : env(e) {};

	std::string leftbutton;
	std::string name;
	std::string rangelimitmaxicon;
	std::string rangelimitminicon;
	std::string rightbutton;
	std::string slider;
	std::string track;

	std::vector<uint16_t> buttons_added;
	std::vector<uint16_t> icons_added;

	void finalize() {
		if (leftbutton.length() == 0) {
			env.errors_generated.emplace_back(env.file, ui::errors::missing_necessary_scrollbar_component);
		} else {
			for (auto i : buttons_added) {
				if (env.nmaps.button_names[i - 1] == leftbutton) {
					internal_definition.minimum_button = i;
					break;
				}
			}
			if (internal_definition.minimum_button == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::scrollbar_component_not_found);
			}
		}

		if (rightbutton.length() == 0) {
			env.errors_generated.emplace_back(env.file, ui::errors::missing_necessary_scrollbar_component);
		} else {
			for (auto i : buttons_added) {
				if (env.nmaps.button_names[i - 1] == rightbutton) {
					internal_definition.maximum_button = i;
					break;
				}
			}
			if (internal_definition.maximum_button == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::scrollbar_component_not_found);
			}
		}

		if (rangelimitmaxicon.length() != 0) {
			for (auto i : icons_added) {
				if (env.nmaps.icon_names[i - 1] == rangelimitmaxicon) {
					internal_definition.maximum_limit_icon = i;
					break;
				}
			}
			if (internal_definition.maximum_limit_icon == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::scrollbar_component_not_found);
			}
		}

		if (rangelimitminicon.length() != 0) {
			for (auto i : icons_added) {
				if (env.nmaps.icon_names[i - 1] == rangelimitminicon) {
					internal_definition.minimum_limit_icon = i;
					break;
				}
			}
			if (internal_definition.minimum_limit_icon == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::scrollbar_component_not_found);
			}
		}

		if (track.length() == 0) {
			env.errors_generated.emplace_back(env.file, ui::errors::missing_necessary_scrollbar_component);
		} else {
			for (auto i : buttons_added) {
				if (env.nmaps.button_names[i - 1] == track) {
					internal_definition.track = i;
					break;
				}
			}
			if (internal_definition.track == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::scrollbar_component_not_found);
			}
		}

		if (slider.length() == 0) {
			env.errors_generated.emplace_back(env.file, ui::errors::missing_necessary_scrollbar_component);
		} else {
			for (auto i : buttons_added) {
				if (env.nmaps.button_names[i - 1] == slider) {
					internal_definition.slider = i;
					break;
				}
			}
			if (internal_definition.slider == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::scrollbar_component_not_found);
			}
		}
	}

	void gui_button(const guiButtonType& b) {
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
		buttons_added.push_back((uint16_t)env.defs.buttons.size());
	}
	void gui_iconType(const iconType& b) {
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
		icons_added.push_back((uint16_t)env.defs.icons.size());
	}
	void step_size(const token_and_type& t) {
		if (is_fixed_token_ci(t, "1")) {
			internal_definition.flags |= ui::scrollbar_def::step_one;
		} else if (is_fixed_token_ci(t, "2")) {
			internal_definition.flags |= ui::scrollbar_def::step_two;
		} else if (is_fixed_token_ci(t, "0.1")) {
			internal_definition.flags |= ui::scrollbar_def::step_one_tenth;
		} else if (is_fixed_token_ci(t, "0.01")) {
			internal_definition.flags |= ui::scrollbar_def::step_one_hundredth;
		} else if (is_fixed_token_ci(t, "0.001")) {
			internal_definition.flags |= ui::scrollbar_def::step_one_thousandth;
		} else {
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_scrollbar_step_size);
		}
	}
	void userangelimit(bool use) {
		if (use) {
			internal_definition.flags |= ui::scrollbar_def::has_range_limit;
		}
	}
	void lockable(bool l) {
		if (l) {
			internal_definition.flags |= ui::scrollbar_def::is_lockable;
		}
	}
	void priority(int v) {
		if (v != 100)
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_scrollbar_priority);
	}
	void horizontal(int v) {
		if (v == 1)
			internal_definition.flags |= ui::scrollbar_def::is_horizontal;
		else if (v == 0)
			; //do nothing
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_scrollbar_horizontal_value);
	}
	void minvalue(int v) {
		if (v != 0)
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_scrollbar_minimum_value);
	}

	void ignore_value(int) {}
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_scrollbar_attribute);
	};
};

MEMBER_FDEF(scrollbarType, horizontal, "horizontal");
MEMBER_DEF(scrollbarType, leftbutton, "leftbutton");
MEMBER_FDEF(scrollbarType, lockable, "lockable");
MEMBER_DEF(scrollbarType, internal_definition.max_value, "maxvalue");
MEMBER_FDEF(scrollbarType, minvalue, "minvalue");
MEMBER_DEF(scrollbarType, name, "name");
MEMBER_FDEF(scrollbarType, priority, "priority");
MEMBER_FDEF(scrollbarType, ignore_value, "rangelimitmax");
MEMBER_FDEF(scrollbarType, ignore_value, "rangelimitmin");
MEMBER_DEF(scrollbarType, rangelimitmaxicon, "rangelimitmaxicon");
MEMBER_DEF(scrollbarType, rangelimitminicon, "rangelimitminicon");
MEMBER_DEF(scrollbarType, rightbutton, "rightbutton");
MEMBER_DEF(scrollbarType, slider, "slider");
MEMBER_FDEF(scrollbarType, ignore_value, "startvalue");
MEMBER_FDEF(scrollbarType, step_size, "stepsize");
MEMBER_DEF(scrollbarType, track, "track");
MEMBER_FDEF(scrollbarType, userangelimit, "userangelimit");
MEMBER_DEF(scrollbarType, internal_definition.border_size, "bordersize");
MEMBER_DEF(scrollbarType, internal_definition.position, "position");
MEMBER_DEF(scrollbarType, internal_definition.size, "size");
MEMBER_FDEF(scrollbarType, gui_button, "guiButtonType");
MEMBER_FDEF(scrollbarType, gui_iconType, "iconType");
MEMBER_FDEF(scrollbarType, add_unknown_key, "unknown_key");

std::optional<uint8_t> overlapping_orientation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "center")) {
		return ui::overlapping_region_def::orientation_center;
	} else if (is_fixed_token_ci(t, "upper_left")) {
		return ui::overlapping_region_def::orientation_upper_left;
	} else if (is_fixed_token_ci(t, "upper_right")) {
		return ui::overlapping_region_def::orientation_upper_right;
	}
	return std::optional<uint8_t>();
}

std::optional<uint16_t> overlapping_format_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "centre") || is_fixed_token_ci(t, "center")) {
		return ui::overlapping_region_def::format_center;
	} else if (is_fixed_token_ci(t, "left")) {
		return ui::overlapping_region_def::format_left;
	} else if (is_fixed_token_ci(t, "right")) {
		return ui::overlapping_region_def::format_right;
	}
	return std::optional<uint16_t>();
}

struct OverlappingElementsBoxType {
	ui::overlapping_region_def internal_definition;
	const parsing_environment& env;

	OverlappingElementsBoxType(const parsing_environment& e) : env(e) {};

	std::string name;

	void orientation(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_overlapping_region_orientation);
	}
	void format(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_overlapping_region_format);
	}

	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_overlapping_region_attribute);
	};
};

MEMBER_FDEF(OverlappingElementsBoxType, orientation, "orientation");
MEMBER_FDEF(OverlappingElementsBoxType, format, "format");
MEMBER_DEF(OverlappingElementsBoxType, name, "name");
MEMBER_DEF(OverlappingElementsBoxType, internal_definition.spacing, "spacing");
MEMBER_DEF(OverlappingElementsBoxType, internal_definition.position, "position");
MEMBER_DEF(OverlappingElementsBoxType, internal_definition.size, "size");
MEMBER_FDEF(OverlappingElementsBoxType, add_unknown_key, "unknown_key");

std::optional<uint8_t> window_orientation_from_rh(association_type, const token_and_type& t) {
	if (is_fixed_token_ci(t, "center")) {
		return ui::window_def::orientation_center;
	} else if (is_fixed_token_ci(t, "upper_left")) {
		return ui::window_def::orientation_upper_left;
	} else if (is_fixed_token_ci(t, "upper_right")) {
		return ui::window_def::orientation_upper_right;
	} else if (is_fixed_token_ci(t, "lower_left")) {
		return ui::window_def::orientation_lower_left;
	} else if (is_fixed_token_ci(t, "lower_right")) {
		return ui::window_def::orientation_lower_right;
	}
	return std::optional<uint8_t>();
}

struct windowType {
	ui::window_def internal_definition;
	const parsing_environment& env;

	windowType(const parsing_environment& e) : env(e) {};

	std::string background;
	std::string name;

	void finalize() {
		if (background.length() != 0) {
			for (auto i : internal_definition.sub_object_definitions) {
				auto [type, handle] = ui::unpack_ui_definition_handle(i);
				if (type == ui::element_type::button && env.nmaps.button_names[handle - 1] == background) {
					internal_definition.background_handle = handle;
					break;
				}
			}
			if (internal_definition.background_handle == 0) {
				env.errors_generated.emplace_back(env.file, ui::errors::window_background_not_found);
			}
		}
	}
	void fullscreen(bool v) {
		if(v)
			internal_definition.flags |= ui::window_def::is_fullscreen;
	}
	void moveable(int v) {
		if (v == 1)
			internal_definition.flags |= ui::window_def::is_moveable;
		else if (v == 0)
			; // do nothing
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unexpected_window_moveable_value);
	}
	void orientation(std::optional<uint8_t> f) {
		if (f)
			internal_definition.flags |= *f;
		else
			env.errors_generated.emplace_back(env.file, ui::errors::unknown_window_orientation);
	}
	void discard_value(int) {
	}

	void gui_button(const guiButtonType& b) {
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::button,(uint16_t)env.defs.buttons.size()));
	}
	void gui_iconType(const iconType& b) {
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::icon, (uint16_t)env.defs.icons.size()));
	}
	void gui_instantTextBoxType(allTextBoxType&& b) {
		b.internal_definition.flags |= ui::text_def::instant;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::text, (uint16_t)env.defs.text.size()));
	}
	void gui_instantTextBoxType(allTextBoxType& b) {
		b.internal_definition.flags |= ui::text_def::instant;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::text, (uint16_t)env.defs.text.size()));
	}
	void gui_textBoxType(const allTextBoxType& b) {
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::text, (uint16_t)env.defs.text.size()));
	}
	void gui_checkboxType(guiButtonType&& b) {
		b.internal_definition.flags |= ui::button_def::is_checkbox;
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::button, (uint16_t)env.defs.buttons.size()));
	}
	void gui_checkboxType(guiButtonType& b) {
		b.internal_definition.flags |= ui::button_def::is_checkbox;
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::button, (uint16_t)env.defs.buttons.size()));
	}
	void gui_shieldtype(iconType&& b) {
		b.internal_definition.flags |= ui::icon_def::is_shield;
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::icon, (uint16_t)env.defs.icons.size()));
	}
	void gui_shieldtype(iconType& b) {
		b.internal_definition.flags |= ui::icon_def::is_shield;
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::icon, (uint16_t)env.defs.icons.size()));
	}
	void gui_editBoxType(allTextBoxType&& b) {
		b.internal_definition.flags |= ui::text_def::is_edit_box;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::text, (uint16_t)env.defs.text.size()));
	}
	void gui_editBoxType(allTextBoxType& b) {
		b.internal_definition.flags |= ui::text_def::is_edit_box;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::text, (uint16_t)env.defs.text.size()));
	}
	void gui_OverlappingElementsBoxType(const OverlappingElementsBoxType& b) {
		env.defs.overlapping_regions.emplace_back(b.internal_definition);
		env.nmaps.overlapping_region_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::overlapping_region, (uint16_t)env.defs.overlapping_regions.size()));
	}
	void gui_listBoxType(const listBoxType& b) {
		env.defs.listboxes.emplace_back(b.internal_definition);
		env.nmaps.listbox_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::listbox, (uint16_t)env.defs.listboxes.size()));
	}
	void gui_scrollbarType(scrollbarType&& b) {
		b.finalize();
		env.defs.scrollbars.emplace_back(b.internal_definition);
		env.nmaps.scrollbar_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::scrollbar, (uint16_t)env.defs.scrollbars.size()));
	}
	void gui_scrollbarType(scrollbarType& b) {
		b.finalize();
		env.defs.scrollbars.emplace_back(b.internal_definition);
		env.nmaps.scrollbar_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::scrollbar, (uint16_t)env.defs.scrollbars.size()));
	}
	void gui_eu3dialogtype(windowType&& b) {
		b.finalize();
		b.internal_definition.flags |= ui::window_def::is_dialog;
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::window, (uint16_t)env.defs.windows.size()));
	}
	void gui_eu3dialogtype(windowType& b) {
		b.finalize();
		b.internal_definition.flags |= ui::window_def::is_dialog;
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::window, (uint16_t)env.defs.windows.size()));
	}
	void gui_windowType(windowType&& b) {
		b.finalize();
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::window, (uint16_t)env.defs.windows.size()));
	}
	void gui_windowType(windowType& b) {
		b.finalize();
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
		internal_definition.sub_object_definitions.push_back(ui::pack_ui_definition_handle(ui::element_type::window, (uint16_t)env.defs.windows.size()));
	}

	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unexpected_window_attribute);
	}
};


MEMBER_FDEF(windowType, orientation, "orientation");
MEMBER_DEF(windowType, background, "background");
MEMBER_FDEF(windowType, discard_value, "dontrender");
MEMBER_FDEF(windowType, discard_value, "downsound");
MEMBER_FDEF(windowType, discard_value, "upsound");
MEMBER_FDEF(windowType, fullscreen, "fullscreen");
MEMBER_FDEF(windowType, discard_value, "horizontalborder");
MEMBER_FDEF(windowType, discard_value, "verticalborder");
MEMBER_FDEF(windowType, moveable, "moveable");
MEMBER_DEF(windowType, name, "name");
MEMBER_DEF(windowType, internal_definition.position, "position");
MEMBER_DEF(windowType, internal_definition.size, "size");
MEMBER_FDEF(windowType, gui_button, "guiButtonType");
MEMBER_FDEF(windowType, gui_iconType, "iconType");
MEMBER_FDEF(windowType, gui_instantTextBoxType, "instantTextBoxType");
MEMBER_FDEF(windowType, gui_listBoxType, "listBoxType");
MEMBER_FDEF(windowType, gui_scrollbarType, "scrollbarType");
MEMBER_FDEF(windowType, gui_windowType, "windowType");
MEMBER_FDEF(windowType, gui_checkboxType, "checkboxType");
MEMBER_FDEF(windowType, gui_OverlappingElementsBoxType, "OverlappingElementsBoxType");
MEMBER_FDEF(windowType, gui_editBoxType, "editBoxType");
MEMBER_FDEF(windowType, gui_textBoxType, "textBoxType");
MEMBER_FDEF(windowType, gui_shieldtype, "shieldtype");
MEMBER_FDEF(windowType, add_unknown_key, "unknown_key");
MEMBER_FDEF(windowType, gui_eu3dialogtype, "eu3dialogtype");

struct gui_file {
	const parsing_environment& env;
	gui_file(const parsing_environment& e) : env(e) {}

	void sub_gui_file(const gui_file& b) {
	}
	void gui_button(const guiButtonType& b) {
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
	}
	void gui_iconType(const iconType& b) {
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
	}
	void gui_instantTextBoxType(allTextBoxType&& b) {
		b.internal_definition.flags |= ui::text_def::instant;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
	}
	void gui_instantTextBoxType(allTextBoxType& b) {
		b.internal_definition.flags |= ui::text_def::instant;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
	}
	void gui_textBoxType(const allTextBoxType& b) {
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
	}
	void gui_positionType(const positionType& b) {
		env.defs.positions.emplace_back(b.internal_definition);
		env.nmaps.position_names.emplace_back(b.name);
	}
	void gui_checkboxType(guiButtonType&& b) {
		b.internal_definition.flags |= ui::button_def::is_checkbox;
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
	}
	void gui_checkboxType(guiButtonType& b) {
		b.internal_definition.flags |= ui::button_def::is_checkbox;
		env.defs.buttons.emplace_back(b.internal_definition);
		env.nmaps.button_names.emplace_back(b.name);
	}
	void gui_shieldtype(iconType&& b) {
		b.internal_definition.flags |= ui::icon_def::is_shield;
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
	}
	void gui_shieldtype(iconType& b) {
		b.internal_definition.flags |= ui::icon_def::is_shield;
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
	}
	void gui_editBoxType(allTextBoxType&& b) {
		b.internal_definition.flags |= ui::text_def::is_edit_box;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
	}
	void gui_editBoxType(allTextBoxType& b) {
		b.internal_definition.flags |= ui::text_def::is_edit_box;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
	}
	void gui_OverlappingElementsBoxType(const OverlappingElementsBoxType& b) {
		env.defs.overlapping_regions.emplace_back(b.internal_definition);
		env.nmaps.overlapping_region_names.emplace_back(b.name);
	}
	void gui_listBoxType(const listBoxType& b) {
		env.defs.listboxes.emplace_back(b.internal_definition);
		env.nmaps.listbox_names.emplace_back(b.name);
	}
	void gui_scrollbarType(scrollbarType&& b) {
		b.finalize();
		env.defs.scrollbars.emplace_back(b.internal_definition);
		env.nmaps.scrollbar_names.emplace_back(b.name);
	}
	void gui_scrollbarType(scrollbarType& b) {
		b.finalize();
		env.defs.scrollbars.emplace_back(b.internal_definition);
		env.nmaps.scrollbar_names.emplace_back(b.name);
	}
	void gui_eu3dialogtype(windowType&& b) {
		b.finalize();
		b.internal_definition.flags |= ui::window_def::is_dialog;
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
	}
	void gui_eu3dialogtype(windowType& b) {
		b.finalize();
		b.internal_definition.flags |= ui::window_def::is_dialog;
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
	}
	void gui_windowType(windowType&& b) {
		b.finalize();
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
	}
	void gui_windowType(windowType& b) {
		b.finalize();
		env.defs.windows.emplace_back(b.internal_definition);
		env.nmaps.window_names.emplace_back(b.name);
	}
	void add_unknown_key(int) {
		env.errors_generated.emplace_back(env.file, ui::errors::unknown_definition_type);
	}
};


MEMBER_FDEF(gui_file, gui_button, "guiButtonType");
MEMBER_FDEF(gui_file, gui_iconType, "iconType");
MEMBER_FDEF(gui_file, gui_eu3dialogtype, "eu3dialogtype");
MEMBER_FDEF(gui_file, gui_instantTextBoxType, "instantTextBoxType");
MEMBER_FDEF(gui_file, gui_listBoxType, "listBoxType");
MEMBER_FDEF(gui_file, gui_positionType, "positionType");
MEMBER_FDEF(gui_file, gui_scrollbarType, "scrollbarType");
MEMBER_FDEF(gui_file, gui_windowType, "windowType");
MEMBER_FDEF(gui_file, gui_checkboxType, "checkboxType");
MEMBER_FDEF(gui_file, gui_shieldtype, "shieldtype");
MEMBER_FDEF(gui_file, gui_OverlappingElementsBoxType, "OverlappingElementsBoxType");
MEMBER_FDEF(gui_file, gui_editBoxType, "editBoxType");
MEMBER_FDEF(gui_file, gui_textBoxType, "textBoxType");
MEMBER_FDEF(gui_file, sub_gui_file, "gui_file");
MEMBER_FDEF(gui_file, add_unknown_key, "unknown_key");


bool accept_all(const char*, const char*) {
	return true;
}

BEGIN_DOMAIN(gui_file_domain)
EMPTY_TYPE(empty_type)
	BEGIN_TYPE(ui::xy_pair)
		MEMBER_ASSOCIATION("x", "x", value_from_rh<int>)
		MEMBER_ASSOCIATION("y", "y", value_from_rh<int>)
	END_TYPE
	BEGIN_TYPE(gui_file)
	    MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
    	MEMBER_TYPE_ASSOCIATION("eu3dialogtype", "eu3dialogtype", windowType)
	    MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", allTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("positionType", "positiontype", positionType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("shieldtype", "shieldtype", iconType)
		MEMBER_TYPE_ASSOCIATION("OverlappingElementsBoxType", "overlappingelementsboxtype", OverlappingElementsBoxType)
		MEMBER_TYPE_ASSOCIATION("editBoxType", "editboxtype", allTextBoxType)
		MEMBER_TYPE_ASSOCIATION("textBoxType", "textboxtype", allTextBoxType)
	    MEMBER_TYPE_ASSOCIATION("gui_file", "guitypes", gui_file)
	    MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
	    MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(guiButtonType)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_ASSOCIATION("delayedtooltipText", "delayedtooltiptext", token_from_rh)
		MEMBER_ASSOCIATION("format", "format", button_format_from_rh)
		MEMBER_ASSOCIATION("spriteType", "spritetype", token_from_rh)
		MEMBER_ASSOCIATION("rotation", "rotation", button_rotation_from_rh)
		MEMBER_ASSOCIATION("shortcut", "shortcut", virtual_key_from_rh)
		MEMBER_ASSOCIATION("tooltip", "tooltip", token_from_rh)
		MEMBER_ASSOCIATION("tooltipText", "tooltiptext", token_from_rh)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("quadTextureSprite", "quadtexturesprite", token_from_rh)
		MEMBER_ASSOCIATION("buttonText", "buttontext", token_from_rh)
		MEMBER_ASSOCIATION("buttonFont", "buttonfont", token_from_rh)
		MEMBER_ASSOCIATION("Orientation", "orientation", button_orientation_from_rh)
		MEMBER_ASSOCIATION("clicksound", "clicksound", token_from_rh)
	    MEMBER_ASSOCIATION("parent", "parent", discard_from_rh)
	    MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
	    MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(iconType)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_ASSOCIATION("orientation", "orientation", icon_orientation_from_rh)
		MEMBER_ASSOCIATION("buttonMesh", "buttonmesh", token_from_rh)
		MEMBER_ASSOCIATION("frame", "frame", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spriteType", "spritetype", token_from_rh)
		MEMBER_ASSOCIATION("rotation", "rotation", icon_rotation_from_rh)
		MEMBER_ASSOCIATION("scale", "scale", value_from_rh<float>)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(allTextBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", textbox_orientation_from_rh)
		MEMBER_ASSOCIATION("alwaysTransparent", "allwaystransparent", value_from_rh<bool>)
		MEMBER_ASSOCIATION("fixedSize", "fixedsize", value_from_rh<bool>)
		MEMBER_ASSOCIATION("font", "font", token_from_rh)
		MEMBER_ASSOCIATION("format", "format", textbox_format_from_rh)
		MEMBER_ASSOCIATION("maxHeight", "maxheight", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("maxWidth", "maxwidth", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
	    MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_ASSOCIATION("text", "text", token_from_rh)
		MEMBER_ASSOCIATION("textureFile", "texturefile", textbox_background_from_rh)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("borderSize", "bordersize", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(listBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", listbox_orientation_from_rh)
		MEMBER_ASSOCIATION("alwaystransparent", "allwaystransparent", value_from_rh<bool>)
		MEMBER_ASSOCIATION("background", "background", token_from_rh)
		MEMBER_ASSOCIATION("horizontal", "horizontal", value_from_rh<bool>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("priority", "priority", value_from_rh<int>)
		MEMBER_ASSOCIATION("scrollbartype", "scrollbartype", token_from_rh)
		MEMBER_ASSOCIATION("spacing", "spacing", value_from_rh<int>)
		MEMBER_ASSOCIATION("step", "step", value_from_rh<int>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("offset", "offset", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(positionType)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(scrollbarType)
		MEMBER_ASSOCIATION("horizontal", "horizontal", value_from_rh<int>)
		MEMBER_ASSOCIATION("leftbutton", "leftbutton", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("lockable", "lockable", value_from_rh<bool>)
		MEMBER_ASSOCIATION("maxvalue", "maxvalue", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("minvalue", "minvalue", value_from_rh<int>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("priority", "priority", value_from_rh<int>)
		MEMBER_ASSOCIATION("rangelimitmax", "rangelimitmax", discard_from_rh)
		MEMBER_ASSOCIATION("rangelimitmin", "rangelimitmin", discard_from_rh)
		MEMBER_ASSOCIATION("rangelimitmaxicon", "rangelimitmaxicon", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rangelimitminicon", "rangelimitminicon", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rightbutton", "rightbutton", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("slider", "slider", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("startvalue", "startvalue", discard_from_rh)
		MEMBER_ASSOCIATION("stepsize", "stepsize", token_from_rh)
		MEMBER_ASSOCIATION("track", "track", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("userangelimit", "userangelimit", value_from_rh<bool>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(windowType)
		MEMBER_ASSOCIATION("orientation", "orientation", window_orientation_from_rh)
		MEMBER_ASSOCIATION("background", "background", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("dontrender", "dontrender", value_from_rh<bool>)
		MEMBER_ASSOCIATION("downsound", "downsound", discard_from_rh)
		MEMBER_ASSOCIATION("upsound", "upsound", discard_from_rh)
		MEMBER_ASSOCIATION("fullscreen", "fullscreen", value_from_rh<bool>)
		MEMBER_ASSOCIATION("horizontalborder", "horizontalborder", discard_from_rh)
		MEMBER_ASSOCIATION("verticalborder", "verticalborder", discard_from_rh)
		MEMBER_ASSOCIATION("moveable", "moveable", value_from_rh<int>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("eu3dialogtype", "eu3dialogtype", windowType)
		MEMBER_TYPE_ASSOCIATION("shieldtype", "shieldtype", iconType)
		MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", allTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("OverlappingElementsBoxType", "overlappingelementsboxtype", OverlappingElementsBoxType)
		MEMBER_TYPE_ASSOCIATION("editBoxType", "editboxtype", allTextBoxType)
		MEMBER_TYPE_ASSOCIATION("textBoxType", "textboxtype", allTextBoxType)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(OverlappingElementsBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", overlapping_orientation_from_rh)
		MEMBER_ASSOCIATION("format", "format", overlapping_format_from_rh)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spacing", "spacing", value_from_rh<float>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
END_DOMAIN;


void load_ui_definitions_from_directory(
	const directory& source_directory,
	ui::name_maps& nmaps,
	ui::definitions& defs,
	std::vector<std::pair<std::string,ui::errors>>& errors_generated,
	const text_handle_lookup& th_f,
	const font_handle_lookup& fh_f,
	const gobj_lookup& qt_f,
	const sound_lookup& sl_f) {

	std::vector<token_group> parse_tree;

	parsing_environment e(nmaps, defs, errors_generated, th_f, fh_f, qt_f, sl_f);

	const auto gui_files = source_directory.list_files(u".gui");

	for (const auto& f : gui_files) {
		if (f.file_name() == u"battleplansinterface.gui" || f.file_name() == u"news.gui" || f.file_name() == u"nudge.gui"
			|| f.file_name() == u"tutorial.gui" || f.file_name() == u"tutorial_control.gui") {
			//skip
		} else {
			auto open_f = f.open_file();
			if (open_f) {
				e.file = std::string(f.file_name().begin(), f.file_name().end()); // just dropping unicode

				const auto sz = open_f->size();
				auto buffer = new char[sz];

				open_f->read_to_buffer(buffer, sz);

				parse_tree.clear();
				parse_pdx_file(parse_tree, buffer, buffer + sz);

				if (parse_tree.size() > 0)
					parse_object<gui_file, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e);

				delete[] buffer;
			}
		}
	}
}

const char* ui::format_error(ui::errors  e) {
	switch (e) {
		case ui::errors::expected_tooltip_empty_for_button:
			return "expected empty tooltip for button";
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
	}
}