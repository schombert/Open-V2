#include "variables.h"

namespace variables {
	global_variable_tag  variables_manager::get_named_global_variable(text_data::text_tag name) {
		const auto fr = named_global_variables.find(name);
		if (fr != named_global_variables.end())
			return fr->second;
		
		const global_variable_tag nt(static_cast<global_variable_tag::value_base_t>(count_global_variables++));
		named_global_variables.emplace(name, nt);
		return nt;
	}
	national_variable_tag  variables_manager::get_named_national_variable(text_data::text_tag name) {
		const auto fr = named_national_variables.find(name);
		if (fr != named_national_variables.end())
			return fr->second;

		const national_variable_tag nt(static_cast<national_variable_tag::value_base_t>(count_national_variables++));
		named_national_variables.emplace(name, nt);
		return nt;
	}
	national_flag_tag variables_manager::get_named_national_flag(text_data::text_tag name) {
		const auto fr = named_national_flags.find(name);
		if(fr != named_national_flags.end())
			return fr->second;

		const national_flag_tag nt(static_cast<national_flag_tag::value_base_t>(count_national_flags++));
		named_national_flags.emplace(name, nt);
		return nt;
	}
}
