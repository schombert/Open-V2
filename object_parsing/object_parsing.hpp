#pragma once
#include "object_parsing.h"
#include "Parsers\\parsers.h"
#include "text_classifier\\text_classifiers.h"
#include <string>

#ifdef _DEBUG
#include <Windows.h>
#endif


template<typename result_type>
result_type object_parser<result_type>::operator()(
	const token_group* start,
	const token_group* end) const {

	result_type under_construction;

	forall_tokens(start, end, [_this = this, &under_construction](const token_group& value, const token_group* start, const token_group* end) {
		if (const auto tag = _this->tag_classifier.classify(value.token.start, value.token.end); tag != 0) {
			_this->tag_handlers[tag - 1].second(under_construction, value, start, end);
		} else {
			_this->default_handler(under_construction, value, start, end);
		}
	});

	return under_construction;
}

template<typename result_type>
void object_parser<result_type>::add_case_handler(const char* tag, const handling_function &handle) {
	tag_classifier.add_option(text_identifier{ tag, tag + strnlen_s(tag, 256), (unsigned char)(tag_handlers.size() + 1) });
	tag_handlers.emplace_back(tag, handle);
}

template<typename result_type>
void object_parser<result_type>::reset_default_handler(const handling_function& f) {
	default_handler = f;
}

template<typename result_type>
void object_parser<result_type>::add_special_case_handler(
	const boolean_classifying_function &test,
	const handling_function &handle) {

	default_handler = [old = this->default_handler, test, handle](result_type& r, const token_group& value, const token_group* start, const token_group* end){
		if (test(value.token.start, value.token.end))
			handle(r, value, start, end);
		else
			old(r, value, start, end);
	};
}
