#pragma once

#include <stdint.h>
#include <vector>
#include <atomic>
#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-template"
#pragma clang diagnostic ignored "-Wshadow"
#include "boost\\container\\flat_map.hpp"
#include "boost\\container\\small_vector.hpp"
#include "boost\\container\\flat_set.hpp"
#include "Eigen\\Dense"
#pragma clang diagnostic pop

#ifndef _DEBUG
#define CALL __vectorcall
#else
#define CALL
#endif

#undef max
#undef min

template<typename T>
constexpr T null_value_of = std::numeric_limits<T>::max();

template<typename T>
constexpr T to_index(T in) { return in; }

template<typename T>
constexpr bool is_valid_index(T in) { return in != std::numeric_limits<T>::max(); }

template<typename T>
struct value_base_of_s { using type = T; };

template<typename T>
struct zero_is_null_of_s { using type = std::false_type; };

template<typename T>
struct individuator_of_s { using type = std::false_type; };

template<typename value_base, typename zero_is_null, typename individuator>
struct tag_type {
	static constexpr value_base null_value = std::is_same_v<std::true_type, zero_is_null> ? 0 : std::numeric_limits<value_base>::max();
	using value_base_t = value_base;
	using zero_is_null_t = zero_is_null;
	using individuator_t = individuator;

	value_base value;

	explicit constexpr tag_type(value_base v) : value(v + (std::is_same_v<std::true_type, zero_is_null> ? 1 : 0)) {}
	constexpr tag_type(const tag_type& v) : value(v.value) {}
	explicit constexpr tag_type(value_base v, std::true_type) : value(v) {}

	constexpr tag_type() : value(null_value) {}

	constexpr value_base index() const {
		if constexpr(std::is_same_v<std::true_type, zero_is_null>)
			return value - 1;
		else
			return value;
	}
	constexpr bool is_valid() const {
		if constexpr(std::is_same_v<std::true_type, zero_is_null>)
			return value != 0;
		else
			return value != std::numeric_limits<value_base>::max();
	}
	void operator=(value_base v) { value = v + (std::is_same_v<std::true_type, zero_is_null> ? 1 : 0); }
	void operator=(tag_type v) { value = v.value; }

	constexpr bool operator==(tag_type v) const { return value == v.value; }
	constexpr bool operator==(value_base v) const { return *this == tag_type(v); }
	constexpr bool operator!=(tag_type v) const { return value != v.value; }
	constexpr bool operator!=(value_base v) const { return *this != tag_type(v); }
	constexpr bool operator<(tag_type v) const { return value < v.value; }
	constexpr bool operator<(value_base v) const { return *this < tag_type(v); }
};

template<typename value_base, typename zero_is_null, typename individuator>
constexpr value_base null_value_of<tag_type<value_base, zero_is_null, individuator>> = tag_type<value_base, zero_is_null, individuator>::null_value;

template<typename value_base, typename zero_is_null, typename individuator>
constexpr value_base to_index(tag_type<value_base, zero_is_null, individuator> in) { return in.index(); }

template<typename value_base, typename zero_is_null, typename individuator>
constexpr bool is_valid_index(tag_type<value_base, zero_is_null, individuator> in) { return in.is_valid(); }

template<typename value_base, typename zero_is_null, typename individuator>
struct value_base_of_s<tag_type<value_base, zero_is_null, individuator>> { using type = value_base; };

template<typename value_base, typename zero_is_null, typename individuator>
struct zero_is_null_of_s<tag_type<value_base, zero_is_null, individuator>> { using type = zero_is_null; };

template<typename value_base, typename zero_is_null, typename individuator>
struct individuator_of_s<tag_type<value_base, zero_is_null, individuator>> { using type = individuator; };

template<typename T>
using value_base_of = typename value_base_of_s<T>::type;

template<typename T>
using zero_is_null_of = typename zero_is_null_of_s<T>::type;

template<typename T>
using individuator_of = typename individuator_of_s<T>::type;

template<typename tag_base>
struct atomic_tag {
	using value_base_t = value_base_of<tag_base>;
	using zero_is_null_t = zero_is_null_of<tag_base>;
	using individuator_t = individuator_of<tag_base>;

	static constexpr value_base_t null_value = null_value_of<tag_base>;

