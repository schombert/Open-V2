#include <iostream>
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"
#include <map>
#include <set>
#include "object_parsing\\object_parsing.hpp"
#include "fake_fs\\fake_fs.h"
#include "graphics\\v2_window.hpp"
#include "graphics\\test_helpers.h"
#include <string>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class parse_result_holder {
public:
	char* buffer_ptr = nullptr;
	std::vector<token_group> parse_tree;

	parse_result_holder() : buffer_ptr(nullptr) {};
	parse_result_holder(const parse_result_holder& other) = delete;
	parse_result_holder(parse_result_holder&& other) : parse_tree(std::move(other.parse_tree)) { buffer_ptr = other.buffer_ptr; other.buffer_ptr = nullptr; };

	~parse_result_holder() {
		if (buffer_ptr)
			delete[] buffer_ptr;
		buffer_ptr = nullptr;
	}
};

struct mod_description {
	std::string name;
	std::string path;
	std::string user_dir;
	std::vector<std::string> dependencies;
};

MEMBER_DEF(mod_description, name, "name");
MEMBER_DEF(mod_description, path, "path");
MEMBER_DEF(mod_description, user_dir, "user_dir");
MEMBER_DEF(mod_description, dependencies, "dependencies");

struct rebel_types {
	double infantry = 0.0;
	double irregular = 0.0;

	bool operator==(const rebel_types& other) const {
		return (infantry == other.infantry) & (irregular == other.irregular);
	}
};

MEMBER_DEF(rebel_types, infantry, "infantry");
MEMBER_DEF(rebel_types, irregular, "irregular");

enum class special_values {
	none, this_t, from_t
};

struct double_trigger {
	enum class double_trigger_type {
		unknown, unemployment, agree_with_ruling_party, clergymen, consciousness,
		crime_fighting, education_spending, everyday_needs, life_needs, life_rating,
		literacy, luxury_needs, middle_strata_militancy, militancy, political_reform_want,
		poor_strata_luxury_needs, poor_strata_militancy, revanchism, social_reform_want,
		soldiers, war_exhaustion, bureaucrats, capitalists, middle_strata_everyday_needs,
		military_score, plurality, poor_strata_everyday_needs, poor_strata_life_needs
	};
	double_trigger_type type = double_trigger_type::unknown;
	association_type association = association_type::eq_default;
	double value = 0.0;
	special_values special_value = special_values::none;

	bool operator==(const double_trigger& other) const {
		return (type == other.type) & (association == other.association) & (value == other.value) & (special_value == other.special_value);
	}
};

double_trigger double_trigger_from_association(association_type a, const token_and_type& e, double_trigger::double_trigger_type type) {
	if (is_fixed_token_ci(e.start, e.end, "this"))
		return double_trigger{ type, a, 0.0, special_values::this_t };
	else if (is_fixed_token_ci(e.start, e.end, "from"))
		return double_trigger{ type, a, 0.0, special_values::from_t };
	else 
		return double_trigger{ type, a, parse_double(e.start, e.end), special_values::none };
}

struct int_trigger {
	enum class int_trigger_type {
		unknown, total_pops
	};
	int_trigger_type type = int_trigger_type::unknown;
	association_type association = association_type::eq_default;
	int value = 0;
	special_values special_value = special_values::none;

	bool operator==(const int_trigger& other) const {
		return (type == other.type) & (association == other.association) & (value == other.value) & (special_value == other.special_value);
	}
};

int_trigger int_trigger_from_association(association_type a, const token_and_type& e, int_trigger::int_trigger_type type) {
	if (is_fixed_token_ci(e.start, e.end, "this"))
		return int_trigger{ type, a, 0, special_values::this_t };
	else if (is_fixed_token_ci(e.start, e.end, "from"))
		return int_trigger{ type, a, 0, special_values::from_t };
	else
		return int_trigger{ type, a, parse_int(e.start, e.end), special_values::none };
}

struct bool_trigger {
	enum class bool_trigger_type {
		unknown, can_build_factory, civilized, is_accepted_culture, is_capital, is_coastal,
		is_greater_power, is_primary_culture /*or this*/, is_state_religion, is_state_capital,
		war, has_factories, is_colonial, is_slave
	};
	bool_trigger_type type = bool_trigger_type::unknown;
	bool value = false;
	special_values special_value = special_values::none;

	bool operator==(const bool_trigger& other) const {
		return (type == other.type) & (value == other.value) & (special_value == other.special_value);
	}
};

bool_trigger bool_trigger_from_association(association_type, const token_and_type& e, bool_trigger::bool_trigger_type type) {
	if (is_fixed_token_ci(e.start, e.end, "this"))
		return bool_trigger{ type, false, special_values::this_t };
	else if (is_fixed_token_ci(e.start, e.end, "from"))
		return bool_trigger{ type, false, special_values::from_t };
	else
		return bool_trigger{ type, parse_bool(e.start, e.end), special_values::none };
}

