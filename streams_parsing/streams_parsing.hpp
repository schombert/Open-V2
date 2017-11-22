#pragma once
#include "stdafx.h"
#include "streams_parsing.h"
#include "streams\\streams.h"
#include "text_classifier\\text_classifiers.h"
#include "object_parsing\\\object_parsing.h"

class token_to_object_stream_operation_base;
template<typename result_type>
class token_to_object_stream_operation;

template<typename result_type>
class object_parsing_definition {
private:
	using boolean_classifying_function = std::function<bool(const char*, const char*)>;
	using pair_handler = std::function<void(result_type&, const token_and_type&, association_type, const token_and_type&)>;
	using passthrough_function = std::function<std::unique_ptr<type_erased_stream<token_and_type>>(token_to_object_stream_operation_base&)>;

	binary_search_classifier tag_classifier;
	binary_search_classifier tag_pair_classifier;
	std::vector<passthrough_function> tag_handlers;
	std::vector<pair_handler> tag_pair_handlers;
	std::vector<std::pair<boolean_classifying_function, passthrough_function>> special_handlers;
	std::vector<std::pair<boolean_classifying_function, pair_handler>> special_pair_handlers;

	passthrough_function default_passthrough_function;

	template<typename value, typename stream_conversion, typename apply_result>
	passthrough_function make_passthrough(const stream_conversion& scv, const apply_result& apply_result_f);

	template<typename>
	friend class object_parsing_definition;
public:
	object_parsing_definition();

	template<typename other_type>
	object_parsing_definition(const object_parsing_definition<other_type> &o) :
		tag_classifier(o.tag_classifier),
		tag_pair_classifier(o.tag_pair_classifier),
		tag_handlers(o.tag_handlers),
		special_handlers(o.special_handlers) {
		for (const auto& f : o.tag_pair_handlers)
			tag_pair_handlers.emplace_back([f](result_type& r, const token_and_type& a, association_type b, const token_and_type& c) {
			f(r, a, b, c);
		});
		for (const auto& f : o.special_pair_handlers)
			special_pair_handlers.emplace_back(f.first, [g = f.second](result_type& r, const token_and_type& a, association_type b, const token_and_type& c) {
			g(r, a, b, c);
		});
	}

	template<typename value, typename parsing_operation, typename handling_function>
	void add_case_handler(const char* tag, const parsing_operation &parse, const handling_function &handle);

	template<typename handling_function>
	void add_case_handler(const char* tag, const handling_function &handle);

	template<typename value, typename parsing_operation, typename handling_function>
	void add_special_case_handler(const boolean_classifying_function &test, const parsing_operation &parse, const handling_function &handle);

	template<typename handling_function>
	void add_special_case_handler(const boolean_classifying_function &test, const handling_function &handle);

	friend class token_to_object_stream_operation<result_type>;
	friend class token_to_object_stream_operation_base;
};


class token_to_object_stream_operation_base {
protected:
	token_and_type left = token_and_type{ nullptr, nullptr, token_type::unknown };
	association_type assoc = association_type::none;
	std::unique_ptr<type_erased_stream<token_and_type>> passthrough_function;
	bool passthrough = false;

	token_to_object_stream_operation_base() {}
	token_to_object_stream_operation_base(const token_to_object_stream_operation_base& o) : left(o.left), assoc(o.assoc) {}
	token_to_object_stream_operation_base(token_to_object_stream_operation_base&& o) : left(o.left), assoc(o.assoc), passthrough_function(std::move(o.passthrough_function)) {}

	template<typename>
	friend class object_parsing_definition;
};

template<typename result_type>
class token_to_object_stream_operation : public token_to_object_stream_operation_base {
private:
	const object_parsing_definition<result_type>* const definition;
	result_type under_construction;
public:
	token_to_object_stream_operation(object_parsing_definition<result_type>& def) : definition(&def) {};
	token_to_object_stream_operation(const token_to_object_stream_operation& o) : token_to_object_stream_operation_base(o), definition(o.definition), under_construction(o.under_construction) {};
	token_to_object_stream_operation(token_to_object_stream_operation&& o) : token_to_object_stream_operation_base(std::move(o)), definition(o.definition), under_construction(std::move(o.under_construction)) {};
	void reset();

