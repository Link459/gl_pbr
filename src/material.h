#ifndef MATERIAL_H
#define MATERIAL_H

#include "pipeline/pipeline.h"
#include <cglm/cglm.h>

typedef struct {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
} Material;

void material_bind(const Material *material, const Pipeline *pipeline);

#endif /* MATERIAL_H */
