#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
  BYTE = GL_BYTE,
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  SHORT = GL_SHORT,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  INT = GL_INT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  HALF_FLOAT = GL_HALF_FLOAT,
  FLOAT = GL_FLOAT,
  DOUBLE = GL_DOUBLE,
  FIXED = GL_FIXED,
} VertexAttributeType;

typedef struct {
  uint32_t vertex_array_id;
} VertexArray;

VertexArray vertex_array_create();
void vertex_array_bind(const VertexArray *);
void vertex_array_set_attribute(uint32_t index, uint32_t size, VertexAttributeType type,
                                size_t stride, const void *pointer);
void vertex_array_disable_attribute(uint32_t attribute);
void vertex_array_enable_attribute(uint32_t attribute);
void vertex_array_destroy(const VertexArray *);

#endif /* VERTEX_ARRAY_H */