	template<typename NEXT>
	void operator()(const token_and_type& in, NEXT& next);

	template<typename NEXT>
	void yield(NEXT& next);

	friend class object_parsing_definition<result_type>;
};

class end_brace_finder {
private:
	int count = 1;
public:
	void reset() {
		count = 1;
	}
	template<typename NEXT>
	void operator()(const token_and_type& tok, NEXT& next) {
		if (tok.type == token_type::brace) {
			if (*(tok.start) == '{')
				++count;
			else {
				--count;
				if (count == 0) {
					next(count);
					reset();
				}
			}
		}
	}
};

class vector_of_strings_operation {
private:
	std::vector<std::string> contents;
	int braces_count = 1;
public:
	void reset() {
		braces_count = 1;
		contents.clear();
	}
	template<typename NEXT>
	void operator()(const token_and_type& tok, NEXT& next) {
		if (tok.type == token_type::brace) {
			if (*(tok.start) == '{')
				++braces_count;
			else {
				--braces_count;
				if (braces_count == 0) {
					next(contents);
					reset();
				}
			}
		} else if (braces_count == 1) {
			contents.emplace_back(tok.start, tok.end);
		}
	}
	template<typename NEXT>
	void yield(NEXT& next) {
		if (!contents.empty()) {
			next(contents);
		}
		reset();
	}
};

class vector_of_ints_operation {
private:
	std::vector<int> contents;
	int braces_count = 1;
public:
	void reset() {
		braces_count = 1;
		contents.clear();
	}
	template<typename NEXT>
	void operator()(const token_and_type& tok, NEXT& next) {
		if (tok.type == token_type::brace) {
			if (*(tok.start) == '{')
				++braces_count;
			else {
				--braces_count;
				if (braces_count == 0) {
					next(contents);
					reset();
				}
			}
		} else if (braces_count == 1) {
			contents.emplace_back(parse_int(tok.start, tok.end));
		}
	}
	template<typename NEXT>
	void yield(NEXT& next) {
		if (!contents.empty()) {
			next(contents);
		}
		reset();
	}
};

class vector_of_doubles_operation {
private:
	std::vector<double> contents;
	int braces_count = 1;
public:
	void reset() {
		braces_count = 1;
		contents.clear();
	}
	template<typename NEXT>
	void operator()(const token_and_type& tok, NEXT& next) {
		if (tok.type == token_type::brace) {
			if (*(tok.start) == '{')
				++braces_count;
			else {
				--braces_count;
				if (braces_count == 0) {
					next(contents);
					reset();
				}
			}
		} else if (braces_count == 1) {
			contents.emplace_back(parse_double(tok.start, tok.end));
		}
	}
	template<typename NEXT>
	void yield(NEXT& next) {
		if (!contents.empty()) {
			next(contents);
		}
		reset();
	}
};


template<typename result_type, typename context>
class generic_object_parser {
private:
	token_and_type left = token_and_type{ nullptr, nullptr, token_type::unknown };
	association_type assoc = association_type::none;

	std::unique_ptr<type_erased_stream<token_and_type>> passthrough_function;
	bool passthrough = false;

	result_type under_construction;

	void reset_reading_state();

	template<typename to>
	std::unique_ptr<type_erased_stream<token_and_type>> make_passthrough_function(const to t);

	using this_in_context = type_map_get_t<context, result_type>;
	using inheritance_list = type_list_get_t<4ui64, this_in_context>;

	template<size_t nth>
	struct get_nth_list {
		template<typename T>
		struct get_from_context {
			using type = type_list_get_t< nth, type_map_get_t<context, T>>;
		};
	};


