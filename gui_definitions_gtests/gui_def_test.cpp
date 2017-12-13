#include "pch.h"
#include "gui_definitions\\gui_definitions.cpp"

#define RANGE(x) (x), (x) + ((sizeof(x)) / sizeof((x)[0])) - 1

auto fake_text_handle_lookup() {
	return [i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_font_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_gobj_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_sound_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}

TEST(basic_gui_button, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = bname\n"
		"buttonText = dummy\n"
		"buttonFont = dummy\n"
		"tooltip = \"\"\n"
		"tooltipText = \"\"\n"
		"delayedtooltipText = \"\"\n"
		"quadTextureSprite = dummy\n"
	));

	if (parse_tree.size() > 0)
		container.gui_button(parse_object<guiButtonType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.button_names.size());
	EXPECT_EQ(1, defs.buttons.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("bname"), nmaps.button_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::button_def::clicksound_none, defs.buttons[0].flags & ui::button_def::clicksound_mask);
	EXPECT_EQ(ui::button_def::orientation_center, defs.buttons[0].flags & ui::button_def::orientation_mask);
	EXPECT_EQ(ui::button_def::rotation_upright, defs.buttons[0].flags & ui::button_def::rotation_mask);
	EXPECT_EQ(0, defs.buttons[0].flags & ui::button_def::is_checkbox);
	EXPECT_EQ(1, defs.buttons[0].text_handle);
	EXPECT_EQ(1, defs.buttons[0].font_handle);
	EXPECT_EQ(1, defs.buttons[0].graphical_object_handle);
	EXPECT_EQ(virtual_key::NONE, defs.buttons[0].shortcut);
	EXPECT_EQ(0, defs.buttons[0].position.x);
	EXPECT_EQ(0, defs.buttons[0].position.y);
	EXPECT_EQ(0, defs.buttons[0].size.x);
	EXPECT_EQ(0, defs.buttons[0].size.y);
	EXPECT_EQ(0, defs.buttons[0].flags & ui::button_def::format_left);
}

TEST(non_default_gui_button, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = bname\n"
		"buttonText = dummy\n"
		"buttonFont = dummy\n"
		"clicksound = click\n"
		"tooltip = \"\"\n"
		"rotation = -1.5708\n"
		"orientation = UPPER_LEFT\n"
		"shortcut = m\n"
		"tooltipText = \"\"\n"
		"delayedtooltipText = \"\"\n"
		"spriteType = dummy\n"
		"position = { x = 10 y = 20} \n"
		"format = left\n"
		"size = { x = 30 y = 40} \n"
	));

	if (parse_tree.size() > 0)
		container.gui_button(parse_object<guiButtonType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.button_names.size());
	EXPECT_EQ(1, defs.buttons.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("bname"), nmaps.button_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::button_def::clicksound_click, defs.buttons[0].flags & ui::button_def::clicksound_mask);
	EXPECT_EQ(ui::button_def::orientation_upper_left, defs.buttons[0].flags & ui::button_def::orientation_mask);
	EXPECT_EQ(ui::button_def::rotation_90_left, defs.buttons[0].flags & ui::button_def::rotation_mask);
	EXPECT_EQ(0, defs.buttons[0].flags & ui::button_def::is_checkbox);
	EXPECT_EQ(1, defs.buttons[0].text_handle);
	EXPECT_EQ(1, defs.buttons[0].font_handle);
	EXPECT_EQ(1, defs.buttons[0].graphical_object_handle);
	EXPECT_EQ(virtual_key::M, defs.buttons[0].shortcut);
	EXPECT_EQ(10, defs.buttons[0].position.x);
	EXPECT_EQ(20, defs.buttons[0].position.y);
	EXPECT_EQ(30, defs.buttons[0].size.x);
	EXPECT_EQ(40, defs.buttons[0].size.y);
	EXPECT_EQ(ui::button_def::format_left, defs.buttons[0].flags & ui::button_def::format_left);
}

TEST(checkbox, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = bname\n"
		"buttonText = dummy\n"
		"buttonFont = dummy\n"
		"clicksound = close_window\n"
		"tooltip = \"\"\n"
		"rotation = -1.5708\n"
		"orientation = UPPER_LEFT\n"
		"shortcut = m\n"
		"tooltipText = \"\"\n"
		"delayedtooltipText = \"\"\n"
		"spriteType = dummy\n"
		"position = { x = 10 y = 20} \n"
		"format = left\n"
		"size = { x = 30 y = 40} \n"
	));

	if (parse_tree.size() > 0)
		container.gui_checkboxType(parse_object<guiButtonType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.button_names.size());
	EXPECT_EQ(1, defs.buttons.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("bname"), nmaps.button_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::button_def::clicksound_close_window, defs.buttons[0].flags & ui::button_def::clicksound_mask);
	EXPECT_EQ(ui::button_def::orientation_upper_left, defs.buttons[0].flags & ui::button_def::orientation_mask);
	EXPECT_EQ(ui::button_def::rotation_90_left, defs.buttons[0].flags & ui::button_def::rotation_mask);
	EXPECT_EQ(ui::button_def::is_checkbox, defs.buttons[0].flags & ui::button_def::is_checkbox);
	EXPECT_EQ(1, defs.buttons[0].text_handle);
	EXPECT_EQ(1, defs.buttons[0].font_handle);
	EXPECT_EQ(1, defs.buttons[0].graphical_object_handle);
	EXPECT_EQ(virtual_key::M, defs.buttons[0].shortcut);
	EXPECT_EQ(10, defs.buttons[0].position.x);
	EXPECT_EQ(20, defs.buttons[0].position.y);
	EXPECT_EQ(30, defs.buttons[0].size.x);
	EXPECT_EQ(40, defs.buttons[0].size.y);
	EXPECT_EQ(ui::button_def::format_left, defs.buttons[0].flags & ui::button_def::format_left);
}

