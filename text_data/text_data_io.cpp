#include "text_data_io.h"
#include "Parsers\\parsers.hpp"

namespace text_data {
	void load_text_sequences_from_directory(const directory& source_directory, text_data::text_sequences& container) {
		const auto csv_files = source_directory.list_files(u".csv");

		container.all_components.reserve(34000);
		container.all_sequences.reserve(22000);
		container.text_data.reserve(1200000);
		container.key_data.reserve(330000);

		std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(vector_backed_string_less_ci(container.key_data));

		for(auto& f : csv_files) {
			auto op = f.open_file();
			if(op) {
				const auto sz = op->size();
				auto buffer = new char[sz];

				op->read_to_buffer(buffer, sz);

				const bool is_utf8 = sz >= 3 && (((uint8_t)buffer[0] == 0xEF) & ((uint8_t)buffer[1] == 0xBB) & ((uint8_t)buffer[2] == 0xBF));

				if(is_utf8) {
					auto start = (sz > 3 && buffer[3] == '#') ? csv_advance_to_next_line(buffer + 3, buffer + sz) : buffer + 3;
					while(start < buffer + sz) {
						start = parse_first_and_nth_csv_values(2, start, buffer + sz, ';', [&container, &temp_map](const auto& pr_a, const auto& pr_b) {
							add_utf8_sequence(container, temp_map, pr_a.first, pr_a.second, pr_b.first, pr_b.second);
						});
					}
				} else {
					auto start = (sz != 0 && buffer[0] == '#') ? csv_advance_to_next_line(buffer, buffer + sz) : buffer;
					while(start < buffer + sz) {
						start = parse_first_and_nth_csv_values(2, start, buffer + sz, ';', [&container, &temp_map](const auto& pr_a, const auto& pr_b) {
							add_win1250_sequence(container, temp_map, pr_a.first, pr_a.second, pr_b.first, pr_b.second);
						});
					}
				}

				delete[] buffer;
			}
		}

		container.key_to_sequence_map.~flat_map();
		new (&container.key_to_sequence_map) boost::container::flat_map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci>(boost::container::ordered_unique_range_t(), temp_map.begin(), temp_map.end(), vector_backed_string_less_ci(container.key_data));
	}
}
