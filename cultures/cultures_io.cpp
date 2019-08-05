#include "common\\common.h"
#include "cultures_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include <algorithm>
#include <map>
#include "graphics\\texture.h"
#include "simple_fs\\simple_fs.h"
#include "scenario\\scenario.h"
#include "governments\\governments_io.h"
#include "world_state\\world_state.h"

#undef min
#undef max

void serialization::serializer<cultures::national_tag_state>::serialize_object(std::byte *& output, cultures::national_tag_state const & obj, world_state const &) {
	serialize(output, obj.capital);
	serialize(output, obj.is_not_releasable);
}

void serialization::serializer<cultures::national_tag_state>::deserialize_object(std::byte const *& input, cultures::national_tag_state & obj, world_state & ws) {
	deserialize(input, obj.capital);
	deserialize(input, obj.is_not_releasable);
}

size_t serialization::serializer<cultures::national_tag_state>::size() {
	return sizeof(provinces::province_tag) + sizeof(bool);
}


namespace cultures {
	struct parsing_environment {
		std::map<std::string, leader_picture_info> pictures_map;
		text_data::text_sequences& text_lookup;
		culture_manager& manager;
		graphics::texture_manager& tm;
		const directory root;
		bool unicode = false;


		parsing_environment(text_data::text_sequences& tl, culture_manager& m, graphics::texture_manager& t, const directory& r, bool u = false) :
			text_lookup(tl), manager(m), tm(t), root(r), unicode(u) {}
	};

	struct religion_parsing_environment {
		text_data::text_sequences& text_lookup;
		culture_manager& manager;
		bool unicode = false;

		religion_parsing_environment(text_data::text_sequences& tl, culture_manager& m, bool u = false) :
			text_lookup(tl), manager(m), unicode(u) {}
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

	struct country_file_env {
		national_tag_object& under_construction;
		scenario::scenario_manager& s;
		country_file_env(national_tag_object& uc, scenario::scenario_manager& sm) : under_construction(uc), s(sm) {}
	};

	inline int discard_country_section(token_group const*, token_group const*, country_file_env&) {
		return 0;
	}

	inline governments::party_tag inner_read_party(token_group const* s, token_group const* e, country_file_env& env) {
		return governments::read_party(s, e, env.s);
	}

	struct country_file {
		country_file_env& env;
		country_file(country_file_env& e) : env(e) {}
		void set_color(color_builder const& c) {
			env.under_construction.color = c.color;
		}
		void discard(int) {}
		void add_party(governments::party_tag id) {
			if(!is_valid_index(env.under_construction.first_party))
				env.under_construction.first_party = id;
			env.under_construction.last_party = governments::party_tag(governments::party_tag::value_base_t(to_index(id) + 1));
		}
	};

	struct fp_color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(double v) {
			switch(current_color) {
				case 0:
					color.r = static_cast<uint8_t>(v * 255.0);
					break;
				case 1:
					color.g = static_cast<uint8_t>(v * 255.0);
					break;
				case 2:
					color.b = static_cast<uint8_t>(v * 255.0);
					break;
				default:
					break;
			}
			++current_color;
		}
	};

	struct religion_builder {
		fp_color_builder color;
		uint8_t icon = 0;
		bool pagan = false;
	};

	struct religions_group_s {
		religion_parsing_environment& env;
		religions_group_s(religion_parsing_environment& e) : env(e) {}
		void add_religion(const std::pair<token_and_type, religion_builder>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			const auto nt = env.manager.religions.emplace_back();
			auto& nr = env.manager.religions[nt];
			nr.id = nt;
			nr.name = name;
			nr.pagan = p.second.pagan;
			nr.color = p.second.color.color;
			nr.icon = p.second.icon;
			env.manager.named_religion_index.emplace(name, nt);
		}
	};
	struct religions_file {
		religion_parsing_environment& env;
		religions_file(religion_parsing_environment& e) : env(e) {}
		void add_group(int) {}
	};

	struct tags_parsing_env {
		tagged_vector<std::string, national_tag>& tag_files;
		culture_manager& manager;

