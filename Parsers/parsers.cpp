#include "parsers.hpp"
#include <algorithm>
#include <ctype.h>
#include <cstdlib>

#ifdef _DEBUG
#include <Windows.h>
#endif

struct pdx_comment_struct {
	template<typename T>
	static T* comment_filter(T* a, T* b) {
		return advance_position_to_non_comment(a, b);
	}
};
struct lua_comment_struct {
	template<typename T>
	static T* comment_filter(T* a, T* b) {
		return advance_position_to_non_lua_comment(a, b);
	}
};

bool parse_bool(const char* start, const char* end) {
#ifdef _DEBUG
	if (is_fixed_token_ci(start, end, "yes"))
		return true;
	else if (is_fixed_token_ci(start, end, "no"))
		return false;

	OutputDebugStringA("Error: tried to parse ");
	OutputDebugStringA(std::string(start, end).c_str());
	OutputDebugStringA(" as a bool\n");
#endif
	if (start == end)
		return false;
	else
		return (*start == 'Y') | (*start == 'y') | (*start == '1');
}

float parse_float(const char* start, const char* end) {
#ifdef _DEBUG
	if (!is_fp(start, end)) {
		OutputDebugStringA("Error: tried to parse ");
		OutputDebugStringA(std::string(start, end).c_str());
		OutputDebugStringA(" as a float\n");
	}
#endif
	char* buf = (char*)_alloca(end - start + 1);
	memcpy(buf, start, end - start);
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	float result = std::strtof(buf, &out_ptr);

	_freea(buf);
	return result;
}

double parse_double(const char* start, const char* end) {
#ifdef _DEBUG
	if (!is_fp(start, end)) {
		OutputDebugStringA("Error: tried to parse ");
		OutputDebugStringA(std::string(start, end).c_str());
		OutputDebugStringA(" as a double\n");
	}
#endif
	char* buf = (char*)_alloca(end - start + 1);
	memcpy(buf, start, end - start);
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	double result = std::strtod(buf, &out_ptr);

	_freea(buf);
	return result;
}

int32_t parse_int(const char* start, const char* end) {
#ifdef _DEBUG
	if (!is_integer(start, end)) {
		OutputDebugStringA("Error: tried to parse ");
		OutputDebugStringA(std::string(start, end).c_str());
		OutputDebugStringA(" as an integer\n");
	}
#endif

	char* buf = (char*)_alloca(end - start + 1);
	memcpy(buf, start, end - start);
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	int32_t result = std::strtol(buf, &out_ptr, 10);

	_freea(buf);
	return result;
}

uint32_t parse_uint(const char* start, const char* end) {
#ifdef _DEBUG
	if (!is_positive_integer(start, end)) {
		OutputDebugStringA("Error: tried to parse ");
		OutputDebugStringA(std::string(start, end).c_str());
		OutputDebugStringA(" as an unsigned integer\n");
	}
#endif

	char* buf = (char*)_alloca(end - start + 1);
	memcpy(buf, start, end - start);
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	int32_t result = std::strtol(buf, &out_ptr, 10);

	_freea(buf);
	return result >= 0 ? (uint32_t)result : 0ui32;
}

association_type parse_association_type_b(const char* start, const char* end) {
	if (has_fixed_prefix(start, end, "==")) {
		return association_type::eq;
	} else if (has_fixed_prefix(start, end, "<=")) {
		return association_type::le;
	} else if (has_fixed_prefix(start, end, ">=")) {
		return association_type::ge;
	} else if (has_fixed_prefix(start, end, "!=")) {
		return association_type::ne;
	} else if (has_fixed_prefix(start, end, "<>")) {
		return association_type::ne;
	} else if (starts_with(start, end, '<')) {
		return association_type::lt;
	} else if (starts_with(start, end, '>')) {
		return association_type::gt;
	} else if (starts_with(start, end, '=')) {
		return association_type::eq_default;
	}

#ifdef _DEBUG
	if (!is_positive_integer(start, end)) {
		OutputDebugStringA("Error: tried to parse ");
		OutputDebugStringA(std::string(start, end).c_str());
		OutputDebugStringA(" as a comparison operator\n");
	}
#endif

	return association_type::none;
}

template<typename T>
const char* _parse_token_list(std::vector<token_group>& results, const char* start, const char* end);

void parse_pdx_file(std::vector<token_group>& results, const char* file_start, const char* file_end) {
	results.clear();
	_parse_token_list<pdx_comment_struct>(results, file_start, file_end);
}

void parse_lua_file(std::vector<token_group>& results, const char* file_start, const char* file_end) {
	results.clear();
	_parse_token_list<lua_comment_struct>(results, file_start, file_end);
}

bool ignorable_char(char c) {
	return isspace(c) | (c == ',') | (c == ';');
}

bool breaking_char(char c) {
	return ignorable_char(c) | (c == '{') | (c == '}') | special_identifier_char(c) | (c == '\"') | (c == '\'') | (c == '#');
}

bool special_identifier_char(char c) {
	return (c == '!') | (c == '=') | (c == '<') | (c == '>');
}

bool not_special_identifier_char(char c) {
	return !special_identifier_char(c);
}

bool line_termination(char c) {
	return (c == '\r') | (c == '\n');
}

bool double_quote_termination(char c) {
	return (c == '\r') | (c == '\n') | (c == '\"');
}

bool single_quote_termination(char c) {
	return (c == '\r') | (c == '\n') | (c == '\'');
}

