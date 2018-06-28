#include "common\\common.h"
#include "world_state.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_functions.h"
#include "cultures\\cultures_functions.h"

void ready_world_state(world_state& ws) {
	provinces::init_ready_provinces(ws);
	cultures::init_cultures_state(ws);
}
