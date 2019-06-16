#include "common\\common.h"
#include "Parsers\\parsers.hpp"

#pragma warning( push )
#pragma warning( disable : 4065 )

namespace artisan_file {
template<typename ERR_H, typename C = int32_t>
rebel_types parse_rebel_types(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
unemployment_by_type_trigger parse_unemployment_by_type_trigger(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
work_available_trigger parse_work_available_trigger(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
trigger_group parse_trigger_group(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
modifier parse_modifier(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
modifier_group parse_modifier_group(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
simple_modifier_container parse_simple_modifier_container(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
complex_modifier_container parse_complex_modifier_container(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
vec_int parse_vec_int(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
vec_str_double parse_vec_str_double(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
vec_str_simple parse_vec_str_simple(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
vec_str_complex parse_vec_str_complex(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
poptype_file parse_poptype_file(token_generator& gen, ERR_H& err, C&& context = 0);

template<typename ERR_H, typename C>
rebel_types parse_rebel_types(token_generator& gen, ERR_H& err, C&& context) {
	 rebel_types cobj;
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
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7972746E61666E69ui64)) {
						 cobj.infantry = token_to<double>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 9:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x616C756765727269ui64 && (*(cur.start + 8) | 0x20) == 0x72)) {
						 cobj.irregular = token_to<double>(rh_token, err);
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
unemployment_by_type_trigger parse_unemployment_by_type_trigger(token_generator& gen, ERR_H& err, C&& context) {
	 unemployment_by_type_trigger cobj;
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
				 case 4:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x65707974ui32)) {
						 cobj.type = token_to<std::string>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 5:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x756C6176ui32 && (*(cur.start + 4) | 0x20) == 0x65)) {
						 cobj.set_value(assoc_type, token_to<double>(rh_token, err), context);
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
work_available_trigger parse_work_available_trigger(token_generator& gen, ERR_H& err, C&& context) {
	 work_available_trigger cobj;
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
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x6B726F77ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7265)) {
						 cobj.worker = token_to<std::string>(rh_token, err);
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
trigger_group parse_trigger_group(token_generator& gen, ERR_H& err, C&& context) {
	 trigger_group cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 2:
					 if((true && (*(uint16_t*)(cur.start + 0) | 0x2020) == 0x726F)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::or_t>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 3:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x646E)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::and_t>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x746F)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::not_t>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 7:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x6E756F63ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7274 && (*(cur.start + 6) | 0x20) == 0x79)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::country>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 8:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x637F796Eui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x726F && (*(cur.start + 7) | 0x20) == 0x65)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::any_core>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x7461636Fui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6F69 && (*(cur.start + 7) | 0x20) == 0x6E)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::location>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x63737F6574617473ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x706F && (*(cur.start + 10) | 0x20) == 0x65)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::state_scope>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 14:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6176617F6B726F77ui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x62616C69ui32 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x656C)) {
						 cobj.add_other_trigger(parse_work_available_trigger(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 18:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x656E776F7F796E61ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6E69766F72707F64ui64 && (*(uint16_t*)(cur.start + 16) | 0x2020) == 0x6563)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::any_owned_province>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 20:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x686769656E7F796Eui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E756F637F726F62ui64 && (*(uint16_t*)(cur.start + 17) | 0x2020) == 0x7274 && (*(cur.start + 19) | 0x20) == 0x79)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::any_neighbor_country>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x75:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6D796F6C706D656Eui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x747F79627F746E65ui64 && (*(uint16_t*)(cur.start + 17) | 0x2020) == 0x7079 && (*(cur.start + 19) | 0x20) == 0x65)) {
							 cobj.add_other_trigger(parse_unemployment_by_type_trigger(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
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
				 case 3:
					 if((true && (*(uint16_t*)(cur.start + 0) | 0x2020) == 0x6177 && (*(cur.start + 2) | 0x20) == 0x72)) {
						 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::war>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 7:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x73:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6576616Cui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x7972)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::slavery>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x61727265ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6E69)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::terrain>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 8:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6C737F73ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x7661 && (*(cur.start + 7) | 0x20) == 0x65)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_slave>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x72657469ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6361 && (*(cur.start + 7) | 0x20) == 0x79)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::literacy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x69646C6Fui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x7265 && (*(cur.start + 7) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::soldiers>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 9:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x69:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x696C6976ui32 && (*(uint16_t*)(cur.start + 6) | 0x2020) == 0x657A && (*(cur.start + 8) | 0x20) == 0x64)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::civilized>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x79677265ui32 && (*(uint16_t*)(cur.start + 6) | 0x2020) == 0x656D && (*(cur.start + 8) | 0x20) == 0x6E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::clergymen>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x6E69746Eui32 && (*(uint16_t*)(cur.start + 6) | 0x2020) == 0x6E65 && (*(cur.start + 8) | 0x20) == 0x74)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::continent>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x79636E6174696C69ui64)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::militancy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7974696C6172756Cui64)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::plurality>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 10:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x63:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x61746970ui32 && (*(cur.start + 9) | 0x20) == 0x6C)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_capital>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x61747361ui32 && (*(cur.start + 9) | 0x20) == 0x6C)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_coastal>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6465656E7F656669ui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::life_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x736968636E617665ui64 && (*(cur.start + 9) | 0x20) == 0x6D)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::revanchism>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x706F707F6C61746Fui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.add_int_trigger<int_trigger::int_trigger_type::total_pops>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x72756F687F6B726Fui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::work_hours>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 11:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x62:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6172637561657275ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7374)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::bureaucrats>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x73696C6174697061ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7374)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::capitalists>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x61637F68746C6165ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6572)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::health_care>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x696E6F6C6F637F73ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6C61)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_colonial>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x697461727F656669ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x676E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::life_rating>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6F6665727F656761ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6D72)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::wage_reform>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 12:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x747F706F707F7361ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7079 && (*(cur.start + 11) | 0x20) == 0x65)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_pop_type>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x656E7F7972757875ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6465 && (*(cur.start + 11) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::luxury_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6769727F73736572ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7468 && (*(cur.start + 11) | 0x20) == 0x73)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::press_rights>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6C6F707F65646172ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6369 && (*(cur.start + 11) | 0x20) == 0x79)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::trade_policy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x75:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6D796F6C706D656Eui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6E65 && (*(cur.start + 11) | 0x20) == 0x74)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::unemployment>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 13:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x73756F6963736E6Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x7373656Eui32)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::consciousness>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6F746361667F7361ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x73656972ui32)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::has_factories>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x766C616E6F697461ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x65756C61ui32)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::nationalvalue>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x76:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x79737F676E69746Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6D657473ui32)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::voting_system>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 14:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6769667F656D6972ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6E697468ui32 && (*(cur.start + 13) | 0x20) == 0x67)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::crime_fighting>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F79616479726576ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6465656Eui32 && (*(cur.start + 13) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::everyday_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x727F7478656E7F73ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x726F6665ui32 && (*(cur.start + 13) | 0x20) == 0x6D)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::is_next_reform>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F79726174696C69ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x726F6373ui32 && (*(cur.start + 13) | 0x20) == 0x65)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::military_score>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x65727F6C6F6F6863ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6D726F66ui32 && (*(cur.start + 13) | 0x20) == 0x73)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::school_reforms>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x75616878657F7261ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6F697473ui32 && (*(cur.start + 13) | 0x20) == 0x6E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::war_exhaustion>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 15:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F63696D6F6E6F63ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x696C6F70ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x7963)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::economic_policy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x637F706F707F7361ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x75746C75ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6572)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_pop_culture>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x656D7F63696C6275ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6E697465ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x7367)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::public_meetings>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x76:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6E6172667F65746Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x69686373ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6573)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::vote_franschise>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 16:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x68:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x61:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 5) | 0x2020202020202020ui64) == 0x667F7972746E756Fui64 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x616C && (*(cur.start + 15) | 0x20) == 0x67)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_country_flag>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 5) | 0x2020202020202020ui64) == 0x67696C65727F706Fui64 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6F69 && (*(cur.start + 15) | 0x20) == 0x6E)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_pop_religion>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x69:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x67:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x707F726574616572ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x7265776Fui32)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_greater_power>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7061637F65746174ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x6C617469ui32)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_state_capital>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x73756F6967696C65ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6C6F707Fui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6369 && (*(cur.start + 15) | 0x20) == 0x79)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::religious_policy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 17:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x646C6975627F6E61ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x79726F746361667Fui64)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::can_build_factory>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F65746174737F73ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E6F6967696C6572ui64)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_state_religion>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6C61636974696C6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x736569747261707Fui64)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::political_parties>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 18:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6E6F697461637564ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E69646E6570737Fui64 && (*(cur.start + 17) | 0x20) == 0x67)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::education_spending>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x72616D6972707F73ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x7275746C75637F79ui64 && (*(cur.start + 17) | 0x20) == 0x65)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_primary_culture>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x6765727F79746566ui64 && (*(uint64_t*)(cur.start + 10) | 0x2020202020202020ui64) == 0x736E6F6974616C75ui64)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::safety_regulations>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x6665727F6C616963ui64 && (*(uint64_t*)(cur.start + 10) | 0x2020202020202020ui64) == 0x746E61777F6D726Fui64)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::social_reform_want>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
					 break;
				 case 19:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7F64657470656363ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x746C7563ui32 && (*(uint16_t*)(cur.start + 16) | 0x2020) == 0x7275 && (*(cur.start + 18) | 0x20) == 0x65)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_accepted_culture>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7F79676F6C6F6564ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x62616E65ui32 && (*(uint16_t*)(cur.start + 16) | 0x2020) == 0x656C && (*(cur.start + 18) | 0x20) == 0x64)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::is_ideology_enabled>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 21:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 3) | 0x2020202020202020ui64) == 0x727F6C6163697469ui64 && (*(uint64_t*)(cur.start + 11) | 0x2020202020202020ui64) == 0x61777F6D726F6665ui64 && (*(uint16_t*)(cur.start + 19) | 0x2020) == 0x746E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::political_reform_want>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint64_t*)(cur.start + 3) | 0x2020202020202020ui64) == 0x6174617274737F72ui64 && (*(uint64_t*)(cur.start + 11) | 0x2020202020202020ui64) == 0x6E6174696C696D7Fui64 && (*(uint16_t*)(cur.start + 19) | 0x2020) == 0x7963)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_militancy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x61707F676E696C75ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6F6564697F797472ui64 && (*(uint32_t*)(cur.start + 17) | 0x20202020ui32) == 0x79676F6Cui32)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::ruling_party_ideology>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 22:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7274737F726F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6566696C7F617461ui64 && (*(uint32_t*)(cur.start + 16) | 0x20202020ui32) == 0x65656E7Fui32 && (*(uint16_t*)(cur.start + 20) | 0x2020) == 0x7364)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_life_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 23:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7469777F65657267ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x676E696C75727F68ui64 && (*(uint32_t*)(cur.start + 17) | 0x20202020ui32) == 0x7261707Fui32 && (*(uint16_t*)(cur.start + 21) | 0x2020) == 0x7974)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::agree_with_ruling_party>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x74737F656C646469ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6C696D7F61746172ui64 && (*(uint32_t*)(cur.start + 17) | 0x20202020ui32) == 0x6E617469ui32 && (*(uint16_t*)(cur.start + 21) | 0x2020) == 0x7963)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::middle_strata_militancy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 24:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7274737F726F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x7578756C7F617461ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x736465656E7F7972ui64)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_luxury_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 26:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7274737F726F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x726576657F617461ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x65656E7F79616479ui64 && (*(uint16_t*)(cur.start + 24) | 0x2020) == 0x7364)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_everyday_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 28:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x737F656C6464696Dui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x76657F6174617274ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x6E7F796164797265ui64 && (*(uint32_t*)(cur.start + 24) | 0x20202020ui32) == 0x73646565ui32)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::middle_strata_everyday_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
