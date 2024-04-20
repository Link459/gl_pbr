#ifndef BUFFER_H
#define BUFFER_H

#include <glad/glad.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
  STREAM_DRAW = GL_STREAM_DRAW,
  STREAM_READ = GL_STREAM_READ,
  STREAM_COPY = GL_STREAM_COPY,
  STATIC_DRAW = GL_STATIC_DRAW,
  STATIC_READ = GL_STATIC_READ,
  STATIC_COPY = GL_STATIC_COPY,
  DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  DYNAMIC_READ = GL_DYNAMIC_READ,
  DYNAMIC_COPY = GL_DYNAMIC_COPY,
} BufferUsage;

typedef enum {
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
  COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
  DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
  ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
  PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
  PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
  //TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
  TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
  UNIFORM_BUFFER = GL_UNIFORM_BUFFER,
} BufferTarget;

typedef struct {
  uint32_t buffer_id;
  BufferTarget target;
} Buffer;

Buffer buffer_create(BufferTarget target);
Buffer buffer_create_data(BufferTarget target, BufferUsage usage, size_t size,
                          const void *data);
void buffer_data(const Buffer *buffer, BufferUsage usage, size_t size,
                 const void *data);

void buffer_bind(const Buffer *buffer);
void buffer_unbind(const Buffer *buffer);
void buffer_destroy(const Buffer *buffer);

#endif /* BUFFER_H */