	std::atomic<value_base_t> value;

	explicit atomic_tag(value_base_t v) : value(v + (std::is_same_v<std::true_type, zero_is_null_t> ? 1 : 0)) {}
	explicit atomic_tag(tag_base v) : value(to_index(v) + (std::is_same_v<std::true_type, zero_is_null_t> ? 1 : 0)) {}
	atomic_tag(const atomic_tag &v) : value(v.value.load(std::memory_order_acquire)) {}

	atomic_tag() : value(null_value) {}

	value_base_t index() const {
		if constexpr(std::is_same_v<std::true_type, zero_is_null_t>)
			return value.load(std::memory_order_acquire) - 1;
		else
			return value.load(std::memory_order_acquire);
	}
	bool is_valid() const {
		if constexpr(std::is_same_v<std::true_type, zero_is_null_t>)
			return value.load(std::memory_order_acquire) != 0;
		else
			return value.load(std::memory_order_acquire) != std::numeric_limits<value_base_t>::max();
	}

	void operator=(const atomic_tag &v) { value.store(v.value.load(std::memory_order_acquire), std::memory_order_release); }
	void operator=(tag_base v) { value.store(to_index(v) + (std::is_same_v<std::true_type, zero_is_null_t> ? 1 : 0), std::memory_order_release); }

	bool operator==(const atomic_tag &v) const { return value.load(std::memory_order_acquire) == v.value.load(std::memory_order_acquire); }
	bool operator==(tag_base v) const { return index() == to_index(v); }
	bool operator!=(const atomic_tag &v) const { return value.load(std::memory_order_acquire) != v.value.load(std::memory_order_acquire); }
	bool operator!=(tag_base v) const { return index() != to_index(v); }
	
	operator tag_base() const { return tag_base(value.load(std::memory_order_acquire), std::true_type()); }
};

template<typename tag_base>
constexpr typename atomic_tag<tag_base>::value_base_t null_value_of<atomic_tag<tag_base>> = atomic_tag<tag_base>::null_value;

template<typename tag_base>
constexpr typename atomic_tag<tag_base>::value_base_t to_index(const atomic_tag<tag_base>& in) { return in.index(); }

template<typename tag_base>
constexpr bool is_valid_index(const atomic_tag<tag_base>& in) { return in.is_valid(); }

template<typename tag_base>
struct value_base_of_s<atomic_tag<tag_base>> { using type = typename atomic_tag<tag_base>::value_base_t; };

template<typename tag_base>
struct zero_is_null_of_s<atomic_tag<tag_base>> { using type = typename atomic_tag<tag_base>::zero_is_null_t; };

template<typename tag_base>
struct individuator_of_s<atomic_tag<tag_base>> { using type = typename atomic_tag<tag_base>::individuator_t; };

template<typename T, typename tag_type>
struct tagged_object {
	T& object;
	const tag_type id;

	operator T&() const {
		return object;
	}
	operator tag_type() const {
		return id;
	}
};

template<typename value_type, typename tag_type, typename allocator = std::allocator<value_type>>
class tagged_vector {
private:
	std::vector<value_type, allocator> storage;
public:
	const value_type& operator[](tag_type t) const {
		return storage[to_index(t)];
	}
	value_type& operator[](tag_type t) {
		return storage[to_index(t)];
	}
	template<typename ...T>
	tag_type emplace_back(T&& ... ts) {
		storage.emplace_back(std::forward<T>(ts)...);
		return tag_type(static_cast<value_base_of<tag_type>>(storage.size() - 1));
	}
	value_type& safe_get(tag_type t) {
		if (to_index(t) >= storage.size())
			storage.resize(to_index(t) + 1);
		return storage[to_index(t)];
	}
	auto begin() const { return storage.begin(); }
	auto end() const { return storage.end(); }
	auto begin() { return storage.begin(); }
	auto end() { return storage.end(); }
	size_t size() const { return storage.size(); }
	void resize(size_t size) { storage.resize(size); }
	void reserve(size_t size) { storage.reserve(size); }
};

