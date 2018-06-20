#include "common\\common.h"
#include "scenario.h"
#include "sound\\sound_io.h"

namespace scenario {
	void ready_scenario(scenario_manager& s, const directory& root) {
		auto fonts_directory = root.get_directory(u"\\gfx\\fonts");
		s.gui_m.fonts.load_standard_fonts(fonts_directory);
		s.gui_m.fonts.load_metrics_fonts();
		populate_music(s.sound_m, root);
	}
}
