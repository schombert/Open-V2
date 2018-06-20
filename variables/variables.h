#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"

namespace variables {
	class variables_state {
	public:
		tagged_vector<float, global_variable_tag> global_variables;
	};

	class variables_manager {
	public:
		uint32_t count_national_variables = 0;
		uint32_t count_national_flags = 0;
		uint32_t count_global_variables = 0;

		boost::container::flat_map<text_data::text_tag, global_variable_tag> named_global_variables;
		boost::container::flat_map<text_data::text_tag, national_variable_tag> named_national_variables;
		boost::container::flat_map<text_data::text_tag, national_flag_tag> named_national_flags;

		global_variable_tag  get_named_global_variable(text_data::text_tag name);
		national_variable_tag  get_named_national_variable(text_data::text_tag name);
		national_flag_tag  get_named_national_flag(text_data::text_tag name);
	};
}
