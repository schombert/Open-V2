#include "provinces.h"

namespace provinces {
	state_distances_manager::~state_distances_manager() {
		if(distance_data) {
			_aligned_free(distance_data);
			distance_data = nullptr;
		}
	}
}
