#ifndef SHADER_H
#define SHADER_H

#include <cglm/struct.h>
#include <glad/glad.h>

typedef enum {
  VERTEX_SHADER = GL_VERTEX_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
  GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
} ShaderType;

typedef struct {
  uint32_t shader_id;
} Shader;

Shader shader_create(const char *code, ShaderType shader_type);
Shader shader_from_file(const char *file, ShaderType shader_type);

void shader_destroy(Shader shader);

#endif /* SHADER_H */
