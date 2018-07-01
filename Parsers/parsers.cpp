#include "parsers.hpp"
#include <algorithm>
#include <ctype.h>
#include <cstdlib>
#include <Windows.h>

#undef min
#undef max

std::pair<char*, char*> CALL csv_re_write_token(char* start, const char* end, char seperator);

char16_t win1250toUTF16(char in) {
	constexpr static char16_t converted[256] =
//       0       1         2         3         4         5         6         7         8         9         A         B         C         D         E         F
/*0*/{ u' ' ,  u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u'\t' ,   u'\n' ,   u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,
/*1*/  u' ' ,  u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,    u' ' ,
/*2*/  u' ' ,  u'!' ,    u'\"',    u'#' ,    u'$' ,    u'%' ,    u'&' ,    u'\'',    u'(' ,    u')' ,    u'*' ,    u'+' ,    u',' ,    u'-' ,    u'.' ,    u'/' ,
/*3*/  u'0' ,  u'1' ,    u'2' ,    u'3' ,    u'4' ,    u'5' ,    u'6' ,    u'7' ,    u'8' ,    u'9' ,    u':' ,    u';' ,    u'<' ,    u'=' ,    u'>' ,    u'?' ,
/*4*/  u'@' ,  u'A' ,    u'B' ,    u'C' ,    u'D' ,    u'E' ,    u'F' ,    u'G' ,    u'H' ,    u'I' ,    u'J' ,    u'K' ,    u'L' ,    u'M' ,    u'N' ,    u'O' ,
/*5*/  u'P' ,  u'Q' ,    u'R' ,    u'S' ,    u'T' ,    u'U' ,    u'V' ,    u'W' ,    u'X' ,    u'Y' ,    u'Z' ,    u'[' ,    u'\\',    u']' ,    u'^' ,    u'_' ,
/*6*/  u'`' ,  u'a' ,    u'b' ,    u'c' ,    u'd' ,    u'e' ,    u'f' ,    u'g' ,    u'h' ,    u'i' ,    u'j' ,    u'k' ,    u'l',     u'm' ,    u'n' ,    u'o' ,
/*7*/  u'p' ,  u'q' ,    u'r' ,    u's' ,    u't' ,    u'u' ,    u'v' ,    u'w' ,    u'x' ,    u'y' ,    u'z' ,    u'{' ,    u'|',     u'}' ,    u'~' ,    u' ' ,
/*8*/u'\u20AC',u' ',     u'\u201A',u' ',     u'\u201E',u'\u2026',u'\u2020',u'\u2021',u' ',     u'\u2030',u'\u0160',u'\u2039',u'\u015A',u'\u0164',u'\u017D',u'\u0179',
/*9*/u' ',     u'\u2018',u'\u2019',u'\u201C',u'\u201D',u'\u2022',u'\u2013',u'\u2014',u' ',     u'\u2122',u'\u0161',u'\u203A',u'\u015B',u'\u0165',u'\u017E',u'\u017A',
/*A*/u' ',     u'\u02C7',u'\u02D8',u'\u00A2',u'\u00A3',u'\u0104',u'\u00A6',u'\u00A7',u'\u00A8',u'\u00A9',u'\u015E',u'\u00AB',u'\u00AC',u'-',     u'\u00AE',u'\u017B',
/*B*/u'\u00B0',u'\u00B1',u'\u02DB',u'\u0142',u'\u00B4',u'\u00B5',u'\u00B6',u'\u00B7',u'\u00B8',u'\u0105',u'\u015F',u'\u00BB',u'\u013D',u'\u02DD',u'\u013E',u'\u017C',
/*C*/u'\u0154',u'\u00C1',u'\u00C2',u'\u0102',u'\u00C4',u'\u0139',u'\u0106',u'\u00C7',u'\u010C',u'\u00C9',u'\u0118',u'\u00CB',u'\u011A',u'\u00CD',u'\u00CE',u'\u010E',
/*D*/u'\u0110',u'\u0143',u'\u0147',u'\u00D3',u'\u00D4',u'\u0150',u'\u00D6',u'\u00D7',u'\u0158',u'\u016E',u'\u00DA',u'\u0170',u'\u00DC',u'\u00DD',u'\u0162',u'\u00DF',
/*E*/u'\u0115',u'\u00E1',u'\u00E2',u'\u0103',u'\u00E4',u'\u013A',u'\u0107',u'\u00E7',u'\u010D',u'\u00E9',u'\u0119',u'\u00EB',u'\u011B',u'\u00ED',u'\u00EE',u'\u010F',
/*F*/u'\u0111',u'\u0144',u'\u0148',u'\u00F3',u'\u00F4',u'\u0151',u'\u00F6',u'\u00F7',u'\u0159',u'\u016F',u'\u00FA',u'\u0171',u'\u00FC',u'\u00FD',u'\u0163',u'\u02D9'
	};

	return converted[(uint8_t)in];
}

std::u16string win1250toUTF16(const char* start, const char* end) {
	std::u16string result;
	for (; start != end; ++start)
		result += win1250toUTF16(*start);
	return result;
}

std::u16string UTF8toUTF16(const char* start, const char* end) {
	const auto size = MultiByteToWideChar(CP_UTF8, 0, start, (int)(end - start), nullptr, 0);
	wchar_t* buffer = new wchar_t[(size_t)size];
	MultiByteToWideChar(CP_UTF8, 0, start, (int)(end - start), buffer, size);
	std::u16string result((char16_t*)buffer, (char16_t*)(buffer + size));
	delete[] buffer;
	return result;
}

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
	char* buf = (char*)_alloca((size_t)(end - start + 1));
	memcpy(buf, start, (size_t)(end - start));
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	float result = std::strtof(buf, &out_ptr);

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
	char* buf = (char*)_alloca((size_t)(end - start + 1));
	memcpy(buf, start, (size_t)(end - start));
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	double result = std::strtod(buf, &out_ptr);

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

	char* buf = (char*)_alloca((size_t)(end - start + 1));
	memcpy(buf, start, (size_t)(end - start));
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	int32_t result = std::strtol(buf, &out_ptr, 10);

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

	char* buf = (char*)_alloca((size_t)(end - start + 1));
	memcpy(buf, start, (size_t)(end - start));
	char* out_ptr = nullptr;
	buf[end - start] = 0;

	int32_t result = std::strtol(buf, &out_ptr, 10);

	return result >= 0 ? (uint32_t)result : 0ui32;
}

date_tag parse_date(const char* start, const char* end) {
	const auto first_dot = std::find(start, end, '.');
	const auto second_dot = std::find(first_dot + 1, end, '.');

	const auto year = static_cast<uint16_t>(parse_uint(start, first_dot));
	const auto month = static_cast<uint16_t>(parse_uint(first_dot + 1, second_dot));
	const auto day = static_cast<uint16_t>(parse_uint(second_dot + 1, end));

	return date_to_tag(boost::gregorian::date(year, month, day));
}

date_tag parse_date(const char16_t* start, const char16_t* end) {
	const auto first_dot = std::find(start, end, u'.');
	const auto second_dot = std::find(first_dot + 1, end, u'.');

	const auto year = static_cast<uint16_t>(u16atoui(start, first_dot));
	const auto month = static_cast<uint16_t>(u16atoui(first_dot + 1, second_dot));
	const auto day = static_cast<uint16_t>(u16atoui(second_dot + 1, end));

	return date_to_tag(boost::gregorian::date(year, month, day));
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
	return (c == ' ') | (c == '\r') | (c == '\f') | (c == '\n') | (c == '\t') | (c == ',') | (c == ';');
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
