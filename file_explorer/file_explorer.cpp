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
#include "gui_definitions\\gui_definitions.h"
#include "graphics_objects\\graphics_objects.h"
#include "text_data\\text_data.h" 
#include "gui\\gui.hpp"

// #define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class parse_result_holder {
public:
	char* buffer_ptr = nullptr;
	std::vector<token_group> parse_tree;

	parse_result_holder() : buffer_ptr(nullptr) {}
	parse_result_holder(const parse_result_holder& other) = delete;
	parse_result_holder(parse_result_holder&& other) noexcept : parse_tree(std::move(other.parse_tree)) { buffer_ptr = other.buffer_ptr; other.buffer_ptr = nullptr; }

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

struct work_available_trigger {
	std::string worker;
	bool operator==(const work_available_trigger& other) const {
		return (worker == other.worker);
	}
};

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

struct modifier : public trigger_group {
	double factor = 0.0;

	bool operator==(const modifier& other) const {
		return factor == other.factor && trigger_group::operator==(other);
	}
};

struct modifier_group {
	std::vector<modifier> members;

	bool operator==(const modifier_group& other) const {
		return members == other.members;
	}
};

struct simple_modifier_container {
	double factor = 0.0;
	std::vector<modifier> members;

	bool operator==(const simple_modifier_container& other) const {
		return factor == other.factor && members == other.members;
	}
};

struct complex_modifier_container {
	double factor = 0.0;
	std::vector<modifier> members;
	std::vector<modifier_group> group_members;

	bool operator==(const complex_modifier_container& other) const {
		return factor == other.factor && members == other.members && group_members == other.group_members;
	}
};

bool_trigger bool_trigger_from_association(association_type, const token_and_type& e, bool_trigger::bool_trigger_type type);
double_trigger double_trigger_from_association(association_type a, const token_and_type& e, double_trigger::double_trigger_type type);
int_trigger int_trigger_from_association(association_type a, const token_and_type& e, int_trigger::int_trigger_type type);
string_trigger string_trigger_from_association(association_type a, const token_and_type& e, string_trigger::string_trigger_type type);
void post_process_trigger_group(trigger_group& g, association_type, trigger_group::trigger_group_type type);
double double_from_association(association_type, const token_and_type& t);
uint32_t uint_from_association(association_type, const token_and_type& t);
int int_from_association(association_type, const token_and_type& t);
int int_from_full_association(const token_and_type& t, association_type, const token_and_type&);
bool bool_from_association(association_type, const token_and_type& t);
std::string string_from_association(association_type, const token_and_type& t);
std::pair<association_type, double> double_and_association(association_type a, const token_and_type& t);
std::pair<std::string, double> string_double_from_full_association(const token_and_type& t, association_type, const token_and_type& e);
std::pair<std::string, complex_modifier_container> label_complex_container(const token_and_type& a, association_type, complex_modifier_container& c);
std::pair<std::string, simple_modifier_container> label_simple_container(const token_and_type& a, association_type, simple_modifier_container& c);

double_trigger double_trigger_from_association(association_type a, const token_and_type& e, double_trigger::double_trigger_type type) {
	if (is_fixed_token_ci(e.start, e.end, "this"))
		return double_trigger{ type, a, 0.0, special_values::this_t };
	else if (is_fixed_token_ci(e.start, e.end, "from"))
		return double_trigger{ type, a, 0.0, special_values::from_t };
	else
		return double_trigger{ type, a, parse_double(e.start, e.end), special_values::none };
}

int_trigger int_trigger_from_association(association_type a, const token_and_type& e, int_trigger::int_trigger_type type) {
	if (is_fixed_token_ci(e.start, e.end, "this"))
		return int_trigger{ type, a, 0, special_values::this_t };
	else if (is_fixed_token_ci(e.start, e.end, "from"))
		return int_trigger{ type, a, 0, special_values::from_t };
	else
		return int_trigger{ type, a, parse_int(e.start, e.end), special_values::none };
}

