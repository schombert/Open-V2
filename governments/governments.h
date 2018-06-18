#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"


namespace ideologies {
	class ideologies_manager;
}
namespace issues {
	class issues_manager;
}
namespace scenario {
	class scenario_manager;
}

namespace governments {
	enum class flag_type : uint8_t {
		communist, republic, fascist, monarchy, not_specified
	};

	flag_type text_to_flag_type(const char* start, const char* end);
	const char* flag_type_to_text(flag_type t);

	struct government_type {
		text_data::text_tag name;
		uint32_t duration = 48;
		government_tag id;
		flag_type flag = flag_type::not_specified;
		bool appoint_ruling_party = true;
		bool election = false;
	};

	struct party {
		text_data::text_tag name;
		date_tag start_date;
		date_tag end_date;

		party_tag id;

		ideologies::ideology_tag ideology;
	};

	class governments_manager {
	public:
		boost::container::flat_map<text_data::text_tag, government_tag> named_government_index;

		tagged_vector<government_type, government_tag> governments_container;
		tagged_fixed_2dvector<uint8_t, government_tag, ideologies::ideology_tag> permitted_ideologies;
		tagged_vector<party, party_tag> parties;
		tagged_fixed_2dvector<issues::option_tag, party_tag, uint32_t> party_issues; // inner index = nth party issue
	};
}
