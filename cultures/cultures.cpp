#include "cultures.h"


namespace cultures {

	tag_as_text encoded_tag_to_text_tag(uint32_t tag_value) {
		union {
			tag_as_text tag;
			uint32_t value;
		} local_u;

		local_u.value = tag_value;
		return local_u.tag;
	}

	
}
