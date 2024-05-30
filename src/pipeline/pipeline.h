#ifndef PIPELINE_H
#define PIPELINE_H

#include "shader.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
  Shader *shaders;
  size_t length;
  // wheter to delete the shaders after creation or if to keep them (so you can
  // reuse them)
  bool delete;
} PipelineCreateInfo;

typedef struct {
  uint32_t pipeline_id;
} Pipeline;

Pipeline pipeline_create(const PipelineCreateInfo *);
void pipeline_bind(const Pipeline *pipeline);
void pipeline_set_float(const Pipeline *pipeline, const char *v, const float f);
void pipeline_set_vec2(const Pipeline *pipeline, const char *v,
                       const vec2 *vec);
void pipeline_set_vec3(const Pipeline *pipeline, const char *v,
                       const vec3 *vec);
void pipeline_set_vec4(const Pipeline *pipeline, const char *v,
                       const vec4 *vec);
void pipeline_set_mat2(const Pipeline *pipeline, const char *v,
                       const mat2 *mat);
void pipeline_set_mat3(const Pipeline *pipeline, const char *v,
                       const mat3 *mat);
void pipeline_set_mat4(const Pipeline *pipeline, const char *v,
                       const mat4 *mat);
void pipeline_destroy(const Pipeline *pipeline);

#endif /* PIPELINE_H */