bool_trigger bool_trigger_from_association(association_type, const token_and_type& e, bool_trigger::bool_trigger_type type) {
	if (is_fixed_token_ci(e.start, e.end, "this"))
		return bool_trigger{ type, false, special_values::this_t };
	else if (is_fixed_token_ci(e.start, e.end, "from"))
		return bool_trigger{ type, false, special_values::from_t };
	else
		return bool_trigger{ type, parse_bool(e.start, e.end), special_values::none };
}

string_trigger string_trigger_from_association(association_type a, const token_and_type& e, string_trigger::string_trigger_type type) {
	return string_trigger{ type, a, std::string(e.start, e.end) };
}




MEMBER_DEF(unemployment_by_type_trigger, type, "type");
MEMBER_DEF(unemployment_by_type_trigger, associated_value(), "value");


MEMBER_DEF(work_available_trigger, worker, "worker");


void post_process_trigger_group(trigger_group& g, association_type, trigger_group::trigger_group_type type) {
	g.type = type;
}

MEMBER_DEF(trigger_group, members, "trigger");

MEMBER_DEF(modifier, members, "trigger");
MEMBER_DEF(modifier, factor, "factor");

MEMBER_DEF(modifier_group, members, "modifier");

MEMBER_DEF(simple_modifier_container, members, "modifier");
MEMBER_DEF(simple_modifier_container, factor, "factor");

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
struct empty_type {

};

std::string label_empty_type(const token_and_type& a, association_type, empty_type&) {
	return std::string(a.start, a.end);
}

struct gfx_container {
	std::set<std::string> unknown_gfx_keys;
	std::set<std::string> unknown_bitmapfonts_keys;
	std::set<std::string> unknown_fonts_keys;
	std::set<std::string> unknown_lighttypes_keys;
	std::set<std::string> unknown_objecttypes_keys;
	std::set<std::string> unknown_spritetypes_keys;
};


struct discard_type {
	void any(int) {}
};

int discard_to_int(const token_and_type&, association_type, const discard_type&) {
	return 0;
}
std::string label_discard_type(const token_and_type& a, association_type, const discard_type&) {
	return std::string(a.start, a.end);
}


MEMBER_FDEF(discard_type, any, "any");

struct barcharttype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	barcharttype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> barcharttype::keys;
std::set<std::string> barcharttype::unknown_types;

MEMBER_FDEF(barcharttype, key_value, "key-value");
MEMBER_FDEF(barcharttype, unknown_t, "unknown_type");

struct linecharttype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	linecharttype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> linecharttype::keys;
std::set<std::string> linecharttype::unknown_types;

MEMBER_FDEF(linecharttype, key_value, "key-value");
MEMBER_FDEF(linecharttype, unknown_t, "unknown_type");

struct piecharttype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	piecharttype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> piecharttype::keys;
std::set<std::string> piecharttype::unknown_types;

MEMBER_FDEF(piecharttype, key_value, "key-value");
MEMBER_FDEF(piecharttype, unknown_t, "unknown_type");

struct tilespritetype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	tilespritetype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> tilespritetype::keys;
std::set<std::string> tilespritetype::unknown_types;

MEMBER_FDEF(tilespritetype, key_value, "key-value");
MEMBER_FDEF(tilespritetype, unknown_t, "unknown_type");

struct textspritetype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	textspritetype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> textspritetype::keys;
std::set<std::string> textspritetype::unknown_types;

MEMBER_FDEF(textspritetype, key_value, "key-value");
MEMBER_FDEF(textspritetype, unknown_t, "unknown_type");

struct progressbartype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	progressbartype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> progressbartype::keys;
std::set<std::string> progressbartype::unknown_types;

MEMBER_FDEF(progressbartype, key_value, "key-value");
MEMBER_FDEF(progressbartype, unknown_t, "unknown_type");

struct maskedshieldtype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	maskedshieldtype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> maskedshieldtype::keys;
std::set<std::string> maskedshieldtype::unknown_types;

