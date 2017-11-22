#pragma once
#include "stdafx.h"
#include "Parsers\\parsers.h"
#include "streams\\streams.h"
#include "object_parsing\\\object_parsing.h"

class end_brace_finder;
class vector_of_strings_operation;
class vector_of_ints_operation;
class vector_of_doubles_operation;
class tokenize_char_stream_operation;

class token_to_object_stream_operation_base;

template<typename result_type>
class token_to_object_stream_operation;

template<typename result_type>
class object_parsing_definition;

template<typename result_type, typename context>
class generic_object_parser;