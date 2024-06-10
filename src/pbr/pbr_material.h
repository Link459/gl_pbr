#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include "../texture.h"
#include "cglm/types.h"
#include "pbr_pipeline.h"

typedef struct {
  vec4 albedo_factor;
  vec4 emissive_factor;
  vec4 diffuse_factor;
  vec4 specular_factor;
  float metallic_factor;
  float roughness_factor;
  float emissive_strength;

  Texture albedo_texture;
  Texture normal_texture;
  Texture metallic_texture;
  Texture roughness_texture;
  Texture ao_texture;
  Texture emissive_texture;
  // TODO: maybe implement a reflectiveness value for the fresnel-schlick
  // approximation
} PbrMaterial;

void pbr_material_bind(const PbrPipeline *pipeline,
                       const PbrMaterial *material);

void pbr_material_destroy(const PbrMaterial *material);

/// creates a repeating 1x1 texture based on a value
/// useful when you need a consistent pbr map or don't wan't to make one
Texture pbr_material_texture(vec3 value);
#endif /* PBR_MATERIAL_H */
