#include "object.h"

void object_draw(const Object *object, const Pipeline *pipeline) {
  material_bind(&object->material, pipeline);
  mesh_draw(object->mesh, pipeline);
}
