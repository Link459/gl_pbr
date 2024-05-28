#ifndef MODELS_H
#define MODELS_H

#include "mesh.h"

Mesh *model_flat_plane();
Mesh *model_cube(vec3 color);
Mesh *model_cube_with_normals(vec3 color);

#endif /* MODELS_H */
