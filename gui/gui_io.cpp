#include "gui_io.h"
#include "text_data\\text_data_io.h"
#include "gui_definitions\\gui_definitions_io.h"
#include "concurrency_tools\\concurrency_tools.hpp"

#ifdef _DEBUG
#include "Windows.h"
#undef min
#undef max
#endif

void ui::load_gui_from_directory(const directory& source_directory, gui_static& manager) {
	auto fonts_directory = source_directory.get_directory(u"\\gfx\\fonts");
	manager.fonts.load_standard_fonts(fonts_directory);

	manager.fonts.load_metrics_fonts();

	manager.textures.load_standard_textures(source_directory);

	auto localisation_directory = source_directory.get_directory(u"\\localisation");
	load_text_sequences_from_directory(localisation_directory, manager.text_data_sequences);

	auto interface_directory = source_directory.get_directory(u"\\interface");

	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;

	graphics::name_maps gobj_nmaps;
	std::vector<std::pair<std::string, graphics::errors>> gobj_errors_generated;

	ui::load_ui_definitions_from_directory(
		interface_directory, manager.nmaps, manager.ui_definitions, errors_generated,
		[&manager](const char* a, const char* b) { return text_data::get_text_handle(manager.text_data_sequences, a, b); },
		[&manager](const char* a, const char* b) { return graphics::pack_font_handle(manager.fonts.find_font(a, b), manager.fonts.is_black(a, b), manager.fonts.find_font_size(a, b)); },
		[&gobj_nmaps](const char* a, const char* b) { return graphics::reserve_graphics_object(gobj_nmaps, a, b); });

#ifdef _DEBUG
	for(auto& e : errors_generated) {
		OutputDebugStringA(e.first.c_str());
		OutputDebugStringA(": ");
		OutputDebugStringA(ui::format_error(e.second));
		OutputDebugStringA("\n");
	}
#endif

	graphics::load_graphics_object_definitions_from_directory(
		interface_directory,
		gobj_nmaps,
		manager.graphics_object_definitions,
		gobj_errors_generated,
		[&manager, &source_directory](const char* a, const char* b) { return manager.textures.retrieve_by_name(source_directory, a, b); });

#ifdef _DEBUG
	for(auto& e : gobj_errors_generated) {
		OutputDebugStringA(e.first.c_str());
		OutputDebugStringA(": ");
		OutputDebugStringA(graphics::format_error(e.second));
		OutputDebugStringA("\n");
	}
#endif
}
