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
	std::vector<ui::errors>& errors_generated;
	const text_handle_lookup& th_f;
	const font_handle_lookup& fh_f;
	const qtex_lookup& qt_f;
	const sprite_type_lookup& st_f;
	const sound_lookup& sl_f;
};

std::string label_empty_type(const token_and_type& a, association_type, empty_type&) {
	return std::string(a.start, a.end);
}

struct assign_to_bitfield8 {
	uint8_t& target;
	assign_to_bitfield8(uint8_t& t) : target(t) {}
	assign_to_bitfield8& operator=(uint8_t i) { target |= i; return *this; }
};

struct assign_to_bitfield8_or_err {
	uint8_t& target;
	std::vector<ui::errors>& product;
	const ui::errors error_type;
	assign_to_bitfield8_or_err(uint8_t& t, std::vector<ui::errors>& p, ui::errors e) : target(t), product(p), error_type(e) {}
	assign_to_bitfield8_or_err& operator=(std::optional<uint8_t> &i) {
		if (i)
			target |= *i;
		else
			product.push_back(error_type);
		return *this;
	}
};

struct error_on_assignment {
	std::vector<ui::errors>& product;
	const ui::errors error_type;
	error_on_assignment(std::vector<ui::errors>& p, ui::errors e) : product(p), error_type(e) {}
	error_on_assignment& operator=(empty_type&) { product.push_back(error_type); return *this; }
	error_on_assignment& operator=(int) { product.push_back(error_type); return *this; }
};

struct error_on_non_empty {
	std::vector<ui::errors>& product;
	const ui::errors error_type;
	error_on_non_empty(std::vector<ui::errors>& p, ui::errors e) : product(p), error_type(e) {}
	error_on_non_empty& operator=(const token_and_type& t) {
		if(t.start != t.end)
			product.push_back(error_type);
		return *this; 
	}
};

struct token_to_texth_assignment {
	uint16_t& handle;
	parsing_environment& env;
	token_to_texth_assignment(uint16_t& h, parsing_environment& e) : handle(h), env(e) {}
	token_to_texth_assignment& operator=(const token_and_type& t) {
		handle = env.th_f(t.start, t.end);
		return *this;
	}
};

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

struct discard_int {
	discard_int& operator=(int) {
		return *this;
	}
};

struct xy_pair {
	int x;
	int y;
};

MEMBER_DEF(xy_pair, x, "x");
MEMBER_DEF(xy_pair, y, "y");

