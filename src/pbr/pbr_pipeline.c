#include "pbr_pipeline.h"

PbrPipeline pbr_pipeline_create() {
  Shader vertex = shader_from_file("shaders/pbr/vert.glsl", VERTEX_SHADER);
  Shader fragment =
      shader_from_file("shaders/pbr/tex_frag.glsl", FRAGMENT_SHADER);

  Shader shaders[] = {vertex, fragment};
  PipelineCreateInfo info = {
      .shaders = shaders,
      .length = 2,
      .delete = true,
  };

  PbrPipeline pipeline = {
      .pipeline = pipeline_create(&info),
  };

  pbr_pipeline_bind(&pipeline);
  pipeline_set_int(&pipeline.pipeline, "albedo_map", 0);
  pipeline_set_int(&pipeline.pipeline, "normal_map", 1);
  pipeline_set_int(&pipeline.pipeline, "metallic_map", 2);
  pipeline_set_int(&pipeline.pipeline, "roughness_map", 3);
  pipeline_set_int(&pipeline.pipeline, "ao_map", 4);
  pipeline_set_int(&pipeline.pipeline, "irradiance_map", 5);
  pipeline_set_int(&pipeline.pipeline, "prefilter_map", 6);
  pipeline_set_int(&pipeline.pipeline, "brdf_lut", 7);
  return pipeline;
}

void pbr_pipeline_destroy(const PbrPipeline *pipeline) {
  pipeline_destroy(&pipeline->pipeline);
}

void pbr_pipeline_bind(const PbrPipeline *pipeline) {
  pipeline_bind(&pipeline->pipeline);
}