	using tag_to_function =
		sorted_t<
		merge_lists<
		type_list_get_t<0ui64, this_in_context>,
		flatten_list<
		apply_to_all<get_nth_list<0ui64>::template get_from_context, inheritance_list>
		>
		>
		>;
	using tag_to_generator =
		sorted_t<
		merge_lists<
		type_list_get_t<1ui64, this_in_context>,
		flatten_list<
		apply_to_all<get_nth_list<1ui64>::template get_from_context, inheritance_list>
		>
		>
		>;
	using function_to_function =
		sorted_t<
		merge_lists<
		type_list_get_t<2ui64, this_in_context>,
		flatten_list<
		apply_to_all<get_nth_list<2ui64>::template get_from_context, inheritance_list>
		>
		>
		>;
	using function_to_generator =
		sorted_t<
		merge_lists<
		type_list_get_t<3ui64, this_in_context>,
		flatten_list<
		apply_to_all<get_nth_list<3ui64>::template get_from_context, inheritance_list>
		>
		>
		>;
public:
	using output_type = result_type;

	generic_object_parser() {}
	generic_object_parser(const generic_object_parser& other) {};
	generic_object_parser(generic_object_parser&& other) :
		left(std::move(other.left)),
		assoc(std::move(other.assoc)),
		passthrough_function(std::move(other.passthrough_function)),
		passthrough(other.passthrough),
		under_construction(std::move(other.under_construction)) {
	};

	void reset();
	void process_token(const token_and_type& left, association_type asc, const token_and_type& right);
	void process_list();

	template<typename NEXT>
	void operator()(const token_and_type& in, NEXT& next);

	template<typename NEXT>
	void yield(NEXT& next);
};

template<typename result_type, typename context>
template<typename NEXT>
void generic_object_parser<result_type, context>::yield(NEXT& next) {
	if (passthrough)
		passthrough_function->yield();
	else if ((left.type == token_type::identifier) | (left.type == token_type::quoted_string))
		process_token(left, association_type::none, token_and_type{ nullptr, nullptr, token_type::unknown });

	next(under_construction);
	reset();
}

template<typename result_type, typename context>
template<typename NEXT>
void generic_object_parser<result_type, context>::operator()(const token_and_type& in, NEXT& next) {
	if (!passthrough) {
		if ((left.type != token_type::identifier) & (left.type != token_type::quoted_string) & ((in.type == token_type::identifier) | (in.type == token_type::quoted_string))) {
			left = in;
		} else if ((assoc == association_type::none) & (in.type == token_type::special_identifier)) {
			assoc = parse_association_type_b(in.start, in.end);
		} else if ((in.type == token_type::brace) & (*in.start == '{')) {
			process_list();
		} else if ((in.type == token_type::brace) & (*in.start == '}')) {
			if ((left.type == token_type::identifier) | (left.type == token_type::quoted_string))
				process_token(left, association_type::none, token_and_type{ nullptr, nullptr, token_type::unknown });
			next(under_construction);
			reset();
		} else if (assoc != association_type::none) {
			process_token(left, assoc, in);
			reset_reading_state();
		} else if ((in.type == token_type::identifier) | (in.type == token_type::quoted_string)) {
			process_token(left, association_type::none, token_and_type{ nullptr, nullptr, token_type::unknown });
			left = in;
		}
	} else {
		(*passthrough_function)(in);
	}
}

template<typename result_type, typename context>
void generic_object_parser<result_type, context>::process_list() {
	passthrough_function = map_call_functions<tag_to_generator>
		::template bt_scan_ci<std::unique_ptr<type_erased_stream<token_and_type>>>(
			left.start, left.end,
			[_this = this](const auto t) { return _this->make_passthrough_function(t); });
	if (!bool(passthrough_function)) {
		passthrough_function = map_call_functions<function_to_generator>
			::template scan_by_predicate<std::unique_ptr<type_erased_stream<token_and_type>>>(
				[_this = this](const auto t) { return _this->make_passthrough_function(t); },
				left.start, left.end);
		if (!bool(passthrough_function)) {
#ifdef _DEBUG
			OutputDebugStringA("Error: failed to find token list association for token ");
			OutputDebugStringA(std::string(left.start, left.end).c_str());
			OutputDebugStringA("\n");
#endif
			auto cleanup_lambda = [_this = this](int) { _this->reset_reading_state(); };
			passthrough_function = std::make_unique<
				generic_stream_t<token_and_type, decltype(make_stream(end_brace_finder(), cleanup_lambda))>>(std::true_type(), end_brace_finder(), cleanup_lambda);
		}
	}
	passthrough = true;
}

