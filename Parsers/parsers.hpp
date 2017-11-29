#pragma once
#include "parsers.h"
#include <utility>
#include <algorithm>
#include <ctype.h>
#include <string>

template<>
inline int32_t token_to<int32_t>(const token_and_type& in) {
	return parse_int(in.start, in.end);
}

template<>
inline uint32_t token_to<uint32_t>(const token_and_type& in) {
	return parse_uint(in.start, in.end);
}

template<>
inline int16_t token_to<int16_t>(const token_and_type& in) {
	return (int16_t)parse_int(in.start, in.end);
}

template<>
inline uint16_t token_to<uint16_t>(const token_and_type& in) {
	return (uint16_t)parse_uint(in.start, in.end);
}

template<>
inline int8_t token_to<int8_t>(const token_and_type& in) {
	return (int8_t)parse_int(in.start, in.end);
}

template<>
inline uint8_t token_to<uint8_t>(const token_and_type& in) {
	return (uint8_t)parse_uint(in.start, in.end);
}

template<>
inline float token_to<float>(const token_and_type& in) {
	return parse_float(in.start, in.end);
}

template<>
inline double token_to<double>(const token_and_type& in) {
	return parse_double(in.start, in.end);
}

template<>
inline std::string token_to<std::string>(const token_and_type& in) {
	return std::string(in.start, in.end);
}

template<>
inline bool token_to<bool>(const token_and_type& in) {
	return parse_bool(in.start, in.end);
}


template<typename T>
T* advance_position_to_non_whitespace(T* start, T* end) {
	return std::find_if_not(start, end, ignorable_char);
}

template<typename T>
T* advance_position_to_breaking_char(T* start, T* end) {
	return std::find_if(start, end, breaking_char);
}

template<typename T>
T* advance_position_to_next_line(T* start, T* end) {
	const auto start_lterm = std::find_if(start, end, line_termination);
	return std::find_if_not(start_lterm, end, line_termination);
}

template<typename T>
T* advance_position_to_non_comment(T* start, T* end) {
	auto position = advance_position_to_non_whitespace(start, end);
	while (position < end && *position == '#') {
		position = advance_position_to_non_whitespace(advance_position_to_next_line(position, end), end);
	}
	return position;
}

template<typename T>
T* advance_position_to_non_lua_comment(T* start, T* end) {
	auto position = advance_position_to_non_whitespace(start, end);
	while ((position + 1 < end) && ((*position == '-') & (*(position + 1) == '-'))) {
		position = advance_position_to_non_whitespace(advance_position_to_next_line(position, end), end);
	}
	return position;
}


const char* safe_advance(unsigned int n, const char* current, const char* end);

bool ignorable_char(char c);
bool breaking_char(char c);
bool special_identifier_char(char c);
bool not_special_identifier_char(char c);
bool double_quote_termination(char c);
bool single_quote_termination(char c);
bool line_termination(char c);
bool is_positive_integer(const char* start, const char* end);
bool is_integer(const char* start, const char* end);
bool is_positive_fp(const char* start, const char* end);
bool is_fp(const char* start, const char* end);

template<typename T>
T* CALL csv_advance(T* start, T* end, char seperator) {
	while (start != end) {
		if (line_termination(*start))
			return start;
		else if (*start == '\"')
			start = csv_advdance_to_quote_end(start + 1, end);
		else if (*start == seperator)
			return start + 1;
		else
			++start;
	}
	return start;
}

template<typename T>
T* CALL csv_advance_n(uint32_t n, T* start, T* end, char seperator) {
	if (n == 0)
		return start;
	--n;
	
	while (start != end) {
		if (line_termination(*start))
			return start;
		else if (*start == '\"')
			start = csv_advdance_to_quote_end(start + 1, end) - 1;
		else if (*start == seperator) {
			if (n == 0)
				return start + 1;
			else
				--n;
		}
		++start;
	}
	return start;
}

template<typename T>
T* CALL csv_advdance_to_quote_end(T* start, T* end) {
	while (start != end) {
		if (*start == '\"') {
			if (start + 1 == end || *(start + 1) != '\"')
				return start + 1;
			else
				start += 2;
		} else {
			++start;
		}
	}
	return start;
}

std::pair<char*,char*> CALL csv_consume_token(char* start, const char* end, char seperator);

template<typename T>
T* CALL csv_advance_to_next_line(T* start, T* end) {
	while (start != end && !line_termination(*start)) {
		if (*start == '\"')
			start = csv_advdance_to_quote_end(start + 1, end);
		else
			++start;
	}
	while (start != end && line_termination(*start))
		++start;
	return start;
}

template<size_t count_values, typename T>
char* CALL parse_fixed_amount_csv_values(char* start, char* end, char seperator, const T& function) {
	std::pair<char*, char*> values[count_values];
	for (uint32_t i = 0; i < count_values; ++i) {
		auto r = csv_consume_token(start, end, seperator);
		values[i] = std::make_pair(start, r.first);
		start = r.second;
	}
	function(values);
	return csv_advance_to_next_line(start, end);
}

