#include "cultures.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include <algorithm>

#undef min
#undef max

namespace cultures {
	uint32_t tag_to_encoding(const char* start, const char* end) {
		union {
			tag_as_text tag;
			uint32_t value;
		} local_u;

		memset(&local_u, 0, sizeof(local_u));
		memcpy(&local_u.tag, start, std::min(4ui64, static_cast<size_t>(end - start)));
		return local_u.value;
	}

	tag_as_text encoded_tag_to_text_tag(uint32_t tag_value) {
		union {
			tag_as_text tag;
			uint32_t value;
		} local_u;

		local_u.value = tag_value;
		return local_u.tag;
	}

	struct parsing_environment {
		text_handle_lookup text_lookup;
		culture_manager& manager;
		bool unicode = false;

		parsing_environment(const text_handle_lookup& tl, culture_manager& m, bool u = false) :
			text_lookup(tl), manager(m), unicode(u) {
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

	struct fp_color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(double v) {
			switch (current_color) {
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
		parsing_environment& env;
		religions_group_s(parsing_environment& e) : env(e) {}
		void add_religion(const std::pair<token_and_type, religion_builder>& p) {
			const auto name = env.text_lookup(p.first.start, p.first.end);
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
		parsing_environment& env;
		religions_file(parsing_environment& e) : env(e) {}
		void add_group(int) {}
	};

	struct tags_parsing_env {
		tagged_vector<std::string, national_tag>& tag_files;
		culture_manager& manager;

		tags_parsing_env(tagged_vector<std::string, national_tag>& tv, culture_manager& m) :
			tag_files(tv), manager(m) {
		}
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

		names_builder(parsing_environment& e) : env(e) {
		}
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
			env.manager.first_names_by_culture.add_range_to_row(to_index(ctag), names.names.begin(), names.names.end());
		}
		void add_last_names(const names_builder& names) {
			env.manager.last_names_by_culture.add_range_to_row(to_index(ctag), names.names.begin(), names.names.end());
		}
		void set_radicalism(float f) {
			c->radicalism = f;
		}
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
		void add_leader(const token_and_type&) {

		}
		void add_culture(const std::pair<token_and_type, culture_builder>& p) {
			const auto name = env.text_lookup(p.first.start, p.first.end);

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
			const auto name = env.text_lookup(p.first.start, p.first.end);
			p.second.group->name = name;
			env.manager.named_culture_group_index.emplace(name, p.second.group_t);
		}
	};

	inline int discard_group(const token_and_type&, association_type, const religions_group_s&) {
		return 0;
	}
	inline std::pair<token_and_type, religion_builder> bind_religion(const token_and_type& t, association_type, religion_builder& f) {
		return std::pair<token_and_type, religion_builder>(t, std::move(f));
	}
	inline std::pair<token_and_type, culture_group_builder> bind_culture_group(const token_and_type& t, association_type, culture_group_builder& f) {
		return std::pair<token_and_type, culture_group_builder>(t, std::move(f));
	}
	inline std::pair<token_and_type, culture_builder> bind_culture(const token_and_type& t, association_type, culture_builder& f) {
		return std::pair<token_and_type, culture_builder>(t, std::move(f));
	}
}

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
MEMBER_FDEF(cultures::names_builder, add_name, "name");

namespace cultures {
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
		MEMBER_ASSOCIATION("radicalism", "radicalism", value_from_rh<float> )
		END_TYPE
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(names_builder)
		MEMBER_VARIABLE_ASSOCIATION("name", accept_all, token_from_lh)
		END_TYPE
	END_DOMAIN;

	void parse_religions(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"religion.txt");

		parsing_environment return_state(text_function, manager);

		if (file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if (parse_results.size() > 0) {
				parse_object<religions_file, parse_religions_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);
			}
		}
	}

	void parse_cultures(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"cultures.txt");

		if (file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);

			const auto[file_start, is_unicode] = bom_test(parse_data.get(), sz);

			parsing_environment return_state(text_function, manager, is_unicode);
			parse_pdx_file(parse_results, file_start, file_start + sz - (is_unicode ? 3 : 0));

			if (parse_results.size() > 0) {
				parse_object<cultures_file, parse_cultures_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					return_state);
			}
		}
	}

	tagged_vector<std::string, national_tag> parse_national_tags(
		culture_manager& manager,
		const directory& source_directory) {

		tagged_vector<std::string, national_tag> tag_files;
		tags_parsing_env env(tag_files, manager);

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"countries.txt");

		if (file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if (parse_results.size() > 0) {
				parse_object<tags_file, parse_tags_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					env);
			}
		}

		return tag_files;
	}
}
