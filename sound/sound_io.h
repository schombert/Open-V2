#pragma once
#include "common\\common.h"
#include "sound.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"

template<>
class serialization::serializer<sound::sound_effect> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, sound::sound_effect const& obj) {
		serialize(output, obj.filename);
		serialize(output, obj.volume);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, sound::sound_effect& obj, CONTEXT&& ... c) {
		deserialize(input, obj.filename);
		deserialize(input, obj.volume);
	}
	static size_t size(sound::sound_effect const& obj) {
		return serialize_size(obj.filename) +
			serialize_size(obj.volume);
	}
};

template<>
class serialization::serializer<sound::sound_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, sound::sound_manager const& obj) {
		serialize(output, obj.sound_effects);
		serialize(output, obj.named_sound_effects);
		serialize(output, obj.click);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, sound::sound_manager& obj, CONTEXT&& ... c) {
		deserialize(input, obj.sound_effects);
		deserialize(input, obj.named_sound_effects);
		deserialize(input, obj.click);
	}
	static size_t size(sound::sound_manager const& obj) {
		return serialize_size(obj.sound_effects) +
			serialize_size(obj.named_sound_effects) +
			serialize_size(obj.click);
	}
};

namespace sound {
	void populate_music(sound_manager&, const directory& root);
	void read_effects(sound_manager& m, text_data::text_sequences& text, const directory& root);
}