		tags_parsing_env(tagged_vector<std::string, national_tag>& tv, culture_manager& m) :
			tag_files(tv), manager(m) {}
	};
	struct tags_file {
		tags_parsing_env& env;
		tags_file(tags_parsing_env& e) : env(e) {}
		void add_tag(const std::pair<token_and_type, token_and_type>& p) {
			const auto tvalue = tag_to_encoding(p.first.start, p.first.end);
			const auto tt = env.manager.national_tags.emplace_back();
			auto& nt = env.manager.national_tags[tt];
			nt.id = tt;
			nt.tag_code = tvalue;
			env.manager.national_tags_index.emplace(tvalue, tt);

			const auto fresult = std::find(p.second.start, p.second.end, '/');
			const auto fstart = (fresult != p.second.end) ? (fresult + 1) : p.second.start;
			env.tag_files.safe_get(tt) = std::string(fstart, p.second.end);

		}
	};

	struct names_builder {
		parsing_environment& env;
		std::vector<vector_backed_string<char16_t>> names;

		names_builder(parsing_environment& e) : env(e) {}
		void add_name(const token_and_type& t) {
			names.emplace_back(
				vector_backed_string<char16_t>::create_unique(
					env.unicode ? UTF8toUTF16(t.start, t.end) : win1250toUTF16(t.start, t.end),
					env.manager.name_data));
		}
	};

	struct culture_builder {
		parsing_environment& env;
		culture_tag ctag;
		culture* c;

		culture_builder(parsing_environment& e) : env(e) {
			ctag = e.manager.culture_container.emplace_back();
			c = &e.manager.culture_container[ctag];
			c->id = ctag;
		}

		void add_color(const color_builder& color) {
			c->color = color.color;
		}

		void add_first_names(const names_builder& names) {
			env.manager.first_names_by_culture.add_range_to_row(ctag, names.names.begin(), names.names.end());
		}
		void add_last_names(const names_builder& names) {
			env.manager.last_names_by_culture.add_range_to_row(ctag, names.names.begin(), names.names.end());
		}
		void set_radicalism(float f) {
			c->radicalism = f;
		}
		void discard(int) {}
	};

	struct culture_group_builder {
		parsing_environment& env;
		culture_group_tag group_t;
		culture_group* group;

		culture_group_builder(parsing_environment& e) : env(e) {
			group_t = e.manager.culture_groups.emplace_back();
			group = &e.manager.culture_groups[group_t];
			group->id = group_t;
		}

		void add_union_tag(const token_and_type& t) {
			group->union_tag = env.manager.national_tags_index[tag_to_encoding(t.start, t.end)];
		}
		void add_unit(const token_and_type&) {

		}
		void add_leader(const token_and_type& t) {
			std::string key(t.start, t.end);
			if(auto f = env.pictures_map.find(key); f != env.pictures_map.end()) {
				group->leader_pictures = f->second;
			} else {
				leader_picture_info i;
				i.admiral_offset = static_cast<uint16_t>(env.manager.leader_pictures.size());

				std::string cbase = std::string("\\gfx\\interface\\leaders\\") + key + "_admiral_";

				int32_t count = 0;
				std::string cname = cbase + std::to_string(count) + ".dds";
				graphics::texture_tag current_tag = env.tm.retrieve_by_name(env.root, cname.c_str(), cname.c_str() + cname.length());

				while(is_valid_index(current_tag)) {
					env.manager.leader_pictures.push_back(current_tag);

					++count;
					cname = cbase + std::to_string(count) + ".dds";
					current_tag = env.tm.retrieve_by_name(env.root, cname.c_str(), cname.c_str() + cname.length());
				}
				i.admiral_size = static_cast<uint16_t>(count);

				i.general_offset = static_cast<uint16_t>(env.manager.leader_pictures.size());

				cbase = std::string("\\gfx\\interface\\leaders\\") + key + "_general_";

				count = 0;
				cname = cbase + std::to_string(count) + ".dds";
				current_tag = env.tm.retrieve_by_name(env.root, cname.c_str(), cname.c_str() + cname.length());
				while(is_valid_index(current_tag)) {
					env.manager.leader_pictures.push_back(current_tag);

					++count;
					cname = cbase + std::to_string(count) + ".dds";
					current_tag = env.tm.retrieve_by_name(env.root, cname.c_str(), cname.c_str() + cname.length());
				}
				i.general_size = static_cast<uint16_t>(count);

				group->leader_pictures = i;
				env.pictures_map.emplace(key, i);
			}
		}
		void add_culture(const std::pair<token_and_type, culture_builder>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);

