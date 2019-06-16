#pragma once
#include <vector>
#include <memory>
#include "common\\common.h"
#include "common\\shared_tags.h"

enum class association_type : unsigned short { none, eq, lt, le, gt, ge, ne, eq_default, list};

enum class token_type {
	identifier,
	quoted_string,
	special_identifier,
	brace,
	open_brace,
	close_brace,
	unknown
};

struct token_and_type {
	const char* start = nullptr;
	const char* end = nullptr;
	token_type type = token_type::unknown;
};

class token_group {
public:
	token_and_type token;
	association_type association = association_type::none;
	unsigned short group_size = 0;
};

class parsed_data {
public:
	std::vector<token_group> parse_results;
	std::unique_ptr<char[]> parse_data;
};

class token_generator {
private:
	char const* position;
	char const* const file_end;

	token_and_type peek_1;
	token_and_type peek_2;

	token_and_type internal_next();
public:
	token_generator(char const* file_start, char const* fe) : position(file_start), file_end(fe) {}

	bool at_end() const {
		return peek_2.type == token_type::unknown &&  peek_1.type == token_type::unknown && position >= file_end;
	}
	token_and_type get();
	token_and_type next();
	token_and_type next_next();
};

void discard_group(token_generator& gen);
template<typename T>
void finish_parse(T&) {}

class empty_error_handler {
public:
	static void unhandled_group_key(token_and_type const&) {}
	static void unhandled_association_key(token_and_type const&) {}
	static void unhandled_free_value(token_and_type const&) {}
	static void unhandled_free_set() {}
	static void bad_date(token_and_type const&) {}
	static void bad_value(token_and_type const&) {}
};

class abort_error_handler {
public:
	static void unhandled_group_key(token_and_type const&) { std::abort(); }
	static void unhandled_association_key(token_and_type const&) { std::abort(); }
	static void unhandled_free_value(token_and_type const&) { std::abort(); }
	static void unhandled_free_set() { std::abort(); }
	static void bad_date(token_and_type const&) { std::abort(); }
	static void bad_value(token_and_type const&) { std::abort(); }
};

template<typename T>
T token_to(const token_and_type& in);

template<typename T, typename ERR_H>
T token_to(const token_and_type& in, ERR_H& err);

class end_brace_finder;
class vector_of_strings_operation;
class tokenize_char_stream_operation;

float parse_float(const char* start, const char* end);
bool parse_bool(const char* start, const char* end);
double parse_double(const char* start, const char* end);
int32_t parse_int(const char* start, const char* end);
uint32_t parse_uint(const char* start, const char* end);
date_tag parse_date(const char* start, const char* end);
date_tag parse_date(const char16_t* start, const char16_t* end);

template<size_t N>
void parse_pdx_file(std::vector<token_group>& results, const char(&t)[N]);
void parse_pdx_file(std::vector<token_group>& results, const char* file_start, const char* file_end);
void parse_lua_file(std::vector<token_group>& results, const char* file_start, const char* file_end);

template<size_t count_values, typename T>
const char* CALL parse_fixed_amount_csv_values(char const* start, char const* end, char seperator, const T& function);
template<typename T>
const char* CALL parse_first_and_nth_csv_values(uint32_t nth, char const* start, char const* end, char seperator, const T& function);
association_type parse_association_type_b(const char* start, const char* end);

char16_t win1250toUTF16(char in);
std::u16string win1250toUTF16(const char* start, const char* end);
std::u16string UTF8toUTF16(const char* start, const char* end);
std::string UTF16toUTF8(const char16_t* start, const char16_t* end);
std::u16string UTF8toUTF16(std::string const& str);
std::string UTF16toUTF8(std::u16string const& str);

template<size_t N>
bool is_fixed_token(const token_and_type& g, const char(&t)[N]);
template<size_t N>
bool is_fixed_token_ci(const token_and_type& g, const char(&t)[N]);
template<typename T>
bool any_token(const token_group* start, const token_group* end, const T& function);
template<typename T>
bool any_token_recursive(const token_group* start, const token_group* end, const T& function);
template<typename T>
bool all_tokens(const token_group* start, const token_group* end, const T& function);
template<typename T>
bool all_tokens_recursive(const token_group* start, const token_group* end, const T& function);
template<typename T>
token_group* find_token(token_group* start, const token_group* end, const T& function);
template<typename T>
token_group* find_token_recursive(token_group* start, const token_group* end, const T& function);
template<typename T>
void forall_tokens(token_group* start, const token_group* end, const T& function);
template<typename T>
void forall_tokens_recursive(token_group* start, const token_group* end, const T& function);
template<typename T>
void forall_tokens(const token_group* start, const token_group* end, const T& function);
template<typename T>
void forall_tokens_recursive(const token_group* start, const token_group* end, const T& function);
