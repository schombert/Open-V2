#include "common\\common.h"
#include "provinces\\provinces_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "modifiers\\modifiers_io.h"
#include "soil\\SOIL.h"
#include <Windows.h>
#include "scenario\\scenario.h"
#include "province_functions.h"
#include "nations\\nations_functions.h"
#include "world_state\\world_state.h"

namespace provinces {
	using factory_level_pair = std::pair<economy::factory_type_tag, int32_t>;
	using ideology_loyalty_pair = std::pair<ideologies::ideology_tag, float>;

	struct province_history_block;

	void merge_province_history_blocks(province_history_block& base_block, province_history_block const& new_block);

	struct province_history_environment {
		scenario::scenario_manager& s;
		date_tag target_date;

		province_history_environment(scenario::scenario_manager& sm, date_tag d) : s(sm), target_date(d) {}
	};

	struct factory_pair_reader {
		province_history_environment& env;
		int32_t level = 0;
		economy::factory_type_tag building;

		factory_pair_reader(province_history_environment& e) : env(e) {}

		void set_building(const token_and_type& t) {
			building = tag_from_text(
				env.s.economy_m.named_factory_types_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void discard(int) {}
	};

	struct party_loyalty_reader {
		province_history_environment& env;
		float loyalty_value = 0.0f;
		ideologies::ideology_tag ideology;

		party_loyalty_reader(province_history_environment& e) : env(e) {}

		void set_ideology(const token_and_type& t) {
			ideology = tag_from_text(
				env.s.ideologies_m.named_ideology_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
	};

	struct province_history_block {
		province_history_environment& env;
		province_history_block(province_history_environment& e) : env(e) {}

		economy::goods_tag trade_goods;
		std::optional<int16_t> life_rating = std::optional<int16_t>();
		cultures::national_tag owner;
		cultures::national_tag controller;
		modifiers::provincial_modifier_tag terrain;
		std::vector<cultures::national_tag> add_cores;
		std::vector<cultures::national_tag> remove_cores;
		std::vector<factory_level_pair> factories;
		std::optional<int32_t> fort_level = std::optional<int32_t>();
		std::optional<int32_t> naval_base_level = std::optional<int32_t>();
		std::optional<int32_t> railroad_level = std::optional<int32_t>();
		std::vector<ideology_loyalty_pair> party_loyalty;
		std::optional<int32_t> colony = std::optional<int32_t>();

		void discard(int) {}
		void add_dated_block(std::pair<token_and_type, province_history_block> const& p) {
			const auto date_tag = parse_date(p.first.start, p.first.end);
			if(!(env.target_date < date_tag))
				merge_province_history_blocks(*this, p.second);
		}
		void set_trade_goods(const token_and_type& t) {
			trade_goods = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_owner(const token_and_type& t) {
			owner = tag_from_text(
				env.s.culture_m.national_tags_index,
				cultures::tag_to_encoding(t.start, t.end));
		}
		void set_controller(const token_and_type& t) {
			controller = tag_from_text(
				env.s.culture_m.national_tags_index,
				cultures::tag_to_encoding(t.start, t.end));
		}
		void set_terrain(const token_and_type& t) {
			terrain = tag_from_text(
				env.s.modifiers_m.named_provincial_modifiers_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_core(const token_and_type& t) {
			add_cores.push_back(tag_from_text(
				env.s.culture_m.national_tags_index,
				cultures::tag_to_encoding(t.start, t.end)));
		}
		void remove_core(const token_and_type& t) {
			const auto tag = tag_from_text(
				env.s.culture_m.national_tags_index,
				cultures::tag_to_encoding(t.start, t.end));

			if(auto f = std::find(add_cores.begin(), add_cores.end(), tag); f != add_cores.end()) {
				*f = add_cores.back();
				add_cores.pop_back();
			} else {
				remove_cores.push_back(tag);
			}
		}
		void add_factory_pair(factory_pair_reader const& fp) {
			factories.emplace_back(fp.building, fp.level);
		}
		void add_loyalty_pair(party_loyalty_reader const& lp) {
			party_loyalty.emplace_back(lp.ideology, lp.loyalty_value);
		}
	};

	void merge_province_history_blocks(province_history_block& base_block, province_history_block const& new_block) {
		if(is_valid_index(new_block.trade_goods))
			base_block.trade_goods = new_block.trade_goods;
		if(new_block.life_rating)
			base_block.life_rating = new_block.life_rating;
		if(is_valid_index(new_block.owner))
			base_block.owner = new_block.owner;
		if(is_valid_index(new_block.controller))
			base_block.controller = new_block.controller;
		if(is_valid_index(new_block.terrain))
			base_block.terrain = new_block.terrain;

		base_block.add_cores.insert(base_block.add_cores.end(), new_block.add_cores.begin(), new_block.add_cores.end());
		for(auto c : new_block.remove_cores) {
			if(auto fr = std::find(base_block.add_cores.begin(), base_block.add_cores.end(), c); fr != base_block.add_cores.end()) {
				*fr = base_block.add_cores.back();
				base_block.add_cores.pop_back();
			}
		}
		for(auto fa : new_block.factories) {
			if(auto efa =
				std::find_if(base_block.factories.begin(), base_block.factories.end(),
					[tag = fa.first](std::pair<economy::factory_type_tag, int32_t>& p) {return p.first == tag; });
			efa != base_block.factories.end()) {
				efa->second = fa.second;
			} else {
				base_block.factories.push_back(fa);
			}
		}
		if(new_block.fort_level)
			base_block.fort_level = new_block.fort_level;
		if(new_block.naval_base_level)
			base_block.naval_base_level = new_block.naval_base_level;
		if(new_block.railroad_level)
			base_block.railroad_level = new_block.railroad_level;
		if(new_block.colony)
			base_block.colony = new_block.colony;

		for(auto p : new_block.party_loyalty) {
			if(auto epl =
				std::find_if(base_block.party_loyalty.begin(), base_block.party_loyalty.end(),
					[tag = p.first](std::pair<ideologies::ideology_tag, float>& ip) {return ip.first == tag; });
			epl != base_block.party_loyalty.end()) {
				epl->second = p.second;
			} else {
				base_block.party_loyalty.push_back(p);
			}
		}
	}

	inline int discard_section(token_group const*, token_group const*, province_history_environment&) {
		return 0;
	}
	inline std::pair<token_and_type, province_history_block> name_block(token_and_type const& l, association_type, province_history_block&& r) {
		return std::pair<token_and_type, province_history_block>(l, std::move(r));
	}

	struct parsing_environment {
		text_data::text_sequences& text_lookup;

		province_manager& manager;
		modifiers::modifiers_manager& mod_manager;

		parsed_data climate_file;
		parsed_data terrain_file;

		parsing_environment(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm) :
			text_lookup(tl), manager(m), mod_manager(mm) {}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm) :
		impl(std::make_unique<parsing_environment>(tl, m, mm)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct empty_type {
		void add_unknown_key(int) {}
	};

	struct sea_starts {
		parsing_environment& env;
		sea_starts(parsing_environment& e) : env(e) {}

		void add_sea_start(uint16_t v) {
			env.manager.province_container[province_tag(v)].flags |= province::sea;
		}
	};

	struct default_map_file {
		parsing_environment& env;
		default_map_file(parsing_environment& e) : env(e) {}

		void discard(int) {}
		void discard_empty(const empty_type&) {}
		void set_province_count(size_t v) {
			env.manager.province_container.resize(v);

			const auto prov_count = env.manager.province_container.size();
			for(uint32_t i = 0; i < prov_count; ++i) {
				auto& p = env.manager.province_container[province_tag(static_cast<uint16_t>(i))];
				p.id = province_tag(static_cast<uint16_t>(i));
			}
		}
		void handle_sea_starts(const sea_starts&) {}
	};

	struct terrain_parsing_environment {
		text_data::text_sequences& text_lookup;

		province_manager& manager;
		modifiers::modifiers_manager& mod_manager;
		color_to_terrain_map& terrain_color_map;

		terrain_parsing_environment(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm, color_to_terrain_map& tcm) :
			text_lookup(tl), manager(m), mod_manager(mm), terrain_color_map(tcm) {}
	};

	struct color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(int v) {
			switch(current_color) {
				case 0:
					color.r = static_cast<uint8_t>(v);
					break;
				case 1:
					color.g = static_cast<uint8_t>(v);
					break;
				case 2:
					color.b = static_cast<uint8_t>(v);
					break;
				default:
					break;
			}
			++current_color;
		}
	};

	struct color_list_builder {
		std::vector<uint8_t> colors;
		void add_value(uint8_t v) { colors.push_back(v); }
	};

	struct color_assignment {
		terrain_parsing_environment& env;

		color_list_builder colors;
		modifiers::provincial_modifier_tag tag;

		color_assignment(terrain_parsing_environment& e) : env(e) {}

		void set_type(token_and_type const& t) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.text_lookup, t.start, t.end);
			tag = tag_from_text(env.mod_manager.named_provincial_modifiers_index, name);
		}
		void discard(int) {}
	};

	inline color_assignment get_color_assignment(token_and_type const &, association_type, color_assignment&& v) {
		return std::move(v);
	}

	struct preparse_terrain_category : public modifiers::modifier_reading_base {
		terrain_parsing_environment& env;

		preparse_terrain_category(terrain_parsing_environment& e) : env(e) {}
		void add_color(const color_builder& ) {}
		void discard(int) {}
	};

	struct preparse_terrain_categories {
		terrain_parsing_environment& env;

		preparse_terrain_categories(terrain_parsing_environment& e) : env(e) {}

		void add_category(std::pair<token_and_type, preparse_terrain_category>&& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			modifiers::add_provincial_modifier(name, p.second, env.mod_manager);
		}

	};

	struct preparse_terrain_file {
		terrain_parsing_environment& env;
		preparse_terrain_file(terrain_parsing_environment& e) : env(e) {}

		void add_categories(const preparse_terrain_categories&) {}
		void add_color_assignment(color_assignment const & a) {
			for(auto v : a.colors.colors)
				env.terrain_color_map.data[v] = a.tag;
		}
		void discard(int) {}
	};

	inline int discard_empty_type(const token_and_type&, association_type, const empty_type&) { return 0; }
	inline std::pair<token_and_type, preparse_terrain_category>
		bind_terrain_category(const token_and_type& t, association_type, const preparse_terrain_category& f) {
		return std::pair<token_and_type, preparse_terrain_category>(t, std::move(f));
	}

	struct state_parse {
		parsing_environment& env;
		state_tag tag;

		state_parse(parsing_environment& e) : env(e) {
			tag = e.manager.state_names.emplace_back();
		}

		void add_province(uint16_t v) {
			env.manager.province_container[province_tag(v)].state_id = tag;
			env.manager.states_to_province_index.add_to_row(tag, province_tag(v));
		}
	};

	struct region_file {
		parsing_environment& env;
		region_file(parsing_environment& e) : env(e) {}
		void add_state(const std::pair<token_and_type, state_tag>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			env.manager.state_names[p.second] = name;
			env.manager.named_states_index.emplace(name, p.second);
		}
	};

	inline std::pair<token_and_type, state_tag>
		bind_state(const token_and_type& t, association_type, const state_parse& f) {

		return std::pair<token_and_type, state_tag>(t, f.tag);
	}

	struct parse_continent : public modifiers::modifier_reading_base {
		parsing_environment& env;
		modifiers::provincial_modifier_tag tag;

		parse_continent(parsing_environment& e) : env(e) {
			tag = e.mod_manager.provincial_modifiers.emplace_back();
			e.mod_manager.provincial_modifiers[tag].id = tag;
		}

		void add_continent_provinces(const std::vector<uint16_t>& v) {
			for(auto i : v) {
				env.manager.province_container[province_tag(i)].continent = tag;
			}
		}
	};

	struct continents_parse_file {
		parsing_environment& env;
		continents_parse_file(parsing_environment& e) : env(e) {}

		void add_continent(const std::pair<token_and_type, modifiers::provincial_modifier_tag>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			env.mod_manager.named_provincial_modifiers_index.emplace(name, p.second);
			env.mod_manager.provincial_modifiers[p.second].name = name;
		}
	};

	inline std::pair<token_and_type, modifiers::provincial_modifier_tag>
		bind_continent(const token_and_type& t, association_type, parse_continent&& f) {

		modifiers::set_provincial_modifier(f.tag, f, f.env.mod_manager);
		return std::pair<token_and_type, modifiers::provincial_modifier_tag>(t, f.tag);
	}

	inline modifiers::provincial_modifier_tag get_or_make_prov_modifier(text_data::text_tag name, modifiers::modifiers_manager& m) {
		const auto existing_tag = m.named_provincial_modifiers_index.find(name);
		if(existing_tag != m.named_provincial_modifiers_index.end()) {
			return existing_tag->second;
		} else {
			const auto ntag = m.provincial_modifiers.emplace_back();
			m.provincial_modifiers[ntag].id = ntag;
			m.provincial_modifiers[ntag].name = name;
			m.named_provincial_modifiers_index.emplace(name, ntag);
			return ntag;
		}
	}
	struct climate_province_values {
		std::vector<uint16_t> values;
		void add_province(uint16_t v) {
			values.push_back(v);
		}
	};

	int add_individual_climate(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env);

	struct climate_pre_parse_file {
		parsing_environment& env;
		climate_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_climate(int) {}
	};

	inline std::pair<token_and_type, float> full_to_tf_pair(const token_and_type& t, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(t, token_to<float>(r));
	}
}

MEMBER_DEF(provinces::province_history_block, life_rating, "life_rating");
MEMBER_DEF(provinces::province_history_block, fort_level, "fort");
MEMBER_DEF(provinces::province_history_block, naval_base_level, "naval_base");
MEMBER_DEF(provinces::province_history_block, railroad_level, "railroad");
MEMBER_DEF(provinces::province_history_block, colony, "colony");
MEMBER_FDEF(provinces::province_history_block, discard, "discard");
MEMBER_FDEF(provinces::province_history_block, add_dated_block, "dated_block");
MEMBER_FDEF(provinces::province_history_block, set_trade_goods, "trade_goods");
MEMBER_FDEF(provinces::province_history_block, set_owner, "owner");
MEMBER_FDEF(provinces::province_history_block, set_controller, "controller");
MEMBER_FDEF(provinces::province_history_block, set_terrain, "terrain");
MEMBER_FDEF(provinces::province_history_block, add_core, "add_core");
MEMBER_FDEF(provinces::province_history_block, remove_core, "remove_core");
MEMBER_FDEF(provinces::province_history_block, add_factory_pair, "state_building");
MEMBER_FDEF(provinces::province_history_block, add_loyalty_pair, "party_loyalty");
MEMBER_DEF(provinces::factory_pair_reader, level, "level");
MEMBER_FDEF(provinces::factory_pair_reader, discard, "discard");
MEMBER_FDEF(provinces::factory_pair_reader, set_building, "building");
MEMBER_DEF(provinces::party_loyalty_reader, loyalty_value, "loyalty_value");
MEMBER_FDEF(provinces::party_loyalty_reader, set_ideology, "ideology");


MEMBER_DEF(provinces::color_assignment, colors, "color");
MEMBER_FDEF(provinces::color_assignment, discard, "discard");
MEMBER_FDEF(provinces::color_assignment, set_type, "type");
MEMBER_FDEF(provinces::color_list_builder, add_value, "value");
MEMBER_FDEF(provinces::preparse_terrain_file, add_color_assignment, "color_assignment");
MEMBER_FDEF(provinces::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(provinces::default_map_file, discard, "unknown_key");
MEMBER_FDEF(provinces::default_map_file, discard_empty, "discard_empty");
MEMBER_FDEF(provinces::default_map_file, set_province_count, "max_provinces");
MEMBER_FDEF(provinces::default_map_file, handle_sea_starts, "sea_starts");
MEMBER_FDEF(provinces::sea_starts, add_sea_start, "add_sea_start");
MEMBER_FDEF(provinces::color_builder, add_value, "color");
MEMBER_FDEF(provinces::preparse_terrain_file, add_categories, "categories");
MEMBER_FDEF(provinces::preparse_terrain_file, discard, "unknown_key");
MEMBER_FDEF(provinces::preparse_terrain_categories, add_category, "category");
MEMBER_FDEF(provinces::preparse_terrain_category, add_color, "color");
MEMBER_DEF(provinces::preparse_terrain_category, icon, "icon");
MEMBER_FDEF(provinces::preparse_terrain_category, discard, "discard");
MEMBER_FDEF(provinces::preparse_terrain_category, add_attribute, "attribute");
MEMBER_FDEF(provinces::region_file, add_state, "state");
MEMBER_FDEF(provinces::state_parse, add_province, "province");
MEMBER_FDEF(provinces::continents_parse_file, add_continent, "continent");
MEMBER_FDEF(provinces::parse_continent, discard, "unknown_key");
MEMBER_FDEF(provinces::parse_continent, add_continent_provinces, "provinces");
MEMBER_DEF(provinces::parse_continent, icon, "icon");
MEMBER_FDEF(provinces::parse_continent, add_attribute, "attribute");
MEMBER_FDEF(provinces::climate_pre_parse_file, add_climate, "climate");
MEMBER_FDEF(provinces::climate_province_values, add_province, "value");

namespace provinces {
	BEGIN_DOMAIN(province_history_domain)
		BEGIN_TYPE(party_loyalty_reader)
			MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
			MEMBER_ASSOCIATION("loyalty_value", "loyalty_value", value_from_rh<float>)
		END_TYPE
		BEGIN_TYPE(factory_pair_reader)
			MEMBER_ASSOCIATION("building", "building", token_from_rh)
			MEMBER_ASSOCIATION("discard", "upgrade", discard_from_rh)
			MEMBER_ASSOCIATION("level", "level", value_from_rh<int32_t>)
		END_TYPE
		BEGIN_TYPE(province_history_block)
			MEMBER_ASSOCIATION("life_rating", "life_rating", value_from_rh<int16_t>)
			MEMBER_ASSOCIATION("fort", "fort", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("naval_base", "naval_base", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("railroad", "railroad", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("colony", "colony", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("colony", "colonial", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("trade_goods", "trade_goods", token_from_rh)
			MEMBER_ASSOCIATION("owner", "owner", token_from_rh)
			MEMBER_ASSOCIATION("controller", "controller", token_from_rh)
			MEMBER_ASSOCIATION("terrain", "terrain", token_from_rh)
			MEMBER_ASSOCIATION("add_core", "add_core", token_from_rh)
			MEMBER_ASSOCIATION("remove_core", "remove_core", token_from_rh)
			MEMBER_TYPE_ASSOCIATION("party_loyalty", "party_loyalty", party_loyalty_reader)
			MEMBER_TYPE_ASSOCIATION("state_building", "state_building", factory_pair_reader)
			MEMBER_TYPE_EXTERN("discard", "revolt", int, discard_section)
			MEMBER_ASSOCIATION("discard", "is_slave", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "set_province_flag", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "clr_province_flag", discard_from_rh)
			MEMBER_VARIABLE_TYPE_ASSOCIATION("dated_block", accept_all, province_history_block, name_block)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(default_map_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(sea_starts)
		MEMBER_VARIABLE_ASSOCIATION("add_sea_start", accept_all, value_from_lh<uint16_t>)
		END_TYPE
		BEGIN_TYPE(default_map_file)
		MEMBER_ASSOCIATION("max_provinces", "max_provinces", value_from_rh<uint32_t>)
		MEMBER_TYPE_ASSOCIATION("sea_starts", "sea_starts", sea_starts)
		MEMBER_TYPE_ASSOCIATION("discard_empty", "border_heights", empty_type)
		MEMBER_TYPE_ASSOCIATION("discard_empty", "terrain_sheet_heights", empty_type)
		MEMBER_ASSOCIATION("unknown_key", "definitions", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "provinces", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "positions", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "terrain", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "rivers", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "terrain_definition", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "tree_definition", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "continent", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "adjacencies", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "region", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "region_sea", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "province_flag_sprite", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "tree", discard_from_rh)
		MEMBER_ASSOCIATION("unknown_key", "border_cutoff", discard_from_rh)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(preparse_terrain_domain)
		BEGIN_TYPE(color_assignment)
		MEMBER_ASSOCIATION("discard", "priority", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "has_texture", discard_from_rh)
		MEMBER_ASSOCIATION("type", "type", token_from_rh)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_list_builder)
		END_TYPE
		BEGIN_TYPE(preparse_terrain_file)
		MEMBER_TYPE_ASSOCIATION("categories", "categories", preparse_terrain_categories)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("color_assignment", accept_all, color_assignment, get_color_assignment)
		END_TYPE
		BEGIN_TYPE(preparse_terrain_categories)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("category", accept_all, preparse_terrain_category, bind_terrain_category)
		END_TYPE
		BEGIN_TYPE(preparse_terrain_category)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint32_t>)
		MEMBER_VARIABLE_ASSOCIATION("attribute", accept_all, full_to_tf_pair)
		MEMBER_ASSOCIATION("discard", "is_water", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(color_list_builder)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, value_from_lh<uint8_t>)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(read_states_domain)
		BEGIN_TYPE(region_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("state", accept_all, state_parse, bind_state)
		END_TYPE
		BEGIN_TYPE(state_parse)
		MEMBER_VARIABLE_ASSOCIATION("province", accept_all, value_from_lh<uint16_t>)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(continents_domain)
		BEGIN_TYPE(continents_parse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("continent", accept_all, parse_continent, bind_continent)
		END_TYPE
		BEGIN_TYPE(parse_continent)
		MEMBER_TYPE_ASSOCIATION("provinces", "provinces", std::vector<uint16_t>)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint32_t>)
		MEMBER_VARIABLE_ASSOCIATION("attribute", accept_all, full_to_tf_pair)
		END_TYPE
		BEGIN_TYPE(std::vector<uint16_t>)
		MEMBER_VARIABLE_ASSOCIATION("this", accept_all, value_from_lh<uint16_t>)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(preparse_climate_domain)
		BEGIN_TYPE(climate_pre_parse_file)
		MEMBER_VARIABLE_TYPE_EXTERN("climate", accept_all, int, add_individual_climate)
		END_TYPE
		BEGIN_TYPE(climate_province_values)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, value_from_lh<uint16_t>)
		END_TYPE
		END_DOMAIN;

	void read_province_history(world_state& ws, province_state& ps, date_tag target_date, token_group const* start, token_group const* end) {
		province_history_environment env(ws.s, target_date);
		province_history_block result = parse_object<province_history_block, province_history_domain>(start, end, env);

		for(auto c : result.add_cores)
			add_core(ws.w, ps.id, c);
		if(is_valid_index(result.trade_goods))
			ps.rgo_production = result.trade_goods;
		if(is_valid_index(result.terrain))
			ps.terrain = result.terrain;
		if(result.life_rating)
			ps.life_rating = *result.life_rating;
		if(is_valid_index(result.owner))
			nations::silent_set_province_owner(ws, nations::make_nation_for_tag(ws, result.owner), ps.id);
		if(is_valid_index(result.controller))
			nations::silent_set_province_controller(ws.w, nations::make_nation_for_tag(ws, result.controller), ps.id);
		if(result.colony && ps.state_instance) {
			if(*result.colony == 2)
				ps.state_instance->flags |= nations::state_instance::is_colonial;
			else if(*result.colony == 1)
				ps.state_instance->flags |= nations::state_instance::is_protectorate;
			else if(*result.colony == 0)
				ps.state_instance->flags = decltype(ps.state_instance->flags)(0);
		}
		if(ps.state_instance) {
			for(uint32_t i = 0; i < result.factories.size() && i < std::extent_v<decltype(ps.state_instance->factories)>; ++i) {
				if(is_valid_index(result.factories[i].first)) {
					ps.state_instance->factories[i].type = &(ws.s.economy_m.factory_types[result.factories[i].first]);
					ps.state_instance->factories[i].level = uint16_t(result.factories[i].second);
				}
			}
		}
		if(result.railroad_level)
			ps.railroad_level = uint8_t(*result.railroad_level);
		if(result.fort_level)
			ps.fort_level = uint8_t(*result.fort_level);
		if(result.naval_base_level)
			ps.naval_base_level = uint8_t(*result.naval_base_level);
		for(auto p : result.party_loyalty)
			ws.w.province_s.party_loyalty.get(ps.id, p.first) = p.second;
	}

	void read_province_histories(world_state& ws, const directory& root, date_tag target_date) {
		const auto history_dir = root.get_directory(u"\\history");
		const auto provinces_dir = history_dir.get_directory(u"\\provinces");

		const auto sub_dirs = provinces_dir.list_directories();
		for(auto& sd : sub_dirs) {
			const auto province_files = sd.list_files(u".txt");
			for(auto& pfile : province_files) {
				auto file_name =  pfile.file_name();
				auto name_break = std::find_if(file_name.begin(), file_name.end(), [](char16_t c) { return c == u' ' || c == u'-'; });
				uint16_t prov_id = uint16_t(u16atoui(file_name.begin().operator->(), name_break.operator->()));

				auto fi = pfile.open_file();
				if(fi && prov_id != 0ui16 && prov_id < ws.s.province_m.province_container.size()) {
					const auto sz = fi->size();
					std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
					std::vector<token_group> presults;

					fi->read_to_buffer(parse_data.get(), sz);
					parse_pdx_file(presults, parse_data.get(), parse_data.get() + sz);

					read_province_history(
						ws,
						ws.w.province_s.province_state_container[province_tag(prov_id)],
						target_date,
						presults.data(), presults.data() + presults.size());
				}
			}
		}
	}

	int add_individual_climate(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto fr = env.mod_manager.named_provincial_modifiers_index.find(name);
		if(fr == env.mod_manager.named_provincial_modifiers_index.end()) {
			modifiers::parse_provincial_modifier(name, env.mod_manager, s, e);
		} else {
			const auto vals = parse_object<climate_province_values, preparse_climate_domain>(s, e, env);
			for(auto v : vals.values)
				env.manager.province_container[province_tag(v)].climate = fr->second;
		}
		return 0;
	}

	void read_default_map_file(
		parsing_state& state,
		const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		parsed_data main_results;

		const auto fi = map_dir.open_file(u"default.map");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<default_map_file, default_map_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	color_to_terrain_map read_terrain(
			parsing_state& state,
			const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		auto& main_results = state.impl->terrain_file;
		color_to_terrain_map result_map;

		terrain_parsing_environment tstate(state.impl->text_lookup, state.impl->manager, state.impl->mod_manager, result_map);

		const auto fi = map_dir.open_file(u"terrain.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<preparse_terrain_file, preparse_terrain_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					tstate);
			}
		}

		return result_map;
	}

	void read_states(
		parsing_state& state,
		const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		parsed_data main_results;

		const auto fi = map_dir.open_file(u"region.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<region_file, read_states_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_continents(
		parsing_state& state,
		const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		parsed_data main_results;

		const auto fi = map_dir.open_file(u"continent.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<continents_parse_file, continents_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_climates(
		parsing_state& state,
		const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		auto& main_results = state.impl->climate_file;

		const auto fi = map_dir.open_file(u"climate.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<climate_pre_parse_file, preparse_climate_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}
	boost::container::flat_map<uint32_t, province_tag> read_province_definition_file(directory const & source_directory) {
		boost::container::flat_map<uint32_t, province_tag> t;

		const auto map_dir = source_directory.get_directory(u"\\map");
		const auto fi = map_dir.open_file(u"definition.csv");

		if(fi) {
			const auto sz = fi->size();
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			char* position = parse_data.get();
			fi->read_to_buffer(position, sz);

			if(sz != 0 && position[0] == '#')
				position = csv_advance_to_next_line(position, parse_data.get() + sz);

			while(position < parse_data.get() + sz) {
				position = parse_fixed_amount_csv_values<4>(position, parse_data.get() + sz, ';',
					[&t](std::pair<char*, char*> const* values) {
					if(is_integer(values[0].first, values[0].second)) {
						t.emplace(rgb_to_prov_index(
							uint8_t(parse_int(values[1].first, values[1].second)),
							uint8_t(parse_int(values[2].first, values[2].second)),
							uint8_t(parse_int(values[3].first, values[3].second))),
							province_tag(province_tag::value_base_t(parse_int(values[0].first, values[0].second))));
					}
				});
			}

		}
		return t;
	}

	tagged_vector<uint8_t, province_tag> load_province_map_data(province_manager& m, directory const& root) {
		const auto map_dir = root.get_directory(u"\\map");
		
		auto map_peek = map_dir.peek_file(u"provinces.png");
		if(!map_peek)
			map_peek = map_dir.peek_file(u"provinces.bmp");
		if(map_peek) {
			auto fi = map_peek->open_file();
			if(fi) {
				const auto sz = fi->size();
				std::unique_ptr<char[]> file_data = std::unique_ptr<char[]>(new char[sz]);
				fi->read_to_buffer(file_data.get(), sz);

				int32_t channels = 3;
				const auto raw_data = SOIL_load_image_from_memory((unsigned char*)(file_data.get()), static_cast<int32_t>(sz), &m.province_map_width, &m.province_map_height, &channels, 3);
				m.province_map_data.resize(static_cast<size_t>(m.province_map_width * m.province_map_height));

				const auto color_mapping = read_province_definition_file(root);

				const auto last = m.province_map_width * m.province_map_height - 1;
				uint32_t previous_color_index = provinces::rgb_to_prov_index(raw_data[last * 3 + 0], raw_data[last * 3 + 1], raw_data[last * 3 + 2]);
				province_tag prev_result = province_tag(0);
				if(auto it = color_mapping.find(previous_color_index); it != color_mapping.end()) {
					prev_result = it->second;
					m.province_map_data[static_cast<size_t>(last)] = to_index(it->second);
				}

				for(int32_t t = m.province_map_width * m.province_map_height - 2; t >= 0; --t) {
					uint32_t color_index = provinces::rgb_to_prov_index(raw_data[t * 3 + 0], raw_data[t * 3 + 1], raw_data[t * 3 + 2]);
					if(color_index == previous_color_index) {
						m.province_map_data[static_cast<size_t>(t)] = to_index(prev_result);
					} else {
						previous_color_index = color_index;
						if(auto it = color_mapping.find(color_index); it != color_mapping.end())
							m.province_map_data[static_cast<size_t>(t)] = to_index(it->second);
						else
							m.province_map_data[static_cast<size_t>(t)] = 0ui16;
						prev_result = m.province_map_data[static_cast<size_t>(t)];
					}
				}


				SOIL_free_image_data(raw_data);
			}
		}

		auto terrain_peek = map_dir.peek_file(u"terrain.png");
		if(terrain_peek) {
			auto fi = terrain_peek->open_file();
			if(fi) {
				const auto sz = fi->size();
				std::unique_ptr<char[]> file_data = std::unique_ptr<char[]>(new char[sz]);
				fi->read_to_buffer(file_data.get(), sz);

				int32_t terrain_width = 0;
				int32_t terrain_height = 0;
				int32_t channels = 1;
				const auto raw_data = SOIL_load_image_from_memory((unsigned char*)(file_data.get()), static_cast<int32_t>(sz), &terrain_width, &terrain_height, &channels, 1);

				if(terrain_height != m.province_map_height || terrain_width != m.province_map_width)
					std::abort();

				const auto t_vector = generate_province_terrain(m.province_container.size(), m.province_map_data.data(), (uint8_t const*)(raw_data), terrain_height, terrain_width);

				SOIL_free_image_data(raw_data);

				return t_vector;
			}
		} else {
			if(terrain_peek) {
				terrain_peek = map_dir.peek_file(u"terrain.bmp");
				auto fi = terrain_peek->open_file();
				if(fi) {
					const auto sz = fi->size();
					std::unique_ptr<char[]> file_data = std::unique_ptr<char[]>(new char[sz]);
					fi->read_to_buffer(file_data.get(), sz);

					BITMAPFILEHEADER header;
					memcpy(&header, file_data.get(), sizeof(BITMAPFILEHEADER));
					BITMAPINFOHEADER info_header;
					memcpy(&info_header, file_data.get() + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));

					if(info_header.biHeight != m.province_map_height || info_header.biWidth != m.province_map_width)
						std::abort();

					return generate_province_terrain_inverse(m.province_container.size(), m.province_map_data.data(), (uint8_t const*)(file_data.get() + header.bfOffBits), info_header.biHeight, info_header.biWidth);
				}
			}
		}

		return tagged_vector<uint8_t, province_tag>();
	}

	tagged_vector<uint8_t, province_tag> generate_province_terrain_inverse(size_t province_count, uint16_t const* province_map_data, uint8_t const* terrain_color_map_data, int32_t height, int32_t width) {
		tagged_vector<uint8_t, province_tag> terrain_out;
		tagged_vector<int32_t, province_tag> count;

		terrain_out.resize(province_count);
		count.resize(province_count);


		for(int32_t j = height - 1; j >= 0; --j) {
			for(int32_t i = width - 1; i >= 0; --i) {
				auto this_province = province_tag(province_map_data[j * width + i]);
				const auto t_color = terrain_color_map_data[(height - j - 1) * width + i];
				if(count[this_province] == 0) {
					terrain_out[this_province] = t_color;
					count[this_province] = 1;
				} else if(terrain_out[this_province] == t_color) {
					count[this_province] += 1;
				} else {
					count[this_province] -= 1;
				}
			}
		}

		return terrain_out;
	}

	tagged_vector<uint8_t, province_tag> generate_province_terrain(size_t province_count, uint16_t const* province_map_data, uint8_t const* terrain_color_map_data, int32_t height, int32_t width) {
		tagged_vector<uint8_t, province_tag> terrain_out;
		tagged_vector<int32_t, province_tag> count;

		terrain_out.resize(province_count);
		count.resize(province_count);
		
		for(int32_t i = height  * width - 1; i >= 0; --i) {
			auto this_province = province_tag(province_map_data[i]);
			const auto t_color = terrain_color_map_data[i];
			if(count[this_province] == 0) {
				terrain_out[this_province] = t_color;
				count[this_province] = 1;
			} else if(terrain_out[this_province] == t_color) {
				count[this_province] += 1;
			} else {
				count[this_province] -= 1;
			}
			
		}

		return terrain_out;
	}

	void assign_terrain_color(provinces_state& m, tagged_vector<uint8_t, province_tag> const & terrain_colors, color_to_terrain_map const & terrain_map) {
		int32_t max_province = static_cast<int32_t>(m.province_state_container.size()) - 1;
		for(int32_t i = max_province; i >= 0; --i) {
			const auto this_province = province_tag(static_cast<province_tag::value_base_t>(i));
			const auto this_t_color = terrain_colors[this_province];
			m.province_state_container[this_province].terrain = terrain_map.data[this_t_color];
		}
	}

	std::map<province_tag, boost::container::flat_set<province_tag>> generate_map_adjacencies(uint16_t const* province_map_data, int32_t height, int32_t width) {
		std::map<province_tag, boost::container::flat_set<province_tag>> result;

		for(int32_t j = height - 2; j >= 0; --j) {
			for(int32_t i = width - 2; i >= 0; --i) {
				const auto current_prov = province_map_data[i + j * width];
				const auto rightwards = province_map_data[(i + 1) + j * width];
				const auto downwards = province_map_data[i + (j + 1) * width];

				if(rightwards != current_prov) {
					result[province_tag(current_prov)].insert(province_tag(rightwards));
					result[province_tag(rightwards)].insert(province_tag(current_prov));
				}
				if(downwards != current_prov) {
					result[province_tag(current_prov)].insert(province_tag(downwards));
					result[province_tag(downwards)].insert(province_tag(current_prov));
				}
			}
		}

		for(int32_t j = height - 2; j >= 0; --j) {
			const auto current_prov = province_map_data[(width - 1) + j * width];

			const auto rightwards = province_map_data[0 + j * width];
			const auto downwards = province_map_data[(width - 1) + (j + 1) * width];

			if(rightwards != current_prov) {
				result[province_tag(current_prov)].insert(province_tag(rightwards));
				result[province_tag(rightwards)].insert(province_tag(current_prov));
			}
			if(downwards != current_prov) {
				result[province_tag(current_prov)].insert(province_tag(downwards));
				result[province_tag(downwards)].insert(province_tag(current_prov));
			}
		}

		return result;
	}

	void read_adjacnencies_file(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, std::vector<std::pair<province_tag, province_tag>>& canals, directory const& root) {
		const auto map_dir = root.get_directory(u"\\map");
		const auto fi = map_dir.open_file(u"adjacencies.csv");

		if(fi) {
			const auto sz = fi->size();
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			char* position = parse_data.get();
			fi->read_to_buffer(position, sz);

			if(sz != 0 && position[0] == '#')
				position = csv_advance_to_next_line(position, parse_data.get() + sz);

			while(position < parse_data.get() + sz) {
				position = parse_fixed_amount_csv_values<5>(position, parse_data.get() + sz, ';',
					[&adj_map, &canals](std::pair<char*, char*> const* values) {
					const auto prov_a = province_tag(uint16_t(parse_int(values[0].first, values[0].second)));
					const auto prov_b = province_tag(uint16_t(parse_int(values[1].first, values[1].second)));

					if(is_fixed_token_ci(values[2].first, values[2].second, "impassable")) {
						adj_map[prov_b].erase(prov_a);
						adj_map[prov_a].erase(prov_b);
					} else if(is_fixed_token_ci(values[2].first, values[2].second, "canal")) {
						const int32_t canal_index = parse_int(values[4].first, values[4].second) - 1;
						if(static_cast<size_t>(canal_index) >= canals.size())
							canals.resize(static_cast<size_t>(canal_index + 1));
						if(canal_index >= 0)
							canals[static_cast<size_t>(canal_index)] = std::make_pair(prov_a, prov_b);
					} else {
						adj_map[prov_a].insert(prov_b);
						adj_map[prov_b].insert(prov_a);
					}
				});
			}
		}
	}

	void make_lakes(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, province_manager& m) {
		m.province_container[province_tag(0)].flags = uint16_t(province::lake | province::sea);
		for(auto adj_p : adj_map[province_tag(0)])
			adj_map[adj_p].erase(province_tag(0));
		adj_map[province_tag(0)].clear();

		for(uint16_t i = 1; i < m.province_container.size(); ++i) {
			auto& this_province = m.province_container[province_tag(i)];
			if(this_province.flags & province::sea) {
				bool is_lake = true;
				for(auto adj_p : adj_map[province_tag(i)]) {
					if(m.province_container[adj_p].flags & province::sea)
						is_lake = false;
				}
				if(is_lake) {
					this_province.flags |= province::lake;
					for(auto adj_p : adj_map[province_tag(i)])
						adj_map[adj_p].erase(province_tag(i));
					adj_map[province_tag(i)].clear();
				}
			}
		}
	}

	void make_adjacency(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, province_manager& m) {
		m.same_type_adjacency.expand_rows(static_cast<uint32_t>(m.province_container.size()));
		m.coastal_adjacency.expand_rows(static_cast<uint32_t>(m.province_container.size()));

		for(auto const& adj_set : adj_map) {
			auto& this_province = m.province_container[adj_set.first];
			if(this_province.flags & province::sea) {
				for(auto oprov : adj_set.second) {
					if(m.province_container[oprov].flags & province::sea) {
						m.same_type_adjacency.add_to_row(to_index(adj_set.first), to_index(oprov));
					} else {
						m.coastal_adjacency.add_to_row(to_index(adj_set.first), to_index(oprov));
						this_province.flags |= province::coastal;
					}
				}
			} else {
				for(auto oprov : adj_set.second) {
					if(m.province_container[oprov].flags & province::sea) {
						m.coastal_adjacency.add_to_row(to_index(adj_set.first), to_index(oprov));
						this_province.flags |= province::coastal;
					} else {
						m.same_type_adjacency.add_to_row(to_index(adj_set.first), to_index(oprov));
					}
				}
			}
		}
	}
}