TEST(bad_values_gui_button, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = bname\n"
		"buttonText = dummy\n"
		"buttonFont = dummy\n"
		"clicksound = dummy\n"
		"tooltip = \"has_text\"\n"
		"rotation = 1\n"
		"orientation = none\n"
		"shortcut = m\n"
		"tooltipText = \"has text\"\n"
		"delayedtooltipText = \"has text\"\n"
		"spriteType = dummy\n"
		"position = { x = 10 y = 20} \n"
		"format = right\n"
		"size = { x = 30 y = 40} \n"
	));

	if (parse_tree.size() > 0)
		container.gui_button(parse_object<guiButtonType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.button_names.size());
	EXPECT_EQ(1, defs.buttons.size());
	EXPECT_EQ(7, errors_generated.size());
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_button_clicksound), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_tooltip_empty_for_button), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_button_rotation), errors_generated[2]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_button_orientation), errors_generated[3]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_tooltiptext_empty_for_button), errors_generated[4]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_delayedtooltiptext_empty_for_button), errors_generated[5]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_button_format_to_be_left), errors_generated[6]);
}

TEST(basic_gui_icon, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = iname\n"
	));

	if (parse_tree.size() > 0)
		container.gui_iconType(parse_object<iconType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.icon_names.size());
	EXPECT_EQ(1, defs.icons.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::icon_def::orientation_center, defs.icons[0].flags & ui::icon_def::orientation_mask);
	EXPECT_EQ(ui::icon_def::rotation_upright, defs.icons[0].flags & ui::icon_def::rotation_mask);
	EXPECT_EQ(0, defs.icons[0].flags & ui::icon_def::is_shield);


	EXPECT_EQ(0, defs.icons[0].frame);
	EXPECT_EQ(1.0f, defs.icons[0].scale);
	EXPECT_EQ(0, defs.icons[0].graphical_object_handle);
	EXPECT_EQ(0, defs.icons[0].position.x);
	EXPECT_EQ(0, defs.icons[0].position.y);
}

TEST(non_default_gui_icon, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = iname\n"
		"scale = 0.5\n"
		"buttonMesh = dummy\n"
		"position = { x = 5 y = 10}\n"
		"frame = 6\n"
		"rotation = -1.5708\n"
		"orientation = CENTER_UP\n"
	));

	if (parse_tree.size() > 0)
		container.gui_iconType(parse_object<iconType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.icon_names.size());
	EXPECT_EQ(1, defs.icons.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::icon_def::orientation_center_up, defs.icons[0].flags & ui::icon_def::orientation_mask);
	EXPECT_EQ(ui::icon_def::rotation_90_left, defs.icons[0].flags & ui::icon_def::rotation_mask);
	EXPECT_EQ(0, defs.icons[0].flags & ui::icon_def::is_shield);

	EXPECT_EQ(6, defs.icons[0].frame);
	EXPECT_EQ(0.5f, defs.icons[0].scale);
	EXPECT_EQ(1, defs.icons[0].graphical_object_handle);
	EXPECT_EQ(5, defs.icons[0].position.x);
	EXPECT_EQ(10, defs.icons[0].position.y);
}

TEST(shield_gui_icon, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = iname\n"
		"scale = 0.5\n"
		"buttonMesh = dummy\n"
		"position = { x = 5 y = 10}\n"
		"frame = 6\n"
		"rotation = -1.5708\n"
		"orientation = CENTER_UP\n"
	));

	if (parse_tree.size() > 0)
		container.gui_shieldtype(parse_object<iconType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.icon_names.size());
	EXPECT_EQ(1, defs.icons.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::icon_def::orientation_center_up, defs.icons[0].flags & ui::icon_def::orientation_mask);
	EXPECT_EQ(ui::icon_def::rotation_90_left, defs.icons[0].flags & ui::icon_def::rotation_mask);
	EXPECT_EQ(ui::icon_def::is_shield, defs.icons[0].flags & ui::icon_def::is_shield);

	EXPECT_EQ(6, defs.icons[0].frame);
	EXPECT_EQ(0.5f, defs.icons[0].scale);
	EXPECT_EQ(1, defs.icons[0].graphical_object_handle);
	EXPECT_EQ(5, defs.icons[0].position.x);
	EXPECT_EQ(10, defs.icons[0].position.y);
}

TEST(errors_gui_icon, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = iname\n"
		"scale = 0.5\n"
		"buttonMesh = dummy\n"
		"position = { x = 5 y = 10}\n"
		"frame = 6\n"
		"rotation = -7\n"
		"orientation = mmm\n"
		"bad_key = w"
	));

	if (parse_tree.size() > 0)
		container.gui_iconType(parse_object<iconType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.icon_names.size());
	EXPECT_EQ(1, defs.icons.size());
	EXPECT_EQ(3, errors_generated.size());
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0].get_string(defs.name_data));

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_icon_rotation), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_icon_orientation), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_icon_attribute), errors_generated[2]);
}