bool is_positive_integer(const char* start, const char* end) {
	if (start == end)
		return false;
	return end == std::find_if_not(start, end, isdigit);
}

bool is_integer(const char* start, const char* end) {
	if (start == end)
		return false;
	if (*start == '-')
		return is_positive_integer(start + 1, end);
	else
		return is_positive_integer(start, end);
}

bool is_positive_fp(const char* start, const char* end) {
	const auto decimal = std::find(start, end, '.');
	if (decimal == end) {
		return is_positive_integer(start, end);
	} else if(decimal == start) {
		return is_positive_integer(decimal + 1, end);
	} else {
		return is_positive_integer(start, decimal) && (decimal + 1 == end || is_positive_integer(decimal + 1, end));
	}
}

bool is_fp(const char* start, const char* end) {
	if (start == end)
		return false;
	if (*start == '-')
		return is_positive_fp(start + 1, end);
	else
		return is_positive_fp(start, end);
}

const char* safe_advance(unsigned int n, const char* current, const char* end) {
	if (current + n > end)
		return end;
	return current + n;
}

bool starts_with(const char* start, const char* end, char v) {
	return start != end && *start == v;
}

token_group get_current_token(const char* start, const char* end) {
	if (starts_with(start, end, '\"')) {
		const auto close = std::find_if(start + 1, end, double_quote_termination);
		return token_group{ token_and_type{ start + 1, close, token_type::quoted_string }, association_type::none, 0 };
	} else if(starts_with(start, end, '\'')) {
		const auto close = std::find_if(start + 1, end, single_quote_termination);
		return token_group{ token_and_type{ start + 1, close, token_type::quoted_string }, association_type::none, 0 };
	} else {
		return token_group{ token_and_type{ start, advance_position_to_breaking_char(start, end), token_type::identifier }, association_type::none, 0 };
	}
}

template<typename T>
const char* _parse_token_list(std::vector<token_group>& results, const char* start, const char* end) {
	start = T::comment_filter(start, end);

	while (start != end) {
		if (*start == '}') {
			return start + 1;
		} else if (*start == '{') {
			const auto dest_slot = results.size();
			results.emplace_back(token_group{ token_and_type{ start, start, token_type::identifier }, association_type::list, 0 });

			start = T::comment_filter(_parse_token_list<T>(results, start + 1, end), end);
			results[dest_slot].group_size = static_cast<unsigned short>(results.size() - dest_slot - 1);
		} else {
			const auto dest_slot = results.size();
			results.emplace_back(get_current_token(start, end));
			start = T::comment_filter(results[dest_slot].token.end + (results[dest_slot].token.type == token_type::quoted_string ? 1 : 0), end);

			if (start != end) {
				const auto atype = parse_association_type(start, end);
				if (atype.first != association_type::none) {
					results[dest_slot].association = atype.first;

					const auto righthand = T::comment_filter(atype.second, end);
					if(starts_with(righthand, end, '{')) {
						results[dest_slot].association = association_type::list;
						start = T::comment_filter(_parse_token_list<T>(results, righthand + 1, end), end);
						results[dest_slot].group_size = static_cast<unsigned short>(results.size() - dest_slot - 1);
					} else {
						results.emplace_back(get_current_token(righthand, end));
						results[dest_slot].group_size = 1;
						start = T::comment_filter(results.back().token.end + (results.back().token.type == token_type::quoted_string ? 1 : 0), end);
					}
					
				} else if (*start == '{') {
					results[dest_slot].association = association_type::list;

					start = T::comment_filter(_parse_token_list<T>(results, start+1, end), end);
					results[dest_slot].group_size = static_cast<unsigned short>(results.size() - dest_slot - 1);
				}
			}
		}
	}

	return start;
}

const char* parse_token_list(std::vector<token_group>& results, const char* start, const char* end) {
	return _parse_token_list<pdx_comment_struct>(results, start, end);
}

const char* parse_lua_token_list(std::vector<token_group>& results, const char* start, const char* end) {
	return _parse_token_list<lua_comment_struct>(results, start, end);
}

std::pair<char*, char*> CALL csv_re_write_token(char* start, const char* end, char seperator) {
	char* write_pos = start;
	bool terminate_on_seperator = true;

	if (start + 1 == end) {
		return std::make_pair(write_pos, start + 1);
	} else if (*(start + 1) == '\"') {
		++start;
		*(write_pos++) = *(start++);
	} else {
		terminate_on_seperator = !terminate_on_seperator;
		++start;
	}

	*(write_pos++) = *(start++);
	
	while (start != end) {
		if (line_termination(*start) & terminate_on_seperator)
			return std::make_pair(write_pos, start);
		else if (*start == '\"') {
			if (start + 1 == end) {
				return std::make_pair(write_pos, start + 1);
			} else if (*(start + 1) == '\"') {
				++start;
				*(write_pos++) = *(start++);
			} else {
				terminate_on_seperator = !terminate_on_seperator;
				++start;
			}
		} else if ((*start == seperator) & terminate_on_seperator)
			return std::make_pair(write_pos, start + 1);
		else
			*(write_pos++) = *(start++);
	}
	return std::make_pair(write_pos, start);
}

std::pair<char*, char*> CALL csv_consume_token(char* start, const char* end, char seperator) {
	while (start != end) {
		if (line_termination(*start))
			return std::make_pair(start, start);
		else if (*start == '\"')
			return csv_re_write_token(start, end, seperator);
		else if (*start == seperator)
			return std::make_pair(start, start + 1);
		else
			++start;
	}
	return std::make_pair(start, start);
}
