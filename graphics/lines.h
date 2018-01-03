#pragma once
#include <stdint.h>
#include <atomic>

namespace graphics {
	class lines {
	private:
		float* buffer = nullptr;
		uint32_t buffer_handle = 0;
		std::atomic<bool> pending_date_update = false;
	public:
		const uint32_t count = 0;

		lines(uint32_t c) : count(c) {
			buffer = new float[count * 4];
		}
		lines(const lines& o) : count(o.count) {
			buffer = new float[count * 4];
			memcpy(buffer, o.buffer, count * 4 * sizeof(float));
		}
		lines(lines&& o) noexcept : buffer(o.buffer), count(o.count) {
			o.buffer = nullptr;
		}
		lines& operator=(const lines& o) {
			memcpy(buffer, o.buffer, (count < o.count ? count : o.count) * 4 * sizeof(float));
			return *this;
		}
		~lines() {
			if (buffer)
				delete[] buffer;
		}
		void set_y(float* v);
		void bind_buffer();
	};
}