template<typename value_type, typename variable_tag_type, typename fixed_tag_type, typename allocator = std::allocator<value_type>>
class tagged_fixed_2dvector {
private:
	std::vector<value_type, allocator> storage;
	uint32_t _inner_size = 1;
public:
	value_type & get(variable_tag_type outer, fixed_tag_type inner) {
		return storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size];
	}
	const value_type & get(variable_tag_type outer, fixed_tag_type inner) const {
		return storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size];
	}
	value_type& safe_get(variable_tag_type outer, fixed_tag_type inner) {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size];
	}
	void reset(uint32_t new_inner_size) {
		storage.clear();
		_inner_size = new_inner_size;
	}
	size_t size() const { return storage.size(); }
	size_t outer_size() const { return storage.size() / _inner_size; }
	uint32_t inner_size() const { return _inner_size; }
	void resize(size_t outer_size) { storage.resize(outer_size * _inner_size); }
	void reserve(size_t outer_size) { storage.reserve(outer_size * _inner_size); }
};

template<typename value_type, typename variable_tag_type, typename fixed_tag_type, typename allocator>
class tagged_fixed_blocked_2dvector {
public:
	constexpr static size_t block_size = 32;
private:
	struct alignas(block_size) block_s {
		unsigned char data[block_size] = { 0 };
	};
	std::vector<block_s, typename std::allocator_traits<allocator>::template rebind_alloc<block_s>> storage;
	uint32_t _inner_size = 1;
public:
	value_type & get(variable_tag_type outer, fixed_tag_type inner) {
		return *((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner));
	}
	const value_type & get(variable_tag_type outer, fixed_tag_type inner) const {
		return *((const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner));
	}
	value_type* get_row(variable_tag_type outer) {
		return (value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size);
	}
	const value_type* get_row(variable_tag_type outer) const {
		return (const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size);
	}
	value_type* safe_get_row(variable_tag_type outer) {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return (value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size);
	}
	const value_type* safe_get_row(variable_tag_type outer) const {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return (const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size);
	}
	value_type& safe_get(variable_tag_type outer, fixed_tag_type inner) {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return *((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner));
	}
	void reset(uint32_t new_inner_size) {
		storage.clear();
		_inner_size = (new_inner_size * sizeof(value_type) + (block_size - 1)) / block_size;
	}
	size_t size() const { return storage.size(); }
	size_t outer_size() const { return storage.size() / _inner_size; }
	uint32_t inner_size() const { return _inner_size * block_size / sizeof(value_type); }
	void resize(size_t outer_size) { storage.resize(outer_size * _inner_size); }
	void reserve(size_t outer_size) { storage.reserve(outer_size * _inner_size); }
};

enum key_modifiers {
	modifiers_none = 0x0,
	modifiers_alt = 0x4,
	modifiers_ctrl = 0x2,
	modifiers_shift = 0x1,
	modifiers_ctrl_shift = 0x3,
	modifiers_ctrl_alt = 0x6,
	modifiers_alt_shift = 0x5,
	modifiers_all = 0x7
};

