#include "ibl.h"
#include "asset.h"
#include "log.h"
#include "models.h"
#include "pipeline/pipeline.h"
#include "pipeline/shader.h"

Texture ibl_generate_irradiance(const char *file, int resolution,
                                Texture *env_texture,
                                Texture *prefilter_texture,
                                Texture *brdf_texture) {
  LOG("generating env map");

  Shader frag = shader_from_file("shaders/ibl/frag.glsl", FRAGMENT_SHADER);
  Shader vert = shader_from_file("shaders/ibl/vert.glsl", VERTEX_SHADER);
  Shader shaders[2] = {frag, vert};
  PipelineCreateInfo create_info = {
      .shaders = shaders,
      .length = 2,
  };
  Pipeline equirectangularToCubemapShader = pipeline_create(&create_info);

  unsigned int captureFBO, captureRBO;
  glGenFramebuffers(1, &captureFBO);
  glGenRenderbuffers(1, &captureRBO);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution,
                        resolution);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, captureRBO);

  int res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (res != GL_FRAMEBUFFER_COMPLETE) {
    LOG_ERR("frame buffer is not complete: %d", res);
  }

  Texture hdr_texture = asset_load_hdr(file);
  unsigned int env_cubemap;
  glGenTextures(1, &env_cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution,
                 resolution, 0, GL_RGB, GL_FLOAT, NULL);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  mat4 capture_projection;
  glm_perspective(glm_rad(90.0f), 1.0f, 0.1f, 10.0f, capture_projection);

  mat4 capture_views[6];
  glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){1.0f, 0.0f, 0.0f},
             (vec3){0.0f, -1.0f, 0.0f}, capture_views[0]);
  glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){-1.0f, 0.0f, 0.0f},
             (vec3){0.0f, -1.0f, 0.0f}, capture_views[1]);
  glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f},
             (vec3){0.0f, 0.0f, 1.0f}, capture_views[2]);
  glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, -1.0f, 0.0f},
             (vec3){0.0f, 0.0f, -1.0f}, capture_views[3]);
  glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f},
             (vec3){0.0f, -1.0f, 0.0f}, capture_views[4]);
  glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, -1.0f},
             (vec3){0.0f, -1.0f, 0.0f}, capture_views[5]);

  // convert HDR equirectangular environment map to cubemap equivalent
  pipeline_bind(&equirectangularToCubemapShader);
  pipeline_set_int(&equirectangularToCubemapShader, "equirectangular_map", 0);
  pipeline_set_mat4(&equirectangularToCubemapShader, "projection",
                    &capture_projection);
  glActiveTexture(GL_TEXTURE0);
  texture_bind(&hdr_texture);

  glViewport(0, 0, resolution,
             resolution); // don't forget to configure the viewport to the
                          // capture dimensions.
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {

    // pipeline_bind(&equirectangularToCubemapShader);
    pipeline_set_mat4(&equirectangularToCubemapShader, "view",
                      &capture_views[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_cube();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  unsigned int prefilter_map;
  glGenTextures(1, &prefilter_map);
  glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_map);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0,
                 GL_RGB, GL_FLOAT, NULL);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  Shader prefilter_frag =
      shader_from_file("shaders/ibl/prefilter_frag.glsl", FRAGMENT_SHADER);
  Shader prefilter_vert =
      shader_from_file("shaders/ibl/vert.glsl", VERTEX_SHADER);
  Shader prefilter_shaders[2] = {prefilter_frag, prefilter_vert};
  PipelineCreateInfo prefilter_create_info = {
      .shaders = prefilter_shaders,
      .length = 2,
  };
  Pipeline prefilter_pipeline = pipeline_create(&prefilter_create_info);

  pipeline_set_int(&prefilter_pipeline, "environment_map", 0);
  pipeline_set_mat4(&prefilter_pipeline, "projection", &capture_projection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  unsigned int maxMipLevels = 5;
  for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
    // reisze framebuffer according to mip-level size.
    unsigned int mipWidth = 128 * pow(0.5, mip);
    unsigned int mipHeight = 128 * pow(0.5, mip);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth,
                          mipHeight);
    glViewport(0, 0, mipWidth, mipHeight);
    LOG("mip: %d,width: %d,height: %d", mip, mipWidth, mipHeight);

    float roughness = (float)mip / (float)(maxMipLevels - 1);
    pipeline_set_float(&prefilter_pipeline, "roughness", roughness);
    for (unsigned int i = 0; i < 6; ++i) {

      pipeline_set_mat4(&prefilter_pipeline, "view", &capture_views[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map,
                             mip);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      render_cube();
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  vert = shader_from_file("shaders/ibl/vert.glsl", VERTEX_SHADER);
  Shader irr_frag =
      shader_from_file("shaders/ibl/irradiance_frag.glsl", FRAGMENT_SHADER);
  Shader shaders_irr[2] = {vert, irr_frag};
  PipelineCreateInfo irr_create_info = {
      .shaders = shaders_irr,
      .length = 2,
  };
  Pipeline irradiance_pipeline = pipeline_create(&irr_create_info);

  unsigned int irradiance_map;
  glGenTextures(1, &irradiance_map);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
                 GL_RGB, GL_FLOAT, NULL);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
  pipeline_bind(&irradiance_pipeline);
  pipeline_set_int(&irradiance_pipeline, "environment_map", 0);
  pipeline_set_mat4(&irradiance_pipeline, "projection", &capture_projection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

  glViewport(0, 0, 32,
             32); // don't forget to configure the viewport to the
                  // capture dimensions.
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {

    pipeline_set_mat4(&irradiance_pipeline, "view", &capture_views[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map,
                           0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_cube();
  }
  unsigned int brdfLUTTexture;
  glGenTextures(1, &brdfLUTTexture);

  // pre-allocate enough memory for the LUT texture.
  glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Shader brdf_frag =
      shader_from_file("shaders/ibl/brdf_frag.glsl", FRAGMENT_SHADER);
  Shader brdf_vert =
      shader_from_file("shaders/ibl/brdf_vert.glsl", VERTEX_SHADER);
  Shader brdf_shaders[2] = {brdf_frag, brdf_vert};
  PipelineCreateInfo brdf_create_info = {
      .shaders = brdf_shaders,
      .length = 2,
  };
  Pipeline brdf_pipeline = pipeline_create(&brdf_create_info);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         brdfLUTTexture, 0);

  glViewport(0, 0, 512, 512);
  pipeline_bind(&brdf_pipeline);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  render_quad();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if (env_texture != NULL) {
    env_texture->type = TEXTURE_CUBE_MAP;
    env_texture->texture_id = env_cubemap;
  }

  if (prefilter_texture != NULL) {
    prefilter_texture->type = TEXTURE_CUBE_MAP;
    prefilter_texture->texture_id = prefilter_map;
  }

  if (brdf_texture != NULL) {
    brdf_texture->type = TEXTURE_2D;
    brdf_texture->texture_id = brdfLUTTexture;
  }

  Texture texture = {
      .type = GL_TEXTURE_CUBE_MAP,
      .texture_id = irradiance_map,
  };
  return texture;
}
