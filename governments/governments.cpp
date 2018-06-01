#include "governments.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "ideologies\\ideologies.h"

namespace governments {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;
		governments_manager& manager;
		const ideologies::ideologies_manager& ideologies_source;

		parsing_environment(text_data::text_sequences& tl, governments_manager& m, const ideologies::ideologies_manager& im) :
			text_lookup(tl), manager(m), ideologies_source(im) {
		}
	};

	struct government_builder : public government_type {
		parsing_environment& env;

		government_builder(parsing_environment& e) : env(e) {
			id = e.manager.governments_container.emplace_back();
			env.manager.permitted_ideologies.safe_get(id, ideologies::ideology_tag(0));
		}

		void set_flag_type(const token_and_type& t) {
			flag = text_to_flag_type(t.start, t.end);
		}
		void set_ideology(const std::pair<token_and_type, bool> & p) {
			const auto ideology_name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			if (is_valid_index(ideology_name)) {
				const auto itag = env.ideologies_source.named_ideology_index.find(ideology_name);
				if (itag != env.ideologies_source.named_ideology_index.end() && is_valid_index(itag->second))
					env.manager.permitted_ideologies.safe_get(id, itag->second) = static_cast<uint8_t>(p.second);
			}
		}
	};
	struct governments_file {
		parsing_environment& env;
		governments_file(parsing_environment& e) : env(e) {}

		void add_government(const std::pair<token_and_type, government_builder>& p) {
			const auto gname = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			auto& dest = env.manager.governments_container[p.second.id];
			dest = p.second;
			dest.name = gname;
			dest.id = p.second.id;
			dest.name_base = std::string(p.first.start, p.first.end);
			env.manager.named_government_index.emplace(gname, dest.id);
		}
	};
	inline std::pair<token_and_type, bool> bind_ideology_t(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, bool>(l, token_to<bool>(r));
	}
	inline std::pair<token_and_type, government_builder> bind_government(const token_and_type& t, association_type, government_builder&& f) {
		return std::pair<token_and_type, government_builder>(t, std::move(f));
	}
}

MEMBER_FDEF(governments::government_builder, set_flag_type, "flagType");
MEMBER_DEF(governments::government_builder, appoint_ruling_party, "appoint_ruling_party");
MEMBER_DEF(governments::government_builder, election, "election");
MEMBER_DEF(governments::government_builder, duration, "duration");
MEMBER_FDEF(governments::government_builder, set_ideology, "ideology");
MEMBER_FDEF(governments::governments_file, add_government, "government");

namespace governments {
	BEGIN_DOMAIN(governments_file_domain)
		BEGIN_TYPE(governments_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("government", accept_all, government_builder, bind_government)
		END_TYPE
		BEGIN_TYPE(government_builder)
		MEMBER_VARIABLE_ASSOCIATION("ideology", accept_all, bind_ideology_t)
		MEMBER_ASSOCIATION("appoint_ruling_party", "appoint_ruling_party", value_from_rh<bool>)
		MEMBER_ASSOCIATION("election", "election", value_from_rh<bool>)
		MEMBER_ASSOCIATION("duration", "duration", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("flagType", "flagType", token_from_rh)
		END_TYPE
	END_DOMAIN;

	flag_type text_to_flag_type(const char* start, const char* end) {
		if (is_fixed_token_ci(start, end, "communist")) {
			return flag_type::communist;
		} else if (is_fixed_token_ci(start, end, "fascist")) {
			return flag_type::fascist;
		} else if (is_fixed_token_ci(start, end, "monarchy")) {
			return flag_type::monarchy;
		} else if (is_fixed_token_ci(start, end, "republic")) {
			return flag_type::republic;
		} else {
			return flag_type::not_specified;
		}
	}
	const char* flag_type_to_text(flag_type t) {
		switch (t) {
			case flag_type::communist: return "communist";
			case flag_type::fascist: return "fascist";
			case flag_type::monarchy: return "monarchy";
			case flag_type::republic: return "republic";
			case flag_type::not_specified: return nullptr;
		}
	}

	void read_governments(
		governments_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function,
		const ideologies::ideologies_manager& ideologies_source) {

		manager.permitted_ideologies.reset(static_cast<uint32_t>(ideologies_source.ideology_container.size()));
		parsing_environment e(text_function, manager, ideologies_source);

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"governments.txt");

		if (file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if (parse_results.size() > 0) {
				parse_object<governments_file, governments_file_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					e);
			}
		}
	}
}