struct string_trigger {
	enum class string_trigger_type {
		unknown, continent, has_pop_culture, has_pop_religion, health_care, is_ideology_enabled,
		is_next_reform, nationalvalue, political_parties, press_rights,
		public_meetings, ruling_party_ideology, safety_regulations, school_reforms, terrain,
		vote_franschise, voting_system, wage_reform, work_hours, economic_policy, has_country_flag,
		has_pop_type, religious_policy, slavery, trade_policy
	};
	string_trigger_type type = string_trigger_type::unknown;
	association_type association = association_type::eq_default;
	std::string value;

	bool operator==(const string_trigger& other) const {
		return (type == other.type) & (association == other.association) & (value == other.value);
	}
};

string_trigger string_trigger_from_association(association_type a, const token_and_type& e, string_trigger::string_trigger_type type) {
	return string_trigger{ type, a, std::string(e.start, e.end) };
}

template<typename value_type>
struct value_association_pair {
	association_type& assoc;
	value_type& value;

	value_association_pair(association_type& a, value_type& v) : assoc(a), value(v) {}

	value_association_pair& operator=(const std::pair<association_type, value_type>& in) {
		assoc = in.first;
		value = in.second;
		return *this;
	}
};

struct unemployment_by_type_trigger {
	std::string type;
	double value = 0.0;
	association_type value_association = association_type::eq_default;

	bool operator==(const unemployment_by_type_trigger& other) const {
		return (type == other.type) & (value_association == other.value_association) & (value == other.value);
	}
	auto associated_value() {
		return value_association_pair<double>(value_association, value);
	}
};

MEMBER_DEF(unemployment_by_type_trigger, type, "type");
MEMBER_DEF(unemployment_by_type_trigger, associated_value(), "value");

struct work_available_trigger {
	std::string worker;
	bool operator==(const work_available_trigger& other) const {
		return (worker == other.worker);
	}
};

MEMBER_DEF(work_available_trigger, worker, "worker");

struct trigger_group {
	enum class trigger_group_type {
		and_t, or_t, not_t, any_core, any_neighbor_country, any_owned_province, country, location, state_scope, unemployment_by_type
	};
	trigger_group_type type = trigger_group_type::and_t;
	std::vector<std::variant<double_trigger, int_trigger, bool_trigger, string_trigger, unemployment_by_type_trigger, work_available_trigger, trigger_group>> members;

	bool operator==(const trigger_group& other) const {
		if (type == other.type) {
			return members == other.members;
		}
		return false;
	}
};

void post_process_trigger_group(trigger_group& g, association_type, trigger_group::trigger_group_type type) {
	g.type = type;
}

MEMBER_DEF(trigger_group, members, "trigger");

struct modifier : public trigger_group {
	double factor = 0.0;

	bool operator==(const modifier& other) const {
		return factor == other.factor && trigger_group::operator==(other);
	}
};

MEMBER_DEF(modifier, members, "trigger");
MEMBER_DEF(modifier, factor, "factor");

struct modifier_group {
	std::vector<modifier> members;

	bool operator==(const modifier_group& other) const {
		return members == other.members;
	}
};

MEMBER_DEF(modifier_group, members, "modifier");

struct simple_modifier_container {
	double factor = 0.0;
	std::vector<modifier> members;

	bool operator==(const simple_modifier_container& other) const {
		return factor == other.factor && members == other.members;
	}
};

MEMBER_DEF(simple_modifier_container, members, "modifier");
MEMBER_DEF(simple_modifier_container, factor, "factor");

struct complex_modifier_container {
	double factor = 0.0;
	std::vector<modifier> members;
	std::vector<modifier_group> group_members;

	bool operator==(const complex_modifier_container& other) const {
		return factor == other.factor && members == other.members && group_members == other.group_members;
	}
};

MEMBER_DEF(complex_modifier_container, members, "modifier");
MEMBER_DEF(complex_modifier_container, factor, "factor");
MEMBER_DEF(complex_modifier_container, group_members, "modifier_group");

struct poptype_file {
	uint32_t sprite = 0;
	std::vector<int> color;
	bool is_artisan = false;
	int max_size = 0;
	int merge_max_size = 0;
	std::string strata;

	rebel_types rebel;
	std::vector<std::pair<std::string, double>> luxury_needs;
	std::vector<std::pair<std::string, double>> everyday_needs;
	std::vector<std::pair<std::string, double>> life_needs;

	simple_modifier_container country_migration_target;
	simple_modifier_container migration_target;
	std::vector<std::pair<std::string, complex_modifier_container>> promote_to;
	std::vector<std::pair<std::string, simple_modifier_container>> ideologies;
	std::vector<std::pair<std::string, simple_modifier_container>> issues;

	bool operator==(const poptype_file& other) const {
		return ((sprite == other.sprite) & (is_artisan == other.is_artisan) & (max_size == other.max_size) & (merge_max_size == other.merge_max_size)) &&
			color == other.color && strata == other.strata && rebel == other.rebel && luxury_needs == other.luxury_needs &&
			everyday_needs == other.everyday_needs && life_needs == other.life_needs && country_migration_target == other.country_migration_target &&
			migration_target == other.migration_target && promote_to == other.promote_to && ideologies == other.ideologies && issues == other.issues;
	}
};

