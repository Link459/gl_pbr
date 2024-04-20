#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stdint.h>

typedef enum {
  TEXTURE_1D = GL_TEXTURE_1D,
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_3D = GL_TEXTURE_3D,
  TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
  TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
  TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
  TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
  TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
  TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
  TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
} TextureType;

typedef enum {
  WRAPPING_REPEAT = GL_REPEAT,
  WRAPPING_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  WRAPPING_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  WRAPPING_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
} TextureWrapping;

typedef struct {
  TextureWrapping *wrap_s;
  TextureWrapping *wrap_t;
  TextureWrapping *wrap_r;
} TextureWrappingOptions;

typedef enum {
  FILTERING_LINEAR = GL_LINEAR,
  FILTERING_NEAREST = GL_NEAREST,
  FILTERING_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  FILTERING_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  FILTERING_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  FILTERING_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
} TextureFiltering;

typedef struct {
  TextureFiltering *mag_filtering;
  TextureFiltering *min_filtering;
} TextureFilteringOptions;

typedef struct {
  TextureType type;
  TextureFilteringOptions filtering;
  TextureWrappingOptions wrapping;
} TextureCreateInfo;

typedef struct {
  uint32_t texture_id;
  TextureType type;
} Texture;

Texture texture_create(const TextureCreateInfo *info);
Texture texture_create_file(const TextureCreateInfo *info,
                            const char *file_path);
void texture_bind(const Texture *texture);
void texture_destrox(const Texture *texture);
#endif /* TEXTURE_H */