MEMBER_FDEF(maskedshieldtype, key_value, "key-value");
MEMBER_FDEF(maskedshieldtype, unknown_t, "unknown_type");

struct corneredtilespritetype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	corneredtilespritetype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> corneredtilespritetype::keys;
std::set<std::string> corneredtilespritetype::unknown_types;

MEMBER_FDEF(corneredtilespritetype, key_value, "key-value");
MEMBER_FDEF(corneredtilespritetype, unknown_t, "unknown_type");

struct spritetype {
	static std::map<std::string, std::set<std::string>> keys;
	static std::set<std::string> unknown_types;

	spritetype() {}

	void key_value(std::pair<std::string, std::string>&& p) {
		keys[std::move(p.first)].insert(std::move(p.second));
	}
	void unknown_t(std::string&& s) {
		unknown_types.insert(std::move(s));
	}
};

std::map<std::string, std::set<std::string>> spritetype::keys;
std::set<std::string> spritetype::unknown_types;

MEMBER_FDEF(spritetype, key_value, "key-value");
MEMBER_FDEF(spritetype, unknown_t, "unknown_type");

struct spritetypes {
	gfx_container& g;
	spritetypes(gfx_container* h) : g(*h) {}

	void sprite(const spritetype&) {};
	void corneredtilesprite(const corneredtilespritetype&) {};
	void textsprite(const textspritetype&) {};
	void progressbar(const progressbartype&) {};
	void maskedshield(const maskedshieldtype&) {};
	void tilesprite(const tilespritetype&) {};
	void barchart(const barcharttype&) { };
	void piechart(const piecharttype&) {};
	void linechart(const linecharttype&) {};
	void unknown_key(std::string&& s) { g.unknown_spritetypes_keys.insert(std::move(s)); };
	void discard(const discard_type&) {}
};

MEMBER_FDEF(spritetypes, sprite, "sprite");
MEMBER_FDEF(spritetypes, corneredtilesprite, "corneredtilesprite");
MEMBER_FDEF(spritetypes, progressbar, "progressbar");
MEMBER_FDEF(spritetypes, maskedshield, "maskedshield");
MEMBER_FDEF(spritetypes, textsprite, "textsprite");
MEMBER_FDEF(spritetypes, tilesprite, "tilesprite");
MEMBER_FDEF(spritetypes, barchart, "barchart");
MEMBER_FDEF(spritetypes, piechart, "piechart");
MEMBER_FDEF(spritetypes, linechart, "linechart");
MEMBER_FDEF(spritetypes, unknown_key, "unknown_key");
MEMBER_FDEF(spritetypes, discard, "discard");

struct gfx_file {
	gfx_container& g;
	gfx_file(gfx_container* h) : g(*h) {}

	template<typename T>
	void discard_result(T&&) {};
	void unknown_key(std::string&& s) { g.unknown_gfx_keys.insert(std::move(s)); };
};

MEMBER_FDEF(gfx_file, unknown_key, "unknown_key");
MEMBER_FDEF(gfx_file, discard_result, "discard");

constexpr auto pair_f = pair_from_lh_rh<std::string, std::string>;

