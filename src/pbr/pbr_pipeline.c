#include "pbr_pipeline.h"

PbrPipeline pbr_pipeline_create() {
  Shader vertex = shader_from_file("shaders/pbr/vert.glsl", VERTEX_SHADER);
  Shader fragment = shader_from_file("shaders/pbr/frag.glsl", FRAGMENT_SHADER);

  Shader shaders[] = {vertex, fragment};
  PipelineCreateInfo info = {
      .shaders = shaders,
      .length = 2,
      .delete = true,
  };

  PbrPipeline pipeline = {
      .pipeline = pipeline_create(&info),
  };
  return pipeline;
}

void pbr_pipeline_destroy(const PbrPipeline *pipeline) {
  pipeline_destroy(&pipeline->pipeline);
}

void pbr_pipeline_bind(const PbrPipeline *pipeline) {
  pipeline_bind(&pipeline->pipeline);
}
