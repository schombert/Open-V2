#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>

#undef max
#undef min

std::string extract_string(char const* & input, char const* end) {
	while(input < end && (*input == ' ' || *input == '(' || *input == ',' || *input == '-' || *input == '\r' || *input == '\n'))
		++input;

	if(input >= end)
		return std::string();

	std::string result;
	bool in_quote = false;

	while(input < end && (in_quote || (*input != ' '  && *input != '(' && *input != ')' && *input != '\t' && *input != '-' && *input != '\r' && *input != '\n'))) {
		if(*input == '\"')
			in_quote = !in_quote;
		else
			result += *input;
		++input;
	}

	while(input < end && (*input == ' ' || *input == ',' || *input == ')' || *input == '-' || *input == '\r' || *input == '\n'))
		++input;

	return result;
}

std::string extract_optional(char const* & input, char const* end) {
	while(input < end && (*input == ' ' || *input == ',' || *input == '-' || *input == '\r' || *input == '\n'))
		++input;

	if(input >= end)
		return std::string();

	std::string result;
	bool in_quote = false;

	if(*input == '(') {
		++input;

		while(input < end && (in_quote || *input != ')')) {
			if(*input == '\"')
				in_quote = !in_quote;
			else
				result += *input;
			++input;
		}

		if(input < end)
			++input;
	}

	while(input < end && (*input == ' ' || *input == '-' || *input == '\r' || *input == '\n'))
		++input;

	return result;
}


struct gui_item {
	std::string name;
	std::string type;
};

struct window_def {
	std::string name;
	std::string base;
	std::vector<gui_item> members;
};