TEST(basic_gui_text, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = tname\n"
	));

	if (parse_tree.size() > 0)
		container.gui_instantTextBoxType(parse_object<allTextBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.text_names.size());
	EXPECT_EQ(1, defs.text.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("tname"), nmaps.text_names[0].get_string(defs.name_data));
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::always_transparent);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::fixed_size);
	EXPECT_EQ(ui::text_def::instant, defs.text[0].flags & ui::text_def::instant);
	EXPECT_EQ(ui::text_def::background_none_specified, defs.text[0].flags & ui::text_def::background_mask);
	EXPECT_EQ(ui::text_def::orientation_center, defs.text[0].flags & ui::text_def::orientation_mask);
	EXPECT_EQ(ui::text_def::format_left, defs.text[0].flags & ui::text_def::format_mask);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::is_edit_box);


	EXPECT_EQ(0, defs.text[0].font_handle);
	EXPECT_EQ(0, defs.text[0].max_width);
	EXPECT_EQ(0, defs.text[0].max_height);
	EXPECT_EQ(0, defs.text[0].text_handle);
	EXPECT_EQ(0, defs.text[0].position.x);
	EXPECT_EQ(0, defs.text[0].position.y);
	EXPECT_EQ(0, defs.text[0].border_size.x);
	EXPECT_EQ(0, defs.text[0].border_size.y);
}

TEST(non_default_gui_text, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = tname\n"
		"allwaysTransparent = yes\n"
		"fixedSize = yes\n"
		"textureFile = \"gfx\\\\interface\\\\tiles_dialog.tga\"\n"
		"Orientation = CENTER_UP\n"
		"format = justified\n"
		"font = some_font\n"
		"text = dummy\n"
		"borderSize = {x = 1, y = 2}\n"
		"position = {x=10 y=20}\n"
		"maxHeight = 8\n"
		"maxWidth = 16\n"
	));

	if (parse_tree.size() > 0)
		container.gui_textBoxType(parse_object<allTextBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.text_names.size());
	EXPECT_EQ(1, defs.text.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("tname"), nmaps.text_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::text_def::always_transparent, defs.text[0].flags & ui::text_def::always_transparent);
	EXPECT_EQ(ui::text_def::fixed_size, defs.text[0].flags & ui::text_def::fixed_size);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::instant);
	EXPECT_EQ(ui::text_def::background_tiles_dialog_tga, defs.text[0].flags & ui::text_def::background_mask);
	EXPECT_EQ(ui::text_def::orientation_center_up, defs.text[0].flags & ui::text_def::orientation_mask);
	EXPECT_EQ(ui::text_def::format_justified, defs.text[0].flags & ui::text_def::format_mask);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::is_edit_box);

	EXPECT_EQ(1, defs.text[0].font_handle);
	EXPECT_EQ(16, defs.text[0].max_width);
	EXPECT_EQ(8, defs.text[0].max_height);
	EXPECT_EQ(1, defs.text[0].text_handle);
	EXPECT_EQ(10, defs.text[0].position.x);
	EXPECT_EQ(20, defs.text[0].position.y);
	EXPECT_EQ(1, defs.text[0].border_size.x);
	EXPECT_EQ(2, defs.text[0].border_size.y);
}

TEST(size_gui_text, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = tname\n"
		"allwaysTransparent = yes\n"
		"fixedSize = yes\n"
		"textureFile = \"gfx\\\\interface\\\\tiles_dialog.tga\"\n"
		"Orientation = CENTER_UP\n"
		"format = justified\n"
		"font = some_font\n"
		"text = dummy\n"
		"borderSize = {x = 1, y = 2}\n"
		"position = {x=10 y=20}\n"
		"size = {x = 30 y = 40}\n"
	));

	if (parse_tree.size() > 0)
		container.gui_textBoxType(parse_object<allTextBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.text_names.size());
	EXPECT_EQ(1, defs.text.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("tname"), nmaps.text_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::text_def::always_transparent, defs.text[0].flags & ui::text_def::always_transparent);
	EXPECT_EQ(ui::text_def::fixed_size, defs.text[0].flags & ui::text_def::fixed_size);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::instant);
	EXPECT_EQ(ui::text_def::background_tiles_dialog_tga, defs.text[0].flags & ui::text_def::background_mask);
	EXPECT_EQ(ui::text_def::orientation_center_up, defs.text[0].flags & ui::text_def::orientation_mask);
	EXPECT_EQ(ui::text_def::format_justified, defs.text[0].flags & ui::text_def::format_mask);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::is_edit_box);

	EXPECT_EQ(1, defs.text[0].font_handle);
	EXPECT_EQ(30, defs.text[0].max_width);
	EXPECT_EQ(40, defs.text[0].max_height);
	EXPECT_EQ(1, defs.text[0].text_handle);
	EXPECT_EQ(10, defs.text[0].position.x);
	EXPECT_EQ(20, defs.text[0].position.y);
	EXPECT_EQ(1, defs.text[0].border_size.x);
	EXPECT_EQ(2, defs.text[0].border_size.y);
}

