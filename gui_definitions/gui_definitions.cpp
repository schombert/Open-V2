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

std::string label_empty_type(const token_and_type& a, association_type, empty_type&) {
	return std::string(a.start, a.end);
}

int discard_empty_type(const token_and_type& a, association_type, empty_type&) {
	return 0;
}

template<typename gui_item_type>
struct global_consume_gui_item {
	global_consume_gui_item() {}

	global_consume_gui_item& operator=(gui_item_type&& in) {
		gui_item_type::add_global(std::move(in));
		return *this;
	}
};

struct add_string_to_set {
	std::set<std::string>& set_dest;

	add_string_to_set(std::set<std::string>& s) : set_dest(s) {}

	add_string_to_set& operator=(const std::string& in) {
		set_dest.insert(in);
		return *this;
	}
};

MEMBER_DEF(ui::xy_pair, x, "x");
MEMBER_DEF(ui::xy_pair, y, "y");

token_and_type token_from_rh(association_type, const token_and_type& t) {
	return t;
}

std::optional<virtual_key> virtual_key_from_rh(association_type, const token_and_type& t) {
	if (t.start == t.end)
		return std::optional<virtual_key>();
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
	} else if (is_fixed_token_ci(t, "upper_left")) {
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
	} else if (is_fixed_token_ci(t, "gfx\\interface\\tiles_dialog.tga")) {
		return ui::text_def::background_tiles_dialog_tga;
	} else if (is_fixed_token_ci(t, "gfx\\interface\\transparency.tga")) {
		return ui::text_def::background_transparency_tga;
	} else if (is_fixed_token_ci(t, "gfx\\interface\\small_tiles_dialog.tga")) {
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
MEMBER_DEF(allTextBoxType, internal_definition.max_height, "maxHeight");
MEMBER_DEF(allTextBoxType, internal_definition.max_width, "maxWidth");
MEMBER_DEF(allTextBoxType, name, "name");
MEMBER_DEF(allTextBoxType, internal_definition.position, "position");
MEMBER_FDEF(allTextBoxType, text, "text");
MEMBER_FDEF(allTextBoxType, background, "textureFile");
MEMBER_FDEF(allTextBoxType, add_unknown_key, "unknown_key");

struct listBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<listBoxType> all_items;

	std::string orientation;
	std::string alwaystransparent;
	std::string background;
	std::string horizontal;
	std::string name;
	std::string priority;
	std::string scrollbartype;
	std::string spacing;
	std::string step;
	ui::xy_pair size;
	ui::xy_pair bordersize;
	ui::xy_pair position;
	ui::xy_pair offset;

	static void add_global(listBoxType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<listBoxType> listBoxType::all_items;
std::set<std::string> listBoxType::unknown_keys;

MEMBER_DEF(listBoxType, orientation, "orientation");
MEMBER_DEF(listBoxType, alwaystransparent, "alwaystransparent");
MEMBER_DEF(listBoxType, background, "background");
MEMBER_DEF(listBoxType, horizontal, "horizontal");
MEMBER_DEF(listBoxType, name, "name");
MEMBER_DEF(listBoxType, priority, "priority");
MEMBER_DEF(listBoxType, scrollbartype, "scrollbartype");
MEMBER_DEF(listBoxType, spacing, "spacing");
MEMBER_DEF(listBoxType, step, "step");
MEMBER_DEF(listBoxType, size, "size");
MEMBER_DEF(listBoxType, bordersize, "bordersize");
MEMBER_DEF(listBoxType, position, "position");
MEMBER_DEF(listBoxType, offset, "offset");
MEMBER_DEF(listBoxType, add_unknown_key(), "unknown_key");

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
	static std::set<std::string> unknown_keys;
	static std::vector<scrollbarType> all_items;

	std::string horizontal;
	std::string leftbutton;
	std::string lockable;
	std::string maxvalue;
	std::string minvalue;
	std::string name;
	std::string priority;
	std::string rangelimitmax;
	std::string rangelimitmin;
	std::string rangelimitmaxicon;
	std::string rangelimitminicon;
	std::string rightbutton;
	std::string slider;
	std::string startvalue;
	std::string stepsize;
	std::string track;
	std::string userangelimit;
	ui::xy_pair bordersize;
	ui::xy_pair position;
	ui::xy_pair size;

	global_consume_gui_item<guiButtonType> gui_button() { return global_consume_gui_item<guiButtonType>(); }
	global_consume_gui_item<iconType> gui_iconType() { return global_consume_gui_item<iconType>(); }
	static void add_global(scrollbarType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<scrollbarType> scrollbarType::all_items;
std::set<std::string> scrollbarType::unknown_keys;

MEMBER_DEF(scrollbarType, horizontal, "horizontal");
MEMBER_DEF(scrollbarType, leftbutton, "leftbutton");
MEMBER_DEF(scrollbarType, lockable, "lockable");
MEMBER_DEF(scrollbarType, maxvalue, "maxvalue");
MEMBER_DEF(scrollbarType, minvalue, "minvalue");
MEMBER_DEF(scrollbarType, name, "name");
MEMBER_DEF(scrollbarType, priority, "priority");
MEMBER_DEF(scrollbarType, rangelimitmax, "rangelimitmax");
MEMBER_DEF(scrollbarType, rangelimitmin, "rangelimitmin");
MEMBER_DEF(scrollbarType, rangelimitmaxicon, "rangelimitmaxicon");
MEMBER_DEF(scrollbarType, rangelimitminicon, "rangelimitminicon");
MEMBER_DEF(scrollbarType, rightbutton, "rightbutton");
MEMBER_DEF(scrollbarType, slider, "slider");
MEMBER_DEF(scrollbarType, startvalue, "startvalue");
MEMBER_DEF(scrollbarType, stepsize, "stepsize");
MEMBER_DEF(scrollbarType, track, "track");
MEMBER_DEF(scrollbarType, userangelimit, "userangelimit");
MEMBER_DEF(scrollbarType, bordersize, "bordersize");
MEMBER_DEF(scrollbarType, position, "position");
MEMBER_DEF(scrollbarType, size, "size");
MEMBER_DEF(scrollbarType, gui_button(), "guiButtonType");
MEMBER_DEF(scrollbarType, gui_iconType(), "iconType");
MEMBER_DEF(scrollbarType, add_unknown_key(), "unknown_key");

/*
struct checkboxType {
	static std::set<std::string> unknown_keys;
	static std::vector<checkboxType> all_items;

	std::string orientation;
	std::string buttonfont;
	std::string buttontext;
	std::string delayedtooltiptext;
	std::string name;
	std::string quadtexturesprite;
	std::string shortcut;
	std::string tooltip;
	std::string tooltiptext;
	ui::xy_pair position;

	static void add_global(checkboxType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<checkboxType> checkboxType::all_items;
std::set<std::string> checkboxType::unknown_keys;

MEMBER_DEF(checkboxType, orientation, "orientation");
MEMBER_DEF(checkboxType, buttonfont, "buttonfont");
MEMBER_DEF(checkboxType, buttontext, "buttontext");
MEMBER_DEF(checkboxType, delayedtooltiptext, "delayedtooltiptext");
MEMBER_DEF(checkboxType, name, "name");
MEMBER_DEF(checkboxType, quadtexturesprite, "quadtexturesprite");
MEMBER_DEF(checkboxType, shortcut, "shortcut");
MEMBER_DEF(checkboxType, tooltip, "tooltip");
MEMBER_DEF(checkboxType, tooltiptext, "tooltiptext");
MEMBER_DEF(checkboxType, position, "position");
MEMBER_DEF(checkboxType, add_unknown_key(), "unknown_key");
*/

/*
struct shieldtype {
	static std::set<std::string> unknown_keys;
	static std::vector<shieldtype> all_items;

	std::string name;
	std::string rotation;
	std::string spriteType;
	ui::xy_pair position;

	static void add_global(shieldtype&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<shieldtype> shieldtype::all_items;
std::set<std::string> shieldtype::unknown_keys;

MEMBER_DEF(shieldtype, name, "name");
MEMBER_DEF(shieldtype, rotation, "rotation");
MEMBER_DEF(shieldtype, spriteType, "spriteType");
MEMBER_DEF(shieldtype, position, "position");
MEMBER_DEF(shieldtype, add_unknown_key(), "unknown_key");
*/

struct OverlappingElementsBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<OverlappingElementsBoxType> all_items;

	std::string orientation;
	std::string format;
	std::string name;
	std::string spacing;
	ui::xy_pair position;
	ui::xy_pair size;

	static void add_global(OverlappingElementsBoxType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<OverlappingElementsBoxType> OverlappingElementsBoxType::all_items;
std::set<std::string> OverlappingElementsBoxType::unknown_keys;

MEMBER_DEF(OverlappingElementsBoxType, orientation, "orientation");
MEMBER_DEF(OverlappingElementsBoxType, format, "format");
MEMBER_DEF(OverlappingElementsBoxType, name, "name");
MEMBER_DEF(OverlappingElementsBoxType, spacing, "spacing");
MEMBER_DEF(OverlappingElementsBoxType, position, "position");
MEMBER_DEF(OverlappingElementsBoxType, size, "size");
MEMBER_DEF(OverlappingElementsBoxType, add_unknown_key(), "unknown_key");

/*
struct editBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<editBoxType> all_items;

	std::string orientation;
	std::string font;
	std::string name;
	std::string text;
	std::string texturefile;
	ui::xy_pair bordersize;
	ui::xy_pair position;
	ui::xy_pair size;

	static void add_global(editBoxType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<editBoxType> editBoxType::all_items;
std::set<std::string> editBoxType::unknown_keys;

MEMBER_DEF(editBoxType, orientation, "orientation");
MEMBER_DEF(editBoxType, bordersize, "bordersize");
MEMBER_DEF(editBoxType, font, "font");
MEMBER_DEF(editBoxType, name, "name");
MEMBER_DEF(editBoxType, text, "text");
MEMBER_DEF(editBoxType, texturefile, "texturefile");
MEMBER_DEF(editBoxType, position, "position");
MEMBER_DEF(editBoxType, size, "size");
MEMBER_DEF(editBoxType, add_unknown_key(), "unknown_key");

struct textBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<textBoxType> all_items;

	std::string orientation;
	std::string fixedsize;
	std::string font;
	std::string format;
	std::string maxheight;
	std::string maxwidth;
	std::string name;
	std::string text;
	std::string texturefile;
	ui::xy_pair bordersize;
	ui::xy_pair position;

	static void add_global(textBoxType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<textBoxType> textBoxType::all_items;
std::set<std::string> textBoxType::unknown_keys;

MEMBER_DEF(textBoxType, orientation, "orientation");
MEMBER_DEF(textBoxType, fixedsize, "fixedsize");
MEMBER_DEF(textBoxType, font, "font");
MEMBER_DEF(textBoxType, format, "format");
MEMBER_DEF(textBoxType, maxheight, "maxheight");
MEMBER_DEF(textBoxType, maxwidth, "maxwidth");
MEMBER_DEF(textBoxType, name, "name");
MEMBER_DEF(textBoxType, text, "text");
MEMBER_DEF(textBoxType, texturefile, "texturefile");
MEMBER_DEF(textBoxType, bordersize, "bordersize");
MEMBER_DEF(textBoxType, position, "position");
MEMBER_DEF(textBoxType, add_unknown_key(), "unknown_key");
*/

struct windowType {
	static std::set<std::string> unknown_keys;
	static std::vector<windowType> all_items;


	std::string orientation;
	std::string background;
	std::string dontrender;
	std::string downsound;
	std::string upsound;
	std::string fullscreen;
	std::string horizontalborder;
	std::string verticalborder;
	std::string moveable;
	std::string name;
	ui::xy_pair position;
	ui::xy_pair size;

	static void add_global(windowType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };

	global_consume_gui_item<guiButtonType> gui_button() { return global_consume_gui_item<guiButtonType>(); }
	global_consume_gui_item<iconType> gui_iconType() { return global_consume_gui_item<iconType>(); }
	//global_consume_gui_item<instantTextBoxType> gui_instantTextBoxType() { return global_consume_gui_item<instantTextBoxType>(); }
	global_consume_gui_item<listBoxType> gui_listBoxType() { return global_consume_gui_item<listBoxType>(); }
	global_consume_gui_item<scrollbarType> gui_scrollbarType() { return global_consume_gui_item<scrollbarType>(); }
	global_consume_gui_item<windowType> gui_windowType() { return global_consume_gui_item<windowType>(); }
	//global_consume_gui_item<checkboxType> gui_checkboxType() { return global_consume_gui_item<checkboxType>(); }
	global_consume_gui_item<OverlappingElementsBoxType> gui_OverlappingElementsBoxType() { return global_consume_gui_item<OverlappingElementsBoxType>(); }
	//global_consume_gui_item<editBoxType> gui_editBoxType() { return global_consume_gui_item<editBoxType>(); }
	//global_consume_gui_item<textBoxType> gui_textBoxType() { return global_consume_gui_item<textBoxType>(); }
};

std::vector<windowType> windowType::all_items;
std::set<std::string> windowType::unknown_keys;

MEMBER_DEF(windowType, orientation, "orientation");
MEMBER_DEF(windowType, background, "background");
MEMBER_DEF(windowType, dontrender, "dontrender");
MEMBER_DEF(windowType, downsound, "downsound");
MEMBER_DEF(windowType, upsound, "upsound");
MEMBER_DEF(windowType, fullscreen, "fullscreen");
MEMBER_DEF(windowType, horizontalborder, "horizontalborder");
MEMBER_DEF(windowType, verticalborder, "verticalborder");
MEMBER_DEF(windowType, moveable, "moveable");
MEMBER_DEF(windowType, name, "name");
MEMBER_DEF(windowType, position, "position");
MEMBER_DEF(windowType, size, "size");
// MEMBER_DEF(windowType, gui_button(), "guiButtonType");
MEMBER_DEF(windowType, gui_iconType(), "iconType");
// MEMBER_DEF(windowType, gui_instantTextBoxType(), "instantTextBoxType");
MEMBER_DEF(windowType, gui_listBoxType(), "listBoxType");
MEMBER_DEF(windowType, gui_scrollbarType(), "scrollbarType");
MEMBER_DEF(windowType, gui_windowType(), "windowType");
//MEMBER_DEF(windowType, gui_checkboxType(), "checkboxType");
MEMBER_DEF(windowType, gui_OverlappingElementsBoxType(), "OverlappingElementsBoxType");
// MEMBER_DEF(windowType, gui_editBoxType(), "editBoxType");
// MEMBER_DEF(windowType, gui_textBoxType(), "textBoxType");
MEMBER_DEF(windowType, add_unknown_key(), "unknown_key");

struct eu3dialogtype {
	static std::set<std::string> unknown_keys;
	static std::vector<eu3dialogtype> all_items;

	std::string orientation;
	std::string backGround;
	std::string dontRender;
	std::string fullScreen;
	std::string horizontalBorder;
	std::string verticalBorder;
	std::string moveable;
	std::string name;
	ui::xy_pair position;
	ui::xy_pair size;

	static void add_global(eu3dialogtype&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };

	global_consume_gui_item<guiButtonType> gui_button() { return global_consume_gui_item<guiButtonType>(); }
	global_consume_gui_item<iconType> gui_iconType() { return global_consume_gui_item<iconType>(); }
	//global_consume_gui_item<instantTextBoxType> gui_instantTextBoxType() { return global_consume_gui_item<instantTextBoxType>(); }
	global_consume_gui_item<listBoxType> gui_listBoxType() { return global_consume_gui_item<listBoxType>(); }
	global_consume_gui_item<scrollbarType> gui_scrollbarType() { return global_consume_gui_item<scrollbarType>(); }
	global_consume_gui_item<windowType> gui_windowType() { return global_consume_gui_item<windowType>(); }
	//global_consume_gui_item<checkboxType> gui_checkboxType() { return global_consume_gui_item<checkboxType>(); }
	//global_consume_gui_item<shieldtype> gui_shieldtype() { return global_consume_gui_item<shieldtype>(); }
};

std::vector<eu3dialogtype> eu3dialogtype::all_items;
std::set<std::string> eu3dialogtype::unknown_keys;

MEMBER_DEF(eu3dialogtype, orientation, "orientation");
MEMBER_DEF(eu3dialogtype, backGround, "backGround");
MEMBER_DEF(eu3dialogtype, dontRender, "dontRender");
MEMBER_DEF(eu3dialogtype, fullScreen, "fullScreen");
MEMBER_DEF(eu3dialogtype, horizontalBorder, "horizontalBorder");
MEMBER_DEF(eu3dialogtype, verticalBorder, "verticalBorder");
MEMBER_DEF(eu3dialogtype, moveable, "moveable");
MEMBER_DEF(eu3dialogtype, name, "name");
MEMBER_DEF(eu3dialogtype, position, "position");
MEMBER_DEF(eu3dialogtype, size, "size");
// MEMBER_DEF(eu3dialogtype, gui_button(), "guiButtonType");
MEMBER_DEF(eu3dialogtype, gui_iconType(), "iconType");
// MEMBER_DEF(eu3dialogtype, gui_instantTextBoxType(), "instantTextBoxType");
MEMBER_DEF(eu3dialogtype, gui_listBoxType(), "listBoxType");
MEMBER_DEF(eu3dialogtype, gui_scrollbarType(), "scrollbarType");
MEMBER_DEF(eu3dialogtype, gui_windowType(), "windowType");
//MEMBER_DEF(eu3dialogtype, gui_checkboxType(), "checkboxType");
//MEMBER_DEF(eu3dialogtype, gui_shieldtype(), "shieldtype");
MEMBER_DEF(eu3dialogtype, add_unknown_key(), "unknown_key");


struct gui_file {
	static std::set<std::string> unknown_keys;

	parsing_environment& env;
	gui_file(parsing_environment& e) : env(e) {}

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

	void gui_shieldtype(iconType&& b) {
		b.internal_definition.flags |= ui::icon_def::is_shield;
		env.defs.icons.emplace_back(b.internal_definition);
		env.nmaps.icon_names.emplace_back(b.name);
	}
	void gui_editBoxType(allTextBoxType&& b) {
		b.internal_definition.flags |= ui::text_def::is_edit_box;
		env.defs.text.emplace_back(b.internal_definition);
		env.nmaps.text_names.emplace_back(b.name);
	}

	global_consume_gui_item<eu3dialogtype> gui_eu3dialogtype() { return global_consume_gui_item<eu3dialogtype>(); }
	global_consume_gui_item<listBoxType> gui_listBoxType() { return global_consume_gui_item<listBoxType>(); }
	global_consume_gui_item<scrollbarType> gui_scrollbarType() { return global_consume_gui_item<scrollbarType>(); }
	global_consume_gui_item<windowType> gui_windowType() { return global_consume_gui_item<windowType>(); }
	
	global_consume_gui_item<OverlappingElementsBoxType> gui_OverlappingElementsBoxType() { return global_consume_gui_item<OverlappingElementsBoxType>(); }
	

	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::set<std::string> gui_file::unknown_keys;


MEMBER_FDEF(gui_file, gui_button, "guiButtonType");
MEMBER_FDEF(gui_file, gui_iconType, "iconType");
MEMBER_DEF(gui_file, gui_eu3dialogtype(), "eu3dialogtype");
MEMBER_FDEF(gui_file, gui_instantTextBoxType, "instantTextBoxType");
MEMBER_DEF(gui_file, gui_listBoxType(), "listBoxType");
MEMBER_DEF(gui_file, gui_positionType, "positionType");
MEMBER_DEF(gui_file, gui_scrollbarType(), "scrollbarType");
MEMBER_DEF(gui_file, gui_windowType(), "windowType");
MEMBER_FDEF(gui_file, gui_checkboxType, "checkboxType");
MEMBER_FDEF(gui_file, gui_shieldtype, "shieldtype");
MEMBER_DEF(gui_file, gui_OverlappingElementsBoxType(), "OverlappingElementsBoxType");
MEMBER_FDEF(gui_file, gui_editBoxType, "editBoxType");
MEMBER_FDEF(gui_file, gui_textBoxType, "textBoxType");
MEMBER_DEF(gui_file, add_unknown_key(), "unknown_key");


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
    	MEMBER_TYPE_ASSOCIATION("eu3dialogtype", "eu3dialogtype", eu3dialogtype)
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
	    MEMBER_TYPE_ASSOCIATION("this", "guitypes", gui_file)
	    MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
	    MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
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
	BEGIN_TYPE(eu3dialogtype)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("backGround", "backGround", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("dontRender", "dontRender", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("fullScreen", "fullScreen", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("horizontalBorder", "horizontalBorder", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("verticalBorder", "verticalBorder", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("moveable", "moveable", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		// MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		// MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		// MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", instantTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		// MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", checkboxType)
		// MEMBER_TYPE_ASSOCIATION("shieldtype", "shieldtype", shieldtype)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
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
		MEMBER_ASSOCIATION("text", "text", token_from_rh)
		MEMBER_ASSOCIATION("textureFile", "texturefile", textbox_background_from_rh)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("borderSize", "bordersize", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(listBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("alwaystransparent", "allwaystransparent", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("background", "background", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("horizontal", "horizontal", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("priority", "priority", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("scrollbartype", "scrollbartype", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spacing", "spacing", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("step", "step", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("offset", "offset", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(positionType)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
	END_TYPE
	BEGIN_TYPE(scrollbarType)
		MEMBER_ASSOCIATION("horizontal", "horizontal", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("leftbutton", "leftbutton", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("horizontal", "horizontal", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("lockable", "lockable", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("maxvalue", "maxvalue", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("minvalue", "minvalue", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("priority", "priority", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rangelimitmax", "rangelimitmax", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rangelimitmin", "rangelimitmin", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rangelimitmaxicon", "rangelimitmaxicon", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rangelimitminicon", "rangelimitminicon", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rightbutton", "rightbutton", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("slider", "slider", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("startvalue", "startvalue", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("stepsize", "stepsize", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("track", "track", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("userangelimit", "userangelimit", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(windowType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("background", "background", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("dontrender", "dontrender", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("downsound", "downsound", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("upsound", "upsound", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("fullscreen", "fullscreen", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("horizontalborder", "horizontalborder", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("verticalborder", "verticalborder", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("moveable", "moveable", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		// MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		// MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		// MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", instantTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		// MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", checkboxType)
		MEMBER_TYPE_ASSOCIATION("OverlappingElementsBoxType", "overlappingelementsboxtype", OverlappingElementsBoxType)
		// MEMBER_TYPE_ASSOCIATION("editBoxType", "editboxtype", editBoxType)
		// MEMBER_TYPE_ASSOCIATION("textBoxType", "textboxtype", textBoxType)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(OverlappingElementsBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("format", "format", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spacing", "spacing", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", ui::xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", ui::xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
END_DOMAIN;