modifier parse_modifier(token_generator& gen, ERR_H& err, C&& context) {
	 modifier cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 2:
					 if((true && (*(uint16_t*)(cur.start + 0) | 0x2020) == 0x726F)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::or_t>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 3:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x646E)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::and_t>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x746F)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::not_t>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 7:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x6E756F63ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7274 && (*(cur.start + 6) | 0x20) == 0x79)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::country>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 8:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x637F796Eui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x726F && (*(cur.start + 7) | 0x20) == 0x65)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::any_core>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x7461636Fui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6F69 && (*(cur.start + 7) | 0x20) == 0x6E)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::location>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x63737F6574617473ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x706F && (*(cur.start + 10) | 0x20) == 0x65)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::state_scope>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 14:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6176617F6B726F77ui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x62616C69ui32 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x656C)) {
						 cobj.add_other_trigger(parse_work_available_trigger(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 18:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x656E776F7F796E61ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6E69766F72707F64ui64 && (*(uint16_t*)(cur.start + 16) | 0x2020) == 0x6563)) {
						 cobj.add_other_trigger<trigger_group::trigger_group_type::any_owned_province>(parse_trigger_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 20:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x686769656E7F796Eui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E756F637F726F62ui64 && (*(uint16_t*)(cur.start + 17) | 0x2020) == 0x7274 && (*(cur.start + 19) | 0x20) == 0x79)) {
							 cobj.add_other_trigger<trigger_group::trigger_group_type::any_neighbor_country>(parse_trigger_group(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x75:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6D796F6C706D656Eui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x747F79627F746E65ui64 && (*(uint16_t*)(cur.start + 17) | 0x2020) == 0x7079 && (*(cur.start + 19) | 0x20) == 0x65)) {
							 cobj.add_other_trigger(parse_unemployment_by_type_trigger(gen, err, context), context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
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
				 case 3:
					 if((true && (*(uint16_t*)(cur.start + 0) | 0x2020) == 0x6177 && (*(cur.start + 2) | 0x20) == 0x72)) {
						 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::war>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x74636166ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x726F)) {
						 cobj.factor = token_to<double>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 7:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x73:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6576616Cui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x7972)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::slavery>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x61727265ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6E69)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::terrain>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 8:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6C737F73ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x7661 && (*(cur.start + 7) | 0x20) == 0x65)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_slave>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x72657469ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6361 && (*(cur.start + 7) | 0x20) == 0x79)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::literacy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x69646C6Fui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x7265 && (*(cur.start + 7) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::soldiers>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 9:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x69:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x696C6976ui32 && (*(uint16_t*)(cur.start + 6) | 0x2020) == 0x657A && (*(cur.start + 8) | 0x20) == 0x64)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::civilized>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x79677265ui32 && (*(uint16_t*)(cur.start + 6) | 0x2020) == 0x656D && (*(cur.start + 8) | 0x20) == 0x6E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::clergymen>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x6E69746Eui32 && (*(uint16_t*)(cur.start + 6) | 0x2020) == 0x6E65 && (*(cur.start + 8) | 0x20) == 0x74)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::continent>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x79636E6174696C69ui64)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::militancy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7974696C6172756Cui64)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::plurality>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 10:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x63:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x61746970ui32 && (*(cur.start + 9) | 0x20) == 0x6C)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_capital>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x61747361ui32 && (*(cur.start + 9) | 0x20) == 0x6C)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_coastal>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6465656E7F656669ui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::life_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x736968636E617665ui64 && (*(cur.start + 9) | 0x20) == 0x6D)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::revanchism>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x706F707F6C61746Fui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.add_int_trigger<int_trigger::int_trigger_type::total_pops>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x72756F687F6B726Fui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::work_hours>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 11:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x62:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6172637561657275ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7374)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::bureaucrats>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x73696C6174697061ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7374)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::capitalists>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x61637F68746C6165ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6572)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::health_care>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x696E6F6C6F637F73ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6C61)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_colonial>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x697461727F656669ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x676E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::life_rating>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6F6665727F656761ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6D72)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::wage_reform>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 12:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x747F706F707F7361ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7079 && (*(cur.start + 11) | 0x20) == 0x65)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_pop_type>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x656E7F7972757875ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6465 && (*(cur.start + 11) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::luxury_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6769727F73736572ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x7468 && (*(cur.start + 11) | 0x20) == 0x73)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::press_rights>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6C6F707F65646172ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6369 && (*(cur.start + 11) | 0x20) == 0x79)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::trade_policy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x75:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6D796F6C706D656Eui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6E65 && (*(cur.start + 11) | 0x20) == 0x74)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::unemployment>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 13:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x73756F6963736E6Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x7373656Eui32)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::consciousness>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6F746361667F7361ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x73656972ui32)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::has_factories>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x766C616E6F697461ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x65756C61ui32)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::nationalvalue>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x76:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x79737F676E69746Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6D657473ui32)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::voting_system>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 14:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6769667F656D6972ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6E697468ui32 && (*(cur.start + 13) | 0x20) == 0x67)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::crime_fighting>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F79616479726576ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6465656Eui32 && (*(cur.start + 13) | 0x20) == 0x73)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::everyday_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x727F7478656E7F73ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x726F6665ui32 && (*(cur.start + 13) | 0x20) == 0x6D)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::is_next_reform>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F79726174696C69ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x726F6373ui32 && (*(cur.start + 13) | 0x20) == 0x65)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::military_score>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x65727F6C6F6F6863ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6D726F66ui32 && (*(cur.start + 13) | 0x20) == 0x73)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::school_reforms>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x75616878657F7261ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6F697473ui32 && (*(cur.start + 13) | 0x20) == 0x6E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::war_exhaustion>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 15:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F63696D6F6E6F63ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x696C6F70ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x7963)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::economic_policy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x68:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x637F706F707F7361ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x75746C75ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6572)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_pop_culture>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x656D7F63696C6275ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6E697465ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x7367)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::public_meetings>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x76:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6E6172667F65746Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x69686373ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6573)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::vote_franschise>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 16:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x68:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x61:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 5) | 0x2020202020202020ui64) == 0x667F7972746E756Fui64 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x616C && (*(cur.start + 15) | 0x20) == 0x67)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_country_flag>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 5) | 0x2020202020202020ui64) == 0x67696C65727F706Fui64 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6F69 && (*(cur.start + 15) | 0x20) == 0x6E)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::has_pop_religion>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x69:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x67:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x707F726574616572ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x7265776Fui32)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_greater_power>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7061637F65746174ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x6C617469ui32)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_state_capital>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x73756F6967696C65ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6C6F707Fui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6369 && (*(cur.start + 15) | 0x20) == 0x79)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::religious_policy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 17:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x646C6975627F6E61ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x79726F746361667Fui64)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::can_build_factory>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F65746174737F73ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E6F6967696C6572ui64)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_state_religion>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6C61636974696C6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x736569747261707Fui64)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::political_parties>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 18:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6E6F697461637564ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E69646E6570737Fui64 && (*(cur.start + 17) | 0x20) == 0x67)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::education_spending>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x72616D6972707F73ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x7275746C75637F79ui64 && (*(cur.start + 17) | 0x20) == 0x65)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_primary_culture>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x6765727F79746566ui64 && (*(uint64_t*)(cur.start + 10) | 0x2020202020202020ui64) == 0x736E6F6974616C75ui64)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::safety_regulations>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x6665727F6C616963ui64 && (*(uint64_t*)(cur.start + 10) | 0x2020202020202020ui64) == 0x746E61777F6D726Fui64)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::social_reform_want>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
					 break;
				 case 19:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7F64657470656363ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x746C7563ui32 && (*(uint16_t*)(cur.start + 16) | 0x2020) == 0x7275 && (*(cur.start + 18) | 0x20) == 0x65)) {
							 cobj.add_bool_trigger<bool_trigger::bool_trigger_type::is_accepted_culture>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7F79676F6C6F6564ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x62616E65ui32 && (*(uint16_t*)(cur.start + 16) | 0x2020) == 0x656C && (*(cur.start + 18) | 0x20) == 0x64)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::is_ideology_enabled>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 21:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 3) | 0x2020202020202020ui64) == 0x727F6C6163697469ui64 && (*(uint64_t*)(cur.start + 11) | 0x2020202020202020ui64) == 0x61777F6D726F6665ui64 && (*(uint16_t*)(cur.start + 19) | 0x2020) == 0x746E)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::political_reform_want>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint64_t*)(cur.start + 3) | 0x2020202020202020ui64) == 0x6174617274737F72ui64 && (*(uint64_t*)(cur.start + 11) | 0x2020202020202020ui64) == 0x6E6174696C696D7Fui64 && (*(uint16_t*)(cur.start + 19) | 0x2020) == 0x7963)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_militancy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x61707F676E696C75ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6F6564697F797472ui64 && (*(uint32_t*)(cur.start + 17) | 0x20202020ui32) == 0x79676F6Cui32)) {
							 cobj.add_string_trigger<string_trigger::string_trigger_type::ruling_party_ideology>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 22:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7274737F726F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6566696C7F617461ui64 && (*(uint32_t*)(cur.start + 16) | 0x20202020ui32) == 0x65656E7Fui32 && (*(uint16_t*)(cur.start + 20) | 0x2020) == 0x7364)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_life_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 23:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7469777F65657267ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x676E696C75727F68ui64 && (*(uint32_t*)(cur.start + 17) | 0x20202020ui32) == 0x7261707Fui32 && (*(uint16_t*)(cur.start + 21) | 0x2020) == 0x7974)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::agree_with_ruling_party>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x74737F656C646469ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6C696D7F61746172ui64 && (*(uint32_t*)(cur.start + 17) | 0x20202020ui32) == 0x6E617469ui32 && (*(uint16_t*)(cur.start + 21) | 0x2020) == 0x7963)) {
							 cobj.add_double_trigger<double_trigger::double_trigger_type::middle_strata_militancy>(assoc_type, token_to<token_and_type>(rh_token, err), context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 24:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7274737F726F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x7578756C7F617461ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x736465656E7F7972ui64)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_luxury_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 26:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7274737F726F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x726576657F617461ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x65656E7F79616479ui64 && (*(uint16_t*)(cur.start + 24) | 0x2020) == 0x7364)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::poor_strata_everyday_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 28:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x737F656C6464696Dui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x76657F6174617274ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x6E7F796164797265ui64 && (*(uint32_t*)(cur.start + 24) | 0x20202020ui32) == 0x73646565ui32)) {
						 cobj.add_double_trigger<double_trigger::double_trigger_type::middle_strata_everyday_needs>(assoc_type, token_to<token_and_type>(rh_token, err), context);
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
modifier_group parse_modifier_group(token_generator& gen, ERR_H& err, C&& context) {
	 modifier_group cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7265696669646F6Dui64)) {
						 cobj.add_modifier(parse_modifier(gen, err, context), context);
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
			 err.unhandled_free_value(cur);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
simple_modifier_container parse_simple_modifier_container(token_generator& gen, ERR_H& err, C&& context) {
	 simple_modifier_container cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;		 }		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7265696669646F6Dui64)) {
						 cobj.add_modifier(parse_modifier(gen, err, context), context);
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
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x74636166ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x726F)) {
						 cobj.factor = token_to<double>(rh_token, err);
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
complex_modifier_container parse_complex_modifier_container(token_generator& gen, ERR_H& err, C&& context) {
	 complex_modifier_container cobj;
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
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x756F7267ui32 && (*(cur.start + 4) | 0x20) == 0x70)) {
						 cobj.add_modifier_group(parse_modifier_group(gen, err, context), context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7265696669646F6Dui64)) {
						 cobj.add_modifier(parse_modifier(gen, err, context), context);
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
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x74636166ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x726F)) {
						 cobj.factor = token_to<double>(rh_token, err);
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
vec_int parse_vec_int(token_generator& gen, ERR_H& err, C&& context) {
	 vec_int cobj;
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
			 add_vec_int(cobj, token_to<int32_t>(cur, err), context);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
vec_str_double parse_vec_str_double(token_generator& gen, ERR_H& err, C&& context) {
	 vec_str_double cobj;
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
					 add_vec_str_double(cobj, cur, assoc_type, token_to<double>(rh_token, err), context);
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
vec_str_simple parse_vec_str_simple(token_generator& gen, ERR_H& err, C&& context) {
	 vec_str_simple cobj;
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
					 add_vec_str_simple(cobj, cur, parse_simple_modifier_container(gen, err, context), context);
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
vec_str_complex parse_vec_str_complex(token_generator& gen, ERR_H& err, C&& context) {
	 vec_str_complex cobj;
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
					 add_vec_str_complex(cobj, cur, parse_complex_modifier_container(gen, err, context), context);
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
poptype_file parse_poptype_file(token_generator& gen, ERR_H& err, C&& context) {
	 poptype_file cobj;
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
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x726F6C6Fui32)) {
							 cobj.color = parse_vec_int(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x72:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6C656265ui32)) {
							 cobj.rebel = parse_rebel_types(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x75737369ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7365)) {
						 cobj.issues = parse_vec_str_simple(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 10:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6569676F6C6F6564ui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.ideologies = parse_vec_str_simple(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x6C:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6465656E7F656669ui64 && (*(cur.start + 9) | 0x20) == 0x73)) {
							 cobj.life_needs = parse_vec_str_double(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x747F65746F6D6F72ui64 && (*(cur.start + 9) | 0x20) == 0x6F)) {
							 cobj.promote_to = parse_vec_str_complex(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 12:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6E7F79727578756Cui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x73646565ui32)) {
						 cobj.luxury_needs = parse_vec_str_double(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 14:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7961647972657665ui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x65656E7Fui32 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x7364)) {
						 cobj.everyday_needs = parse_vec_str_double(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 16:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6F6974617267696Dui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x7465677261747F6Eui64)) {
						 cobj.migration_target = parse_simple_modifier_container(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 24:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7F7972746E756F63ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6F6974617267696Dui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x7465677261747F6Eui64)) {
						 cobj.country_migration_target = parse_simple_modifier_container(gen, err, context);
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
				 case 6:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x70:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x65746972ui32)) {
							 cobj.sprite = token_to<uint32_t>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x61746172ui32)) {
							 cobj.strata = token_to<std::string>(rh_token, err);
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
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x657A69737F78616Dui64)) {
						 cobj.max_size = token_to<int32_t>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 10:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x73697472617F7369ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x6E61)) {
						 cobj.is_artisan = token_to<bool>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 14:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x616D7F656772656Dui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x69737F78ui32 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x657A)) {
						 cobj.merge_max_size = token_to<int32_t>(rh_token, err);
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
}
#pragma warning( pop )

