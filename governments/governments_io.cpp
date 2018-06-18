#include "common\\common.h"
#include "governments_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "ideologies\\ideologies.h"
#include "scenario\\scenario.h"


namespace governments {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;
		governments_manager& manager;
		const ideologies::ideologies_manager& ideologies_source;
		tagged_vector<std::string, governments::government_tag>& base_names;

		parsing_environment(text_data::text_sequences& tl, governments_manager& m, const ideologies::ideologies_manager& im, tagged_vector<std::string, governments::government_tag>& bn) :
			text_lookup(tl), manager(m), ideologies_source(im), base_names(bn) {}
	};

#ifdef _DEBUG
	struct bad_option {};
	struct not_a_party_issue {};
#endif

	struct party_reader {
		scenario::scenario_manager& s;
		party& under_construction;

		party_reader(scenario::scenario_manager& sm, party& uc) : s(sm), under_construction(uc) {}
		void set_name(token_and_type const& t) {
			under_construction.name = text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end);
		}
		void set_start_date(date_tag t) {
			under_construction.start_date = t;
		}
		void set_end_date(date_tag t) {
			under_construction.end_date = t;
		}
		void set_ideology(token_and_type const& t) {
			under_construction.ideology = tag_from_text(s.ideologies_m.named_ideology_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_issue(token_and_type const& t) {
			const auto option_tag = tag_from_text(s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, t.start, t.end));
			if(is_valid_index(option_tag)) {
				issues::issue_option& opt = s.issues_m.options[option_tag];
				const auto parent_issue = opt.parent_issue;
				if(auto fr = std::find(s.issues_m.party_issues.begin(), s.issues_m.party_issues.end(), parent_issue); fr != s.issues_m.party_issues.end()) {
					auto offset = fr - s.issues_m.party_issues.begin();
					s.governments_m.party_issues.get(under_construction.id, static_cast<uint32_t>(offset)) = option_tag;
				} else {
#ifdef _DEBUG
					throw not_a_party_issue();
#endif
				}
			} else {
#ifdef _DEBUG
				throw bad_option();
#endif
			}
		}
	};

	struct government_builder : public government_type {
		parsing_environment& env;

		government_builder(parsing_environment& e) : env(e) {
			id = e.manager.governments_container.emplace_back();
			env.manager.permitted_ideologies.resize(to_index(id) + 1);
		}

		void set_flag_type(const token_and_type& t) {
			flag = text_to_flag_type(t.start, t.end);
		}
		void set_ideology(const std::pair<token_and_type, bool> & p) {
			const auto ideology_name = text_data::get_thread_safe_text_handle(env.text_lookup, p.first.start, p.first.end);
			if(is_valid_index(ideology_name)) {
				const auto itag = env.ideologies_source.named_ideology_index.find(ideology_name);
				if(itag != env.ideologies_source.named_ideology_index.end() && is_valid_index(itag->second))
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
			env.base_names.safe_get(p.second.id) = std::string(p.first.start, p.first.end);
			env.manager.named_government_index.emplace(gname, dest.id);
		}
	};
	inline std::pair<token_and_type, bool> bind_ideology_t(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, bool>(l, token_to<bool>(r));
	}
	inline std::pair<token_and_type, government_builder> bind_government(const token_and_type& t, association_type, government_builder&& f) {
		return std::pair<token_and_type, government_builder>(t, std::move(f));
	}
	inline token_and_type const& token_from_full(token_and_type const&, association_type, token_and_type const& r) {
		return r;
	}
}

MEMBER_FDEF(governments::party_reader, set_issue, "issue");
MEMBER_FDEF(governments::party_reader, set_ideology, "ideology");
MEMBER_FDEF(governments::party_reader, set_end_date, "end_date");
MEMBER_FDEF(governments::party_reader, set_start_date, "start_date");
MEMBER_FDEF(governments::party_reader, set_name, "name");
MEMBER_FDEF(governments::government_builder, set_flag_type, "flagType");
MEMBER_DEF(governments::government_builder, appoint_ruling_party, "appoint_ruling_party");
MEMBER_DEF(governments::government_builder, election, "election");
MEMBER_DEF(governments::government_builder, duration, "duration");
MEMBER_FDEF(governments::government_builder, set_ideology, "ideology");
MEMBER_FDEF(governments::governments_file, add_government, "government");

namespace governments {
	BEGIN_DOMAIN(party_reading_domain)
		BEGIN_TYPE(party_reader)
		MEMBER_ASSOCIATION("name", "name", token_from_rh)
		MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
		MEMBER_ASSOCIATION("start_date", "start_date", value_from_rh<date_tag>)
		MEMBER_ASSOCIATION("end_date", "end_date", value_from_rh<date_tag>)
		MEMBER_VARIABLE_ASSOCIATION("issue", accept_all, token_from_full)
		END_TYPE
		END_DOMAIN;

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

	void ready_party_issues(governments_manager& manager, issues::issues_manager& im) {
		manager.party_issues.reset(static_cast<uint32_t>(im.party_issues.size()));
	}

	party_tag read_party(token_group const* start, token_group const* end, scenario::scenario_manager& s) {
		const auto ptag = s.governments_m.parties.emplace_back();
		party& uc = s.governments_m.parties[ptag];
		uc.id = ptag;
		s.governments_m.party_issues.resize(to_index(ptag) + 1);

		if(start != end) {
			parse_object<party_reader, party_reading_domain>(start, end, s, uc);
		}

		return ptag;
	}

	flag_type text_to_flag_type(const char* start, const char* end) {
		if(is_fixed_token_ci(start, end, "communist")) {
			return flag_type::communist;
		} else if(is_fixed_token_ci(start, end, "fascist")) {
			return flag_type::fascist;
		} else if(is_fixed_token_ci(start, end, "monarchy")) {
			return flag_type::monarchy;
		} else if(is_fixed_token_ci(start, end, "republic")) {
			return flag_type::republic;
		} else {
			return flag_type::not_specified;
		}
	}
	const char* flag_type_to_text(flag_type t) {
		switch(t) {
			case flag_type::communist: return "communist";
			case flag_type::fascist: return "fascist";
			case flag_type::monarchy: return "monarchy";
			case flag_type::republic: return "republic";
			case flag_type::not_specified: return nullptr;
		}
	}

	tagged_vector<std::string, governments::government_tag> read_governments(
		governments_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function,
		const ideologies::ideologies_manager& ideologies_source) {

		tagged_vector<std::string, governments::government_tag> base_names_result;

		manager.permitted_ideologies.reset(static_cast<uint32_t>(ideologies_source.ideology_container.size()));
		parsing_environment e(text_function, manager, ideologies_source, base_names_result);

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"governments.txt");

		if(file) {
			const auto sz = file->size();

			std::vector<token_group> parse_results;
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() > 0) {
				parse_object<governments_file, governments_file_domain>(
					parse_results.data(),
					parse_results.data() + parse_results.size(),
					e);
			}
		}

		return base_names_result;
	}
}