			p.second.c->group = group_t;
			p.second.c->name = name;

			env.manager.named_culture_index.emplace(name, p.second.ctag);
		}
		void set_is_overseas(bool v) {
			group->is_overseas = v;
		}
	};

	struct cultures_file {
		parsing_environment& env;
		cultures_file(parsing_environment& e) : env(e) {}

		void add_group(const std::pair<token_and_type, culture_group_builder>& p) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			p.second.group->name = name;
			env.manager.named_culture_group_index.emplace(name, p.second.group_t);
		}
	};

	inline int discard_group(const token_and_type&, association_type, const religions_group_s&) {
		return 0;
	}
	inline std::pair<token_and_type, religion_builder> bind_religion(const token_and_type& t, association_type, religion_builder&& f) {
		return std::pair<token_and_type, religion_builder>(t, std::move(f));
	}
	inline std::pair<token_and_type, culture_group_builder> bind_culture_group(const token_and_type& t, association_type, culture_group_builder&& f) {
		return std::pair<token_and_type, culture_group_builder>(t, std::move(f));
	}
	inline std::pair<token_and_type, culture_builder> bind_culture(const token_and_type& t, association_type, culture_builder&& f) {
		return std::pair<token_and_type, culture_builder>(t, std::move(f));
	}
}

MEMBER_FDEF(cultures::country_file, set_color, "color");
MEMBER_FDEF(cultures::country_file, add_party, "party");
MEMBER_FDEF(cultures::country_file, discard, "discard");
MEMBER_FDEF(cultures::color_builder, add_value, "color");
MEMBER_FDEF(cultures::fp_color_builder, add_value, "color");
MEMBER_FDEF(cultures::religions_file, add_group, "group");
MEMBER_FDEF(cultures::religions_group_s, add_religion, "religion");
MEMBER_DEF(cultures::religion_builder, icon, "icon");
MEMBER_DEF(cultures::religion_builder, color, "color");
MEMBER_DEF(cultures::religion_builder, pagan, "pagan");
MEMBER_FDEF(cultures::tags_file, add_tag, "tag");
MEMBER_FDEF(cultures::cultures_file, add_group, "group");
MEMBER_FDEF(cultures::culture_group_builder, add_culture, "culture");
MEMBER_FDEF(cultures::culture_group_builder, set_is_overseas, "is_overseas");
MEMBER_FDEF(cultures::culture_group_builder, add_leader, "leader");
MEMBER_FDEF(cultures::culture_group_builder, add_unit, "unit");
MEMBER_FDEF(cultures::culture_group_builder, add_union_tag, "union");
MEMBER_FDEF(cultures::culture_builder, add_color, "color");
MEMBER_FDEF(cultures::culture_builder, set_radicalism, "radicalism");
MEMBER_FDEF(cultures::culture_builder, add_first_names, "first_names");
MEMBER_FDEF(cultures::culture_builder, add_last_names, "last_names");
MEMBER_FDEF(cultures::culture_builder, discard, "discard");
MEMBER_FDEF(cultures::names_builder, add_name, "name");

