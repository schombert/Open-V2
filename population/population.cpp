#include "population.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace population {
	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto poptype_dir = source_directory.get_directory(u"\\poptypes");
		const auto poptype_files = poptype_dir.list_files(u".txt");

		for (const auto& file : poptype_files) {
			const auto fname = file.file_name();
			const auto clipped_unicode = fname.size() >= 4ui64 ? std::string(fname.begin(), fname.end() - 4ui32) : std::string("");
			const auto name_tag = text_function(clipped_unicode.c_str(), clipped_unicode.c_str() + clipped_unicode.size());

			++manager.count_poptypes;
			const auto new_ptype_tag = manager.pop_types.emplace_back();
			auto& new_poptype = manager.pop_types[new_ptype_tag];
			new_poptype.id = new_ptype_tag;
			new_poptype.name = name_tag;
			manager.named_pop_type_index.emplace(name_tag, new_ptype_tag);
		}
	}
}
