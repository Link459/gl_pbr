#ifndef MODELS_H
#define MODELS_H

#include "mesh.h"
#include "pbr/pbr_material.h"

Mesh *model_flat_plane();
Mesh *model_cube(vec3 color);
Mesh *model_cube_with_normals(vec3 color);

void render_cube();
void render_quad();

Mesh *model_cerberus(PbrMaterial *material);
Mesh *model_damaged_helmet(PbrMaterial *material);
#endif /* MODELS_H */
