#include "pbr_material.h"

void pbr_material_bind(const PbrPipeline *pipeline,
                       const PbrMaterial *material) {
  glActiveTexture(GL_TEXTURE0);
  texture_bind(&material->albedo);
  glActiveTexture(GL_TEXTURE1);
  texture_bind(&material->normal);
  glActiveTexture(GL_TEXTURE2);
  texture_bind(&material->metallic);
  glActiveTexture(GL_TEXTURE3);
  texture_bind(&material->roughness);
  glActiveTexture(GL_TEXTURE4);
  texture_bind(&material->ao);
}

void pbr_material_destroy(const PbrMaterial *material) {
  texture_destroy(&material->albedo);
  texture_destroy(&material->normal);
  texture_destroy(&material->metallic);
  texture_destroy(&material->roughness);
  texture_destroy(&material->ao);
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
