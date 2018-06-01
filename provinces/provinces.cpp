#include "provinces.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "modifiers\\modifiers.h"
#include <optional>

namespace provinces {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;

		province_manager& manager;
		modifiers::modifiers_manager& mod_manager;

		parsed_data climate_file;
		parsed_data terrain_file;

		parsing_environment(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm) :
			text_lookup(tl), manager(m), mod_manager(mm) {
		}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm) :
		impl(std::make_unique<parsing_environment>(tl, m, mm)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct empty_type {
		void add_unknown_key(int) {
		}
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
			for (uint32_t i = 0; i < v; ++i)
				env.manager.province_container[province_tag(static_cast<uint16_t>(i))].id = province_tag(static_cast<uint16_t>(i));
		}
		void handle_sea_starts(const sea_starts&) {}
	};

	struct terrain_parsing_environment {
		text_data::text_sequences& text_lookup;

		province_manager& manager;
		modifiers::modifiers_manager& mod_manager;
		boost::container::flat_map<uint32_t, modifiers::provincial_modifier_tag>& terrain_color_map;

		terrain_parsing_environment(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm, boost::container::flat_map<uint32_t, modifiers::provincial_modifier_tag>& tcm) :
			text_lookup(tl), manager(m), mod_manager(mm), terrain_color_map(tcm) {
		}
	};

	struct color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(int v) {
			switch (current_color) {
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

	struct preparse_terrain_category {
		terrain_parsing_environment& env;
		modifiers::provincial_modifier_tag tag;

		preparse_terrain_category(terrain_parsing_environment& e) : env(e) {
			tag = e.mod_manager.provincial_modifiers.emplace_back();
			e.mod_manager.provincial_modifiers[tag].id = tag;
		}

		void add_color(const color_builder& c) {
			env.terrain_color_map.emplace(graphics::rgb_to_int(c.color), tag);
		}
		void discard(int) {}
	};

	struct preparse_terrain_categories {
		terrain_parsing_environment& env;

		preparse_terrain_categories(terrain_parsing_environment& e) : env(e) {
		}

		void add_category(const std::pair<token_and_type, modifiers::provincial_modifier_tag>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			env.mod_manager.named_provincial_modifiers_index.emplace(name, p.second);
			env.mod_manager.provincial_modifiers[p.second].name = name;
		}
		
	};

	struct preparse_terrain_file {
		terrain_parsing_environment& env;
		preparse_terrain_file(terrain_parsing_environment& e) : env(e) {}

		void add_categories(const preparse_terrain_categories&) {}
		void discard(int) {}
	};

	inline int discard_empty_type(const token_and_type&, association_type, const empty_type&) { return 0; }
	inline std::pair<token_and_type, modifiers::provincial_modifier_tag>
		bind_terrain_category(const token_and_type& t, association_type, const preparse_terrain_category& f) {

		return std::pair<token_and_type, modifiers::provincial_modifier_tag>(t, f.tag);
	}

	struct state_parse {
		parsing_environment& env;
		state_tag tag;

		state_parse(parsing_environment& e) : env(e) {
			tag = e.manager.state_names.emplace_back();
		}

		void add_province(uint16_t v) {
			env.manager.province_container[province_tag(v)].state_id = tag;
			env.manager.states_to_province_index.emplace(tag, province_tag(v));
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
			for (auto i : v) {
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
		if (existing_tag != m.named_provincial_modifiers_index.end()) {
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
		void add_province(std::optional<uint16_t> v) {
			if (v)
				values.push_back(*v);
		}
	};
	struct climate_pre_parse_file {
		parsing_environment& env;
		climate_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_climate(const std::pair<token_and_type, std::vector<uint16_t>>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			const auto tag = get_or_make_prov_modifier(name, env.mod_manager);
			for (auto i : p.second) {
				env.manager.province_container[province_tag(i)].climate = tag;
			}
		}
	};
	inline std::optional<uint16_t> get_free_values(const token_and_type& v, const association_type a, const token_and_type&) {
		if (a == association_type::none) {
			return std::optional<uint16_t>(token_to<uint16_t>(v));
		} else {
			return std::optional<uint16_t>();
		}
	}
	inline std::pair<token_and_type, std::vector<uint16_t>>
		bind_climate(const token_and_type& t, association_type, climate_province_values&& f) {

		return std::pair<token_and_type, std::vector<uint16_t>>(t, std::move(f.values));
	}
	inline std::pair<token_and_type, float> full_to_tf_pair(const token_and_type& t, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(t, token_to<float>(r));
	}
}

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
MEMBER_FDEF(provinces::preparse_terrain_category, discard, "unknown_key");
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
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(preparse_terrain_file)
		MEMBER_TYPE_ASSOCIATION("categories","categories", preparse_terrain_categories)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(preparse_terrain_categories)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("category", accept_all, preparse_terrain_category, bind_terrain_category)
		END_TYPE
		BEGIN_TYPE(preparse_terrain_category)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		END_TYPE
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
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
		MEMBER_VARIABLE_TYPE_ASSOCIATION("climate", accept_all, climate_province_values, bind_climate)
		END_TYPE
		BEGIN_TYPE(climate_province_values)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, get_free_values)
		END_TYPE
	END_DOMAIN;

	void read_default_map_file(
		parsing_state& state,
		const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		parsed_data main_results;

		const auto fi = map_dir.open_file(u"default.map");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<default_map_file, default_map_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	boost::container::flat_map<uint32_t, modifiers::provincial_modifier_tag>
		pre_parse_terrain(
			parsing_state& state,
			const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		auto& main_results = state.impl->terrain_file;
		boost::container::flat_map<uint32_t, modifiers::provincial_modifier_tag> result_map;

		terrain_parsing_environment tstate(state.impl->text_lookup, state.impl->manager, state.impl->mod_manager, result_map);

		const auto fi = map_dir.open_file(u"terrain.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
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

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
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

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<continents_parse_file, continents_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void pre_parse_climates(
		parsing_state& state,
		const directory& source_directory) {

		const auto map_dir = source_directory.get_directory(u"\\map");
		auto& main_results = state.impl->climate_file;

		const auto fi = map_dir.open_file(u"climate.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<climate_pre_parse_file, preparse_climate_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}
}
