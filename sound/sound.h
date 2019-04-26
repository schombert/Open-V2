#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "simple_fs\\simple_fs.h"

class world_state;

namespace sound {
	class sound_manager;

	class audio_instance {
	private:
		std::u16string filename;
		void* graph_interface = nullptr;
		void* control_interface = nullptr;
		void* audio_interface = nullptr;
		void* seek_interface = nullptr;
		void* event_interface = nullptr;
	public:
		float volume_multiplier = 1.0f;

		audio_instance() {}
		audio_instance(std::u16string const& file) : filename(file) {}
		audio_instance(audio_instance const&) = delete;
		audio_instance(audio_instance&& o) noexcept;
		~audio_instance();

		void set_file(std::u16string const& file) { filename = file; }
		void play(float volume, bool as_music, void* window_handle);
		void stop() const;
		bool is_playing() const;
		void change_volume(float new_volume) const;

		friend class sound_manager;
	};

	class sound_manager {
	private:
		audio_instance* current_effect = nullptr;
		audio_instance* current_interface_sound = nullptr;
	public:
		void* window_handle = nullptr;
		int32_t last_music = -1;
		int32_t first_music = -1;

		audio_instance click_sound;

		std::vector<audio_instance> music_list;

		void play_effect(audio_instance& s, float volume);
		void play_interface_sound(audio_instance& s, float volume);
		void play_music(int32_t track, float volume);

		void change_effect_volume(float v) const;
		void change_interface_volume(float v) const;
		void change_music_volume(float v) const;

		bool music_finished() const;

		sound_manager();
		~sound_manager();
	};

	void init_sound_manager(sound_manager& s, const directory& root, void* window_handle);

	void play_effect(world_state& ws, audio_instance& s);
	void play_interface_sound(world_state& ws, audio_instance& s);
	void play_new_track(world_state& ws);
}
