#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include "../texture.h"
#include "cglm/types.h"
#include "pbr_pipeline.h"

typedef struct {
  vec3 albedo;
  float metallic;
  float roughness;
  float ao;
} PbrMaterial;

typedef struct {
  Texture albedo;
  Texture metallic;
  Texture roughness;
  Texture ao;
} PbrTexMaterial;

void pbr_material_bind(const PbrMaterial *material,
                       const PbrPipeline *pipeline);

/// creates a repeating 1x1 texture based on a value
/// useful when you need a consistent pbr map or don't wan't to make one
Texture pbr_material_texture(float value);
#endif /* PBR_MATERIAL_H */
