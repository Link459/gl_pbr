#include "vertex_array.h"
#include <glad/glad.h>

VertexArray vertex_array_create() {
  uint32_t gl_va;
  glGenVertexArrays(1, &gl_va);
  VertexArray va = {.vertex_array_id = gl_va};
  vertex_array_bind(&va);
  return va;
}
void vertex_array_bind(const VertexArray *va) {
  glBindVertexArray(va->vertex_array_id);
}
void vertex_array_set_attribute(uint32_t index, uint32_t size,
                                VertexAttributeType type, size_t stride,
                                const void *pointer) {
  glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
  vertex_array_enable_attribute(index);
}
void vertex_array_disable_attribute(uint32_t attribute) {
  glDisableVertexAttribArray(attribute);
}
void vertex_array_enable_attribute(uint32_t attribute) {
  glEnableVertexAttribArray(attribute);
}

void vertex_array_destroy(const VertexArray *va) {
  glDeleteVertexArrays(1, &va->vertex_array_id);
}

/* uint32_t VAO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  Buffer vbo =
      buffer_create_data(ARRAY_BUFFER, STATIC_DRAW, sizeof(vertices), vertices);

  Buffer ebo = buffer_create_data(ELEMENT_ARRAY_BUFFER, STATIC_DRAW,
                                  sizeof(indices), indices);
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);*/