TEST(edit_gui_text, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = tname\n"
		"allwaysTransparent = yes\n"
		"fixedSize = yes\n"
		"textureFile = \"gfx\\\\interface\\\\small_tiles_dialog.dds\"\n"
		"Orientation = CENTER_DOWN\n"
		"format = justified\n"
		"font = some_font\n"
		"text = dummy\n"
		"borderSize = {x = 1, y = 2}\n"
		"position = {x=10 y=20}\n"
		"maxHeight = 8\n"
		"maxWidth = 16\n"
	));

	if (parse_tree.size() > 0)
		container.gui_editBoxType(parse_object<allTextBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.text_names.size());
	EXPECT_EQ(1, defs.text.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("tname"), nmaps.text_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::text_def::always_transparent, defs.text[0].flags & ui::text_def::always_transparent);
	EXPECT_EQ(ui::text_def::fixed_size, defs.text[0].flags & ui::text_def::fixed_size);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::instant);
	EXPECT_EQ(ui::text_def::background_small_tiles_dialog_tga, defs.text[0].flags & ui::text_def::background_mask);
	EXPECT_EQ(ui::text_def::orientation_center_down, defs.text[0].flags & ui::text_def::orientation_mask);
	EXPECT_EQ(ui::text_def::format_justified, defs.text[0].flags & ui::text_def::format_mask);
	EXPECT_EQ(ui::text_def::is_edit_box, defs.text[0].flags & ui::text_def::is_edit_box);

	EXPECT_EQ(1, defs.text[0].font_handle);
	EXPECT_EQ(16, defs.text[0].max_width);
	EXPECT_EQ(8, defs.text[0].max_height);
	EXPECT_EQ(1, defs.text[0].text_handle);
	EXPECT_EQ(10, defs.text[0].position.x);
	EXPECT_EQ(20, defs.text[0].position.y);
	EXPECT_EQ(1, defs.text[0].border_size.x);
	EXPECT_EQ(2, defs.text[0].border_size.y);
}

TEST(errors_gui_text, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = tname\n"
		"allwaysTransparent = yes\n"
		"fixedSize = yes\n"
		"textureFile = \"gfx\\interface\\other.tga\"\n"
		"Orientation = CENTER_OTHER\n"
		"format = none\n"
		"font = some_font\n"
		"text = dummy\n"
		"borderSize = {x = 1, y = 2}\n"
		"position = {x=10 y=20}\n"
		"maxHeight = 8\n"
		"maxWidth = 16\n"
		"badkey"
	));

	if (parse_tree.size() > 0)
		container.gui_textBoxType(parse_object<allTextBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.text_names.size());
	EXPECT_EQ(1, defs.text.size());
	EXPECT_EQ(4, errors_generated.size());

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_text_orientation), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_text_format), errors_generated[2]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_text_background), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_text_attribute), errors_generated[3]);

	EXPECT_EQ(std::string("tname"), nmaps.text_names[0].get_string(defs.name_data));
	EXPECT_EQ(ui::text_def::always_transparent, defs.text[0].flags & ui::text_def::always_transparent);
	EXPECT_EQ(ui::text_def::fixed_size, defs.text[0].flags & ui::text_def::fixed_size);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::instant);
	EXPECT_EQ(ui::text_def::orientation_center, defs.text[0].flags & ui::text_def::orientation_mask);
	EXPECT_EQ(ui::text_def::format_left, defs.text[0].flags & ui::text_def::format_mask);
	EXPECT_EQ(ui::text_def::background_none_specified, defs.text[0].flags & ui::text_def::background_mask);
	EXPECT_EQ(0, defs.text[0].flags & ui::text_def::is_edit_box);

	EXPECT_EQ(1, defs.text[0].font_handle);
	EXPECT_EQ(16, defs.text[0].max_width);
	EXPECT_EQ(8, defs.text[0].max_height);
	EXPECT_EQ(1, defs.text[0].text_handle);
	EXPECT_EQ(10, defs.text[0].position.x);
	EXPECT_EQ(20, defs.text[0].position.y);
	EXPECT_EQ(1, defs.text[0].border_size.x);
	EXPECT_EQ(2, defs.text[0].border_size.y);
}

TEST(basic_gui_position, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = pname\n"
		"position = {x = 20 y = 10}"
	));

	if (parse_tree.size() > 0)
		container.gui_positionType(parse_object<positionType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.position_names.size());
	EXPECT_EQ(1, defs.positions.size());
	EXPECT_EQ(0, errors_generated.size());
	EXPECT_EQ(std::string("pname"), nmaps.position_names[0].get_string(defs.name_data));
	EXPECT_EQ(20, defs.positions[0].position.x);
	EXPECT_EQ(10, defs.positions[0].position.y);
}

TEST(errors_gui_position, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = pname\n"
		"badkey\n"
		"position = {x = 20 y = 10}"
	));

	if (parse_tree.size() > 0)
		container.gui_positionType(parse_object<positionType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.position_names.size());
	EXPECT_EQ(1, defs.positions.size());
	EXPECT_EQ(1, errors_generated.size());
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_position_attribute), errors_generated[0]);
	EXPECT_EQ(std::string("pname"), nmaps.position_names[0].get_string(defs.name_data));
	EXPECT_EQ(20, defs.positions[0].position.x);
	EXPECT_EQ(10, defs.positions[0].position.y);
}

TEST(basic_overlapping_region, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = oname\n"
	));

	if (parse_tree.size() > 0)
		container.gui_OverlappingElementsBoxType(parse_object<OverlappingElementsBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.overlapping_region_names.size());
	EXPECT_EQ(1, defs.overlapping_regions.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("oname"), nmaps.overlapping_region_names[0].get_string(defs.name_data));
	EXPECT_EQ(0, defs.overlapping_regions[0].position.x);
	EXPECT_EQ(0, defs.overlapping_regions[0].position.y);
	EXPECT_EQ(0, defs.overlapping_regions[0].size.x);
	EXPECT_EQ(0, defs.overlapping_regions[0].size.y);
	EXPECT_EQ(0.0f, defs.overlapping_regions[0].spacing);
	EXPECT_EQ(ui::overlapping_region_def::orientation_center, defs.overlapping_regions[0].flags & ui::overlapping_region_def::orientation_mask);
	EXPECT_EQ(ui::overlapping_region_def::format_center, defs.overlapping_regions[0].flags & ui::overlapping_region_def::format_mask);
}

