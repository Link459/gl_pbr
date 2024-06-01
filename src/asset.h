#ifndef ASSET_H
#define ASSET_H

#include "mesh.h"
#include "texture.h"

typedef struct {
  char *contents;
  uint32_t lenght;
} FileResult;

FileResult asset_load_file(const char *file_path, const char *access);

Mesh *asset_load_mesh(const char *file_path);

Texture asset_load_texture(const TextureCreateInfo *info,
                           const char *file_path);
Texture asset_load_hdr(const char* file_path);
#endif /* ASSET_H */