enum class virtual_key : uint8_t {
	NONE = 0x00,
	LBUTTON = 0x01,
	RBUTTON = 0x02,
	CANCEL = 0x03,
	MBUTTON = 0x04,
	XBUTTON_1 = 0x05,
	XBUTTON_2 = 0x06,
	BACK = 0x08,
	TAB = 0x09,
	CLEAR = 0x0C,
	RETURN = 0x0D,
	SHIFT = 0x10,
	CONTROL = 0x11,
	MENU = 0x12,
	PAUSE = 0x13,
	CAPITAL = 0x14,
	KANA = 0x15,
	JUNJA = 0x17,
	FINAL = 0x18,
	KANJI = 0x19,
	ESCAPE = 0x1B,
	CONVERT = 0x1C,
	NONCONVERT = 0x1D,
	ACCEPT = 0x1E,
	MODECHANGE = 0x1F,
	SPACE = 0x20,
	PRIOR = 0x21,
	NEXT = 0x22,
	END = 0x23,
	HOME = 0x24,
	LEFT = 0x25,
	UP = 0x26,
	RIGHT = 0x27,
	DOWN = 0x28,
	SELECT = 0x29,
	PRINT = 0x2A,
	EXECUTE = 0x2B,
	SNAPSHOT = 0x2C,
	INSERT = 0x2D,
	DELETE_KEY = 0x2E,
	HELP = 0x2F,
	NUM_0 = 0x30,
	NUM_1 = 0x31,
	NUM_2 = 0x32,
	NUM_3 = 0x33,
	NUM_4 = 0x34,
	NUM_5 = 0x35,
	NUM_6 = 0x36,
	NUM_7 = 0x37,
	NUM_8 = 0x38,
	NUM_9 = 0x39,
	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,
	LWIN = 0x5B,
	RWIN = 0x5C,
	APPS = 0x5D,
	SLEEP = 0x5F,
	NUMPAD0 = 0x60,
	NUMPAD1 = 0x61,
	NUMPAD2 = 0x62,
	NUMPAD3 = 0x63,
	NUMPAD4 = 0x64,
	NUMPAD5 = 0x65,
	NUMPAD6 = 0x66,
	NUMPAD7 = 0x67,
	NUMPAD8 = 0x68,
	NUMPAD9 = 0x69,
	MULTIPLY = 0x6A,
	ADD = 0x6B,
	SEPARATOR = 0x6C,
	SUBTRACT = 0x6D,
	DECIMAL = 0x6E,
	DIVIDE = 0x6F,
	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,
	F13 = 0x7C,
	F14 = 0x7D,
	F15 = 0x7E,
	F16 = 0x7F,
	F17 = 0x80,
	F18 = 0x81,
	F19 = 0x82,
	F20 = 0x83,
	F21 = 0x84,
	F22 = 0x85,
	F23 = 0x86,
	F24 = 0x87,
	NAVIGATION_VIEW = 0x88,
	NAVIGATION_MENU = 0x89,
	NAVIGATION_UP = 0x8A,
	NAVIGATION_DOWN = 0x8B,
	NAVIGATION_LEFT = 0x8C,
	NAVIGATION_RIGHT = 0x8D,
	NAVIGATION_ACCEPT = 0x8E,
	NAVIGATION_CANCEL = 0x8F,
	NUMLOCK = 0x90,
	SCROLL = 0x91,
	OEM_NEC_EQUAL = 0x92,
	LSHIFT = 0xA0,
	RSHIFT = 0xA1,
	LCONTROL = 0xA2,
	RCONTROL = 0xA3,
	LMENU = 0xA4,
	RMENU = 0xA5,
	SEMICOLON = 0xBA,
	PLUS = 0xBB,
	COMMA = 0xBC,
	MINUS = 0xBD,
	PERIOD = 0xBE,
	FORWARD_SLASH = 0xBF,
	TILDA = 0xC0,
	OPEN_BRACKET = 0xDB,
	BACK_SLASH = 0xDC,
	CLOSED_BRACKET = 0xDD,
	QUOTE = 0xDE
};


