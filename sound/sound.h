#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"

namespace sound {
	struct sound_effect {
		std::u16string filename;
		float volume;

		sound_effect() {}
		sound_effect(const std::u16string& fn, float v) : filename(fn), volume(v) {}
	};
	struct music_file {
		std::u16string filename;

		music_file(const std::u16string& fn) : filename(fn) {}
	};

	class sound_manager {
	public:
		tagged_vector<sound_effect, effect_tag> sound_effects;
		tagged_vector<music_file, music_tag> music;
		
		boost::container::flat_map<text_data::text_tag, effect_tag> named_sound_effects;

		effect_tag click;
	};

	class active_sound_state {
	private:
		float _music_volume = 1.0f;
		float _effect_volume = 1.0f;
	public:
		float effect_volume() const noexcept { return _effect_volume; }
		float music_volume() const noexcept { return _music_volume; }
		void set_effect_volume(float v);
		void set_music_volume(float v);
	};

	void play_effect(active_sound_state& state, sound_manager& maanger, effect_tag effect);
	void play_effect_immediate(active_sound_state& state, sound_manager& maanger, effect_tag effect);
	void start_music(active_sound_state& state, sound_manager& maanger);
	void stop_music(active_sound_state& state, sound_manager& maanger);
}