BEGIN_DOMAIN(gfx_file_domain)
	EMPTY_TYPE(empty_type)
	BEGIN_TYPE(discard_type)
		MEMBER_VARIABLE_ASSOCIATION("any", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("any", accept_all, discard_type, discard_to_int)
	END_TYPE
	BEGIN_TYPE(gfx_file)
		MEMBER_TYPE_ASSOCIATION("discard", "bitmapfonts", discard_type)
		MEMBER_TYPE_ASSOCIATION("discard", "fonts", discard_type)
		MEMBER_TYPE_ASSOCIATION("discard", "lighttypes", discard_type)
		MEMBER_TYPE_ASSOCIATION("discard", "objecttypes", discard_type)
		MEMBER_TYPE_ASSOCIATION("discard", "spritetypes", spritetypes)
	    MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
	    MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, label_empty_type)
	END_TYPE
	BEGIN_TYPE(spritetypes)
		MEMBER_TYPE_ASSOCIATION("sprite", "spritetype", spritetype)
		MEMBER_TYPE_ASSOCIATION("corneredtilesprite", "corneredtilespritetype", corneredtilespritetype)
		MEMBER_TYPE_ASSOCIATION("barchart", "barcharttype", barcharttype)
		MEMBER_TYPE_ASSOCIATION("maskedshield", "maskedshieldtype", maskedshieldtype)
		MEMBER_TYPE_ASSOCIATION("piechart", "piecharttype", piecharttype)
		MEMBER_TYPE_ASSOCIATION("progressbar", "progressbartype", progressbartype)
		MEMBER_TYPE_ASSOCIATION("linechart", "linecharttype", linecharttype)
		MEMBER_TYPE_ASSOCIATION("tilesprite", "tilespritetype", tilespritetype)
		MEMBER_TYPE_ASSOCIATION("textsprite", "textspritetype", textspritetype)
	    MEMBER_TYPE_ASSOCIATION("discard", "scrollingsprite", discard_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, value_from_lh<std::string>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(spritetype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(progressbartype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(corneredtilespritetype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(textspritetype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(barcharttype)
		MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(maskedshieldtype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(linecharttype)
		MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(tilespritetype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
	BEGIN_TYPE(piecharttype)
	MEMBER_VARIABLE_ASSOCIATION("key-value", accept_all, pair_f)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_type", accept_all, discard_type, label_discard_type)
	END_TYPE
END_DOMAIN;
*/

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
	graphics::texture test_tex;
	graphics::texture strip_tex;
	graphics::texture mask_tex;
	graphics::texture prog1;
	graphics::texture prog2;
	graphics::texture bord;
	graphics::texture bar_tex;

	graphics::lines graph;

	graphics::font test_fallback;
	graphics::font test_font;
	
	empty_window_handler() :
		//test_tex("F:\\VS2007Projects\\open_v2_test_data\\army_icon_2.dds"),
		test_tex("F:\\VS2007Projects\\open_v2_test_data\\test_tx.bmp"),
		strip_tex("F:\\VS2007Projects\\open_v2_test_data\\strip10.dds"),
		mask_tex("F:\\VS2007Projects\\open_v2_test_data\\mask.tga"),
		prog1("F:\\VS2007Projects\\open_v2_test_data\\progress1.tga"),
		prog2("F:\\VS2007Projects\\open_v2_test_data\\progress2.tga"),
		bord("F:\\VS2007Projects\\open_v2_test_data\\border.dds"),
		bar_tex("F:\\VS2007Projects\\open_v2_test_data\\barchar.tga"),
		graph(10),
		test_fallback("F:\\VS2007Projects\\open_v2_test_data\\unifont-9.0.02.ttf"),
		//test_font("F:\\VS2007Projects\\open_v2_test_data\\Primitive.ttf", test_fallback) {}
		test_font("F:\\VS2007Projects\\open_v2_test_data\\CreteRound-Regular.otf", test_fallback) {
		float yval[] = { 0.3f, 0.6f, 0.5f, 1.0f, 0.4f, 0.5f, 0.0f, 0.3f, 0.2f, 0.6f};
		graph.set_y(yval);
	}

	template<typename T>
	void operator()(T&&, ui::window_base& w) const {
		// do nothing;
	}
	void initialize_graphics(graphics::open_gl_wrapper& ogl) {
		test_fallback.load_font(ogl);
		test_font.load_font(ogl);
	}

	void render(graphics::open_gl_wrapper& ogl) {
		ogl.render_outlined_text(u"明Tasy", 5, true, 80.0f, 40.0f, 32.0f, graphics::color{ 1.0f,1.0f,1.0f }, test_font);

		ogl.render_outlined_text(u"明Tesy", 5, true, 80.0f, 80.0f, 32.0f, graphics::color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(u"明Tasy", 5, true, 80.0f, 112.0f, 32.0f, graphics::color{ 1.0f,1.0f,1.0f }, test_font);

		ogl.render_character(u'y', true, 140.0f, 60.0f, 128.0f, test_font);
		ogl.render_character(u'A', true, 10.0f, 138.0f, 512.0f, test_font);

	}
};

template<int n>
class mb_button {
public:
	void button_function(ui::gui_object_tag, ui::gui_manager&) {
		MessageBoxA(nullptr, std::to_string(n).c_str(), "MB_A", MB_OK | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION);
	}
};

class tt_holder : public ui::draggable_region {
public:
	virtual ui::tooltip_behavior has_tooltip(ui::gui_object_tag, ui::gui_manager&, const ui::mouse_move&) override { return ui::tooltip_behavior::tooltip; }
	virtual void create_tooltip(ui::gui_object_tag, ui::gui_manager& m, const ui::mouse_move&, ui::tagged_gui_object tw) override {
		ui::text_chunk_to_instances(
			m,
			vector_backed_string<char16_t>(u"test tooltip"),
			tw,
			ui::xy_pair{ 0,0 },
			ui::text_format{ui::text_color::white, graphics::font_tag(1), 16},
			ui::single_line_manager());
	}
};

class debug_scrollbar {
public:
	void on_position(int32_t pos) {
		OutputDebugStringA("on_position; ");
		OutputDebugStringA(std::to_string(pos).c_str());
		OutputDebugStringA("\n");
	}
};

class empty_gui_obj {

};

class simple_button_group {
public:
	void on_select(ui::gui_manager&, uint32_t i) {
		MessageBoxA(nullptr, std::to_string(i).c_str(), "GROUP BUTTON", MB_OK | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION);
	}
};

using budget_window_t = ui::gui_window<
	CT_STRING("tab_takenloans"), ui::button_group_member,
	CT_STRING("tab_givenloans"), ui::button_group_member,
	CT_STRING("loans_tab_group"), ui::button_group<CT_STRING("tab_takenloans"), CT_STRING("tab_givenloans"), simple_button_group>,
	CT_STRING("debt_sort_country"), ui::simple_button<mb_button<3>>,
	CT_STRING("debt_sort_amount"), ui::simple_button<mb_button<4>>,
	CT_STRING("tax_0_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tax_1_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tax_2_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("land_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("naval_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("projects_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_0_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_1_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_2_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_3_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tariff_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("close_button"), ui::simple_button<mb_button<5>>,
	CT_STRING("take_loan"), ui::simple_button<mb_button<6>>,
	CT_STRING("repay_loan"), ui::simple_button<mb_button<7>>,
	CT_STRING("chart_0"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_1"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_2"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_debt"), ui::piechart<empty_gui_obj>,
	ui::draggable_region>;

struct gui_window_handler {
	ui::gui_manager& gui_m;

	//ui::simple_button<mb_button> mb_button_a;
	tt_holder mb_button_a;
	ui::scrollbar<debug_scrollbar> test_sb;

	budget_window_t budget_window;

	gui_window_handler(ui::gui_manager& m) : gui_m(m) {}

	template<typename T>
	void operator()(const T&, ui::window_base& w) const {
		// do nothing;
	}

	void operator()(const ui::creation&, ui::window_base& ) {
		ui::create_static_element(gui_m, std::get<ui::window_tag>(gui_m.ui_definitions.name_to_element_map["country_budget"]), ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) }, budget_window);
		auto& pc = budget_window.get<CT_STRING("chart_0")>();
		pc.add_entry(gui_m, vector_backed_string<char16_t>(u"category 1"), 0.4f, graphics::color_rgb{ 255,0,0 } );
		pc.add_entry(gui_m, vector_backed_string<char16_t>(u"category 2"), 0.1f, graphics::color_rgb{ 255,255,0 });
		pc.add_entry(gui_m, vector_backed_string<char16_t>(u"category 3"), 0.25f, graphics::color_rgb{ 255,0,255 });
		pc.add_entry(gui_m, vector_backed_string<char16_t>(u"category 4"), 0.25f, graphics::color_rgb{ 0,255,255 });
		pc.update_display(gui_m);

		budget_window.get<CT_STRING("debt_sort_country")>().associated_object->flags.fetch_or(ui::gui_object::force_transparency_check, std::memory_order_acq_rel);
		budget_window.get<CT_STRING("debt_sort_amount")>().associated_object->flags.fetch_or(ui::gui_object::force_transparency_check, std::memory_order_acq_rel);

		//ui::create_static_element(gui_m, ui::button_tag(8), ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) }, mb_button_a);
		//mb_button_a.shortcut = virtual_key::A;

		//const auto new_button = ui::detail::create_element_instance(gui_m, ui::button_tag(8));
		//ui::add_to_back(gui_m, ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) }, new_button);

		const auto new_icon = ui::detail::create_element_instance(gui_m, ui::icon_tag(19));
		ui::add_to_back(gui_m, ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) }, new_icon);

		const auto new_text = ui::detail::create_element_instance(gui_m, ui::text_tag(29));
		ui::add_to_back(gui_m, ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) }, new_text);

		ui::create_static_element(gui_m, ui::scrollbar_tag(10), ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) }, test_sb);
		test_sb.set_limits(gui_m, 0, 75);

		ui::create_scrollable_text_block(gui_m, ui::text_tag(571), text_data::text_tag(1001), ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) });

		//ui::create_dynamic_window(gui_m, ui::window_tag(8), ui::tagged_gui_object{ gui_m.root, ui::gui_object_tag(0) });
	}

	void operator()(const ui::resize& r, ui::window_base& ) {
		gui_m.on_resize(r);
	}

	void operator()(const ui::lbutton_down& m, ui::window_base& ) {
		gui_m.on_lbutton_down(m);
	}
	void operator()(const ui::key_down& m, ui::window_base& ) {
		gui_m.on_keydown(m);
	}
	void operator()(const ui::scroll& s, ui::window_base&) {
		gui_m.on_scroll(s);
	}
	void operator()(const ui::mouse_drag& m, ui::window_base&) {
		gui_m.on_mouse_drag(m);
	}
	void operator()(const ui::mouse_move& m, ui::window_base&) {
		gui_m.on_mouse_move(m);
	}

	void initialize_graphics(graphics::open_gl_wrapper& ogl) {
		gui_m.fonts.load_fonts(ogl);
	}

	void render(graphics::open_gl_wrapper& ogl) {
		ui::render(gui_m, ogl);
	}
};

