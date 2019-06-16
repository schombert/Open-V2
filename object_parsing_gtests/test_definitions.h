#include "common\\common.h"
#include "Parsers\\parsers.hpp"

#pragma warning( push )
#pragma warning( disable : 4065 )

namespace test_definitions {
template<typename ERR_H, typename C = int32_t>
three_bool parse_three_bool(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
three_bool_recursive parse_three_bool_recursive(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
vector_of_int parse_vector_of_int(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
three_bool_vector parse_three_bool_vector(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
association_container parse_association_container(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
int_vector_summation parse_int_vector_summation(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
base_parse parse_base_parse(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
derived_parse parse_derived_parse(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
variable_named_set parse_variable_named_set(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
variable_named_set_container parse_variable_named_set_container(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
extern_reader parse_extern_reader(token_generator& gen, ERR_H& err, C&& context = 0);

template<typename ERR_H, typename C>
three_bool parse_three_bool(token_generator& gen, ERR_H& err, C&& context) {
	 three_bool cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true)) {
							 cobj.a = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x62:
						 if((true)) {
							 cobj.b = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true)) {
							 cobj.c = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x64:
						 if((true)) {
							 cobj.d = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
three_bool_recursive parse_three_bool_recursive(token_generator& gen, ERR_H& err, C&& context) {
	 three_bool_recursive cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 if((true && (*(cur.start + 0) | 0x20) == 0x6B)) {
						 add_tbr(cobj, parse_three_bool_recursive(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true)) {
							 cobj.a = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x62:
						 if((true)) {
							 cobj.b = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true)) {
							 cobj.c = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x64:
						 if((true)) {
							 cobj.d = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
vector_of_int parse_vector_of_int(token_generator& gen, ERR_H& err, C&& context) {
	 vector_of_int cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 add_int_to_vector(cobj, token_to<int32_t>(cur, err), context);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
three_bool_vector parse_three_bool_vector(token_generator& gen, ERR_H& err, C&& context) {
	 three_bool_vector cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 if((true && (*(cur.start + 0) | 0x20) == 0x6B)) {
						 cobj.j = parse_vector_of_int(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true)) {
							 cobj.a = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x62:
						 if((true)) {
							 cobj.b = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true)) {
							 cobj.c = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x64:
						 if((true)) {
							 cobj.d = token_to<bool>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
association_container parse_association_container(token_generator& gen, ERR_H& err, C&& context) {
	 association_container cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 add_int(cobj, cur, parse_int_vector_summation(gen, err, context), context);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true)) {
							 cobj.a = token_to<bool>(rh_token, err);
						 } else {
							 add_assoc_int_pair(cobj, cur, assoc_type, token_to<int32_t>(rh_token, err), context);
						 }
						 break;
					 case 0x62:
						 if((true)) {
							 cobj.b = token_to<bool>(rh_token, err);
						 } else {
							 add_assoc_int_pair(cobj, cur, assoc_type, token_to<int32_t>(rh_token, err), context);
						 }
						 break;
					 default:
						 add_assoc_int_pair(cobj, cur, assoc_type, token_to<int32_t>(rh_token, err), context);
					 }
					 break;
				 default:
					 add_assoc_int_pair(cobj, cur, assoc_type, token_to<int32_t>(rh_token, err), context);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
int_vector_summation parse_int_vector_summation(token_generator& gen, ERR_H& err, C&& context) {
	 int_vector_summation cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 cobj.add_int(token_to<int32_t>(cur, err), context);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
base_parse parse_base_parse(token_generator& gen, ERR_H& err, C&& context) {
	 base_parse cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 if((true && (*(cur.start + 0) | 0x20) == 0x78)) {
						 cobj.x = token_to<double>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
derived_parse parse_derived_parse(token_generator& gen, ERR_H& err, C&& context) {
	 derived_parse cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 1:
					 if((true && (*(cur.start + 0) | 0x20) == 0x78)) {
						 cobj.x = token_to<double>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x6C617673ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x6575)) {
						 cobj.svalue = token_to<std::string>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
variable_named_set parse_variable_named_set(token_generator& gen, ERR_H& err, C&& context) {
	 variable_named_set cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 5:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x74706D65ui32 && (*(cur.start + 4) | 0x20) == 0x79)) {
						 discard_group(gen);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 default:
					 err.unhandled_group_key(cur); discard_group(gen);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 cobj.add_int(token_to<int32_t>(cur, err), context);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
variable_named_set_container parse_variable_named_set_container(token_generator& gen, ERR_H& err, C&& context) {
	 variable_named_set_container cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 cobj.add_set(cur, parse_variable_named_set(gen, err, context), context);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
extern_reader parse_extern_reader(token_generator& gen, ERR_H& err, C&& context) {
	 extern_reader cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 10:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6F627F6565726874ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x6C6F)) {
						 cobj.m_a = gen_make_three_bool(gen, err, context);
					 } else {
						 cobj.add_variable_named_set(cur, gen_make_var_set(cur, gen, err, context), context);
					 }
					 break;
				 default:
					 cobj.add_variable_named_set(cur, gen_make_var_set(cur, gen, err, context), context);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 err.unhandled_association_key(cur);
				 }
			 }
		 } else {
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
}
#pragma warning( pop )

