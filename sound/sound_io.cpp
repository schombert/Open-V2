#include "common\\common.h"
#include "sound_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace sound {
	struct sound_env {
		sound_manager& s;
		text_data::text_sequences& text;
		const directory& fx_dir;

		sound_env(sound_manager& sm, text_data::text_sequences& tm, const directory& f) : s(sm), text(tm), fx_dir(f) {}
	};

	struct effect_reader {
		sound_env& env;
		std::u16string file;
		float volume = 1.0f;

		effect_reader(sound_env& e) : env(e) {}
		void set_file(const token_and_type& t) {
			auto unopened = env.fx_dir.peek_file(t.start, t.end);
			if(unopened) {
				file = unopened->file_path() + u'\\' + unopened->file_name();
			}
		}
	};
	struct sound_sfx_file {
		sound_env& env;
		sound_sfx_file(sound_env& e) : env(e) {}

		void add_effect(const std::pair<token_and_type, effect_reader>& p) {
			const auto tag = env.s.sound_effects.emplace_back(p.second.file, p.second.volume);
			env.s.named_sound_effects.emplace(text_data::get_thread_safe_text_handle(env.text, p.first.start, p.first.end), tag);

			if(is_fixed_token_ci(p.first, "click")) {
				env.s.click = tag;
			}
		}
	};

	inline std::pair<token_and_type, effect_reader> name_effect(const token_and_type& t, association_type, effect_reader&& r) {
		return std::pair<token_and_type, effect_reader>(t, std::move(r));
	}
}

MEMBER_DEF(sound::effect_reader, volume, "volume");
MEMBER_FDEF(sound::effect_reader, set_file, "file");
MEMBER_FDEF(sound::sound_sfx_file, add_effect, "effect");

namespace sound {
	BEGIN_DOMAIN(fx_file_domain)
		BEGIN_TYPE(sound_sfx_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("effect", accept_all, effect_reader, name_effect)
		END_TYPE
		BEGIN_TYPE(effect_reader)
		MEMBER_ASSOCIATION("volume", "volume", value_from_rh<float>)
		MEMBER_ASSOCIATION("file", "file", token_from_rh)
		END_TYPE
		END_DOMAIN;

	void populate_music(sound_manager& m, const directory& root) {
		const auto music_directory = root.get_directory(u"\\music");
		for(const auto& mp3_file : music_directory.list_files(u".mp3")) {
			m.music.emplace_back(mp3_file.file_path() + u'\\' + mp3_file.file_name());
		}
	}

	void read_effects(sound_manager& m, text_data::text_sequences& text, const directory& root) {
		const auto fx_dir = root.get_directory(u"\\sound");
		const auto interface_dir = root.get_directory(u"\\interface");

		const auto fi = interface_dir.open_file(u"sound.sfx");

		if(fi) {
			parsed_data main_results;
			sound_env env(m, text, fx_dir);

			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<sound_sfx_file, fx_file_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					env);
			}
		}
	}
}