TEST(non_default_overlapping_region, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = oname\n"
		"position = {x=10, y=20}\n"
		"size = {x=4 y=8}\n"
		"orientation = UPPER_LEFT\n"
		"format = left\n"
		"spacing =0.5\n"
	));

	if (parse_tree.size() > 0)
		container.gui_OverlappingElementsBoxType(parse_object<OverlappingElementsBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.overlapping_region_names.size());
	EXPECT_EQ(1, defs.overlapping_regions.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("oname"), nmaps.overlapping_region_names[0].get_string(defs.name_data));
	EXPECT_EQ(10, defs.overlapping_regions[0].position.x);
	EXPECT_EQ(20, defs.overlapping_regions[0].position.y);
	EXPECT_EQ(4, defs.overlapping_regions[0].size.x);
	EXPECT_EQ(8, defs.overlapping_regions[0].size.y);
	EXPECT_EQ(0.5f, defs.overlapping_regions[0].spacing);
	EXPECT_EQ(ui::overlapping_region_def::orientation_upper_left, defs.overlapping_regions[0].flags & ui::overlapping_region_def::orientation_mask);
	EXPECT_EQ(ui::overlapping_region_def::format_left, defs.overlapping_regions[0].flags & ui::overlapping_region_def::format_mask);
}

TEST(errors_overlapping_region, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = oname\n"
		"position = {x=10, y=20}\n"
		"size = {x=4 y=8}\n"
		"orientation = CENTER_DOWN\n"
		"format = justified\n"
		"spacing =0.5\n"
		"badkey"
	));

	if (parse_tree.size() > 0)
		container.gui_OverlappingElementsBoxType(parse_object<OverlappingElementsBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.overlapping_region_names.size());
	EXPECT_EQ(1, defs.overlapping_regions.size());
	EXPECT_EQ(3, errors_generated.size());

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_overlapping_region_orientation), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_overlapping_region_format), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_overlapping_region_attribute), errors_generated[2]);

	EXPECT_EQ(std::string("oname"), nmaps.overlapping_region_names[0].get_string(defs.name_data));
	EXPECT_EQ(10, defs.overlapping_regions[0].position.x);
	EXPECT_EQ(20, defs.overlapping_regions[0].position.y);
	EXPECT_EQ(4, defs.overlapping_regions[0].size.x);
	EXPECT_EQ(8, defs.overlapping_regions[0].size.y);
	EXPECT_EQ(0.5f, defs.overlapping_regions[0].spacing);
	EXPECT_EQ(ui::overlapping_region_def::orientation_center, defs.overlapping_regions[0].flags & ui::overlapping_region_def::orientation_mask);
	EXPECT_EQ(ui::overlapping_region_def::format_center, defs.overlapping_regions[0].flags & ui::overlapping_region_def::format_mask);
}

TEST(basic_listbox, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = lname\n"
		"spacing = 0\n"
	));

	if (parse_tree.size() > 0)
		container.gui_listBoxType(parse_object<listBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.listbox_names.size());
	EXPECT_EQ(1, defs.listboxes.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("lname"), nmaps.listbox_names[0].get_string(defs.name_data));
	EXPECT_EQ(0, defs.listboxes[0].position.x);
	EXPECT_EQ(0, defs.listboxes[0].position.y);
	EXPECT_EQ(0, defs.listboxes[0].size.x);
	EXPECT_EQ(0, defs.listboxes[0].size.y);
	EXPECT_EQ(0, defs.listboxes[0].flags & ui::listbox_def::always_transparent);
	EXPECT_EQ(ui::listbox_def::orientation_center, defs.listboxes[0].flags & ui::listbox_def::orientation_mask);
	EXPECT_EQ(0, defs.listboxes[0].flags & ui::listbox_def::spacing_mask);
	EXPECT_EQ(0, defs.listboxes[0].border_size.x);
	EXPECT_EQ(0, defs.listboxes[0].border_size.y);
	EXPECT_EQ(0, defs.listboxes[0].offset.x);
	EXPECT_EQ(0, defs.listboxes[0].offset.y);
	EXPECT_EQ(0, defs.listboxes[0].background_handle);
}

TEST(non_default_listbox, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = lname\n"
		"orientation = upper_right\n"
		"position = {x=15 y=20}\n"
		"size = {x=10 y=11}\n"
		"allwaystransparent = yes\n"
		"spacing = 5\n"
		"scrollbartype = \"standardlistbox_slider\"\n"
		"backGround=\"dummy\"\n"
		"priority = 100\n"
		"step = 0\n"
		"bordersize = {x=1 y=2}\n"
		"offset = {x=3 y=4}\n"
		"horizontal = no\n"
	));

	if (parse_tree.size() > 0)
		container.gui_listBoxType(parse_object<listBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.listbox_names.size());
	EXPECT_EQ(1, defs.listboxes.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("lname"), nmaps.listbox_names[0].get_string(defs.name_data));
	EXPECT_EQ(15, defs.listboxes[0].position.x);
	EXPECT_EQ(20, defs.listboxes[0].position.y);
	EXPECT_EQ(10, defs.listboxes[0].size.x);
	EXPECT_EQ(11, defs.listboxes[0].size.y);
	EXPECT_EQ(ui::listbox_def::always_transparent, defs.listboxes[0].flags & ui::listbox_def::always_transparent);
	EXPECT_EQ(ui::listbox_def::orientation_upper_right, defs.listboxes[0].flags & ui::listbox_def::orientation_mask);
	EXPECT_EQ(5, defs.listboxes[0].flags & ui::listbox_def::spacing_mask);
	EXPECT_EQ(1, defs.listboxes[0].border_size.x);
	EXPECT_EQ(2, defs.listboxes[0].border_size.y);
	EXPECT_EQ(3, defs.listboxes[0].offset.x);
	EXPECT_EQ(4, defs.listboxes[0].offset.y);
	EXPECT_EQ(1, defs.listboxes[0].background_handle);
}

