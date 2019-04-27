#include "common\\common.h"
#include "settings.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "Shlobj.h"

#undef min
#undef max

template<>
class serialization::serializer<scenario::game_settings> : public serialization::memcpy_serializer<scenario::game_settings> {};

namespace settings {
	std::wstring save_directory() {
		wchar_t* path_out = nullptr;
		SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &path_out);

		std::wstring path_result(path_out);
		CoTaskMemFree(path_out);

		std::wstring save_dir = path_result + L"\\Open V2";
		CreateDirectory(save_dir.c_str(), NULL);

		return save_dir;
	}

	void save_settings(scenario::scenario_manager const& s) {
		size_t total_file_size =
			sizeof(int32_t)
			+ sizeof(int32_t) + serialization::serialize_size(s.settings)
			+ sizeof(int32_t) + sizeof(int8_t) * std::extent_v<decltype(s.message_m.group_importance)>
			+ sizeof(int32_t) + sizeof(messages::message_setting) * std::extent_v<decltype(s.message_m.settings)>
			+ sizeof(int32_t); // placeholder for keybindings

		auto const file_name = save_directory() + L"\\settings";
		serialization::serialize_file_wrapper file((char16_t*)(file_name.c_str()), total_file_size);

		auto ptr = file.get_bytes();

		int32_t version = 1;
		int32_t settings_size = int32_t(sizeof(s.settings));
		int32_t count_groups = int32_t(std::extent_v<decltype(s.message_m.group_importance)>);
		int32_t count_message_settings = int32_t(std::extent_v<decltype(s.message_m.settings)>);
		int32_t count_key_bindings = 0;

		serialization::serialize(ptr, version);

		serialization::serialize(ptr, settings_size);
		serialization::serialize(ptr, s.settings);

		serialization::serialize(ptr, count_groups);
		serialization::serialize_array(ptr, s.message_m.group_importance, count_groups);

		serialization::serialize(ptr, count_message_settings);
		serialization::serialize_array(ptr, s.message_m.settings, count_message_settings);

		serialization::serialize(ptr, count_key_bindings);
		// todo
	}

	void load_settings(scenario::scenario_manager& s) {

		auto const file_name = save_directory() + L"\\settings";
		serialization::serialize_file_wrapper file((char16_t*)(file_name.c_str()));

		if(file.file_valid()) {
			int32_t version = 0;
			int32_t settings_size = 0;
			int32_t count_groups = 0;
			int32_t count_message_settings = 0;
			int32_t count_key_bindings = 0;

			std::byte const* ptr = file.get_bytes();
			serialization::deserialize(ptr, version);

			if(version == 1) {
				serialization::deserialize(ptr, settings_size);

				memcpy(&s.settings, ptr, std::min(settings_size, int32_t(sizeof(s.settings))));
				ptr += settings_size;

				serialization::deserialize(ptr, count_groups);
				if(count_groups <= int32_t(std::extent_v<decltype(s.message_m.group_importance)>)) {
					serialization::deserialize_array(ptr, s.message_m.group_importance, count_groups);
				} else {
					serialization::deserialize_array(ptr, s.message_m.group_importance, std::extent_v<decltype(s.message_m.group_importance)>);
					ptr += sizeof(int8_t) * (count_groups - std::extent_v<decltype(s.message_m.group_importance)>);
				}

				serialization::deserialize(ptr, count_message_settings);
				if(count_message_settings <= int32_t(std::extent_v<decltype(s.message_m.settings)>)) {
					serialization::deserialize_array(ptr, s.message_m.settings, count_message_settings);
				} else {
					serialization::deserialize_array(ptr, s.message_m.settings, std::extent_v<decltype(s.message_m.settings)>);
					ptr += sizeof(messages::message_setting) * (count_message_settings - std::extent_v<decltype(s.message_m.settings)>);
				}

				serialization::deserialize(ptr, count_key_bindings);
				// todo
			}
		}

	}
}
