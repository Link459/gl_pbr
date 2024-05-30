#include "texture.h"
#include <glad/glad.h>

Texture texture_create(const TextureCreateInfo *info) {
  uint32_t gl_texture;
  glGenTextures(1, &gl_texture);
  Texture texture = {.texture_id = gl_texture, .type = info->type};
  texture_bind(&texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  if (info->wrapping.wrap_s != WRAPPING_NONE) {
    glTexParameteri(info->type, GL_TEXTURE_WRAP_S, info->wrapping.wrap_s);
  }
  if (info->wrapping.wrap_t != WRAPPING_NONE) {
    glTexParameteri(info->type, GL_TEXTURE_WRAP_T, info->wrapping.wrap_t);
  }
  if (info->wrapping.wrap_r != WRAPPING_NONE) {
    glTexParameteri(info->type, GL_TEXTURE_WRAP_R, info->wrapping.wrap_r);
  }

  if (info->filtering.min_filtering != FILTERING_NONE) {
    glTexParameteri(info->type, GL_TEXTURE_MIN_FILTER,
                    info->filtering.min_filtering);
  }
  if (info->filtering.mag_filtering != FILTERING_NONE) {
    glTexParameteri(info->type, GL_TEXTURE_MAG_FILTER,
                    info->filtering.mag_filtering);
  }

  return texture;
}

void texture_bind(const Texture *texture) {
  glBindTexture(texture->type, texture->texture_id);
}

void texture_destrox(const Texture *texture) {
  glDeleteTextures(1, &texture->texture_id);
}