template<typename result_type, typename context>
void generic_object_parser<result_type, context>::process_token(const token_and_type& left, association_type asc, const token_and_type& right) {
	const bool found_in_a = map_call_functions<tag_to_function>::template bt_scan_ci<bool>(left.start, left.end,
		[_this = this, asc, &right](auto t) {
		using type_passed = typename decltype(t)::type;
		typename type_passed::function_object()(
			_this->under_construction,
			asc,
			right
			);
		return true;
	});
	if (!found_in_a) {
		const bool found_in_b = map_call_functions<function_to_function>::template scan_by_predicate<bool>(
			[_this = this, &left, asc, &right](auto t) {
			using type_passed = typename decltype(t)::type;
			typename type_passed::function_object()(
				_this->under_construction,
				left,
				asc,
				right
				);
			return true;
		}, left.start, left.end);
		if (!found_in_b) {
#ifdef _DEBUG
			if (right.start != right.end) {
				OutputDebugStringA("Error: failed to find token-identifier pair association for token ");
				OutputDebugStringA(std::string(left.start, left.end).c_str());
				OutputDebugStringA(" = ");
				OutputDebugStringA(std::string(right.start, right.end).c_str());
				OutputDebugStringA("\n");
			} else {
				OutputDebugStringA("Error: failed to find token-identifier association for free token ");
				OutputDebugStringA(std::string(left.start, left.end).c_str());
				OutputDebugStringA("\n");
			}
#endif
		}
	}
}

template<typename result_type, typename context>
void generic_object_parser<result_type, context>::reset() {
	reset_reading_state();
	under_construction.~result_type();
	new (&under_construction) result_type();
}

template<typename result_type, typename context>
void generic_object_parser<result_type, context>::reset_reading_state() {
	left = token_and_type{ nullptr, nullptr, token_type::unknown };
	assoc = association_type::none;
	passthrough = false;
}

template<typename result_type, typename context>
template<typename to>
std::unique_ptr<type_erased_stream<token_and_type>> generic_object_parser<result_type, context>::make_passthrough_function(const to t) {
	using type_passed = typename decltype(t)::type;

	const auto result_l = [_this = this](typename type_passed::type& r) {
		if constexpr(type_passed::tag::length != 0)
			typename type_passed::function_object()(
				_this->under_construction,
				_this->left,
				_this->assoc,
				r
				);
		_this->reset_reading_state();
	};
	return std::make_unique<generic_stream_t<token_and_type, decltype(make_stream(generic_object_parser<typename type_passed::type, context>(), result_l))>>(std::true_type(), generic_object_parser<typename type_passed::type, context>(), result_l);
}

class tokenize_char_stream_operation {
private:
	const char* current_first = nullptr;
	const char* current_last = nullptr;

	enum class internal_state {
		seeking, in_identifier, in_special_identifier, in_quoted_string, in_comment
	};

	internal_state state = internal_state::seeking;

