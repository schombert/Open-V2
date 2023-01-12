#include "common\\common.h"
#include "Parsers\\parsers.hpp"

#pragma warning( push )
#pragma warning( disable : 4065 )
#pragma warning( disable : 4189 )

namespace military_parsing {
template<typename ERR_H, typename C = int32_t>
war_file parse_war_file(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
war_date_block parse_war_date_block(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
war_goal_reader parse_war_goal_reader(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
parsed_relation parse_parsed_relation(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
parsed_leader parse_parsed_leader(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
parsed_regiment parse_parsed_regiment(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
parsed_ship parse_parsed_ship(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
parsed_army_or_navy parse_parsed_army_or_navy(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
oob_file parse_oob_file(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
peace_order parse_peace_order(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
cb_file parse_cb_file(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
traits_file parse_traits_file(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
personalities parse_personalities(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
backgrounds parse_backgrounds(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
trait parse_trait(token_generator& gen, ERR_H& err, C&& context = 0);
template<typename ERR_H, typename C = int32_t>
single_cb parse_single_cb(token_generator& gen, ERR_H& err, C&& context = 0);

template<typename ERR_H, typename C>
war_file parse_war_file(token_generator& gen, ERR_H& err, C&& context) {
	 war_file cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 default:
					 cobj.add_date_block(cur, parse_war_date_block(gen, err, context), err, context);
				 }
			 } else {
				 auto const assoc_token = gen.get();
				 auto const assoc_type = parse_association_type_b(assoc_token.start, assoc_token.end);
				 auto const rh_token = gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 4:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x656D616Eui32)) {
						 
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
war_date_block parse_war_date_block(token_generator& gen, ERR_H& err, C&& context) {
	 war_date_block cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6C616F677F726177ui64)) {
						 cobj.add_war_goal(parse_war_goal_reader(gen, err, context), err, context);
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
				 case 12:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x64:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x64:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x63617474ui32 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x656B && (*(cur.start + 11) | 0x20) == 0x72)) {
							 cobj.add_attacker(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x64:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x6E656665ui32 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6564 && (*(cur.start + 11) | 0x20) == 0x72)) {
							 cobj.add_defender(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
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
					 case 0x72:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x65:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x6D:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x63617474ui32 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x656B && (*(cur.start + 11) | 0x20) == 0x72)) {
							 cobj.remove_attacker(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x64:
						 if((true && (*(uint32_t*)(cur.start + 5) | 0x20202020ui32) == 0x6E656665ui32 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6564 && (*(cur.start + 11) | 0x20) == 0x72)) {
							 cobj.remove_attacker(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
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
war_goal_reader parse_war_goal_reader(token_generator& gen, ERR_H& err, C&& context) {
	 war_goal_reader cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
				 case 5:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x726F7463ui32)) {
							 cobj.set_actor(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x65746174ui32)) {
							 cobj.set_state(assoc_type, token_to<uint16_t>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7265766965636572ui64)) {
						 cobj.set_receiver(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x65627F7375736163ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x6C6C && (*(cur.start + 10) | 0x20) == 0x69)) {
						 cobj.set_casus_belli(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
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
parsed_relation parse_parsed_relation(token_generator& gen, ERR_H& err, C&& context) {
	 parsed_relation cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
				 case 5:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x6C:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6C657665ui32)) {
							 cobj.level = token_to<int32_t>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x76:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x65756C61ui32)) {
							 cobj.value = token_to<int32_t>(rh_token, err);
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
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x65636E65756C666Eui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6C61767Fui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6575)) {
							 cobj.influence_value = token_to<int32_t>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F79726174696C69ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x65636361ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x7373)) {
							 
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
parsed_leader parse_parsed_leader(token_generator& gen, ERR_H& err, C&& context) {
	 parsed_leader cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x64:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x7461 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 cobj.creation_date = token_to<date_tag>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x6D61 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x7079 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 cobj.set_type(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 7:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x74636970ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7275 && (*(cur.start + 6) | 0x20) == 0x65)) {
						 
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6567697473657270ui64)) {
						 
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 10:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x756F72676B636162ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x646E)) {
						 cobj.set_background(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6C616E6F73726570ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x7469 && (*(cur.start + 10) | 0x20) == 0x79)) {
						 cobj.set_personality(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
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
parsed_regiment parse_parsed_regiment(token_generator& gen, ERR_H& err, C&& context) {
	 parsed_regiment cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x68:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x6D6F && (*(cur.start + 3) | 0x20) == 0x65)) {
							 
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x6D61 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x7079 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 
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
parsed_ship parse_parsed_ship(token_generator& gen, ERR_H& err, C&& context) {
	 parsed_ship cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x6E:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x6D61 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x7079 && (*(cur.start + 3) | 0x20) == 0x65)) {
							 
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
parsed_army_or_navy parse_parsed_army_or_navy(token_generator& gen, ERR_H& err, C&& context) {
	 parsed_army_or_navy cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 4:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x70696873ui32)) {
						 cobj.add_ship(parse_parsed_ship(gen, err, context), err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x6461656Cui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7265)) {
						 cobj.add_leader(parse_parsed_leader(gen, err, context), err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x746E656D69676572ui64)) {
						 cobj.add_regiment(parse_parsed_regiment(gen, err, context), err, context);
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
				 case 4:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x656D616Eui32)) {
						 
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6E6F697461636F6Cui64)) {
						 cobj.set_location(assoc_type, token_to<uint16_t>(rh_token, err), err, context);
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
oob_file parse_oob_file(token_generator& gen, ERR_H& err, C&& context) {
	 oob_file cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 4:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x6D72 && (*(cur.start + 3) | 0x20) == 0x79)) {
							 cobj.add_army(parse_parsed_army_or_navy(gen, err, context), err, context);
						 } else {
							 cobj.add_relation(cur, parse_parsed_relation(gen, err, context), err, context);
						 }
						 break;
					 case 0x6E:
						 if((true && (*(uint16_t*)(cur.start + 1) | 0x2020) == 0x7661 && (*(cur.start + 3) | 0x20) == 0x79)) {
							 cobj.add_navy(parse_parsed_army_or_navy(gen, err, context), err, context);
						 } else {
							 cobj.add_relation(cur, parse_parsed_relation(gen, err, context), err, context);
						 }
						 break;
					 default:
						 cobj.add_relation(cur, parse_parsed_relation(gen, err, context), err, context);
					 }
					 break;
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x6461656Cui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7265)) {
						 cobj.add_leader(parse_parsed_leader(gen, err, context), err, context);
					 } else {
						 cobj.add_relation(cur, parse_parsed_relation(gen, err, context), err, context);
					 }
					 break;
				 default:
					 cobj.add_relation(cur, parse_parsed_relation(gen, err, context), err, context);
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
peace_order parse_peace_order(token_generator& gen, ERR_H& err, C&& context) {
	 peace_order cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
			 cobj.add_cb(token_to<token_and_type>(cur, err), err, context);
		 }
	 }
	 finish_parse(cobj);
	 return cobj;
}
template<typename ERR_H, typename C>
cb_file parse_cb_file(token_generator& gen, ERR_H& err, C&& context) {
	 cb_file cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x726F7F6563616570ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x6564 && (*(cur.start + 10) | 0x20) == 0x72)) {
						 cobj.handle_peace_order(parse_peace_order(gen, err, context), err, context);
					 } else {
						 cobj.reserve_cb(cur, find_group_range(cur, gen, err, context), err, context);
					 }
					 break;
				 default:
					 cobj.reserve_cb(cur, find_group_range(cur, gen, err, context), err, context);
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
traits_file parse_traits_file(token_generator& gen, ERR_H& err, C&& context) {
	 traits_file cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 10:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x756F72676B636162ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x646E)) {
						 cobj.add_backgrounds(parse_backgrounds(gen, err, context), err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6C616E6F73726570ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x7469 && (*(cur.start + 10) | 0x20) == 0x79)) {
						 cobj.add_personalities(parse_personalities(gen, err, context), err, context);
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
personalities parse_personalities(token_generator& gen, ERR_H& err, C&& context) {
	 personalities cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 14:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6F737265707F6F6Eui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x696C616Eui32 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x7974)) {
						 cobj.set_no_personality(parse_trait(gen, err, context), err, context);
					 } else {
						 cobj.add_trait(cur, parse_trait(gen, err, context), err, context);
					 }
					 break;
				 default:
					 cobj.add_trait(cur, parse_trait(gen, err, context), err, context);
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
backgrounds parse_backgrounds(token_generator& gen, ERR_H& err, C&& context) {
	 backgrounds cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 13:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x676B6361627F6F6Eui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x6E756F72ui32 && (*(cur.start + 12) | 0x20) == 0x64)) {
						 cobj.set_no_background(parse_trait(gen, err, context), err, context);
					 } else {
						 cobj.add_trait(cur, parse_trait(gen, err, context), err, context);
					 }
					 break;
				 default:
					 cobj.add_trait(cur, parse_trait(gen, err, context), err, context);
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
trait parse_trait(token_generator& gen, ERR_H& err, C&& context) {
	 trait cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
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
				 case 5:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x65657073ui32 && (*(cur.start + 4) | 0x20) == 0x64)) {
						 cobj.speed = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 6:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x63617474ui32 && (*(cur.start + 5) | 0x20) == 0x6B)) {
							 cobj.attack = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6C61726Fui32 && (*(cur.start + 5) | 0x20) == 0x65)) {
							 cobj.morale = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 7:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x65666564ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x636E && (*(cur.start + 6) | 0x20) == 0x65)) {
						 cobj.defence = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 10:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6E65697265707865ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x6563)) {
						 cobj.experience = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 11:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6C696261696C6572ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x7469 && (*(cur.start + 10) | 0x20) == 0x79)) {
						 cobj.reliability = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 12:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6173696E6167726Fui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x6E6F6974ui32)) {
						 cobj.organisation = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 14:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x69616E6E6F636572ui64 && (*(uint32_t*)(cur.start + 8) | 0x20202020ui32) == 0x6E617373ui32 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x6563)) {
						 cobj.reconnaissance = token_to<float>(rh_token, err);
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
single_cb parse_single_cb(token_generator& gen, ERR_H& err, C&& context) {
	 single_cb cobj;
	 for(token_and_type cur = gen.get(); cur.type != token_type::unknown && cur.type != token_type::close_brace; cur = gen.get()) {
		 if(cur.type == token_type::open_brace) { 
			 err.unhandled_free_set(); discard_group(gen);
			 continue;
		 }
		 auto peek_result = gen.next();
		 if(peek_result.type == token_type::special_identifier) {
			 auto peek2_result = gen.next_next();
			 if(peek2_result.type == token_type::open_brace) {
				 gen.get(); gen.get();
				 switch(int32_t(cur.end - cur.start)) {
				 case 6:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x617F6E6Fui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x6464)) {
						 cobj.on_add = read_cb_nation_effect(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 7:
					 if((true && (*(uint32_t*)(cur.start + 0) | 0x20202020ui32) == 0x7F6E6163ui32 && (*(uint16_t*)(cur.start + 4) | 0x2020) == 0x7375 && (*(cur.start + 6) | 0x20) == 0x65)) {
						 cobj.can_use = read_cb_nation_trigger(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 8:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x64696C61767F7369ui64)) {
						 discard_group(gen);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 14:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x737F6465776F6C6Cui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x65746174ui32 && (*(cur.start + 13) | 0x20) == 0x73)) {
							 cobj.allowed_states = read_cb_state_trigger(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6363617F6F707F6Eui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x65747065ui32 && (*(cur.start + 13) | 0x20) == 0x64)) {
							 cobj.on_po_accepted = read_cb_nation_effect(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 default:
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 17:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x7F6465776F6C6C61ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x656972746E756F63ui64 && (*(cur.start + 16) | 0x20) == 0x73)) {
						 cobj.allowed_countries = read_cb_nation_trigger(gen, err, context);
					 } else {
						 err.unhandled_group_key(cur); discard_group(gen);
					 }
					 break;
				 case 24:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6C:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x6C:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 4))) {
					 case 0x77:
					 switch(0x20 | int32_t(*(cur.start + 5))) {
					 case 0x65:
					 switch(0x20 | int32_t(*(cur.start + 6))) {
					 case 0x64:
					 switch(0x20 | int32_t(*(cur.start + 7))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 8))) {
					 case 0x73:
					 switch(0x20 | int32_t(*(cur.start + 9))) {
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 10) | 0x2020202020202020ui64) == 0x7F6E697F73657461ui64 && (*(uint32_t*)(cur.start + 18) | 0x20202020ui32) == 0x73697263ui32 && (*(uint16_t*)(cur.start + 22) | 0x2020) == 0x7369)) {
							 cobj.allowed_states_in_crisis = read_cb_state_trigger(gen, err, context);
						 } else {
							 err.unhandled_group_key(cur); discard_group(gen);
						 }
						 break;
					 case 0x75:
						 if((true && (*(uint64_t*)(cur.start + 10) | 0x2020202020202020ui64) == 0x727F657461747362ui64 && (*(uint32_t*)(cur.start + 18) | 0x20202020ui32) == 0x6F696765ui32 && (*(uint16_t*)(cur.start + 22) | 0x2020) == 0x736E)) {
							 cobj.allowed_substate_regions = read_cb_state_trigger(gen, err, context);
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
						 break;
					 default:
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
						 break;
					 default:
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
						 break;
					 default:
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
				 case 6:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x61:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x7961776Cui32 && (*(cur.start + 5) | 0x20) == 0x73)) {
							 cobj.set_always(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x69736972ui32 && (*(cur.start + 5) | 0x20) == 0x73)) {
							 cobj.set_crisis(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6F:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x7368746Eui32)) {
							 cobj.months = token_to<uint8_t>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x75:
						 if((true && (*(uint32_t*)(cur.start + 2) | 0x20202020ui32) == 0x6C617574ui32)) {
							 
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
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x70:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6E617F6Fui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x656E && (*(cur.start + 7) | 0x20) == 0x78)) {
							 cobj.set_po_annex(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x77:
						 if((true && (*(uint32_t*)(cur.start + 1) | 0x20202020ui32) == 0x6E7F7261ui32 && (*(uint16_t*)(cur.start + 5) | 0x2020) == 0x6D61 && (*(cur.start + 7) | 0x20) == 0x65)) {
							 cobj.set_war_name(assoc_type, token_to<token_and_type>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 9:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6E6F6C6F637F6F70ui64 && (*(cur.start + 8) | 0x20) == 0x79)) {
						 cobj.set_po_colony(assoc_type, token_to<bool>(rh_token, err), err, context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 10:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6F626E75677F6F70ui64 && (*(uint16_t*)(cur.start + 8) | 0x2020) == 0x7461)) {
						 cobj.set_po_gunboat(assoc_type, token_to<bool>(rh_token, err), err, context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 12:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7F6C697669637F73ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6177 && (*(cur.start + 11) | 0x20) == 0x72)) {
							 cobj.set_is_civil_war(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x73:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6E697F6574697270ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6564 && (*(cur.start + 11) | 0x20) == 0x78)) {
							 cobj.sprite_index = token_to<uint8_t>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6E6F6D7F65637572ui64 && (*(uint16_t*)(cur.start + 9) | 0x2020) == 0x6874 && (*(cur.start + 11) | 0x20) == 0x73)) {
							 cobj.truce_months = token_to<uint8_t>(rh_token, err);
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
					 case 0x62:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x61667F796F626461ui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x726F7463ui32)) {
							 cobj.badboy_factor = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7375746174737F6Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x6F75717Fui32)) {
							 cobj.set_po_status_quo(assoc_type, token_to<bool>(rh_token, err), err, context);
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
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x61667F79746C616Eui64 && (*(uint32_t*)(cur.start + 10) | 0x20202020ui32) == 0x726F7463ui32)) {
							 cobj.penalty_factor = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x64:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x656D616D72617369ui64 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x746E)) {
							 cobj.set_po_disarmament(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6D:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x707075707F656B61ui64 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x7465)) {
							 cobj.set_po_make_puppet(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x6F69746172617065ui64 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x736E)) {
							 cobj.set_po_reparations(assoc_type, token_to<bool>(rh_token, err), err, context);
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
				 case 15:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7463757274736E6Fui64 && (*(uint32_t*)(cur.start + 9) | 0x20202020ui32) == 0x7F676E69ui32 && (*(uint16_t*)(cur.start + 13) | 0x2020) == 0x6263)) {
							 cobj.set_constructing_cb(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x64:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x74737F646E616D65ui64 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x7461 && (*(cur.start + 14) | 0x20) == 0x65)) {
							 cobj.set_po_demand_state(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x72:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x6F637F65766F6D65ui64 && (*(uint16_t*)(cur.start + 12) | 0x2020) == 0x6572 && (*(cur.start + 14) | 0x20) == 0x73)) {
							 cobj.set_po_remove_cores(assoc_type, token_to<bool>(rh_token, err), err, context);
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
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x667F656769747365ui64 && (*(uint32_t*)(cur.start + 10) | 0x20202020ui32) == 0x6F746361ui32 && (*(cur.start + 14) | 0x20) == 0x72)) {
							 cobj.prestige_factor = token_to<float>(rh_token, err);
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
				 case 16:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x70737F6F747F6464ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x65726568ui32)) {
							 cobj.set_po_add_to_sphere(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x64:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x667F796F72747365ui64 && (*(uint32_t*)(cur.start + 12) | 0x20202020ui32) == 0x7374726Fui32)) {
							 cobj.set_po_destroy_forts(assoc_type, token_to<bool>(rh_token, err), err, context);
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
				 case 17:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x69:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6567676972747F73ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x796C6E6F7F646572ui64)) {
							 cobj.set_is_triggered_only(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x65:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x74736F637F656361ui64 && (*(uint32_t*)(cur.start + 10) | 0x20202020ui32) == 0x6361667Fui32 && (*(uint16_t*)(cur.start + 14) | 0x2020) == 0x6F74 && (*(cur.start + 16) | 0x20) == 0x72)) {
							 cobj.peace_cost_factor = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x6F:
						 if((true && (*(uint64_t*)(cur.start + 2) | 0x2020202020202020ui64) == 0x657361656C65727Fui64 && (*(uint32_t*)(cur.start + 10) | 0x20202020ui32) == 0x7075707Fui32 && (*(uint16_t*)(cur.start + 14) | 0x2020) == 0x6570 && (*(cur.start + 16) | 0x20) == 0x74)) {
							 cobj.set_po_release_puppet(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6C747461627F7377ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x726F746361667F65ui64)) {
							 cobj.tws_battle_factor = token_to<float>(rh_token, err);
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
					 case 0x61:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x776F6C6C617F6C6Cui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x65746174737F6465ui64 && (*(cur.start + 17) | 0x20) == 0x73)) {
							 cobj.set_all_allowed_states(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7463757274736E6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x656570737F6E6F69ui64 && (*(cur.start + 17) | 0x20) == 0x64)) {
							 cobj.construction_speed = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x65766F6D65727F6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x676974736572707Fui64 && (*(cur.start + 17) | 0x20) == 0x65)) {
							 cobj.set_po_remove_prestige(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 20:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x61777F7461657267ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6167696C626F7F72ui64 && (*(uint32_t*)(cur.start + 16) | 0x20202020ui32) == 0x79726F74ui32)) {
						 cobj.set_great_war_obligatory(assoc_type, token_to<bool>(rh_token, err), err, context);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 21:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x70:
					 switch(0x20 | int32_t(*(cur.start + 1))) {
					 case 0x6F:
					 switch(0x20 | int32_t(*(cur.start + 2))) {
					 case 0x7F:
					 switch(0x20 | int32_t(*(cur.start + 3))) {
					 case 0x63:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x696E757F7261656Cui64 && (*(uint64_t*)(cur.start + 12) | 0x2020202020202020ui64) == 0x72656870737F6E6Fui64 && (*(cur.start + 20) | 0x20) == 0x65)) {
							 cobj.set_po_clear_union_sphere(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x74:
						 if((true && (*(uint64_t*)(cur.start + 4) | 0x2020202020202020ui64) == 0x7F726566736E6172ui64 && (*(uint64_t*)(cur.start + 12) | 0x2020202020202020ui64) == 0x65636E69766F7270ui64 && (*(cur.start + 20) | 0x20) == 0x73)) {
							 cobj.set_po_transfer_provinces(assoc_type, token_to<bool>(rh_token, err), err, context);
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
				 case 25:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x72747F6B61657262ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x61666E697F656375ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x6F746361667F796Dui64 && (*(cur.start + 24) | 0x20) == 0x72)) {
						 cobj.break_truce_infamy_factor = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 27:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x62:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x7572747F6B616572ui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x74736572707F6563ui64 && (*(uint64_t*)(cur.start + 17) | 0x2020202020202020ui64) == 0x746361667F656769ui64 && (*(uint16_t*)(cur.start + 25) | 0x2020) == 0x726F)) {
							 cobj.break_truce_prestige_factor = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x67:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x616C65727F646F6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x666E697F6E6F6974ui64 && (*(uint64_t*)(cur.start + 17) | 0x2020202020202020ui64) == 0x746361667F796D61ui64 && (*(uint16_t*)(cur.start + 25) | 0x2020) == 0x726F)) {
							 cobj.good_relation_infamy_factor = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 28:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x72747F6B61657262ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x696C696D7F656375ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x61667F79636E6174ui64 && (*(uint32_t*)(cur.start + 24) | 0x20202020ui32) == 0x726F7463ui32)) {
						 cobj.break_truce_militancy_factor = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 29:
					 switch(0x20 | int32_t(*(cur.start + 0))) {
					 case 0x67:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x616C65727F646F6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6572707F6E6F6974ui64 && (*(uint64_t*)(cur.start + 17) | 0x2020202020202020ui64) == 0x61667F6567697473ui64 && (*(uint32_t*)(cur.start + 25) | 0x20202020ui32) == 0x726F7463ui32)) {
							 cobj.good_relation_prestige_factor = token_to<float>(rh_token, err);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 case 0x70:
						 if((true && (*(uint64_t*)(cur.start + 1) | 0x2020202020202020ui64) == 0x6C6174736E697F6Fui64 && (*(uint64_t*)(cur.start + 9) | 0x2020202020202020ui64) == 0x6E756D6D6F637F6Cui64 && (*(uint64_t*)(cur.start + 17) | 0x2020202020202020ui64) == 0x7F766F677F747369ui64 && (*(uint32_t*)(cur.start + 25) | 0x20202020ui32) == 0x65707974ui32)) {
							 cobj.set_po_install_communist_gov_type(assoc_type, token_to<bool>(rh_token, err), err, context);
						 } else {
							 err.unhandled_association_key(cur);
						 }
						 break;
					 default:
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 30:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x6C65727F646F6F67ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x696D7F6E6F697461ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x7F79636E6174696Cui64 && (*(uint32_t*)(cur.start + 24) | 0x20202020ui32) == 0x74636166ui32 && (*(uint16_t*)(cur.start + 28) | 0x2020) == 0x726F)) {
						 cobj.good_relation_militancy_factor = token_to<float>(rh_token, err);
					 } else {
						 err.unhandled_association_key(cur);
					 }
					 break;
				 case 31:
					 if((true && (*(uint64_t*)(cur.start + 0) | 0x2020202020202020ui64) == 0x736E696E757F6F70ui64 && (*(uint64_t*)(cur.start + 8) | 0x2020202020202020ui64) == 0x6D6F637F6C6C6174ui64 && (*(uint64_t*)(cur.start + 16) | 0x2020202020202020ui64) == 0x677F7473696E756Dui64 && (*(uint32_t*)(cur.start + 24) | 0x20202020ui32) == 0x747F766Fui32 && (*(uint16_t*)(cur.start + 28) | 0x2020) == 0x7079 && (*(cur.start + 30) | 0x20) == 0x65)) {
						 cobj.set_po_uninstall_communist_gov_type(assoc_type, token_to<bool>(rh_token, err), err, context);
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

