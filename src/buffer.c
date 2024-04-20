#include "buffer.h"

Buffer buffer_create(BufferTarget target) {
  uint32_t gl_buffer;
  glGenBuffers(1, &gl_buffer);

  Buffer buffer = {.buffer_id = gl_buffer, .target = target};
  return buffer;
}

Buffer buffer_create_data(BufferTarget target, BufferUsage usage, size_t size,
                          const void *data) {
  Buffer buffer = buffer_create(target);
  buffer_data(&buffer, usage, size, data);
  return buffer;
}

void buffer_data(const Buffer *buffer, BufferUsage usage, size_t size,
                 const void *data) {
  buffer_bind(buffer);
  glBufferData(buffer->target, size, data, usage);
}

void buffer_bind(const Buffer *buffer) {
  glBindBuffer(buffer->target, buffer->buffer_id);
}

void buffer_unbind(const Buffer *buffer) { glBindBuffer(buffer->target, 0); }
void buffer_destroy(const Buffer *buffer) {
    if (buffer->buffer_id != 0) {
  glDeleteBuffers(1, &buffer->buffer_id);
    }
}