inline char ascii_to_lower(char in) {
	constexpr static char converted[256] = {
		//      0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
		/*0*/'\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07','\x08','\x09','\x0A','\x0B','\x0C','\x0D','\x0E','\x0F',
		/*1*/'\x10','\x11','\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19','\x1A','\x1B','\x1C','\x1D','\x1E','\x1F',
		/*2*/  ' ' ,  '!' ,  '\"',  '#' ,  '$' ,  '%' ,  '&' ,  '\'',  '(' ,  ')' ,  '*' ,  '+' ,  ',' ,  '-' ,  '.' ,  '/' ,
		/*3*/  '0' ,  '1' ,  '2' ,  '3' ,  '4' ,  '5' ,  '6' ,  '7' ,  '8' ,  '9' ,  ':' ,  ';' ,  '<' ,  '=' ,  '>' ,  '?' ,
		/*4*/  '@' ,  'a' ,  'b' ,  'c' ,  'd' ,  'e' ,  'f' ,  'g' ,  'h' ,  'i' ,  'j' ,  'k' ,  'l' ,  'm' ,  'n' ,  'o' ,
		/*5*/  'p' ,  'q' ,  'r' ,  's' ,  't' ,  'u' ,  'v' ,  'w' ,  'x' ,  'y' ,  'z' ,  '[' ,  '\\',  ']' ,  '^' ,  '_' ,
		/*6*/  '`' ,  'a' ,  'b' ,  'c' ,  'd' ,  'e' ,  'f' ,  'g' ,  'h' ,  'i' ,  'j' ,  'k' ,  'l',   'm' ,  'n' ,  'o' ,
		/*7*/  'p' ,  'q' ,  'r' ,  's' ,  't' ,  'u' ,  'v' ,  'w' ,  'x' ,  'y' ,  'z' ,  '{' ,  '|',   '}' ,  '~' ,'\x7F',
		/*8*/'\x80','\x81','\x82','\x83','\x84','\x85','\x86','\x87','\x88','\x89','\x8A','\x8B','\x8C','\x8D','\x8E','\x8F',
		/*9*/'\x90','\x91','\x92','\x93','\x94','\x95','\x96','\x97','\x98','\x99','\x9A','\x9B','\x9C','\x9D','\x9E','\x9F',
		/*A*/'\xA0','\xA1','\xA2','\xA3','\xA4','\xA5','\xA6','\xA7','\xA8','\xA9','\xAA','\xAB','\xAC','\xAD','\xAE','\xAF',
		/*B*/'\xB0','\xB1','\xB2','\xB3','\xB4','\xB5','\xB6','\xB7','\xB8','\xB9','\xBA','\xBB','\xBC','\xBD','\xBE','\xBF',
		/*C*/'\xC0','\xC1','\xC2','\xC3','\xC4','\xC5','\xC6','\xC7','\xC8','\xC9','\xCA','\xCB','\xCC','\xCD','\xCE','\xCF',
		/*D*/'\xD0','\xD1','\xD2','\xD3','\xD4','\xD5','\xD6','\xD7','\xD8','\xD9','\xDA','\xDB','\xDC','\xDD','\xDE','\xDF',
		/*E*/'\xE0','\xE1','\xE2','\xE3','\xE4','\xE5','\xE6','\xE7','\xE8','\xE9','\xEA','\xEB','\xEC','\xED','\xEE','\xEF',
		/*F*/'\xF0','\xF1','\xF2','\xF3','\xF4','\xF5','\xF6','\xF7','\xF8','\xF9','\xFA','\xFB','\xFC','\xFD','\xFE','\xFF'
	};

	return converted[(uint8_t)in];
}

inline char16_t* _u16itoa(uint32_t i, char16_t* buffer) {
	if (i >= 10) {
		const auto num = i / 10;
		const auto rem = i % 10;

		const auto tail = _u16itoa(num, buffer);
		tail[0] = static_cast<char16_t>(u'0' + rem);
		return tail + 1;
	} else {
		buffer[0] = static_cast<char16_t>(u'0' + i);
		return buffer + 1;
	}
}

inline void u16itoa(int32_t i, char16_t* buffer) {
	if (i < 0) {
		buffer[0] = u'-';
		const auto res = _u16itoa(uint32_t(-i), buffer + 1);
		res[0] = 0;
	} else {
		const auto res = _u16itoa(uint32_t(i), buffer);
		res[0] = 0;
	}
}

inline std::pair<char*, bool> bom_test(char* buffer, size_t buffer_size) {
	const bool is_utf8 = buffer_size >= 3 && (((uint8_t)buffer[0] == 0xEF) & ((uint8_t)buffer[1] == 0xBB) & ((uint8_t)buffer[2] == 0xBF));
	if (is_utf8)
		return std::make_pair(buffer + 3, true);
	else
		return std::make_pair(buffer, false);
}

struct vector_backed_string_data {
	uint32_t offset;
	uint16_t length;
	uint16_t high_mask;
};

