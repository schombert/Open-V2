#pragma once
#include "common\\common.h"
#include "sound.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"


template<>
class serialization::serializer<sound::sound_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, sound::sound_manager const& obj) {
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, sound::sound_manager& obj, CONTEXT&& ... ) {
	}
	static size_t size(sound::sound_manager const& obj) {
		return 0;
	}
	template<typename T>
	static size_t size(sound::sound_manager const& obj, T const&) {
		return size(obj);
	}
};
