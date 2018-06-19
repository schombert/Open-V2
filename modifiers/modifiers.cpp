#include "modifiers.h"

namespace modifiers {
	national_modifier_tag modifiers_manager::fetch_unique_national_modifier(text_data::text_tag n) {
		const auto f = named_national_modifiers_index.find(n);
		if(f != named_national_modifiers_index.end())
			return f->second;
		const auto tag = national_modifiers.emplace_back();
		auto& nm = national_modifiers[tag];
		nm.id = tag;
		nm.name = n;
		if(is_valid_index(n))
			named_national_modifiers_index.emplace(n, tag);
		return tag;
	}

	provincial_modifier_tag modifiers_manager::fetch_unique_provincial_modifier(text_data::text_tag n) {
		const auto f = named_provincial_modifiers_index.find(n);
		if(f != named_provincial_modifiers_index.end())
			return f->second;
		const auto tag = provincial_modifiers.emplace_back();
		auto& nm = provincial_modifiers[tag];
		nm.id = tag;
		nm.name = n;
		if(is_valid_index(n))
			named_provincial_modifiers_index.emplace(n, tag);
		return tag;
	}
}