TEST(errors_listbox, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = lname\n"
		"orientation = unk\n"
		"position = {x=15 y=20}\n"
		"size = {x=10 y=11}\n"
		"allwaystransparent = yes\n"
		"spacing = 500\n"
		"scrollbartype = \"bad type\"\n"
		"backGround=\"dummy\"\n"
		"priority = 101\n"
		"step = 1\n"
		"bordersize = {x=1 y=2}\n"
		"offset = {x=3 y=4}\n"
		"horizontal = yes\n"
		"badattribute\n"
	));

	if (parse_tree.size() > 0)
		container.gui_listBoxType(parse_object<listBoxType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.listbox_names.size());
	EXPECT_EQ(1, defs.listboxes.size());
	EXPECT_EQ(7, errors_generated.size());

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_listbox_orientation), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_listbox_spacing_value), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unsupported_listbox_scrollbar_type), errors_generated[2]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_listbox_priority), errors_generated[3]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_listbox_step_value), errors_generated[4]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::horizontal_listboxes_not_supported), errors_generated[5]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_listbox_attribute), errors_generated[6]);

	EXPECT_EQ(std::string("lname"), nmaps.listbox_names[0].get_string(defs.name_data));
	EXPECT_EQ(15, defs.listboxes[0].position.x);
	EXPECT_EQ(20, defs.listboxes[0].position.y);
	EXPECT_EQ(10, defs.listboxes[0].size.x);
	EXPECT_EQ(11, defs.listboxes[0].size.y);
	EXPECT_EQ(ui::listbox_def::always_transparent, defs.listboxes[0].flags & ui::listbox_def::always_transparent);
	EXPECT_EQ(ui::listbox_def::orientation_center, defs.listboxes[0].flags & ui::listbox_def::orientation_mask);
	EXPECT_EQ(0, defs.listboxes[0].flags & ui::listbox_def::spacing_mask);
	EXPECT_EQ(1, defs.listboxes[0].border_size.x);
	EXPECT_EQ(2, defs.listboxes[0].border_size.y);
	EXPECT_EQ(3, defs.listboxes[0].offset.x);
	EXPECT_EQ(4, defs.listboxes[0].offset.y);
	EXPECT_EQ(1, defs.listboxes[0].background_handle);
}

TEST(basic_scrollbar, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = sname\n"
		"rightbutton = b\n"
		"leftbutton = a\n"
		"track = c\n"
		"slider = d\n"
		"guiButtonType = { name = a }\n"
		"guiButtonType = { name = b }\n"
		"guiButtonType = { name = c }\n"
		"guiButtonType = { name = d }\n"
	));

	if (parse_tree.size() > 0)
		container.gui_scrollbarType(parse_object<scrollbarType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.scrollbar_names.size());
	EXPECT_EQ(1, defs.scrollbars.size());
	EXPECT_EQ(4, defs.buttons.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("sname"), nmaps.scrollbar_names[0].get_string(defs.name_data));
	EXPECT_EQ(0, defs.scrollbars[0].position.x);
	EXPECT_EQ(0, defs.scrollbars[0].position.y);
	EXPECT_EQ(0, defs.scrollbars[0].size.x);
	EXPECT_EQ(0, defs.scrollbars[0].size.y);
	EXPECT_EQ(0, defs.scrollbars[0].border_size.x);
	EXPECT_EQ(0, defs.scrollbars[0].border_size.y);
	EXPECT_EQ(1, defs.scrollbars[0].max_value);
	EXPECT_EQ(0, defs.scrollbars[0].flags & ui::scrollbar_def::is_horizontal);
	EXPECT_EQ(0, defs.scrollbars[0].flags & ui::scrollbar_def::has_range_limit);
	EXPECT_EQ(0, defs.scrollbars[0].flags & ui::scrollbar_def::is_lockable);
	EXPECT_EQ(ui::scrollbar_def::step_one, defs.scrollbars[0].flags & ui::scrollbar_def::step_mask);
	EXPECT_EQ(2, defs.scrollbars[0].maximum_button);
	EXPECT_EQ(1, defs.scrollbars[0].minimum_button);
	EXPECT_EQ(3, defs.scrollbars[0].track);
	EXPECT_EQ(4, defs.scrollbars[0].slider);
	EXPECT_EQ(0, defs.scrollbars[0].maximum_limit_icon);
	EXPECT_EQ(0, defs.scrollbars[0].minimum_limit_icon);
}

