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

typedef struct {
  vec3 albedo;
  float metallic;
  float roughness;
  float ao;
} PbrMaterial;

void pbr_material_bind(const PbrMaterial *material, const Pipeline *pipeline);

#endif /* MATERIAL_H */
