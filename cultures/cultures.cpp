#include "cultures.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include <algorithm>

#undef min
#undef max

namespace cultures {
	uint32_t tag_to_encoding(const char* start, const char* end) {
		union {
			char tag[4];
			uint32_t value;
		} local_u;
		memset(&local_u, 0, sizeof(local_u));
		memcpy(local_u.tag, start, std::min(4i64, end - start));
		return local_u.value;
	}

	struct parsing_environment {
		text_handle_lookup text_lookup;
		culture_manager& manager;

		parsing_environment(const text_handle_lookup& tl, culture_manager& m) :
			text_lookup(tl), manager(m) {
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

	struct religion_builder {
		color_builder color;
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
		const text_handle_lookup& text_function;

		tags_parsing_env(tagged_vector<std::string, national_tag>& tv, culture_manager& m, text_handle_lookup& tf) :
			tag_files(tv), manager(m), text_function(tf) {
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
			env.manager.national_tags_index.emplace(tvalue, tt);

			const auto fresult = std::find(p.second.start, p.second.end, '/');
			const auto fstart = (fresult != p.second.end) ? (fresult + 1) : p.second.start;
			env.tag_files.safe_get(tt) = std::string(fstart, p.second.end);


			nt.name = env.text_function(p.first.start, p.first.end);
			std::string tagstr(p.first.start, p.first.end);

			std::string adj_str = tagstr + "_ADJ";
			nt.adjective = env.text_function(adj_str.c_str(), adj_str.c_str() + adj_str.length());

			std::string d_str = tagstr + "_democracy";
			nt.democracy_name = env.text_function(d_str.c_str(), d_str.c_str() + d_str.length());

			std::string d_a_str = tagstr + "_democracy_ADJ";
			nt.democracy_adjective = env.text_function(d_a_str.c_str(), d_a_str.c_str() + d_a_str.length());

			std::string pd_str = tagstr + "_proletarian_dictatorship";
			nt.proletarian_dictatorship_name = env.text_function(pd_str.c_str(), pd_str.c_str() + pd_str.length());

			std::string pd_a_str = tagstr + "_proletarian_dictatorship_ADJ";
			nt.proletarian_dictatorship_adjective = env.text_function(pd_a_str.c_str(), pd_a_str.c_str() + pd_a_str.length());

			std::string am_str = tagstr + "_absolute_monarchy";
			nt.absolute_monarchy_name = env.text_function(am_str.c_str(), am_str.c_str() + am_str.length());

			std::string am_a_str = tagstr + "_absolute_monarchy_ADJ";
			nt.absolute_monarchy_adjective = env.text_function(am_a_str.c_str(), am_a_str.c_str() + am_a_str.length());

			std::string pc_str = tagstr + "_prussian_constitutionalism";
			nt.prussian_constitutionalism_name = env.text_function(pc_str.c_str(), pc_str.c_str() + pc_str.length());

			std::string pc_a_str = tagstr + "_prussian_constitutionalism_ADJ";
			nt.prussian_constitutionalism_adjective = env.text_function(pc_a_str.c_str(), pc_a_str.c_str() + pc_a_str.length());
		}
	};

	inline int discard_group(const token_and_type&, association_type, const religions_group_s&) {
		return 0;
	}
	inline std::pair<token_and_type, religion_builder> bind_religion(const token_and_type& t, association_type, religion_builder& f) {
		return std::pair<token_and_type, religion_builder>(t, std::move(f));
	}

	
}

MEMBER_FDEF(cultures::color_builder, add_value, "color");
MEMBER_FDEF(cultures::religions_file, add_group, "group");
MEMBER_FDEF(cultures::religions_group_s, add_religion, "religion");
MEMBER_DEF(cultures::religion_builder, icon, "icon");
MEMBER_DEF(cultures::religion_builder, color, "color");
MEMBER_DEF(cultures::religion_builder, pagan, "pagan");
MEMBER_FDEF(cultures::tags_file, add_tag, "tag");

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
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		END_TYPE
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(parse_tags_domain)
		BEGIN_TYPE(tags_file)
		MEMBER_VARIABLE_ASSOCIATION("tag", accept_all, token_pair_from_full)
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

	}

	tagged_vector<std::string, national_tag> parse_national_tags(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		tagged_vector<std::string, national_tag> tag_files;
		tags_parsing_env env(tag_files, manager, text_function);

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