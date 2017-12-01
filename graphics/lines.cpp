#include "lines.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"

void lines::set_y(float* v) {
	for (int32_t i = 0; i < count; ++i) {
		buffer[i * 4 ] = static_cast<float>(i) / static_cast<float>(count - 1);
		buffer[i * 4 + 1] = 1.0f - v[i];
		buffer[i * 4 + 2] = 0.5f;
		buffer[i * 4 + 3] = v[i];
	}
	pending_date_update.store(true, std::memory_order::memory_order_release);
}

void lines::bind_buffer() {
	if (buffer_handle == 0) {
		glGenBuffers(1, &buffer_handle);

		glBindBuffer(GL_ARRAY_BUFFER, buffer_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count * 4, nullptr, GL_DYNAMIC_DRAW);
	}

	bool expected_b = true;
	if (pending_date_update.compare_exchange_strong(expected_b, false, std::memory_order::memory_order_release, std::memory_order::memory_order_acquire)) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer_handle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * count * 4, buffer);
	}

	glBindVertexBuffer(0, buffer_handle, 0, sizeof(GLfloat) * 4);
}