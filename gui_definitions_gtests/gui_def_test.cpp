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
	auto sl = fake_sound_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt, sl);
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
	EXPECT_EQ(std::string("bname"), nmaps.button_names[0]);
	EXPECT_EQ(ui::button_def::graphical_obj_qtex_type, defs.buttons[0].flags & ui::button_def::graphical_obj_type_mask);
	EXPECT_EQ(ui::button_def::orientation_center, defs.buttons[0].flags & ui::button_def::orientation_mask);
	EXPECT_EQ(ui::button_def::rotation_upright, defs.buttons[0].flags & ui::button_def::rotation_mask);
	EXPECT_EQ(1, defs.buttons[0].text_handle);
	EXPECT_EQ(1, defs.buttons[0].font_handle);
	EXPECT_EQ(1, defs.buttons[0].graphical_object_handle);
	EXPECT_EQ(0, defs.buttons[0].clicksound_handle);
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
	auto sl = fake_sound_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt, sl);
	e.file = "fake_file";

	gui_file container(e);

	std::vector<token_group> parse_tree;
	parse_pdx_file(parse_tree, RANGE(
		"name = bname\n"
		"buttonText = dummy\n"
		"buttonFont = dummy\n"
		"clicksound = dummy\n"
		"tooltip = \"\"\n"
		"rotation = 1.5708\n"
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
	EXPECT_EQ(std::string("bname"), nmaps.button_names[0]);
	EXPECT_EQ(ui::button_def::graphical_obj_sprite_type, defs.buttons[0].flags & ui::button_def::graphical_obj_type_mask);
	EXPECT_EQ(ui::button_def::orientation_upper_left, defs.buttons[0].flags & ui::button_def::orientation_mask);
	EXPECT_EQ(ui::button_def::rotation_90_right, defs.buttons[0].flags & ui::button_def::rotation_mask);
	EXPECT_EQ(1, defs.buttons[0].text_handle);
	EXPECT_EQ(1, defs.buttons[0].font_handle);
	EXPECT_EQ(1, defs.buttons[0].graphical_object_handle);
	EXPECT_EQ(1, defs.buttons[0].clicksound_handle);
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
	auto sl = fake_sound_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt, sl);
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
	EXPECT_EQ(6, errors_generated.size());
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_tooltip_empty_for_button), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_button_rotation), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_button_orientation), errors_generated[2]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_tooltiptext_empty_for_button), errors_generated[3]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_delayedtooltiptext_empty_for_button), errors_generated[4]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::expected_button_format_to_be_left), errors_generated[5]);
}

TEST(basic_gui_icon, gui_definitions_tests) {
	ui::name_maps nmaps;
	ui::definitions defs;
	std::vector<std::pair<std::string, ui::errors>> errors_generated;
	auto th = fake_text_handle_lookup();
	auto fh = fake_font_handle_lookup();
	auto qt = fake_gobj_lookup();
	auto sl = fake_sound_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt, sl);
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
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0]);
	EXPECT_EQ(ui::icon_def::orientation_center, defs.icons[0].flags & ui::icon_def::orientation_mask);
	EXPECT_EQ(ui::icon_def::rotation_upright, defs.icons[0].flags & ui::icon_def::rotation_mask);

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
	auto sl = fake_sound_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt, sl);
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
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0]);
	EXPECT_EQ(ui::icon_def::orientation_center_up, defs.icons[0].flags & ui::icon_def::orientation_mask);
	EXPECT_EQ(ui::icon_def::rotation_90_left, defs.icons[0].flags & ui::icon_def::rotation_mask);

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
	auto sl = fake_sound_lookup();

	parsing_environment e(nmaps, defs, errors_generated, th, fh, qt, sl);
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
	EXPECT_EQ(std::string("iname"), nmaps.icon_names[0]);

	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_icon_rotation), errors_generated[0]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unknown_icon_orientation), errors_generated[1]);
	EXPECT_EQ(std::make_pair(std::string("fake_file"), ui::errors::unexpected_icon_attribute), errors_generated[2]);
}