auto fake_text_handle_lookup();
auto fake_font_handle_lookup();
auto fake_gobj_lookup();

auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_font_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_gobj_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}

int __cdecl main() {
	/*{
		window<empty_window_handler> test_window(400, 400);


		std::cout << "test window created" << std::endl;
		getchar();

		test_window.close_window();
	}*/
	
	

	file_system fs;
	fs.set_root(u"F:\\programs\\V2");

	ui::gui_manager gui_m(850, 650);
	ui::load_gui_from_directory(fs.get_root(), gui_m);

	{
		ui::window<gui_window_handler> test_window(850, 650, gui_m);

		std::cout << "test window created" << std::endl;
		getchar();

		test_window.close_window();
	}

	/*
	auto localisation_directory = fs.get_root().get_directory(u"\\localisation");

	text_data::text_sequences all_text;
	load_text_sequences_from_directory(localisation_directory, all_text);

	std::cout << all_text.all_sequences.size() << " total sequnces recorded" << std::endl;
	std::cout << all_text.all_components.size() << " components recorded" << std::endl;
	std::cout << all_text.text_data.size() << " characters of data" << std::endl;
	std::cout << all_text.key_data.size() << " characters of key data" << std::endl;
	
	auto interface_directory = fs.get_root().get_directory(u"\\interface");

	graphics::texture_manager tm;
	graphics::font_manager fm;

	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;

	graphics::name_maps gobj_nmaps;
	graphics::object_definitions gobj_defs;
	std::vector<std::pair<std::string, graphics::errors>> gobj_errors_generated;

	ui::load_ui_definitions_from_directory(
		interface_directory, nmaps, defs, errors_generated,
		[&all_text](const char* a, const char* b) { return get_text_handle(all_text, a, b); },
		[&fm](const char* a, const char* b) { return graphics::pack_font_handle(fm.find_font(a,b), fm.find_font_size(a,b)); },
		[&gobj_nmaps](const char* a, const char* b) { return graphics::reserve_graphics_object(gobj_nmaps, a, b); });

	for (auto& e : errors_generated) {
		std::cout << e.first << ": " << ui::format_error(e.second) << std::endl;
	}

	std::cout << "done with " << defs.buttons.size() + defs.icons.size() + defs.listboxes.size() + defs.overlapping_regions.size() + defs.positions.size() + defs.scrollbars.size() + defs.text.size() + defs.windows.size() << " objects" << std::endl;
	
	std::cout << all_text.all_sequences.size() << " total sequnces recorded" << std::endl;
	std::cout << all_text.all_components.size() << " components recorded" << std::endl;
	std::cout << all_text.text_data.size() << " characters of data" << std::endl;
	std::cout << all_text.key_data.size() << " characters of key data" << std::endl;
	
	std::cout << gobj_nmaps.names.size() << " graphics objects pending" << std::endl;

	graphics::load_graphics_object_definitions_from_directory(
		interface_directory,
		gobj_nmaps,
		gobj_defs,
		gobj_errors_generated,
		[&tm, &fs](const char* a, const char* b) { return tm.retrieve_by_name(fs.get_root(),a,b); });

	for (auto& e : gobj_errors_generated) {
		std::cout << e.first << ": " << graphics::format_error(e.second) << std::endl;
	}
	std::cout << "textures in manager: " << tm.count() << std::endl;
	std::cout << "finished" << std::endl;
	*/

	/*
	{
		file_system fs;
		fs.set_root(u"F:\\programs\\V2\\interface");
		//fs.add_root(u"F:\\programs\\V2\\mod\\HPM\\interface");
		const auto gfx_files = fs.get_root().list_files(u".gfx");

		
		std::vector<token_group> parse_tree;

		std::cout << "files count: " << gfx_files.size() << std::endl;

		gfx_container g;

		for (const auto& f : gfx_files) {
			auto open_f = f.open_file();
			if (open_f) {
				const auto sz = open_f->size();
				auto buffer = new char[sz];

				open_f->read_to_buffer(buffer, sz);
				
				parse_tree.clear();
				parse_pdx_file(parse_tree, buffer, buffer + sz);

				if (parse_tree.size() > 0) {
					parse_object<gfx_file, gfx_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), &g);
					
				}

				delete[] buffer;
			}
		}

		std::cout << "gfx file unknown keys: " << std::endl;
		for (const auto& k : g.unknown_gfx_keys) {
			std::cout << "\t" << k << std::endl;
		}
		std::cout << "spritetypes unknown keys: " << std::endl;
		for (const auto& k : g.unknown_spritetypes_keys) {
			std::cout << "\t" << k << std::endl;
		}
		std::cout << "textspritetype keys:" << std::endl;
		for (const auto& k : textspritetype::keys) {
			std::cout << "\t" << k.first << std::endl;
			if (k.first != "name" && k.first != "texturefile") {
				for(const auto& l : k.second)
					std::cout << "\t\t" << l << std::endl;
			}
		}
		std::cout << "textspritetype types:" << std::endl;
		for (const auto& k : textspritetype::unknown_types) {
			std::cout << "\t" << k << std::endl;
		}
	}*/

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

