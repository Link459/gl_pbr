#include "light.h"

void light_bind(const Light *light, const Pipeline *pipeline) {
  pipeline_set_vec3(pipeline, "light.position", &light->position);
  pipeline_set_vec3(pipeline, "light.ambient", &light->ambient);
  pipeline_set_vec3(pipeline, "light.diffuse", &light->diffuse);
  pipeline_set_vec3(pipeline, "light.specular", &light->specular);
}