TEST(nondefault_scrollbar, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = sname\n"
		"rightbutton = b\n"
		"leftbutton = a\n"
		"track = c\n"
		"slider = d\n"
		"rangelimitmaxicon = b\n"
		"rangelimitminicon = a\n"
		"horizontal = 1\n"
		"lockable = yes\n"
		"position = {x=10 y = 20}\n"
		"size = {x = 5 y = 6}\n"
		"priority = 100\n"
		"rangelimitmax = 100\n"
		"rangelimitmin = 0\n"
		"minvalue = 0\n"
		"maxvalue = 100\n"
		"startvalue = 0\n"
		"stepsize = 0.01\n"
		"userangelimit = yes\n"
		"borderSize = {x = 9 y = 11}\n"
		"iconType = { name = a }\n"
		"iconType = { name = b }\n"
		"guiButtonType = { name = a }\n"
		"guiButtonType = { name = b }\n"
		"guiButtonType = { name = c }\n"
		"guiButtonType = { name = d }\n"
	));

	if (parse_tree.size() > 0)
		container.gui_scrollbarType(parse_object<scrollbarType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.scrollbar_names.size());
	EXPECT_EQ(1, defs.scrollbars.size());
	EXPECT_EQ(4, defs.buttons.size());
	EXPECT_EQ(2, defs.icons.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("sname"), nmaps.scrollbar_names[0].get_string(defs.name_data));
	EXPECT_EQ(10, defs.scrollbars[0].position.x);
	EXPECT_EQ(20, defs.scrollbars[0].position.y);
	EXPECT_EQ(5, defs.scrollbars[0].size.x);
	EXPECT_EQ(6, defs.scrollbars[0].size.y);
	EXPECT_EQ(9, defs.scrollbars[0].border_size.x);
	EXPECT_EQ(11, defs.scrollbars[0].border_size.y);
	EXPECT_EQ(100, defs.scrollbars[0].max_value);
	EXPECT_EQ(ui::scrollbar_def::is_horizontal, defs.scrollbars[0].flags & ui::scrollbar_def::is_horizontal);
	EXPECT_EQ(ui::scrollbar_def::is_lockable, defs.scrollbars[0].flags & ui::scrollbar_def::is_lockable);
	EXPECT_EQ(ui::scrollbar_def::has_range_limit, defs.scrollbars[0].flags & ui::scrollbar_def::has_range_limit);
	EXPECT_EQ(ui::scrollbar_def::step_one_hundredth, defs.scrollbars[0].flags & ui::scrollbar_def::step_mask);
	EXPECT_EQ(2, defs.scrollbars[0].maximum_button);
	EXPECT_EQ(1, defs.scrollbars[0].minimum_button);
	EXPECT_EQ(3, defs.scrollbars[0].track);
	EXPECT_EQ(4, defs.scrollbars[0].slider);
	EXPECT_EQ(2, defs.scrollbars[0].maximum_limit_icon);
	EXPECT_EQ(1, defs.scrollbars[0].minimum_limit_icon);
}

TEST(errors_scrollbar, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = sname\n"
		"rightbutton = b\n"
		"track = c\n"
		"slider = d\n"
		"rangelimitmaxicon = b\n"
		"rangelimitminicon = c\n"
		"horizontal = 2\n"
		"lockable = yes\n"
		"position = {x=10 y = 20}\n"
		"size = {x = 5 y = 6}\n"
		"priority = 10\n"
		"rangelimitmax = 100\n"
		"rangelimitmin = 0\n"
		"startvalue = 0\n"
		"minvalue = 1\n"
		"stepsize = 0.5\n"
		"userangelimit = yes\n"
		"borderSize = {x = 9 y = 11}\n"
		"iconType = { name = a }\n"
		"iconType = { name = b }\n"
		"guiButtonType = { name = a }\n"
		"guiButtonType = { name = b }\n"
		"guiButtonType = { name = c }\n"
		"guiButtonType = { name = d }\n"
		"badattribute\n"
	));

	if (parse_tree.size() > 0)
		container.gui_scrollbarType(parse_object<scrollbarType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.scrollbar_names.size());
	EXPECT_EQ(1, defs.scrollbars.size());
	EXPECT_EQ(4, defs.buttons.size());
	EXPECT_EQ(2, defs.icons.size());
	EXPECT_EQ(7, errors_generated.size());

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_scrollbar_horizontal_value), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_scrollbar_priority), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_scrollbar_minimum_value), errors_generated[2]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_scrollbar_step_size), errors_generated[3]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_scrollbar_attribute), errors_generated[4]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::missing_necessary_scrollbar_component), errors_generated[5]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::scrollbar_component_not_found), errors_generated[6]);

	EXPECT_EQ(std::string("sname"), nmaps.scrollbar_names[0].get_string(defs.name_data));
	EXPECT_EQ(10, defs.scrollbars[0].position.x);
	EXPECT_EQ(20, defs.scrollbars[0].position.y);
	EXPECT_EQ(5, defs.scrollbars[0].size.x);
	EXPECT_EQ(6, defs.scrollbars[0].size.y);
	EXPECT_EQ(9, defs.scrollbars[0].border_size.x);
	EXPECT_EQ(11, defs.scrollbars[0].border_size.y);
	EXPECT_EQ(1, defs.scrollbars[0].max_value);
	EXPECT_EQ(0, defs.scrollbars[0].flags & ui::scrollbar_def::is_horizontal);
	EXPECT_EQ(ui::scrollbar_def::is_lockable, defs.scrollbars[0].flags & ui::scrollbar_def::is_lockable);
	EXPECT_EQ(ui::scrollbar_def::has_range_limit, defs.scrollbars[0].flags & ui::scrollbar_def::has_range_limit);
	EXPECT_EQ(ui::scrollbar_def::step_one, defs.scrollbars[0].flags & ui::scrollbar_def::step_mask);
	EXPECT_EQ(2, defs.scrollbars[0].maximum_button);
	EXPECT_EQ(0, defs.scrollbars[0].minimum_button);
	EXPECT_EQ(3, defs.scrollbars[0].track);
	EXPECT_EQ(4, defs.scrollbars[0].slider);
	EXPECT_EQ(2, defs.scrollbars[0].maximum_limit_icon);
	EXPECT_EQ(0, defs.scrollbars[0].minimum_limit_icon);
}

