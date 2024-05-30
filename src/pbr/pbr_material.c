#include "pbr_material.h"

void pbr_material_bind(const PbrMaterial *material,
                       const PbrPipeline *pipeline) {
  pipeline_set_vec3(&pipeline->pipeline, "material.albedo", &material->albedo);
  pipeline_set_float(&pipeline->pipeline, "material.roughness",
                     material->roughness);
  pipeline_set_float(&pipeline->pipeline, "material.metallic",
                     material->metallic);
  pipeline_set_float(&pipeline->pipeline, "material.ao", material->ao);
}

Texture pbr_material_texture(float value) {
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
          },
  };
  Texture texture = texture_create(&info);
  void *data = &value;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);

  return texture;
}
