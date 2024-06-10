#ifndef ASSET_H
#define ASSET_H

#include "mesh.h"
#include "pbr/pbr_material.h"
#include "texture.h"

typedef struct {
  char *contents;
  uint32_t lenght;
} FileResult;

FileResult asset_load_file(const char *file_path, const char *access);

// subtract_tex_coords is to sub 1-tex_coord[1] because some models from gltf
// need it
Mesh *asset_load_mesh(const char *file_path, bool subtract_tex_coords);

Mesh *asset_load_mesh_gltf(const char *file_path, PbrMaterial *material);

Texture asset_load_texture(const TextureCreateInfo *info,
                           const char *file_path);
Texture asset_load_hdr(const char *file_path);

#endif /* ASSET_H */