template<typename char_type>
struct vector_backed_string {
	union {
		vector_backed_string_data vbs;
		const char_type* ptr;
	} data;
	vector_backed_string() { data.vbs = vector_backed_string_data{0, 0, (uint16_t)(-1)}; }
	vector_backed_string(const char_type* c) { data.ptr = c; }
	vector_backed_string(const char_type* start, const char_type* end, std::vector<char_type>& vec) {
		data.vbs.offset = static_cast<uint32_t>(vec.size());
		data.vbs.length = (uint16_t)(end - start);
		data.vbs.high_mask = (uint16_t)(-1);
		vec.insert(vec.end(), start, end);
	}
	vector_backed_string(uint32_t offset, uint16_t length) {
		data.vbs.offset = offset;
		data.vbs.length = length;
		data.vbs.high_mask = (uint16_t)(-1);
	}
	vector_backed_string(const std::basic_string<char_type>& str, std::vector<char_type>& vec) {
		data.vbs.offset = (uint32_t)vec.size();
		data.vbs.length = (uint16_t)(str.length());
		data.vbs.high_mask = (uint16_t)(-1);
		vec.insert(vec.end(), str.begin(), str.end());
	}
	static vector_backed_string<char_type> create_unique(const std::basic_string<char_type>& str, std::vector<char_type>& vec) {
		const auto search_result = std::search(vec.begin(), vec.end(), str.begin(), str.end());
		if (search_result != vec.end()) {
			return vector_backed_string<char_type>(static_cast<uint32_t>(search_result - vec.begin()), static_cast<uint16_t>(str.length()));
		} else {
			return vector_backed_string<char_type>(str, vec);
		}
	}
	static vector_backed_string<char_type> create_unique(const char_type* start, const char_type* end, std::vector<char_type>& vec) {
		const auto search_result = std::search(vec.begin(), vec.end(), start, end);
		if (search_result != vec.end()) {
			return vector_backed_string<char_type>(static_cast<uint32_t>(search_result - vec.begin()), static_cast<uint16_t>(end - start));
		} else {
			return vector_backed_string<char_type>(start, end, vec);
		}
	}
	int32_t length() const {
		return data.vbs.high_mask == (uint16_t)-1 ? data.vbs.length : (data.ptr ? static_cast<int32_t>(std::char_traits<char_type>::length(data.ptr)) : 0);
	}
	const char_type* get_str(const std::vector<char_type>& vec) const {
		return data.vbs.high_mask == (uint16_t)-1 ? vec.data() + data.vbs.offset : data.ptr;
	}
	std::basic_string<char_type> get_string(const std::vector<char_type>& vec) const {
		return data.vbs.high_mask == (uint16_t)-1 ? std::basic_string<char_type>(vec.data() + data.vbs.offset, size_t(data.vbs.length)) : std::basic_string<char_type>(data.ptr);
	}
	void operator=(vector_backed_string other) {
		static_assert(sizeof(data.ptr) >= sizeof(data.vbs));
		data.ptr = other.data.ptr;
	}
	bool operator==(vector_backed_string other) const {
		return other.data.ptr == data.ptr;
	}
};

template<typename char_type>
struct vector_backed_string_less {
	const std::vector<char_type>& backing;
	vector_backed_string_less(const std::vector<char_type>& b) : backing(b) {}
	bool operator()(vector_backed_string<char_type> a, vector_backed_string<char_type> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len < b_len)
			return true;
		else if (a_len > b_len)
			return false;
		else {
			const char_type* __restrict a_str = a.get_str(backing);
			const char_type* __restrict b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (a_str[i] < b_str[i])
					return true;
				if (a_str[i] > b_str[i])
					return false;
			}
		}
		return false;
	}
};

struct vector_backed_string_less_ci {
	const std::vector<char>& backing;
	vector_backed_string_less_ci(const std::vector<char>& b) : backing(b) {}
	bool operator()(vector_backed_string<char> a, vector_backed_string<char> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len < b_len)
			return true;
		else if (a_len > b_len)
			return false;
		else {
			const char* __restrict a_str = a.get_str(backing);
			const char* __restrict b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (ascii_to_lower(a_str[i]) < ascii_to_lower(b_str[i]))
					return true;
				if (ascii_to_lower(a_str[i]) > ascii_to_lower(b_str[i]))
					return false;
			}
		}
		return false;
	}
};

template<typename char_type>
struct vector_backed_string_equality {
	const std::vector<char_type>& backing;
	vector_backed_string_equality(const std::vector<char_type>& b) : backing(b) {}
	bool operator()(vector_backed_string<char_type> a, vector_backed_string<char_type> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len != b_len)
			return false;
		else {
			const auto a_str = a.get_str(backing);
			const auto b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (a_str[i] != b_str[i])
					return false;
			}
		}
		return true;
	}
};

