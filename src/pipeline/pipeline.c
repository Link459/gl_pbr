#include "pipeline.h"
#include "../log.h"
#include <glad/glad.h>

Pipeline pipeline_create(const PipelineCreateInfo *info) {
  uint32_t shader_program = glCreateProgram();
  for (int i = 0; i < info->length; i++) {
    glAttachShader(shader_program, info->shaders[i].shader_id);
  }
  // maybe use pipeline programs to avoid the rigidness
  // glProgramParameter(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
  glLinkProgram(shader_program);
  int success = 0;
  char info_log[512];
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    PANIC("%s", info_log);
  }

  if (info->delete) {
    for (int i = 0; i < info->length; i++) {
      shader_destroy(info->shaders[i]);
    }
  }

  Pipeline pipeline = {
      .pipeline_id = shader_program,
  };
  return pipeline;
}

void pipeline_bind(const Pipeline *pipeline) {
  glUseProgram(pipeline->pipeline_id);
}

void pipeline_set_int(const Pipeline *pipeline, const char *v, const int i) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniform1i(location, i);
}
void pipeline_set_float(const Pipeline *pipeline, const char *v,
                        const float f) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniform1f(location, f);
}
void pipeline_set_vec2(const Pipeline *pipeline, const char *v,
                       const vec2 *vec) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniform2fv(location, 1, *vec);
}
void pipeline_set_vec3(const Pipeline *pipeline, const char *v,
                       const vec3 *vec) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniform3fv(location, 1, *vec);
}

void pipeline_set_vec4(const Pipeline *pipeline, const char *v,
                       const vec4 *vec) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniform4fv(location, 1, *vec);
}

void pipeline_set_mat2(const Pipeline *pipeline, const char *v,
                       const mat2 *mat) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniformMatrix3fv(location, 1, false, **mat);
}
void pipeline_set_mat3(const Pipeline *pipeline, const char *v,
                       const mat3 *mat) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniformMatrix3fv(location, 1, false, **mat);
}
void pipeline_set_mat4(const Pipeline *pipeline, const char *v,
                       const mat4 *mat) {
  int location = glGetUniformLocation(pipeline->pipeline_id, v);
  glUniformMatrix4fv(location, 1, false, **mat);
}

void pipeline_destroy(const Pipeline *pipeline) {
  glDeleteProgram(pipeline->pipeline_id);
}
