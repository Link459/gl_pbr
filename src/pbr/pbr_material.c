#include "pbr_material.h"

void pbr_material_bind(const PbrPipeline *pipeline,
                       const PbrMaterial *material) {

  pipeline_set_vec4(&pipeline->pipeline, "material.albedo_factor",
                    &material->albedo_factor);
  pipeline_set_vec4(&pipeline->pipeline, "material.emissive_factor",
                    &material->emissive_factor);
  pipeline_set_vec4(&pipeline->pipeline, "material.diffuse_factor",
                    &material->diffuse_factor);
  pipeline_set_vec4(&pipeline->pipeline, "material.specular_factor",
                    &material->specular_factor);
  pipeline_set_float(&pipeline->pipeline, "material.metallic_factor",
                     material->metallic_factor);
  pipeline_set_float(&pipeline->pipeline, "material.roughness_factor",
                     material->roughness_factor);
  pipeline_set_float(&pipeline->pipeline, "material.emissive_strength",
                     material->emissive_strength);

  glActiveTexture(GL_TEXTURE0);
  texture_bind(&material->albedo_texture);
  glActiveTexture(GL_TEXTURE1);
  texture_bind(&material->normal_texture);
  glActiveTexture(GL_TEXTURE2);
  texture_bind(&material->metallic_texture);
  glActiveTexture(GL_TEXTURE3);
  texture_bind(&material->roughness_texture);
  glActiveTexture(GL_TEXTURE4);
  texture_bind(&material->ao_texture);
  glActiveTexture(GL_TEXTURE5);
  texture_bind(&material->emissive_texture);
}

void pbr_material_destroy(const PbrMaterial *material) {
  texture_destroy(&material->albedo_texture);
  texture_destroy(&material->normal_texture);
  texture_destroy(&material->metallic_texture);
  texture_destroy(&material->roughness_texture);
  texture_destroy(&material->ao_texture);
  texture_destroy(&material->emissive_texture);
}

Texture pbr_material_texture(vec3 value) {
  TextureCreateInfo info = {
      .type = TEXTURE_2D,
      .wrapping =
          {
              .wrap_r = WRAPPING_REPEAT,
              .wrap_s = WRAPPING_REPEAT,
              .wrap_t = WRAPPING_REPEAT,
          },
      .filtering =
          {
              .mag_filtering = FILTERING_LINEAR,
              .min_filtering = FILTERING_NEAREST_MIPMAP_LINEAR,
          },
  };
  Texture texture = texture_create(&info);
  vec3 values[9] = {
      {*value, *value, *value}, {*value, *value, *value},
      {*value, *value, *value}, {*value, *value, *value},
      {*value, *value, *value}, {*value, *value, *value},
      {*value, *value, *value}, {*value, *value, *value},
      {*value, *value, *value},
  };
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE,
               &values);
  glGenerateMipmap(GL_TEXTURE_2D);

  return texture;
}