MEMBER_DEF(poptype_file, sprite, "sprite");
MEMBER_DEF(poptype_file, color, "color");
MEMBER_DEF(poptype_file, is_artisan, "is_artisan");
MEMBER_DEF(poptype_file, max_size, "max_size");
MEMBER_DEF(poptype_file, merge_max_size, "merge_max_size");
MEMBER_DEF(poptype_file, strata, "strata");
MEMBER_DEF(poptype_file, rebel, "rebel");

MEMBER_DEF(poptype_file, luxury_needs, "luxury_needs");
MEMBER_DEF(poptype_file, everyday_needs, "everyday_needs");
MEMBER_DEF(poptype_file, life_needs, "life_needs");

MEMBER_DEF(poptype_file, country_migration_target, "country_migration_target");
MEMBER_DEF(poptype_file, migration_target, "migration_target");
MEMBER_DEF(poptype_file, promote_to, "promote_to");
MEMBER_DEF(poptype_file, ideologies, "ideologies");
MEMBER_DEF(poptype_file, issues, "issues");


double double_from_association(association_type, const token_and_type& t) {
	return parse_double(t.start, t.end);
};

uint32_t uint_from_association(association_type, const token_and_type& t) {
	return parse_uint(t.start, t.end);
};

int int_from_association(association_type, const token_and_type& t) {
	return parse_int(t.start, t.end);
};

int int_from_full_association(const token_and_type& t, association_type, const token_and_type&) {
	return parse_int(t.start, t.end);
};

bool bool_from_association(association_type, const token_and_type& t) {
	return parse_bool(t.start, t.end);
};

std::string string_from_association(association_type, const token_and_type& t) {
	return std::string(t.start, t.end);
};

std::pair<association_type, double> double_and_association(association_type a, const token_and_type& t) {
	return std::make_pair(a, parse_double(t.start, t.end));
};

bool accept_all(const char*, const char*) {
	return true;
}

std::pair<std::string, double> string_double_from_full_association(const token_and_type& t, association_type, const token_and_type& e) {
	return make_pair(std::string(t.start, t.end), parse_double(e.start, e.end));
};

std::pair<std::string, complex_modifier_container> label_complex_container(const token_and_type& a, association_type, complex_modifier_container& c) {
	return std::pair<std::string, complex_modifier_container>(std::string(a.start, a.end), std::move(c));
}

std::pair<std::string, simple_modifier_container> label_simple_container(const token_and_type& a, association_type, simple_modifier_container& c) {
	return std::pair<std::string, simple_modifier_container>(std::string(a.start, a.end), std::move(c));
}


using vec_str_double = std::vector<std::pair<std::string, double>>;
using vec_str_simple = std::vector<std::pair<std::string, simple_modifier_container>>;
using vec_str_complex = std::vector<std::pair<std::string, complex_modifier_container>>;

/*
MEMBER_DEF(mod_description, name, "name");
MEMBER_DEF(mod_description, path, "path");
MEMBER_DEF(mod_description, user_dir, "user_dir");
MEMBER_DEF(mod_description, dependencies, "dependencies");
*/

struct empty_type {

};