token_and_type token_from_rh(association_type, const token_and_type& t) {
	return t;
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



struct guiButtonType {
	ui::button_def internal_definition;

	parsing_environment& env;

	guiButtonType(parsing_environment& e) : env(e) {};

	static std::set<std::string> unknown_keys;

	static std::vector<guiButtonType> all_buttons;
	static void add_global(guiButtonType&& in) { all_buttons.emplace_back(std::move(in)); }

	std::string name;
	std::string quadTextureSprite;
	std::string buttonText; // should be int from global test key
	std::string buttonFont; // should be id from font table
	std::string clicksound;

	xy_pair position;
	std::string rotation;
	std::string shortcut;
	xy_pair size;
	std::string spriteType;

	token_to_texth_assignment buttonText() {
		return token_to_texth_assignment(internal_definition.text_handle, env);
	}
	error_on_non_empty tooltip() {
		return error_on_non_empty(env.errors_generated, ui::errors::expected_tooltip_empty_for_button);
	}
	error_on_non_empty tooltipText() {
		return error_on_non_empty(env.errors_generated, ui::errors::expected_tooltiptext_empty_for_button);
	}
	error_on_non_empty delayedtoottipText() {
		return error_on_non_empty(env.errors_generated, ui::errors::expected_delayedtooltiptext_empty_for_button);
	}
	assign_to_bitfield8_or_err orientation_bits() {
		return assign_to_bitfield8_or_err(internal_definition.flags, env.errors_generated, ui::errors::unknown_button_orientation);
	}
	assign_to_bitfield8_or_err format_bits() {
		return assign_to_bitfield8_or_err(internal_definition.flags, env.errors_generated, ui::errors::expected_button_format_to_be_left);
	}
	discard_int discard_member() {
		return discard_int();
	}
	error_on_assignment add_unknown_key() {
		return error_on_assignment(env.errors_generated, ui::errors::unexpected_button_attribute);
	};
};

std::vector<guiButtonType> guiButtonType::all_buttons;
std::set<std::string> guiButtonType::unknown_keys;

MEMBER_DEF(guiButtonType, name, "name");
MEMBER_DEF(guiButtonType, quadTextureSprite, "quadTextureSprite");
MEMBER_DEF(guiButtonType, buttonText(), "buttonText");
MEMBER_DEF(guiButtonType, buttonFont, "buttonFont");
MEMBER_DEF(guiButtonType, orientation_bits(), "Orientation");
MEMBER_DEF(guiButtonType, clicksound, "clicksound");
MEMBER_DEF(guiButtonType, delayedtoottipText(), "delayedtoottipText");
MEMBER_DEF(guiButtonType, format_bits(), "format");
MEMBER_DEF(guiButtonType, internal_definition.position, "position");
MEMBER_DEF(guiButtonType, rotation, "rotation");
MEMBER_DEF(guiButtonType, shortcut, "shortcut");
MEMBER_DEF(guiButtonType, internal_definition.size, "size");
MEMBER_DEF(guiButtonType, spriteType, "spriteType");
MEMBER_DEF(guiButtonType, tooltip(), "tooltip");
MEMBER_DEF(guiButtonType, tooltipText(), "tooltipText");
MEMBER_DEF(guiButtonType, discard_member(), "parent");
MEMBER_DEF(guiButtonType, add_unknown_key(), "unknown_key");


struct iconType {
	static std::set<std::string> unknown_keys;
	static std::vector<iconType> all_items;

	std::string orientation;
	std::string buttonMesh;
	std::string frame;
	std::string name;
	xy_pair position;
	std::string rotation;
	std::string scale;
	std::string spriteType;

	static void add_global(iconType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<iconType> iconType::all_items;
std::set<std::string> iconType::unknown_keys;

MEMBER_DEF(iconType, orientation, "orientation");
MEMBER_DEF(iconType, buttonMesh, "buttonMesh");
MEMBER_DEF(iconType, frame, "frame");
MEMBER_DEF(iconType, name, "name");
MEMBER_DEF(iconType, position, "position");
MEMBER_DEF(iconType, rotation, "rotation");
MEMBER_DEF(iconType, scale, "scale");
MEMBER_DEF(iconType, spriteType, "spriteType");
MEMBER_DEF(iconType, add_unknown_key(), "unknown_key");

struct instantTextBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<instantTextBoxType> all_items;

	std::string orientation;
	std::string alwaysTransparent;
	xy_pair borderSize;
	std::string fixedSize;
	std::string font;
	std::string format;
	std::string maxHeight;
	std::string maxWidth;
	std::string name;
	xy_pair position;
	std::string text;
	std::string textureFile;

	static void add_global(instantTextBoxType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<instantTextBoxType> instantTextBoxType::all_items;
std::set<std::string> instantTextBoxType::unknown_keys;

MEMBER_DEF(instantTextBoxType, orientation, "orientation");
MEMBER_DEF(instantTextBoxType, alwaysTransparent, "alwaysTransparent");
MEMBER_DEF(instantTextBoxType, borderSize, "borderSize");
MEMBER_DEF(instantTextBoxType, fixedSize, "fixedSize");
MEMBER_DEF(instantTextBoxType, font, "font");
MEMBER_DEF(instantTextBoxType, format, "format");
MEMBER_DEF(instantTextBoxType, maxHeight, "maxHeight");
MEMBER_DEF(instantTextBoxType, maxWidth, "maxWidth");
MEMBER_DEF(instantTextBoxType, name, "name");
MEMBER_DEF(instantTextBoxType, position, "position");
MEMBER_DEF(instantTextBoxType, text, "text");
MEMBER_DEF(instantTextBoxType, textureFile, "textureFile");
MEMBER_DEF(instantTextBoxType, add_unknown_key(), "unknown_key");

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
	xy_pair size;
	xy_pair bordersize;
	xy_pair position;
	xy_pair offset;

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
	static std::set<std::string> unknown_keys;
	static std::vector<positionType> all_items;

	std::string name;
	xy_pair position;

	static void add_global(positionType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<positionType> positionType::all_items;
std::set<std::string> positionType::unknown_keys;

MEMBER_DEF(positionType, name, "name");
MEMBER_DEF(positionType, position, "position");
MEMBER_DEF(positionType, add_unknown_key(), "unknown_key");

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
	xy_pair bordersize;
	xy_pair position;
	xy_pair size;

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
	xy_pair position;

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

struct shieldtype {
	static std::set<std::string> unknown_keys;
	static std::vector<shieldtype> all_items;

	std::string name;
	std::string rotation;
	std::string spriteType;
	xy_pair position;

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

struct OverlappingElementsBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<OverlappingElementsBoxType> all_items;

	std::string orientation;
	std::string format;
	std::string name;
	std::string spacing;
	xy_pair position;
	xy_pair size;

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

struct editBoxType {
	static std::set<std::string> unknown_keys;
	static std::vector<editBoxType> all_items;

	std::string orientation;
	std::string font;
	std::string name;
	std::string text;
	std::string texturefile;
	xy_pair bordersize;
	xy_pair position;
	xy_pair size;

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
	xy_pair bordersize;
	xy_pair position;

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
	xy_pair position;
	xy_pair size;

	static void add_global(windowType&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };

	global_consume_gui_item<guiButtonType> gui_button() { return global_consume_gui_item<guiButtonType>(); }
	global_consume_gui_item<iconType> gui_iconType() { return global_consume_gui_item<iconType>(); }
	global_consume_gui_item<instantTextBoxType> gui_instantTextBoxType() { return global_consume_gui_item<instantTextBoxType>(); }
	global_consume_gui_item<listBoxType> gui_listBoxType() { return global_consume_gui_item<listBoxType>(); }
	global_consume_gui_item<scrollbarType> gui_scrollbarType() { return global_consume_gui_item<scrollbarType>(); }
	global_consume_gui_item<windowType> gui_windowType() { return global_consume_gui_item<windowType>(); }
	global_consume_gui_item<checkboxType> gui_checkboxType() { return global_consume_gui_item<checkboxType>(); }
	global_consume_gui_item<OverlappingElementsBoxType> gui_OverlappingElementsBoxType() { return global_consume_gui_item<OverlappingElementsBoxType>(); }
	global_consume_gui_item<editBoxType> gui_editBoxType() { return global_consume_gui_item<editBoxType>(); }
	global_consume_gui_item<textBoxType> gui_textBoxType() { return global_consume_gui_item<textBoxType>(); }
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
MEMBER_DEF(windowType, gui_button(), "guiButtonType");
MEMBER_DEF(windowType, gui_iconType(), "iconType");
MEMBER_DEF(windowType, gui_instantTextBoxType(), "instantTextBoxType");
MEMBER_DEF(windowType, gui_listBoxType(), "listBoxType");
MEMBER_DEF(windowType, gui_scrollbarType(), "scrollbarType");
MEMBER_DEF(windowType, gui_windowType(), "windowType");
MEMBER_DEF(windowType, gui_checkboxType(), "checkboxType");
MEMBER_DEF(windowType, gui_OverlappingElementsBoxType(), "OverlappingElementsBoxType");
MEMBER_DEF(windowType, gui_editBoxType(), "editBoxType");
MEMBER_DEF(windowType, gui_textBoxType(), "textBoxType");
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
	xy_pair position;
	xy_pair size;

	static void add_global(eu3dialogtype&& in) { all_items.emplace_back(std::move(in)); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };

	global_consume_gui_item<guiButtonType> gui_button() { return global_consume_gui_item<guiButtonType>(); }
	global_consume_gui_item<iconType> gui_iconType() { return global_consume_gui_item<iconType>(); }
	global_consume_gui_item<instantTextBoxType> gui_instantTextBoxType() { return global_consume_gui_item<instantTextBoxType>(); }
	global_consume_gui_item<listBoxType> gui_listBoxType() { return global_consume_gui_item<listBoxType>(); }
	global_consume_gui_item<scrollbarType> gui_scrollbarType() { return global_consume_gui_item<scrollbarType>(); }
	global_consume_gui_item<windowType> gui_windowType() { return global_consume_gui_item<windowType>(); }
	global_consume_gui_item<checkboxType> gui_checkboxType() { return global_consume_gui_item<checkboxType>(); }
	global_consume_gui_item<shieldtype> gui_shieldtype() { return global_consume_gui_item<shieldtype>(); }
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
MEMBER_DEF(eu3dialogtype, gui_button(), "guiButtonType");
MEMBER_DEF(eu3dialogtype, gui_iconType(), "iconType");
MEMBER_DEF(eu3dialogtype, gui_instantTextBoxType(), "instantTextBoxType");
MEMBER_DEF(eu3dialogtype, gui_listBoxType(), "listBoxType");
MEMBER_DEF(eu3dialogtype, gui_scrollbarType(), "scrollbarType");
MEMBER_DEF(eu3dialogtype, gui_windowType(), "windowType");
MEMBER_DEF(eu3dialogtype, gui_checkboxType(), "checkboxType");
MEMBER_DEF(eu3dialogtype, gui_shieldtype(), "shieldtype");
MEMBER_DEF(eu3dialogtype, add_unknown_key(), "unknown_key");


struct gui_file {
	static std::set<std::string> unknown_keys;

	global_consume_gui_item<guiButtonType> gui_button() { return global_consume_gui_item<guiButtonType>(); }
	global_consume_gui_item<eu3dialogtype> gui_eu3dialogtype() { return global_consume_gui_item<eu3dialogtype>(); }
	global_consume_gui_item<iconType> gui_iconType() { return global_consume_gui_item<iconType>(); }
	global_consume_gui_item<instantTextBoxType> gui_instantTextBoxType() { return global_consume_gui_item<instantTextBoxType>(); }
	global_consume_gui_item<listBoxType> gui_listBoxType() { return global_consume_gui_item<listBoxType>(); }
	global_consume_gui_item<positionType> gui_positionType() { return global_consume_gui_item<positionType>(); }
	global_consume_gui_item<scrollbarType> gui_scrollbarType() { return global_consume_gui_item<scrollbarType>(); }
	global_consume_gui_item<windowType> gui_windowType() { return global_consume_gui_item<windowType>(); }
	global_consume_gui_item<checkboxType> gui_checkboxType() { return global_consume_gui_item<checkboxType>(); }
	global_consume_gui_item<shieldtype> gui_shieldtype() { return global_consume_gui_item<shieldtype>(); }
	global_consume_gui_item<OverlappingElementsBoxType> gui_OverlappingElementsBoxType() { return global_consume_gui_item<OverlappingElementsBoxType>(); }
	global_consume_gui_item<editBoxType> gui_editBoxType() { return global_consume_gui_item<editBoxType>(); }
	global_consume_gui_item<textBoxType> gui_textBoxType() { return global_consume_gui_item<textBoxType>(); }

	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::set<std::string> gui_file::unknown_keys;


MEMBER_DEF(gui_file, gui_button(), "guiButtonType");
MEMBER_DEF(gui_file, gui_iconType(), "iconType");
MEMBER_DEF(gui_file, gui_eu3dialogtype(), "eu3dialogtype");
MEMBER_DEF(gui_file, gui_instantTextBoxType(), "instantTextBoxType");
MEMBER_DEF(gui_file, gui_listBoxType(), "listBoxType");
MEMBER_DEF(gui_file, gui_positionType(), "positionType");
MEMBER_DEF(gui_file, gui_scrollbarType(), "scrollbarType");
MEMBER_DEF(gui_file, gui_windowType(), "windowType");
MEMBER_DEF(gui_file, gui_checkboxType(), "checkboxType");
MEMBER_DEF(gui_file, gui_shieldtype(), "shieldtype");
MEMBER_DEF(gui_file, gui_OverlappingElementsBoxType(), "OverlappingElementsBoxType");
MEMBER_DEF(gui_file, gui_editBoxType(), "editBoxType");
MEMBER_DEF(gui_file, gui_textBoxType(), "textBoxType");
MEMBER_DEF(gui_file, add_unknown_key(), "unknown_key");


bool accept_all(const char*, const char*) {
	return true;
}

BEGIN_DOMAIN(gui_file_domain)
EMPTY_TYPE(empty_type)
	BEGIN_TYPE(xy_pair)
		MEMBER_ASSOCIATION("x", "x", value_from_rh<int>)
		MEMBER_ASSOCIATION("y", "y", value_from_rh<int>)
	END_TYPE
	BEGIN_TYPE(gui_file)
	    MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
    	MEMBER_TYPE_ASSOCIATION("eu3dialogtype", "eu3dialogtype", eu3dialogtype)
	    MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", instantTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("positionType", "positiontype", positionType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", checkboxType)
		MEMBER_TYPE_ASSOCIATION("shieldtype", "shieldtype", shieldtype)
		MEMBER_TYPE_ASSOCIATION("OverlappingElementsBoxType", "overlappingelementsboxtype", OverlappingElementsBoxType)
		MEMBER_TYPE_ASSOCIATION("editBoxType", "editboxtype", editBoxType)
		MEMBER_TYPE_ASSOCIATION("textBoxType", "textboxtype", textBoxType)
	    MEMBER_TYPE_ASSOCIATION("this", "guitypes", gui_file)
	    MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
	    MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(guiButtonType)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
		MEMBER_ASSOCIATION("delayedtoottipText", "delayedtoottiptext", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("format", "format", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spriteType", "spritetype", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rotation", "rotation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("shortcut", "shortcut", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("tooltip", "tooltip", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("tooltipText", "tooltiptext", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("quadTextureSprite", "quadtexturesprite", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("buttonText", "buttontext", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("buttonFont", "buttonfont", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("Orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("clicksound", "clicksound", value_from_rh<std::string>)
	    MEMBER_ASSOCIATION("parent", "parent", discard_from_rh)
	    MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
	    MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
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
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
		MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", instantTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", checkboxType)
		MEMBER_TYPE_ASSOCIATION("shieldtype", "shieldtype", shieldtype)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(iconType)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("buttonMesh", "buttonmesh", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("frame", "frame", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spriteType", "spritetype", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rotation", "rotation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("scale", "scale", value_from_rh<std::string>)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(instantTextBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("alwaysTransparent", "allwaystransparent", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("fixedSize", "fixedsize", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("font", "font", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("format", "format", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("maxHeight", "maxheight", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("maxWidth", "maxwidth", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("text", "text", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("textureFile", "texturefile", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("borderSize", "bordersize", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
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
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", xy_pair)
		MEMBER_TYPE_ASSOCIATION("offset", "offset", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(positionType)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
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
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
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
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
		MEMBER_TYPE_ASSOCIATION("guiButtonType", "guibuttontype", guiButtonType)
		MEMBER_TYPE_ASSOCIATION("iconType", "icontype", iconType)
		MEMBER_TYPE_ASSOCIATION("instantTextBoxType", "instanttextboxtype", instantTextBoxType)
		MEMBER_TYPE_ASSOCIATION("listBoxType", "listboxtype", listBoxType)
		MEMBER_TYPE_ASSOCIATION("scrollbarType", "scrollbartype", scrollbarType)
		MEMBER_TYPE_ASSOCIATION("windowType", "windowtype", windowType)
		MEMBER_TYPE_ASSOCIATION("checkboxType", "checkboxtype", checkboxType)
		MEMBER_TYPE_ASSOCIATION("OverlappingElementsBoxType", "overlappingelementsboxtype", OverlappingElementsBoxType)
		MEMBER_TYPE_ASSOCIATION("editBoxType", "editboxtype", editBoxType)
		MEMBER_TYPE_ASSOCIATION("textBoxType", "textboxtype", textBoxType)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(checkboxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("buttonfont", "buttonfont", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("buttontext", "buttontext", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("delayedtooltiptext", "delayedtooltiptext", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("quadtexturesprite", "quadtexturesprite", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("shortcut", "shortcut", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("tooltip", "tooltip", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("tooltiptext", "tooltiptext", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(shieldtype)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("rotation", "rotation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spriteType", "spritetype", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(OverlappingElementsBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("format", "format", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("spacing", "spacing", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(editBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("font", "font", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("text", "text", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("texturefile", "texturefile", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", xy_pair)
		MEMBER_TYPE_ASSOCIATION("size", "size", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(textBoxType)
		MEMBER_ASSOCIATION("orientation", "orientation", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("fixedsize", "fixedsize", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("font", "font", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("format", "format", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("maxheight", "maxheight", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("maxwidth", "maxwidth", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("text", "text", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("texturefile", "texturefile", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("position", "position", xy_pair)
		MEMBER_TYPE_ASSOCIATION("bordersize", "bordersize", xy_pair)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
END_DOMAIN;