struct vector_backed_string_equality_ci {
	const std::vector<char>& backing;
	vector_backed_string_equality_ci(const std::vector<char>& b) : backing(b) {}
	bool operator()(vector_backed_string<char> a, vector_backed_string<char> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len != b_len)
			return false;
		else {
			const auto a_str = a.get_str(backing);
			const auto b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (ascii_to_lower(a_str[i]) != ascii_to_lower(b_str[i]))
					return false;
			}
		}
		return true;
	}
};


template<typename T, typename I = uint32_t, typename allocator = std::allocator<T>>
class v_vector {
public:
	std::vector<T, typename std::allocator_traits<allocator>::template rebind_alloc<T>> elements;
	std::vector<I, typename std::allocator_traits<allocator>::template rebind_alloc<I>> index;

	v_vector() {
		index.push_back(I());
	}

	void clear() {
		index.clear();
		elements.clear();
		index.push_back(I());
	}

	void insert(typename std::vector<T>::iterator it, T elem) {
		size_t pos = std::distance(elements.begin(), it);
		elements.insert(it, elem);
		size_t i = 0;
		const auto isize = index.size();
		for (; index[i] < pos && i != isize; ++i)
			;
		if (i != isize && index[i] == pos)
			++i;
		for (; i != isize; ++i)
			++index[i];
	}

	void new_row() {
		index.push_back(static_cast<I>(elements.size()));
	}

	void expand_rows(uint32_t sz) {
		if (index.size() < sz + 1)
			index.resize(sz + 1, static_cast<I>(elements.size()));
	}

	uint32_t row_size() const {
		return static_cast<uint32_t>(index.size());
	}

	std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> get_row(uint32_t i) {
		std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> p;
		p.first = elements.begin() + index[i];
		if (i + 1 < index.size()) {
			p.second = elements.begin() + index[i + 1];
		} else {
			p.second = elements.end();
		}
		return p;
	}

	std::pair<typename std::vector<T>::const_iterator, typename std::vector<T>::const_iterator> get_row(uint32_t i) const {
		std::pair<typename std::vector<T>::const_iterator, typename std::vector<T>::const_iterator> p;
		p.first = elements.cbegin() + index[i];
		if (i + 1 < index.size()) {
			p.second = elements.cbegin() + index[i + 1];
		} else {
			p.second = elements.cend();
		}
		return p;
	}

	T& get(uint32_t x, uint32_t y) {
		auto it = elements.begin() + index[x] + y;
		return *it;
	}

	const T& get(uint32_t x, uint32_t y) const {
		const auto it = elements.cbegin() + index[x] + y;
		return *it;
	}

	void push_back(const T& elem) {
		elements.push_back(elem);
	}

	void add_to_row(uint32_t i, const T& elem) {
		if (i >= index.size()) {
			index.resize(i + 1, static_cast<I>(elements.size()));
		}
		elements.insert(elements.begin() + index[i], elem);
		size_t sz = index.size();
		for (++i; i < sz; ++i) {
			++index[i];
		}
	}

	template<typename IT>
	void add_range_to_row(uint32_t i, const IT& start, const IT& end) {
		if (i >= index.size()) {
			index.resize(i + 1, static_cast<I>(elements.size()));
		}
		elements.insert(elements.begin() + index[i], start, end);
		const I num_added = static_cast<I>(end - start);
		size_t sz = index.size();
		for (++i; i < sz; ++i) {
			index[i] += num_added;
		}
	}

	void append_to_row(uint32_t i, const T& elem) {
		size_t sz = index.size();
		if (i + 1 < sz)
			elements.insert(elements.begin() + index[i + 1], elem);
		else
			elements.push_back(elem);
		for (++i; i < sz; ++i) {
			++index[i];
		}
	}
};

namespace graphics {
	struct color_rgba {
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;
	};

	inline uint32_t rgba_to_int(color_rgba c) {
		union local_u {
			color_rgba color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.color = c;
		return u.value;
	}

	inline color_rgba int_to_rgba(uint32_t v) {
		union local_u {
			color_rgba color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.value = v;
		return u.color;
	}

	struct color_rgb {
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
	};

	inline uint32_t rgb_to_int(color_rgb c) {
		union local_u {
			color_rgb color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.color = c;
		return u.value;
	}

	inline color_rgb int_to_rgb(uint32_t v) {
		union local_u {
			color_rgb color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.value = v;
		return u.color;
	}

}
