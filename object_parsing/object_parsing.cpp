#include "stdafx.h"
#include "object_parsing.hpp"
#include "Parsers\parsers.hpp"

std::string associated_string(const token_group& left, const token_group* child_start, const token_group* child_end) {
	if((left.association != association_type::eq) | (left.group_size == 0) | (child_start + 1 != child_end))
		return std::string("");

	return std::string(child_start->token.start, child_start->token.end - child_start->token.start);
}