TEST(basic_window, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = wname\n"
	));

	if (parse_tree.size() > 0)
		container.gui_windowType(parse_object<windowType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.window_names.size());
	EXPECT_EQ(1, defs.windows.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("wname"), nmaps.window_names[0].get_string(defs.name_data));
	EXPECT_EQ(0, defs.windows[0].position.x);
	EXPECT_EQ(0, defs.windows[0].position.y);
	EXPECT_EQ(0, defs.windows[0].size.x);
	EXPECT_EQ(0, defs.windows[0].size.y);
	EXPECT_EQ(0, defs.windows[0].flags & ui::window_def::is_dialog);
	EXPECT_EQ(0, defs.windows[0].flags & ui::window_def::is_fullscreen);
	EXPECT_EQ(0, defs.windows[0].flags & ui::window_def::is_moveable);
	EXPECT_EQ(ui::window_def::orientation_center, defs.windows[0].flags & ui::window_def::orientation_mask);
	EXPECT_EQ(0, defs.windows[0].background_handle);
	EXPECT_EQ(0, defs.windows[0].sub_object_definitions.size());
}


TEST(non_default_window, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = wname\n"
		"size = {x=1 y=2}\n"
		"position = {x=4 y=5}\n"
		"fullscreen = yes\n"
		"moveable = 1\n"
		"Orientation = UPPER_LEFT\n"
		"guiButtonType = { name = x }\n"
		"background = x\n"
		"listBoxType = { name = l }\n"
	));

	if (parse_tree.size() > 0)
		container.gui_eu3dialogtype(parse_object<windowType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.window_names.size());
	EXPECT_EQ(1, defs.windows.size());
	EXPECT_EQ(1, defs.buttons.size());
	EXPECT_EQ(1, defs.listboxes.size());
	EXPECT_EQ(0, errors_generated.size());

	EXPECT_EQ(std::string("wname"), nmaps.window_names[0].get_string(defs.name_data));
	EXPECT_EQ(4, defs.windows[0].position.x);
	EXPECT_EQ(5, defs.windows[0].position.y);
	EXPECT_EQ(1, defs.windows[0].size.x);
	EXPECT_EQ(2, defs.windows[0].size.y);
	EXPECT_EQ(ui::window_def::is_dialog, defs.windows[0].flags & ui::window_def::is_dialog);
	EXPECT_EQ(ui::window_def::is_fullscreen, defs.windows[0].flags & ui::window_def::is_fullscreen);
	EXPECT_EQ(ui::window_def::is_moveable, defs.windows[0].flags & ui::window_def::is_moveable);
	EXPECT_EQ(ui::window_def::orientation_upper_left, defs.windows[0].flags & ui::window_def::orientation_mask);
	EXPECT_EQ(1, defs.windows[0].background_handle);
	EXPECT_EQ(2, defs.windows[0].sub_object_definitions.size());
	EXPECT_EQ(ui::pack_ui_definition_handle(ui::element_type::button, 1), defs.windows[0].sub_object_definitions[0]);
	EXPECT_EQ(ui::pack_ui_definition_handle(ui::element_type::listbox, 1), defs.windows[0].sub_object_definitions[1]);
}

TEST(errors_window, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = wname\n"
		"basattribute\n"
		"size = {x=1 y=2}\n"
		"position = {x=4 y=5}\n"
		"fullscreen = yes\n"
		"moveable = 2\n"
		"Orientation = CENTER_DOWN\n"
		"guiButtonType = { name = x }\n"
		"background = y\n"
		"listBoxType = { name = l }\n"
	));

	if (parse_tree.size() > 0)
		container.gui_eu3dialogtype(parse_object<windowType, gui_file_domain>(&parse_tree[0], &parse_tree[0] + parse_tree.size(), e));

	EXPECT_EQ(1, nmaps.window_names.size());
	EXPECT_EQ(1, defs.windows.size());
	EXPECT_EQ(1, defs.buttons.size());
	EXPECT_EQ(1, defs.listboxes.size());
	EXPECT_EQ(4, errors_generated.size());

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_window_attribute), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_window_moveable_value), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_window_orientation), errors_generated[2]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::window_background_not_found), errors_generated[3]);

	EXPECT_EQ(std::string("wname"), nmaps.window_names[0].get_string(defs.name_data));
	EXPECT_EQ(4, defs.windows[0].position.x);
	EXPECT_EQ(5, defs.windows[0].position.y);
	EXPECT_EQ(1, defs.windows[0].size.x);
	EXPECT_EQ(2, defs.windows[0].size.y);
	EXPECT_EQ(ui::window_def::is_dialog, defs.windows[0].flags & ui::window_def::is_dialog);
	EXPECT_EQ(ui::window_def::is_fullscreen, defs.windows[0].flags & ui::window_def::is_fullscreen);
	EXPECT_EQ(0, defs.windows[0].flags & ui::window_def::is_moveable);
	EXPECT_EQ(ui::window_def::orientation_center, defs.windows[0].flags & ui::window_def::orientation_mask);
	EXPECT_EQ(0, defs.windows[0].background_handle);
	EXPECT_EQ(2, defs.windows[0].sub_object_definitions.size());
	EXPECT_EQ(ui::pack_ui_definition_handle(ui::element_type::button, 1), defs.windows[0].sub_object_definitions[0]);
	EXPECT_EQ(ui::pack_ui_definition_handle(ui::element_type::listbox, 1), defs.windows[0].sub_object_definitions[1]);
}