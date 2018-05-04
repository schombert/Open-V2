#include "triggers.h"
#include "codes.h"

namespace triggers {
	int32_t get_payload_size(const uint16_t* data) {
		const auto payload_mask = uint16_t(-int16_t((trigger_codes::no_payload & data[0]) == 0));
		return payload_mask & data[1];
	}
	int32_t trigger_scope_data_payload(uint16_t code) {
		if (((code & trigger_codes::code_mask) == trigger_codes::x_provinces_in_variable_region) |
			((code & trigger_codes::code_mask) == trigger_codes::tag_scope) |
			((code & trigger_codes::code_mask) == trigger_codes::integer_scope))
			return 1;
		return 0;
	}
	void add_float_to_payload(std::vector<uint16_t>& v, float f) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			float f;
		} pack_float;
		pack_float.f = f;

		v.push_back(pack_float.v.low);
		v.push_back(pack_float.v.high);
	}
	float read_float_from_payload(const uint16_t* data) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			float f;
		} pack_float;

		pack_float.v.low = data[0];
		pack_float.v.high = data[1];

		return pack_float.f;
	}

	void add_int32_t_to_payload(std::vector<uint16_t>& v, int32_t i) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			int32_t i;
		} pack_int;
		pack_int.i = i;

		v.push_back(pack_int.v.low);
		v.push_back(pack_int.v.high);
	}
	int32_t read_int32_t_from_payload(const uint16_t* data) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			int32_t i;
		} pack_int;
		
		pack_int.v.low = data[0];
		pack_int.v.high = data[1];

		return pack_int.i;
	}

	void add_option_identifier_to_payload(std::vector<uint16_t>& v, issues::option_identifier i) {
		v.push_back(trigger_payload(i).value);
		v.push_back(trigger_payload(i.option_id).value);
	}
	issues::option_identifier read_option_identifier_from_payload(const uint16_t* data) {
		trigger_payload low(data[0]);
		trigger_payload high(data[1]);
		issues::option_identifier result;

		if (issues::is_unciv_issue(low.generic_issue.group))
			result.id = low.generic_issue.values.unciv_issue;
		else
			result.id = low.generic_issue.values.civ_issue;
		result.type = low.generic_issue.group;
		result.option_id = high.small.values.option;

		return result;
	}
}
