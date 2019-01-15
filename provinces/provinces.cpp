#include "provinces.h"

namespace provinces {
	state_distances_manager::~state_distances_manager() {
		if(distance_data) {
			_aligned_free(distance_data - 15);
			distance_data = nullptr;
		}
	}
}
