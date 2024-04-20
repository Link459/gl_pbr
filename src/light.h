#ifndef LIGHT_H
#define LIGHT_H

#include "cglm/types.h"
#include "pipeline.h"
typedef struct {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
} Light;

void light_bind(const Light *light, const Pipeline *pipeline);

#endif /* LIGHT_H */