	token_type state_to_token_type() {
		switch (state) {
		case internal_state::seeking: return token_type::unknown;
		case internal_state::in_identifier: return token_type::identifier;
		case internal_state::in_special_identifier: return token_type::special_identifier;
		case internal_state::in_quoted_string: return token_type::quoted_string;
		default: return token_type::unknown;
		}
	}
public:
	template<typename NEXT>
	void operator()(const char* position, NEXT& next) {

		switch (state) {
		case internal_state::seeking:
			if (*position == '\"') {
				current_first = position + 1;
				current_last = position + 1;
				state = internal_state::in_quoted_string;
			} else if ((*position == '{') | (*position == '}')) {
				auto tok = token_and_type{ position, position + 1, token_type::brace };
				next(tok);
			} else if (special_identifier_char(*position)) {
				current_first = position;
				current_last = position + 1;
				state = internal_state::in_special_identifier;
			} else if (*position == '#') {
				state = internal_state::in_comment;
			} else if (!ignorable_char(*position)) {
				current_first = position;
				current_last = position + 1;
				state = internal_state::in_identifier;
			}
			break;
		case internal_state::in_identifier:
			if (breaking_char(*position)) {
				auto tok = token_and_type{ current_first, position, token_type::identifier };
				next(tok);
				current_first = nullptr;
				state = internal_state::seeking;
				operator()(position, next);
			} else {
				current_last = position + 1;
			}
			break;
		case internal_state::in_special_identifier:
			if (!special_identifier_char(*position)) {
				auto tok = token_and_type{ current_first, position, token_type::special_identifier };
				next(tok);
				current_first = nullptr;
				state = internal_state::seeking;
				operator()(position, next);
			} else {
				current_last = position + 1;
			}
			break;
		case internal_state::in_quoted_string:
			if (double_quote_termination(*position)) {
				auto tok = token_and_type{ current_first, position, token_type::quoted_string };
				next(tok);
				current_first = nullptr;
				state = internal_state::seeking;
			} else {
				current_last = position + 1;
			}
			break;
		case internal_state::in_comment:
			if (line_termination(*position)) {
				state = internal_state::seeking;
			}
			break;
		}
	}
	template<typename NEXT>
	void yield(NEXT& next) {
		if (bool(current_first) & (current_first != current_last)) {
			auto tok = token_and_type{ current_first, current_last, state_to_token_type() };
			next(tok);
		}
	}
};

template<typename result_type>
template<typename value, typename stream_conversion, typename apply_result>
typename object_parsing_definition<result_type>::passthrough_function object_parsing_definition<result_type>::make_passthrough(const stream_conversion & scv, const apply_result & apply_result_f) {
	return [scv, apply_result_f](token_to_object_stream_operation_base &owner) {
		auto stream =
			make_stream(scv, [&owner, &apply_result_f](value& r) {
			apply_result_f(static_cast<token_to_object_stream_operation<result_type>&>(owner).under_construction, owner.left, owner.assoc, r);
			owner.left = token_and_type{ nullptr, nullptr, token_type::unknown };
			owner.assoc = association_type::none;
			owner.passthrough = false;
		});
		return std::make_unique<decltype(generic_stream<token_and_type>(stream))>(stream);
	};
}