namespace cultures {
	BEGIN_DOMAIN(culture_file_domain)
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(country_file)
		MEMBER_TYPE_EXTERN("discard", "unit_names", int, discard_country_section)
		MEMBER_ASSOCIATION("discard", "graphical_culture", discard_from_rh)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		MEMBER_TYPE_EXTERN("party", "party", governments::party_tag, inner_read_party)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(parse_religions_domain)
		BEGIN_TYPE(religions_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("group", accept_all, religions_group_s, discard_group)
		END_TYPE
		BEGIN_TYPE(religions_group_s)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("religion", accept_all, religion_builder, bind_religion)
		END_TYPE
		BEGIN_TYPE(religion_builder)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("pagan", "pagan", value_from_rh<bool>)
		MEMBER_TYPE_ASSOCIATION("color", "color", fp_color_builder)
		END_TYPE
		BEGIN_TYPE(fp_color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<double>)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(parse_tags_domain)
		BEGIN_TYPE(tags_file)
		MEMBER_VARIABLE_ASSOCIATION("tag", accept_all, token_pair_from_full)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(parse_cultures_domain)
		BEGIN_TYPE(cultures_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("group", accept_all, culture_group_builder, bind_culture_group)
		END_TYPE
		BEGIN_TYPE(culture_group_builder)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("culture", accept_all, culture_builder, bind_culture)
		MEMBER_ASSOCIATION("leader", "leader", token_from_rh)
		MEMBER_ASSOCIATION("unit", "unit", token_from_rh)
		MEMBER_ASSOCIATION("union", "union", token_from_rh)
		MEMBER_ASSOCIATION("is_overseas", "is_overseas", value_from_rh<bool>)
		END_TYPE
		BEGIN_TYPE(culture_builder)
		MEMBER_TYPE_ASSOCIATION("first_names", "first_names", names_builder)
		MEMBER_TYPE_ASSOCIATION("last_names", "last_names", names_builder)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		MEMBER_ASSOCIATION("radicalism", "radicalism", value_from_rh<float>)
		MEMBER_ASSOCIATION("discard", "primary", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(names_builder)
		MEMBER_VARIABLE_ASSOCIATION("name", accept_all, token_from_lh)
		END_TYPE
		END_DOMAIN;


	void read_flag_graphics(scenario::scenario_manager& s, const directory& source_directory) {

		for(auto& nt : s.culture_m.national_tags) {
			auto tag_name = encoded_tag_to_text_tag(nt.tag_code);

			std::string base("\\gfx\\flags\\");
			base += tag_name.tag;
			std::string communist = base + "_communist.tga";
			std::string republic = base + "_republic.tga";
			std::string fascist = base + "_fascist.tga";
			std::string monarchy = base + "_monarchy.tga";
			base += ".tga";

			nt.base_flag = s.gui_m.textures.retrieve_by_name(source_directory, base.c_str(), base.c_str() + base.length());
			nt.communist_flag = s.gui_m.textures.retrieve_by_name(source_directory, communist.c_str(), communist.c_str() + communist.length());
			nt.republic_flag = s.gui_m.textures.retrieve_by_name(source_directory, republic.c_str(), republic.c_str() + republic.length());
			nt.fascist_flag = s.gui_m.textures.retrieve_by_name(source_directory, fascist.c_str(), fascist.c_str() + fascist.length());
			nt.monarchy_flag = s.gui_m.textures.retrieve_by_name(source_directory, monarchy.c_str(), monarchy.c_str() + monarchy.length());

			if(!nt.communist_flag)
				nt.communist_flag = nt.base_flag;
			if(!nt.republic_flag)
				nt.republic_flag = nt.base_flag;
			if(!nt.fascist_flag)
				nt.fascist_flag = nt.base_flag;
			if(!nt.monarchy_flag)
				nt.monarchy_flag = nt.base_flag;
		}
	}

	void populate_country_names(scenario::scenario_manager& s, tagged_vector<std::string, governments::government_tag> const& gbase_names) {
		s.culture_m.country_names_by_government.reset(static_cast<uint32_t>(s.governments_m.governments_container.size()));
		s.culture_m.country_names_by_government.resize(s.culture_m.national_tags.size());

		for(auto& nt : s.culture_m.national_tags) {
			auto tag_name = encoded_tag_to_text_tag(nt.tag_code);

			std::string base(tag_name.tag);
			std::string base_adj = base + "_ADJ";

			nt.default_name.name = text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, base.c_str(), base.c_str() + base.length());
			nt.default_name.adjective = text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, base_adj.c_str(), base_adj.c_str() + base_adj.length());

			for(int32_t i = 0; i < s.governments_m.governments_container.size(); ++i) {
				const auto gtag = governments::government_tag(static_cast<governments::government_tag::value_base_t>(i));

				std::string base_plus = base + "_" + gbase_names[gtag];
				std::string base_plus_adj = base_plus + "_ADJ";

				auto& pr = s.culture_m.country_names_by_government.get(nt.id, gtag);
				pr.name = text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, base_plus.c_str(), base_plus.c_str() + base_plus.length());
				pr.adjective = text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, base_plus_adj.c_str(), base_plus_adj.c_str() + base_plus_adj.length());
			}
		}
	}

	void read_country_files(tagged_vector<std::string, national_tag> const& v, scenario::scenario_manager& s, const directory& source_directory) {
		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto country_dir = common_dir.get_directory(u"\\countries");

		governments::ready_party_issues(s.governments_m, s.issues_m);

		for(int32_t i = 0; i < v.size(); ++i) {
			const auto this_tag = national_tag(static_cast<national_tag::value_base_t>(i));

			national_tag_object& uc = s.culture_m.national_tags[this_tag];
			const auto file = country_dir.open_file(v[this_tag].c_str(), v[this_tag].c_str() + v[this_tag].length());

			if(file) {
				const auto sz = file->size();

				std::vector<token_group> parse_results;
				const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

				file->read_to_buffer(parse_data.get(), sz);
				parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

				if(!parse_results.empty()) {
					country_file_env env(uc, s);
					parse_object<country_file, culture_file_domain>(
						&parse_results[0],
						&parse_results[0] + parse_results.size(),
						env);
				}
			}
		}
	}

	void read_religions(
		culture_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"religion.txt");

		religion_parsing_environment return_state(text_function, manager);

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(!parse_results.empty()) {
				parse_object<religions_file, parse_religions_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);
			}
		}

		manager.count_religions = uint32_t(manager.religions.size());
	}

	void read_cultures(
		culture_manager& manager,
		graphics::texture_manager &tm,
		const directory& source_directory,
		text_data::text_sequences& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"cultures.txt");

		// create immigrant culture
		char const* name_text = "immigrant";
		auto const immigrant_name = text_data::get_thread_safe_text_handle(text_function, name_text, name_text + 9);

		manager.culture_groups.emplace_back();
		manager.culture_groups[culture_group_tag(0)].id = culture_group_tag(0);
		manager.culture_groups[culture_group_tag(0)].name = immigrant_name;

		manager.culture_container.emplace_back();
		manager.culture_container[culture_tag(0)].id = culture_tag(0);
		manager.culture_container[culture_tag(0)].color = graphics::color_rgb{200ui8, 200ui8, 200ui8};
		manager.culture_container[culture_tag(0)].name = immigrant_name;
		manager.culture_container[culture_tag(0)].group = culture_group_tag(0);

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);

			const auto[file_start, is_unicode] = bom_test(parse_data.get(), sz);

			parsing_environment return_state(text_function, manager, tm, source_directory, is_unicode);
			parse_pdx_file(parse_results, file_start, file_start + sz - (is_unicode ? 3 : 0));

			if(!parse_results.empty()) {
				parse_object<cultures_file, parse_cultures_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);
			}
		}

		for(auto const& i_culture : manager.culture_container) {
			manager.culture_by_culture_group.add_to_row(i_culture.group, i_culture.id);
		}

		std::string noleader_file = std::string("\\gfx\\interface\\leaders\\no_leader.dds");
		manager.no_leader = tm.retrieve_by_name(source_directory, noleader_file.c_str(), noleader_file.c_str() + noleader_file.length());

		manager.count_cultures = uint32_t(manager.culture_container.size());
	}

	tagged_vector<std::string, national_tag> read_national_tags(
		culture_manager& manager,
		const directory& source_directory) {

		tagged_vector<std::string, national_tag> tag_files;
		tags_parsing_env env(tag_files, manager);

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"countries.txt");

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(!parse_results.empty()) {
				parse_object<tags_file, parse_tags_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					env);
			}
		}

		return tag_files;
	}
}
