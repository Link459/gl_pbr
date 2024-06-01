#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include "../texture.h"
#include "cglm/types.h"
#include "pbr_pipeline.h"

typedef struct {
  Texture albedo;
  Texture normal;
  Texture metallic;
  Texture roughness;
  Texture ao;
  // TODO: maybe implement a reflectiveness value for the fresnel-schlick approximation
} PbrMaterial;

void pbr_material_bind(const PbrPipeline *pipeline,
                       const PbrMaterial *material);

void pbr_material_destroy(const PbrMaterial *material);

/// creates a repeating 1x1 texture based on a value
/// useful when you need a consistent pbr map or don't wan't to make one
Texture pbr_material_texture(vec3 value);
#endif /* PBR_MATERIAL_H */