std::string label_empty_type(const token_and_type& a, association_type, empty_type&) {
	return std::string(a.start, a.end);
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

struct guiButtonType {
	static std::set<std::string> unknown_keys;

	static std::vector<guiButtonType> all_buttons;
	static void add_global(guiButtonType&& in) { all_buttons.emplace_back(std::move(in)); }

	std::string name;
	std::string quadTextureSprite;
	std::string buttonText; // should be int from global test key
	std::string buttonFont; // should be id from font table
	std::string Orientation;
	std::string clicksound;

	std::string delayedtooltipText;
	std::string format;
	xy_pair position;
	std::string rotation;
	std::string shortcut;
	xy_pair size;
	std::string spriteType;
	std::string tooltip;
	std::string tooltipText;

	discard_int discard_member() { return discard_int(); }
	add_string_to_set add_unknown_key() { return add_string_to_set(unknown_keys); };
};

std::vector<guiButtonType> guiButtonType::all_buttons;
std::set<std::string> guiButtonType::unknown_keys;

MEMBER_DEF(guiButtonType, name, "name");
MEMBER_DEF(guiButtonType, quadTextureSprite, "quadTextureSprite");
MEMBER_DEF(guiButtonType, buttonText, "buttonText");
MEMBER_DEF(guiButtonType, buttonFont, "buttonFont");
MEMBER_DEF(guiButtonType, Orientation, "Orientation");
MEMBER_DEF(guiButtonType, clicksound, "clicksound");
MEMBER_DEF(guiButtonType, delayedtooltipText, "delayedtooltipText");
MEMBER_DEF(guiButtonType, format, "format");
MEMBER_DEF(guiButtonType, position, "position");
MEMBER_DEF(guiButtonType, rotation, "rotation");
MEMBER_DEF(guiButtonType, shortcut, "shortcut");
MEMBER_DEF(guiButtonType, size, "size");
MEMBER_DEF(guiButtonType, spriteType, "spriteType");
MEMBER_DEF(guiButtonType, tooltip, "tooltip");
MEMBER_DEF(guiButtonType, tooltipText, "tooltipText");
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
		MEMBER_ASSOCIATION("delayedtooltipText", "delayedtooltiptext", value_from_rh<std::string>)
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

/*
BEGIN_DOMAIN(mod_file_domain)
    BEGIN_TYPE(std::vector<std::string>)
	    MEMBER_VARIABLE_ASSOCIATION("this", accept_all, value_from_lh<std::string>)
	END_TYPE
	BEGIN_TYPE(mod_description)
		MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("path", "path", value_from_rh<std::string>)
		MEMBER_ASSOCIATION("user_dir", "user_dir", value_from_rh<std::string>)
		MEMBER_TYPE_ASSOCIATION("dependencies", "dependencies", std::vector<std::string>)
	END_TYPE
END_DOMAIN;
*/

/*
BEGIN_DOMAIN(poptype_file_domain)
	BEGIN_TYPE(rebel_types)
		MEMBER_ASSOCIATION("infantry", "infantry", double_from_association)
		MEMBER_ASSOCIATION("irregular", "irregular", double_from_association)
	END_TYPE
	BEGIN_TYPE(unemployment_by_type_trigger)
		MEMBER_ASSOCIATION("value", "value", double_and_association)
		MEMBER_ASSOCIATION("type", "type", string_from_association)
	END_TYPE
	BEGIN_TYPE(work_available_trigger)
	    MEMBER_ASSOCIATION("worker", "worker", string_from_association)
	END_TYPE
	BEGIN_TYPE(trigger_group)
	    MEMBER_ASSOCIATION_1("trigger", "voting_system", string_trigger_from_association, string_trigger::string_trigger_type::voting_system)
	    MEMBER_ASSOCIATION_1("trigger", "wage_reform", string_trigger_from_association, string_trigger::string_trigger_type::wage_reform)
		MEMBER_ASSOCIATION_1("trigger", "work_hours", string_trigger_from_association, string_trigger::string_trigger_type::work_hours)
		MEMBER_ASSOCIATION_1("trigger", "vote_franschise", string_trigger_from_association, string_trigger::string_trigger_type::vote_franschise)
		MEMBER_ASSOCIATION_1("trigger", "terrain", string_trigger_from_association, string_trigger::string_trigger_type::terrain)
		MEMBER_ASSOCIATION_1("trigger", "school_reforms", string_trigger_from_association, string_trigger::string_trigger_type::school_reforms)
		MEMBER_ASSOCIATION_1("trigger", "safety_regulations", string_trigger_from_association, string_trigger::string_trigger_type::safety_regulations)
		MEMBER_ASSOCIATION_1("trigger", "ruling_party_ideology", string_trigger_from_association, string_trigger::string_trigger_type::ruling_party_ideology)
		MEMBER_ASSOCIATION_1("trigger", "public_meetings", string_trigger_from_association, string_trigger::string_trigger_type::public_meetings)
		MEMBER_ASSOCIATION_1("trigger", "press_rights", string_trigger_from_association, string_trigger::string_trigger_type::press_rights)
		MEMBER_ASSOCIATION_1("trigger", "political_parties", string_trigger_from_association, string_trigger::string_trigger_type::political_parties)
		MEMBER_ASSOCIATION_1("trigger", "nationalvalue", string_trigger_from_association, string_trigger::string_trigger_type::nationalvalue)
		MEMBER_ASSOCIATION_1("trigger", "is_next_reform", string_trigger_from_association, string_trigger::string_trigger_type::is_next_reform)
		MEMBER_ASSOCIATION_1("trigger", "is_ideology_enabled", string_trigger_from_association, string_trigger::string_trigger_type::is_ideology_enabled)
		MEMBER_ASSOCIATION_1("trigger", "health_care", string_trigger_from_association, string_trigger::string_trigger_type::health_care)
		MEMBER_ASSOCIATION_1("trigger", "has_pop_culture", string_trigger_from_association, string_trigger::string_trigger_type::has_pop_culture)
		MEMBER_ASSOCIATION_1("trigger", "has_pop_religion", string_trigger_from_association, string_trigger::string_trigger_type::has_pop_religion)
		MEMBER_ASSOCIATION_1("trigger", "continent", string_trigger_from_association, string_trigger::string_trigger_type::continent)
		MEMBER_ASSOCIATION_1("trigger", "economic_policy", string_trigger_from_association, string_trigger::string_trigger_type::economic_policy)
		MEMBER_ASSOCIATION_1("trigger", "has_country_flag", string_trigger_from_association, string_trigger::string_trigger_type::has_country_flag)
		MEMBER_ASSOCIATION_1("trigger", "has_pop_type", string_trigger_from_association, string_trigger::string_trigger_type::has_pop_type)
		MEMBER_ASSOCIATION_1("trigger", "religious_policy", string_trigger_from_association, string_trigger::string_trigger_type::religious_policy)
		MEMBER_ASSOCIATION_1("trigger", "slavery", string_trigger_from_association, string_trigger::string_trigger_type::slavery)
		MEMBER_ASSOCIATION_1("trigger", "trade_policy", string_trigger_from_association, string_trigger::string_trigger_type::trade_policy)
		MEMBER_ASSOCIATION_1("trigger", "is_primary_culture", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_primary_culture)
		MEMBER_ASSOCIATION_1("trigger", "can_build_factory", bool_trigger_from_association, bool_trigger::bool_trigger_type::can_build_factory)
		MEMBER_ASSOCIATION_1("trigger", "civilized", bool_trigger_from_association, bool_trigger::bool_trigger_type::civilized)
		MEMBER_ASSOCIATION_1("trigger", "is_accepted_culture", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_accepted_culture)
		MEMBER_ASSOCIATION_1("trigger", "is_capital", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_capital)
		MEMBER_ASSOCIATION_1("trigger", "is_coastal", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_coastal)
		MEMBER_ASSOCIATION_1("trigger", "is_greater_power", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_greater_power)
		MEMBER_ASSOCIATION_1("trigger", "is_state_religion", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_state_religion)
		MEMBER_ASSOCIATION_1("trigger", "is_state_capital", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_state_capital)
		MEMBER_ASSOCIATION_1("trigger", "war", bool_trigger_from_association, bool_trigger::bool_trigger_type::war)
		MEMBER_ASSOCIATION_1("trigger", "has_factories", bool_trigger_from_association, bool_trigger::bool_trigger_type::has_factories)
		MEMBER_ASSOCIATION_1("trigger", "is_colonial", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_colonial)
		MEMBER_ASSOCIATION_1("trigger", "is_slave", bool_trigger_from_association, bool_trigger::bool_trigger_type::is_slave)
		MEMBER_ASSOCIATION_1("trigger", "total_pops", int_trigger_from_association, int_trigger::int_trigger_type::total_pops)
		MEMBER_ASSOCIATION_1("trigger", "unemployment", double_trigger_from_association, double_trigger::double_trigger_type::unemployment)
		MEMBER_ASSOCIATION_1("trigger", "soldiers", double_trigger_from_association, double_trigger::double_trigger_type::soldiers)
		MEMBER_ASSOCIATION_1("trigger", "war_exhaustion", double_trigger_from_association, double_trigger::double_trigger_type::war_exhaustion)
		MEMBER_ASSOCIATION_1("trigger", "poor_strata_luxury_needs", double_trigger_from_association, double_trigger::double_trigger_type::poor_strata_luxury_needs)
		MEMBER_ASSOCIATION_1("trigger", "poor_strata_militancy", double_trigger_from_association, double_trigger::double_trigger_type::poor_strata_militancy)
		MEMBER_ASSOCIATION_1("trigger", "revanchism", double_trigger_from_association, double_trigger::double_trigger_type::revanchism)
		MEMBER_ASSOCIATION_1("trigger", "social_reform_want", double_trigger_from_association, double_trigger::double_trigger_type::social_reform_want)
		MEMBER_ASSOCIATION_1("trigger", "political_reform_want", double_trigger_from_association, double_trigger::double_trigger_type::political_reform_want)
		MEMBER_ASSOCIATION_1("trigger", "militancy", double_trigger_from_association, double_trigger::double_trigger_type::militancy)
		MEMBER_ASSOCIATION_1("trigger", "middle_strata_militancy", double_trigger_from_association, double_trigger::double_trigger_type::middle_strata_militancy)
		MEMBER_ASSOCIATION_1("trigger", "luxury_needs", double_trigger_from_association, double_trigger::double_trigger_type::luxury_needs)
		MEMBER_ASSOCIATION_1("trigger", "literacy", double_trigger_from_association, double_trigger::double_trigger_type::literacy)
		MEMBER_ASSOCIATION_1("trigger", "life_rating", double_trigger_from_association, double_trigger::double_trigger_type::life_rating)
		MEMBER_ASSOCIATION_1("trigger", "life_needs", double_trigger_from_association, double_trigger::double_trigger_type::life_needs)
		MEMBER_ASSOCIATION_1("trigger", "everyday_needs", double_trigger_from_association, double_trigger::double_trigger_type::everyday_needs)
		MEMBER_ASSOCIATION_1("trigger", "education_spending", double_trigger_from_association, double_trigger::double_trigger_type::education_spending)
		MEMBER_ASSOCIATION_1("trigger", "crime_fighting", double_trigger_from_association, double_trigger::double_trigger_type::crime_fighting)
		MEMBER_ASSOCIATION_1("trigger", "consciousness", double_trigger_from_association, double_trigger::double_trigger_type::consciousness)
		MEMBER_ASSOCIATION_1("trigger", "clergymen", double_trigger_from_association, double_trigger::double_trigger_type::clergymen)
		MEMBER_ASSOCIATION_1("trigger", "agree_with_ruling_party", double_trigger_from_association, double_trigger::double_trigger_type::agree_with_ruling_party)
		MEMBER_ASSOCIATION_1("trigger", "bureaucrats", double_trigger_from_association, double_trigger::double_trigger_type::bureaucrats)
		MEMBER_ASSOCIATION_1("trigger", "capitalists", double_trigger_from_association, double_trigger::double_trigger_type::capitalists)
		MEMBER_ASSOCIATION_1("trigger", "middle_strata_everyday_needs", double_trigger_from_association, double_trigger::double_trigger_type::middle_strata_everyday_needs)
		MEMBER_ASSOCIATION_1("trigger", "military_score", double_trigger_from_association, double_trigger::double_trigger_type::military_score)
		MEMBER_ASSOCIATION_1("trigger", "plurality", double_trigger_from_association, double_trigger::double_trigger_type::plurality)
		MEMBER_ASSOCIATION_1("trigger", "poor_strata_everyday_needs", double_trigger_from_association, double_trigger::double_trigger_type::poor_strata_everyday_needs)
		MEMBER_ASSOCIATION_1("trigger", "poor_strata_life_needs", double_trigger_from_association, double_trigger::double_trigger_type::poor_strata_life_needs)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "and", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::and_t)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "or", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::or_t)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "not", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::not_t)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "any_core", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::any_core)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "any_neighbor_country", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::any_neighbor_country)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "any_owned_province", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::any_owned_province)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "country", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::country)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "location", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::location)
		MEMBER_TYPE_ASSOCIATION_EX1("trigger", "state_scope", trigger_group, post_process_trigger_group, trigger_group::trigger_group_type::state_scope)
		MEMBER_TYPE_ASSOCIATION("trigger", "unemployment_by_type", unemployment_by_type_trigger)
		MEMBER_TYPE_ASSOCIATION("trigger", "work_available", work_available_trigger)
	END_TYPE
	BEGIN_TYPE(modifier)
		MEMBER_ASSOCIATION("factor", "factor", double_from_association)
		INHERIT_FROM(trigger_group)
	END_TYPE
	BEGIN_TYPE(modifier_group)
	    MEMBER_TYPE_ASSOCIATION("modifier", "modifier", modifier)
	END_TYPE
	BEGIN_TYPE(simple_modifier_container)
	    MEMBER_ASSOCIATION("factor", "factor", double_from_association)
	    MEMBER_TYPE_ASSOCIATION("modifier", "modifier", modifier)
	END_TYPE
	BEGIN_TYPE(complex_modifier_container)
		MEMBER_ASSOCIATION("factor", "factor", double_from_association)
		MEMBER_TYPE_ASSOCIATION("modifier", "modifier", modifier)
		MEMBER_TYPE_ASSOCIATION("modifier_group", "group", modifier_group)
	END_TYPE
	BEGIN_TYPE(vec_str_double)
		MEMBER_VARIABLE_ASSOCIATION("this", accept_all, string_double_from_full_association)
	END_TYPE
	BEGIN_TYPE(std::vector<int>)
		MEMBER_VARIABLE_ASSOCIATION("this", accept_all, int_from_full_association)
	END_TYPE
	BEGIN_TYPE(vec_str_simple)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("this", accept_all, simple_modifier_container, label_simple_container)
	END_TYPE
	BEGIN_TYPE(vec_str_complex)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("this", accept_all, complex_modifier_container, label_complex_container)
	END_TYPE
	BEGIN_TYPE(poptype_file)
		MEMBER_ASSOCIATION("sprite", "sprite", uint_from_association)
		MEMBER_TYPE_ASSOCIATION("color", "color", std::vector<int>)
		MEMBER_ASSOCIATION("is_artisan", "is_artisan", bool_from_association)
		MEMBER_ASSOCIATION("max_size", "max_size", int_from_association)
		MEMBER_ASSOCIATION("merge_max_size", "merge_max_size", int_from_association)
		MEMBER_ASSOCIATION("strata", "strata", string_from_association)
		MEMBER_TYPE_ASSOCIATION("rebel", "rebel", rebel_types)
		MEMBER_TYPE_ASSOCIATION("luxury_needs", "luxury_needs", vec_str_double)
		MEMBER_TYPE_ASSOCIATION("everyday_needs", "everyday_needs", vec_str_double)
		MEMBER_TYPE_ASSOCIATION("life_needs", "life_needs", vec_str_double)
		MEMBER_TYPE_ASSOCIATION("country_migration_target", "country_migration_target", simple_modifier_container)
		MEMBER_TYPE_ASSOCIATION("migration_target", "migration_target", simple_modifier_container)
		MEMBER_TYPE_ASSOCIATION("promote_to", "promote_to", vec_str_complex)
		MEMBER_TYPE_ASSOCIATION("ideologies", "ideologies", vec_str_simple)
		MEMBER_TYPE_ASSOCIATION("issues", "issues", vec_str_simple)
	END_TYPE
END_DOMAIN;
*/

struct unknown_type {
	std::set<std::string> subtypes;
	std::map<std::string, std::string> associations;
	std::string free_value;

	void print_type() {
		if (subtypes.size() != 0) {
			std::cout << "\tsubtypes:" << std::endl;
			for(const auto& s: subtypes)
				std::cout << "\t\t" << s << std::endl;
		}
		if (associations.size() != 0) {
			std::cout << "\tassociations:" << std::endl;
			for (const auto& v : associations)
				std::cout << "\t\t" << v.first << " = " << v.second << std::endl;
		}
		if (free_value.length() != 0) {
			std::cout << "\tfree value:" << free_value << std::endl;
		}
	}
};

struct empty_window_handler {
	texture test_tex;

	font test_fallback;
	font test_font;
	
	empty_window_handler() :
		//test_tex("F:\\VS2007Projects\\open_v2_test_data\\army_icon_2.dds"),
		test_tex("F:\\VS2007Projects\\open_v2_test_data\\test_tx.bmp"),
		test_fallback("F:\\VS2007Projects\\open_v2_test_data\\unifont-9.0.02.ttf"),
		//test_font("F:\\VS2007Projects\\open_v2_test_data\\Primitive.ttf", test_fallback) {}
		test_font("F:\\VS2007Projects\\open_v2_test_data\\CreteRound-Regular.otf", test_fallback) {}

	template<typename T>
	void operator()(T&&, window_base& w) const {
		// do nothing;
	}
	void initialize_graphics(open_gl_wrapper& ogl) {
		test_fallback.load_font(ogl);
		test_font.load_font(ogl);
	}

	void render(open_gl_wrapper& ogl) {
		//ogl.render_textured_rect(true, 100.0f, 74.0f, 10.0f, 50.0f, test_tex);

		//ogl.render_textured_rect(true, 50.0f, 150.0f, 80.0f, 40.0f, test_tex);
		//ogl.render_textured_rect(false, 50.0f, 200.0f, 80.0f, 80.0f, test_tex);

		//
		ogl.render_textured_rect(true, 0.0f, 0.0f, 80.0f, 40.0f, test_tex);
		//ogl.render_outlined_text(u"明Tast", 5, true, 80.0f, 40.0f, 16.0f, color{ 1.0f,1.0f,1.0f }, test_font);

		ogl.render_outlined_text(u"明Test", 5, true, 80.0f, 40.0f, 16.0f, color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(u"明Tast", 5, true, 80.0f, 56.0f, 16.0f, color{ 1.0f,1.0f,1.0f }, test_font);

		ogl.render_character(u'T', true, 26.0f, 60.0f, 16.0f, test_font);

		ogl.render_character(u'a', true, 10.0f, 10.0f, 16.0f, test_font);

		//ogl.render_character(u'A', true, 26.0f, 10.0f, 64.0f, test_font);
		ogl.render_character(u'y', true, 80.0f, 60.0f, 128.0f, test_font);
		ogl.render_character(u'A', true, 10.0f, 138.0f, 512.0f, test_font);
	}
};

int __cdecl main() {
	/*{
		window<empty_window_handler> test_window(400, 400);


		std::cout << "test window created" << std::endl;
		getchar();

		test_window.close_window();
	}*/
	{
		file_system fs;
		fs.set_root(u"F:\\programs\\V2\\interface");
		//fs.add_root(u"F:\\programs\\V2\\mod\\HPM\\interface");
		const auto gui_files = fs.get_root().list_files(u".gui");

		std::vector<token_group> parse_tree;

		std::cout << "files count: " << gui_files.size() << std::endl;

		for (const auto& f : gui_files) {
			auto open_f = f.open_file();
			if (open_f) {
				const auto sz = open_f->size();
				auto buffer = new char[sz];

				open_f->read_to_buffer(buffer, sz);
				
				parse_tree.clear();
				parse_pdx_file(parse_tree, buffer, buffer + sz);

				if(parse_tree.size() > 0)
					gui_file g = parse_object<gui_file, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size());

				delete[] buffer;
			}
		}


		std::set<std::string> entries;
		for (auto& i : textBoxType::all_items) {
			entries.insert(i.texturefile);
		}

		std::cout << "checkboxType.XXXX entries: " << std::endl;

		for (auto& k : entries) {
			std::cout << k << std::endl;
		}

		std::cout << "guiButtonType count: " << guiButtonType::all_buttons.size() << std::endl;

		std::cout << "iconType count: " << iconType::all_items.size() << std::endl;
		std::cout << "iconType unknown keys:" << std::endl;
		for (auto& k : iconType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "eu3dialogtype count: " << eu3dialogtype::all_items.size() << std::endl;
		std::cout << "eu3dialogtype unknown keys:" << std::endl;
		for (auto& k : eu3dialogtype::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "instantTextBoxType count: " << instantTextBoxType::all_items.size() << std::endl;
		std::cout << "instantTextBoxType unknown keys:" << std::endl;
		for (auto& k : instantTextBoxType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "listBoxType count: " << listBoxType::all_items.size() << std::endl;
		std::cout << "listBoxType unknown keys:" << std::endl;
		for (auto& k : listBoxType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "positionType count: " << positionType::all_items.size() << std::endl;
		std::cout << "positionType unknown keys:" << std::endl;
		for (auto& k : positionType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "scrollbarType count: " << scrollbarType::all_items.size() << std::endl;
		std::cout << "scrollbarType unknown keys:" << std::endl;
		for (auto& k : scrollbarType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "windowType count: " << windowType::all_items.size() << std::endl;
		std::cout << "windowType unknown keys:" << std::endl;
		for (auto& k : windowType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "checkboxType count: " << checkboxType::all_items.size() << std::endl;
		std::cout << "checkboxType unknown keys:" << std::endl;
		for (auto& k : checkboxType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "shieldtype count: " << shieldtype::all_items.size() << std::endl;
		std::cout << "shieldtype unknown keys:" << std::endl;
		for (auto& k : shieldtype::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "OverlappingElementsBoxType count: " << OverlappingElementsBoxType::all_items.size() << std::endl;
		std::cout << "OverlappingElementsBoxType unknown keys:" << std::endl;
		for (auto& k : OverlappingElementsBoxType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "editBoxType count: " << editBoxType::all_items.size() << std::endl;
		std::cout << "editBoxType unknown keys:" << std::endl;
		for (auto& k : editBoxType::unknown_keys) {
			std::cout << k << std::endl;
		}
		std::cout << "textBoxType count: " << textBoxType::all_items.size() << std::endl;
		std::cout << "textBoxType unknown keys:" << std::endl;
		for (auto& k : textBoxType::unknown_keys) {
			std::cout << k << std::endl;
		}


		std::cout << std::endl << "gui_file unknown keys:" << std::endl;
		for (auto& k : gui_file::unknown_keys) {
			std::cout << k << std::endl;
		}
	}

	/*{
		file_system fs;
		fs.set_root(u"F:\\programs\\V2\\mod");


		const auto mod_files = fs.get_root().list_files(u".mod");
		std::vector<parse_result_holder> all_parse_results;
		for (const auto& f : mod_files) {
			auto open_f = f.open_file();

			if (open_f) {
				//	std::cout << "mod file opened ok" << std::endl;

				all_parse_results.push_back(parse_result_holder());
				if (all_parse_results.size() > 0) {
					parse_result_holder& new_back = all_parse_results[all_parse_results.size() - 1];
					const auto sz = open_f->size();
					new_back.buffer_ptr = new char[sz];
					open_f->read_to_buffer(new_back.buffer_ptr, sz);

					parse_pdx_file(new_back.parse_tree, new_back.buffer_ptr, new_back.buffer_ptr + sz);
				}
			}
		}

		std::vector<mod_description> mod_contents;

		for (auto& f : all_parse_results) {
			if (f.parse_tree.size() != 0) {
				//mod_contents.emplace_back(mod_file_parser(&f.parse_tree[0], &f.parse_tree[0] + f.parse_tree.size()));
				mod_contents.emplace_back(parse_object<mod_description, mod_file_domain>(&f.parse_tree[0], &f.parse_tree[0] + f.parse_tree.size()));
			}
		}

		std::cout << all_parse_results.size() << " files parsed" << std::endl;
		for (const auto& kp : mod_contents) {
			std::cout << "name: " << kp.name << std::endl;
			std::cout << "\tpath: " << kp.path << std::endl;
			std::cout << "\tuser_dir: " << kp.user_dir << std::endl;
			std::cout << "\tdependencies: " << std::endl;
			for (const auto& val : kp.dependencies) {
				std::cout << "\t\t" << val << std::endl;
			}
		}
	} */

	{
		/*
		std::map<std::string, unknown_type> unknown_types;
		
		file_system fs;
		fs.set_root(u"F:\\programs\\V2\\poptypes");

		const char fname[] = "artisans.txt";
		auto artisans = fs.get_root().open_file(fname, fname + sizeof(fname) - 1);

		if (artisans) {
			const auto file_size = artisans->size();
			char* buffer = new char[file_size];
			artisans->read_to_buffer(buffer, file_size);

			
			poptype_file destination_c;
			std::vector<token_group> file_pdx_parse;
			parse_pdx_file(file_pdx_parse, buffer, buffer + file_size);
			destination_c = parse_object<poptype_file, poptype_file_domain>(&file_pdx_parse[0], &file_pdx_parse[0] + file_pdx_parse.size());
			delete[] buffer;

			std::cout << "unknown types: " << std::endl << std::endl;
			for (auto& t : unknown_types) {
				std::cout << t.first << ":" << std::endl;
				t.second.print_type();
				getchar();
			}

		} else {
			std::cout << "could not open artisans.txt" << std::endl;
		}
		*/
	}
	getchar();

    return 0;
}