int wmain(int argc, wchar_t *argv[]) {
	for(int32_t i = 1; i < argc; ++i) {

		if(lstrlen(argv[i]) <= 0)
			continue;

		const auto handle = CreateFile(argv[i], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(handle != INVALID_HANDLE_VALUE) {

			LARGE_INTEGER file_size;
			GetFileSizeEx(handle, &file_size);
			auto const sz = (size_t)(file_size.QuadPart);

			char* buffer = new char[sz];
			DWORD read;
			ReadFile(handle, buffer, (DWORD)sz, &read, nullptr);

			char const* input = buffer;

			std::string out_name = extract_string(input, buffer + sz);

			std::vector<window_def> contents;

			while(input < buffer + sz) {
				contents.emplace_back();

				contents.back().name = extract_string(input, buffer + sz);
				contents.back().base = extract_string(input, buffer + sz);

				while(input < buffer + sz && *input == '\t') {
					++input;
					auto item_name = extract_string(input, buffer + sz);
					auto item_type = extract_string(input, buffer + sz);
					contents.back().members.push_back(gui_item{ item_name, item_type });
				}
			}

			delete[] buffer;
			CloseHandle(handle);

			std::string output;

			for(auto& win : contents) {
				output += "class " + win.name + ";\r\n";
				output += "ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, " + win.name + "& b);\r\n";
				output += "ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, " + win.name + "& b);\r\n";
			}
			output += "\r\n";

			for(auto& win : contents) {
				if(win.name.length() > 0) {
					output += "template<typename = void>\r\n";
					output += "class " + win.name + "_internal_class" + " : public " + win.base + " {\r\n";
					output += "public:\r\n";

					for(auto& i : win.members) {
						output += "\t " + i.type + " " + i.name + ";\r\n";
					}
					output += "\t ui::gui_object_tag window_object;\r\n";
					output += "\r\n";

					for(auto& i : win.members) {
						output += "\t template<typename index>\r\n";
						output += "\t std::enable_if_t<std::is_same_v<CT_STRING(\"" + i.name + "\"),index > ," + i.type + "&> get() {\r\n";
						output += "\t\t return " + i.name + ";\r\n";
						output += "\t }\r\n";
						output += "\t template<typename index>\r\n";
						output += "\t std::enable_if_t<std::is_same_v<CT_STRING(\"" + i.name + "\"),index > ," + i.type + " const&> get() const {\r\n";
						output += "\t\t return " + i.name + ";\r\n";
						output += "\t }\r\n";
					}

					output += "\r\n";
					output += "\t void on_create(world_state& ws) {\r\n";
					output += "\t\t if constexpr(ui::detail::has_on_create<" + win.base + ", world_state&>) {\r\n";
					output += "\t\t\t " + win.base + "::on_create(ws);\r\n";
					output += "\t\t } else if constexpr(ui::detail::has_on_create<" + win.base + ", " + win.name + "_internal_class" + "&, world_state&>) {\r\n";
					output += "\t\t\t " + win.base + "::on_create(*this, ws);\r\n";
					output += "\t\t }\r\n";
					output += "\t }\r\n";
					output += "\t virtual void update_data(ui::gui_object_tag, world_state& ws) override {\r\n";
					output += "\t\t if constexpr(ui::detail::has_update<" + win.base + ", world_state&>) \r\n";
					output += "\t\t\t " + win.base + "::update(ws);\r\n";

					for(auto& i : win.members) {
						output += "\t\t if constexpr(ui::detail::has_windowed_update<" + i.type + ", " + win.name + "_internal_class" + "&, world_state&>) \r\n";
						output += "\t\t\t " + i.name + ".windowed_update(*this, ws);\r\n";
					}

					output += "\t }\r\n";

					output += "\t ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {\r\n";
					// window type creation

					output += "\t\t const auto window = ws.w.gui_m.gui_objects.emplace();\r\n";
					output += "\t\t window.object.align = alignment_from_definition(definition);\r\n";
					output += "\t\t if (is_valid_index(definition.background_handle)) {\r\n";
					output += "\t\t\t const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];\r\n";
					output += "\t\t\t ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);\r\n";
					output += "\t\t } else {\r\n";
					output += "\t\t\t window.object.type_dependant_handle.store(0, std::memory_order_release);\r\n";
					output += "\t\t }\r\n";
					output += "\t\t window.object.associated_behavior = this;\r\n";
					output += "\t\t " + win.base + "::associated_object = &window.object;\r\n";
					output += "\t\t window.object.size = definition.size;\r\n";
					output += "\t\t window.object.position = definition.position;\r\n";


					// member creation
					output += "\t\t for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {\r\n";
					output += "\t\t\t auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);\r\n";
					output += "\t\t\t const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);\r\n";
					output += "\t\t\t const char* rn_e = rn_s + rn.length();\r\n";

					bool first = true;
					for(auto &i : win.members) {
						if(first) {
							first = false;
							output += "\t\t\t if(compile_time_str_compare_ci<CT_STRING(\"" + i.name + "\")>(rn_s, rn_e) == 0) {\r\n";
						} else {
							output += "\t\t\t } else if(compile_time_str_compare_ci<CT_STRING(\"" + i.name + "\")>(rn_s, rn_e) == 0) {\r\n";
						}
						output += "#ifdef _DEBUG\r\n";
						output += "\t\t\t\t ui::detail::visitor_helper<" + i.type + "> vhelper(" + i.name + ", window, ws, rn_s, rn_e);\r\n";
						output += "#else\r\n";
						output += "\t\t\t\t ui::detail::visitor_helper<" + i.type + "> vhelper(" + i.name + ", window, ws);\r\n";
						output += "#endif\r\n";
						output += "\t\t\t\t std::visit(vhelper, *i);\r\n";
					}
					if(win.members.size() > 0)
						output += "\t\t\t } else {\r\n";

					// dynamic case
					output += "\t\t\t\t if constexpr(ui::detail::can_create_dynamic<" + win.base + ", world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {\r\n";
					output += "\t\t\t\t\t if(!" + win.base + "::create_dynamic(ws, window, *i, rn_s, rn_e)) {\r\n";
					output += "\t\t\t\t\t\t std::visit([&ws, &window](auto tag) {\r\n";
					output += "\t\t\t\t\t\t\t if constexpr(!std::is_same_v<decltype(tag), std::monostate>)\r\n";
					output += "\t\t\t\t\t\t\t\t ui::create_dynamic_element(ws, tag, window);\r\n";
					output += "\t\t\t\t\t\t }, *i);\r\n";
					output += "\t\t\t\t\t }\r\n";
					output += "\t\t\t\t } else {\r\n";
					output += "\t\t\t\t\t std::visit([&ws, &window](auto tag) {\r\n";
					output += "\t\t\t\t\t\t if constexpr(!std::is_same_v<decltype(tag), std::monostate>)\r\n";
					output += "\t\t\t\t\t\t\t ui::create_dynamic_element(ws, tag, window);\r\n";
					output += "\t\t\t\t\t }, *i);\r\n";
					output += "\t\t\t\t }\r\n";


					if(win.members.size() > 0)
						output += "\t\t\t }\r\n";

					output += "\t\t }\r\n";

					output += "\t\t return window;\r\n";
					output += "\t }\r\n";

					output += "\t ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {\r\n";
					// icon type creation

					output += "\t\t const auto new_gobj = ws.w.gui_m.gui_objects.emplace();\r\n";
					output += "\t\t const uint16_t rotation =\r\n";
					output += "\t\t\t (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?\r\n";
					output += "\t\t\t ui::gui_object::rotation_upright :\r\n";
					output += "\t\t\t ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);\r\n";
					output += "\t\t new_gobj.object.position = icon_def.position;\r\n";
					output += "\t\t new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);\r\n";
					output += "\t\t new_gobj.object.align = alignment_from_definition(icon_def);\r\n";
					output += "\t\t ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);\r\n";
					output += "\t\t if(rotation == ui::gui_object::rotation_right) {\r\n";
					output += "\t\t\t new_gobj.object.position = ui::xy_pair{\r\n";
					output += "\t\t\t\t int16_t(new_gobj.object.position.x - new_gobj.object.size.y),\r\n";
					output += "\t\t\t\t int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };\r\n";
					output += "\t\t\t new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };\r\n";
					output += "\t\t }\r\n";
					output += "\t\t new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);\r\n";
					output += "\t\t new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);\r\n";
					output += "\t\t new_gobj.object.associated_behavior = this;\r\n";
					output += "\t\t " + win.base + "::associated_object = &new_gobj.object;\r\n";
					output += "\t\t return new_gobj;\r\n";
					output += "\t }\r\n";

					output += "\t template<typename def_type>\r\n";
					output += "\t ui::tagged_gui_object create(world_state& ws, def_type const& definition) {\r\n";
					// creation
					output += "\t\t const auto win = create_gui_obj(ws, definition);\r\n";
					output += "\t\t window_object = win.id;\r\n";
					output += "\t\t if constexpr(ui::detail::has_on_create<" + win.base + ", world_state&>) {\r\n";
					output += "\t\t\t " + win.base + "::on_create(ws);\r\n";
					output += "\t\t } else if constexpr(ui::detail::has_on_create<" + win.base + ", " + win.name + "_internal_class" + "&, world_state&>) {\r\n";
					output += "\t\t\t " + win.base + "::on_create(*this, ws);\r\n";
					output += "\t\t }\r\n";
					output += "\t\t return win;\r\n";

					output += "\t }\r\n";

					output += "};\r\n"; // end class
					output += "class " + win.name + " : public " + win.name + "_internal_class<void> {};\r\n";

					output += "inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, " + win.name + "& b) {\r\n";
					output += "\t const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];\r\n";
					output += "\t const auto res = b.create(ws, window_definition);\r\n";
					output += "\t ui::add_to_back(ws.w.gui_m, parent, res);\r\n";
					output += "\t ws.w.gui_m.flag_minimal_update();\r\n";
					output += "\t return res;\r\n";
					output += "}\r\n"; 
					output += "inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, " + win.name + "& b) {\r\n";
					output += "\t const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];\r\n";
					output += "\t const auto res = b.create(ws, window_definition);\r\n";
					output += "\t ui::add_to_back(ws.w.gui_m, parent, res);\r\n";
					output += "\t ws.w.gui_m.flag_minimal_update();\r\n";
					output += "\t return res;\r\n";
					output += "}\r\n"; 
				}
			}


			std::wstring output_name;
			for(auto c : out_name) {
				output_name += wchar_t(c);
			}
			output_name += wchar_t('.');
			output_name += wchar_t('h');

			const auto handleb = CreateFile(output_name.c_str(), GENERIC_WRITE | GENERIC_READ, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			if(handleb != INVALID_HANDLE_VALUE) {
				DWORD written;
				WriteFile(handleb, output.c_str(), DWORD(output.length()), &written, nullptr);
				CloseHandle(handleb);
			}
		} else {
			MessageBox(NULL, argv[i], L"File not found", MB_OK);
		}
	}

	return 0;
}