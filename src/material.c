#include "material.h"

void material_bind(const Material *material, const Pipeline *pipeline) {
  pipeline_set_vec3(pipeline, "material.ambient", &material->ambient);
  pipeline_set_vec3(pipeline, "material.diffuse", &material->diffuse);
  pipeline_set_vec3(pipeline, "material.specular", &material->specular);
  pipeline_set_float(pipeline, "material.shininess", material->shininess);
}
