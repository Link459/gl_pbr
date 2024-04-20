#ifndef OBJECT_H
#define OBJECT_H

#include "material.h"
#include "mesh.h"

typedef struct {
  Mesh *mesh;
  Material material;
} Object;

void object_draw(const Object* object,const Pipeline* pipeline);

#endif /* OBJECT_H */