template<typename result_type>
template<typename NEXT>
void token_to_object_stream_operation<result_type>::operator()(const token_and_type& in, NEXT& next) {
	if (!passthrough) {
		if ((left.type != token_type::identifier) & (left.type != token_type::quoted_string) & ((in.type == token_type::identifier) | (in.type == token_type::quoted_string))) {
			left = in;
		} else if ((assoc == association_type::none) & (in.type == token_type::special_identifier)) {
			assoc = parse_association_type_b(in.start, in.end);
		} else if ((in.type == token_type::brace) & (*in.start == '{')) {
			if ((left.type != token_type::identifier) & (left.type != token_type::quoted_string)) {
				passthrough_function = definition->default_passthrough_function(*this);
				passthrough = true;
			} else {
				auto classify = definition->tag_classifier.classify(left.start, left.end);
				if (classify != (unsigned char)0) {
					passthrough_function = definition->tag_handlers[classify - 1](*this);
					passthrough = true;
				} else {
					for (auto& x : definition->special_handlers) {
						if (x.first(left.start, left.end)) {
							passthrough_function = x.second(*this);
							passthrough = true;
							return;
						}
					}
					passthrough_function = definition->default_passthrough_function(*this);
					passthrough = true;
#ifdef _DEBUG
					OutputDebugStringA("Error: failed to find token list association for token ");
					OutputDebugStringA(std::string(left.start, left.end).c_str());
					OutputDebugStringA("\n");
#endif
				}
			}
		} else if ((in.type == token_type::brace) & (*in.start == '}')) {
			next(under_construction);
			reset();
		} else if (assoc != association_type::none) {
			auto classify = definition->tag_pair_classifier.classify(left.start, left.end);
			if (classify != (unsigned char)0) {
				definition->tag_pair_handlers[classify - 1](under_construction, left, assoc, in);
				left = token_and_type{ nullptr, nullptr, token_type::unknown };
				assoc = association_type::none;
			} else {
				for (auto& x : definition->special_pair_handlers) {
					if (x.first(left.start, left.end)) {
						x.second(under_construction, left, assoc, in);
						left = token_and_type{ nullptr, nullptr, token_type::unknown };
						assoc = association_type::none;
						return;
					}
				}
#ifdef _DEBUG
				OutputDebugStringA("Error: failed to find token-identifier pair association for token ");
				OutputDebugStringA(std::string(left.start, left.end).c_str());
				OutputDebugStringA("\n");
#endif
				left = token_and_type{ nullptr, nullptr, token_type::unknown };
				assoc = association_type::none;
			}

		} else if ((in.type == token_type::identifier) | (in.type == token_type::quoted_string)) {
			auto classify = definition->tag_pair_classifier.classify(left.start, left.end);
			if (classify != (unsigned char)0) {
				definition->tag_pair_handlers[classify - 1](under_construction, left, association_type::none, token_and_type{ nullptr, nullptr, token_type::unknown });
				left = in;
			} else {
				for (auto& x : definition->special_pair_handlers) {
					if (x.first(left.start, left.end)) {
						x.second(under_construction, left, association_type::none, token_and_type{ nullptr, nullptr, token_type::unknown });
						left = in;
						return;
					}
				}
			}
		}
	} else {
		(*passthrough_function)(in);
	}
}

template<typename result_type>
template<typename NEXT>
void token_to_object_stream_operation<result_type>::yield(NEXT& next) {
	if (passthrough)
		passthrough_function->yield();
	next(under_construction);
	reset();
}

template<typename result_type>
template<typename value, typename parsing_operation, typename handling_function>
void object_parsing_definition<result_type>::add_case_handler(const char * tag, const parsing_operation & parse, const handling_function & handle) {
	tag_classifier.add_option(text_identifier{ tag, tag + strnlen_s(tag, 256), (unsigned char)(tag_handlers.size() + 1) });
	tag_handlers.emplace_back(make_passthrough<value>(parse, handle));
}

template<typename result_type>
template<typename handling_function>
void object_parsing_definition<result_type>::add_case_handler(const char * tag, const handling_function & handle) {
	tag_pair_classifier.add_option(text_identifier{ tag, tag + strnlen_s(tag, 256), (unsigned char)(tag_pair_handlers.size() + 1) });
	tag_pair_handlers.emplace_back(handle);
}

template<typename result_type>
template<typename value, typename parsing_operation, typename handling_function>
void object_parsing_definition<result_type>::add_special_case_handler(const boolean_classifying_function & test, const parsing_operation & parse, const handling_function & handle) {
	special_handlers.emplace_back(test, make_passthrough<value>(parse, handle));
}

template<typename result_type>
template<typename handling_function>
void object_parsing_definition<result_type>::add_special_case_handler(const boolean_classifying_function & test, const handling_function & handle) {
	special_pair_handlers.emplace_back(test, handle);
}

template<typename result_type>
object_parsing_definition<result_type>::object_parsing_definition() {
	default_passthrough_function = make_passthrough<int>(end_brace_finder(), [](const result_type&, const token_and_type&, association_type, int) {});
}

template<typename result_type>
inline void token_to_object_stream_operation<result_type>::reset() {
	left = token_and_type{ nullptr, nullptr, token_type::unknown };
	assoc = association_type::none;
	passthrough = false;

	under_construction.~result_type();
	new (&under_construction) result_type();
}

#include "Parsers\\parsers.hpp"
#include "streams\\streams.hpp"
#include "object_parsing\\object_parsing.hpp"