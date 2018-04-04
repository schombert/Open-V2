#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include <variant>

namespace issues {
	enum class issue_group : uint8_t {
		party,
		social,
		political,
		economic,
		military
	};
	inline constexpr bool is_unciv_issue(issue_group type) {
		return type == issue_group::economic || type == issue_group::military;
	}
	struct issue_option {
		text_data::text_tag name;
		option_tag id;
	};
	struct issue {
		tagged_vector<issue_option, option_tag> options;

		text_data::text_tag name;
		issue_group type;
		bool next_step_only;
		bool administrative;
		
		issue_tag id;
	};
	struct unciv_issue_option {
		text_data::text_tag name;
		option_tag id;
	};
	struct unciv_issue {
		tagged_vector<unciv_issue_option, option_tag> options;

		text_data::text_tag name;
		issue_group type;
		bool next_step_only;
		bool administrative;

		unciv_issue_tag id;
	};
	struct issue_idenfitier {
		std::variant<std::monostate, issue_tag, unciv_issue_tag> id;
		issue_group type;
	};
	struct option_identifier : public issue_idenfitier {
		option_tag option_id;
	};
	static_assert(sizeof(option_identifier) <= 4);

	class issues_manager {
	public:
		boost::container::flat_map<text_data::text_tag, issue_idenfitier> named_issue_index;
		boost::container::flat_map<text_data::text_tag, option_identifier> named_option_index;

		tagged_vector<issue, issue_tag> issues_cotnainer;
		tagged_vector<unciv_issue, unciv_issue_tag> unciv_issues_cotnainer;
		std::vector<issue_tag> party_issues;
		std::vector<issue_tag> political_issues;
		std::vector<issue_tag> social_issues;
		std::vector<unciv_issue_tag> economic_issues;
		std::vector<unciv_issue_tag> military_issues;
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, issues_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	parsing_state pre_parse_issues(
		issues_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
}