template<typename T>
char* CALL parse_first_and_nth_csv_values(uint32_t nth, char* start, char* end, char seperator, const T& function) {
	auto [first_end, first_next] = csv_consume_token(start, end, seperator);
	std::pair<char*, char*> first_value = std::make_pair(start, first_end);

	start = csv_advance_n(nth - 2ui32, first_next, end, seperator);

	auto [second_end, second_next] = csv_consume_token(start, end, seperator);

	function(first_value, std::make_pair(start, second_end));
	return csv_advance_to_next_line(second_next, end);
}

template<size_t N>
bool is_fixed_token(const char* start, const char* end, const char(&t)[N]) {
	if (end - start != (N-1))
		return false;
	for (unsigned int i = 0; i < N-1; ++i) {
		if (start[i] != t[i])
			return false;
	}
	return true;
}

template<size_t N>
bool is_fixed_token_ci(const char* start, const char* end, const char(&t)[N]) {
	if (end - start != (N-1))
		return false;
	for (unsigned int i = 0; i < N-1; ++i) {
		if (tolower(start[i]) != t[i])
			return false;
	}
	return true;
}

template<size_t N>
bool has_fixed_prefix(const  char* start, const  char* end, const char(&t)[N]) {
	if (end - start < ((ptrdiff_t)N - 1))
		return false;
	for (unsigned int i = 0; i < N - 1; ++i) {
		if (start[i] != t[i])
			return false;
	}
	return true;
}

bool starts_with(const char* start, const char* end, char v);
token_group get_current_token(const char* start, const char* end);

template<typename T>
std::pair<association_type, T*> parse_association_type(T* start, const char* end) {
	if (has_fixed_prefix(start, end, "==")) {
		return std::make_pair(association_type::eq, start + 2);
	} else if (has_fixed_prefix(start, end, "<=")) {
		return std::make_pair(association_type::le, start + 2);
	} else if (has_fixed_prefix(start, end, ">=")) {
		return std::make_pair(association_type::ge, start + 2);
	} else if (has_fixed_prefix(start, end, "!=")) {
		return std::make_pair(association_type::ne, start + 2);
	} else if (has_fixed_prefix(start, end, "<>")) {
		return std::make_pair(association_type::ne, start + 2);
	} else if (starts_with(start, end, '<')) {
		return std::make_pair(association_type::lt, start + 1);
	} else if (starts_with(start, end, '>')) {
		return std::make_pair(association_type::gt, start + 1);
	} else if (starts_with(start, end, '=')) {
		return std::make_pair(association_type::eq_default, start + 1);
	}
	return std::make_pair(association_type::none, start);
}

template<size_t N>
bool is_fixed_token(const token_and_type& g, const char(&t)[N]) {
	return is_fixed_token(g.start, g.end, t);
}

template<size_t N>
bool is_fixed_token_ci(const token_and_type& g, const char(&t)[N]) {
	return is_fixed_token_ci(g.start, g.end, t);
}

const char* parse_token_list(std::vector<token_group>& results, const char* start, const char* end);
const char* parse_lua_token_list(std::vector<token_group>& results, const char* start, const char* end);

template<typename T>
bool any_token(const token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		if (function(*start, start + 1, start + start->group_size + 1))
			return true;
		start += start->group_size + 1;
	}
	return false;
}

template<typename T>
bool any_token_recursive(const token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		if (function(*start, start + 1, start + start->group_size + 1))
			return true;
		++start;
	}
	return false;
}

template<typename T>
bool all_tokens(const token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		if (!function(*start, start + 1, start + start->group_size + 1))
			return false;
		start += start->group_size + 1;
	}
	return true;
}

template<typename T>
bool all_tokens_recursive(const token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		if (!function(*start, start + 1, start + start->group_size + 1))
			return false;
		++start;
	}
	return true;
}

template<typename T>
token_group* find_token(token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		if (function(*start, start + 1, start + start->group_size + 1))
			return start;
		start += start->group_size + 1;
	}
	return start;
}

template<typename T>
token_group* find_token_recursive(token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		if (function(*start, start + 1, start + start->group_size + 1))
			return start;
		++start;
	}
	return start;
}

template<typename T>
void forall_tokens(token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		function(*start, start + 1, start + start->group_size + 1);
		start += start->group_size + 1;
	}
}

template<typename T>
void forall_tokens_recursive(token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		function(*start, start + 1, start + start->group_size + 1);
		++start;
	}
}

template<typename T>
void forall_tokens(const token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		function(*start, start + 1, start + start->group_size + 1);
		start += start->group_size + 1;
	}
}

template<typename T>
void forall_tokens_recursive(const token_group* start, const token_group* end, const T& function) {
	while (start != end) {
		function(*start, start + 1, start + start->group_size + 1);
		++start;
	}
}

#include "streams\\streams.hpp"

