#include "shader.h"
#include "asset.h"
#include <stdio.h>
#include <stdlib.h>

Shader shader_create(const char *code, ShaderType shader_type) {
  uint32_t shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &code, NULL);
  glCompileShader(shader_id);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
  }

  Shader shader;
  shader.shader_id = shader_id;

  return shader;
}

Shader shader_from_file(const char *file_path, ShaderType shader_type) {
  FileResult file = asset_load_file(file_path, "rb");
  Shader shader = shader_create(file.contents, shader_type);

  free(file.contents);

  return shader;
}

void shader_destroy(Shader shader) { glDeleteShader(shader.shader_id); }
