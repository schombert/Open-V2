#include "cultures.h"


namespace cultures {
	uint32_t tag_to_encoding(const char* start, const char* end) {
		union {
			tag_as_text tag;
			uint32_t value;
		} local_u;

		memset(&local_u, 0, sizeof(local_u));
		memcpy(&local_u.tag, start, std::min(4ui64, static_cast<size_t>(end - start)));
		return local_u.value;
	}

	tag_as_text encoded_tag_to_text_tag(uint32_t tag_value) {
		union {
			tag_as_text tag;
			uint32_t value;
		} local_u;

		local_u.value = tag_value;
		return local_u.tag;
	}

	
}
