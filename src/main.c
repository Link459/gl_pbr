#include <glad/glad.h>

#include <unistd.h>

#include "asset.h"
#include "camera.h"
#include "log.h"
#include "mesh.h"
#include "pbr/pbr_material.h"
#include "pbr/pbr_pipeline.h"
#include "pipeline/pipeline.h"
#include "vertex_array.h"
#include "window.h"

void process_input(GLFWwindow *window);
void renderCube();

Texture ibl() {
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
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, captureRBO);

  int res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (res != GL_FRAMEBUFFER_COMPLETE) {
    LOG_ERR("frame buffer is not complete: %d", res);
  }

  Texture hdr_texture = asset_load_hdr("assets/new_port_loft.hdr");
  unsigned int env_cubemap;
  glGenTextures(1, &env_cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0,
                 GL_RGB, GL_FLOAT, NULL);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  mat4 captureProjection;
  glm_perspective(glm_rad(90.0f), 1.0f, 0.1f, 10.0f, captureProjection);

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
                    &captureProjection);
  glActiveTexture(GL_TEXTURE0);
  texture_bind(&hdr_texture);

  glViewport(0, 0, 512,
             512); // don't forget to configure the viewport to the
                   // capture dimensions.
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {

    // pipeline_bind(&equirectangularToCubemapShader);
    pipeline_set_mat4(&equirectangularToCubemapShader, "view",
                      &capture_views[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderCube();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  Texture texture = {
      .type = GL_TEXTURE_CUBE_MAP,
      .texture_id = env_cubemap,
  };
  return texture;
}

int main() {
  LOG("creating window");
  Window *window = window_create();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERR("failed to load opengl");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_MULTISAMPLE);

  int width, height;
  glfwGetWindowSize(window->window, &width, &height);
  CameraCreateInfo camera_info = {
      .type = PERSPECTIVE,
      .fov = 45.0f,
      .aspect = window->width / window->height, // NOLINT
      .near_z = 0.1f,
      .far_z = 100.0f,
      .window = window,
  };

  Camera *camera = camera_create(&camera_info);

  WindowInfo window_info = {
      .last_x = 400.0f,
      .last_y = 300.0f,
      .yaw = -90.0f,
      .pitch = 0.0f,
      .first_mouse = 1.0f,
      .fov = 45.0f,
      .camera = camera,
  };

  glfwSetWindowUserPointer(window->window, &window_info);

  LOG("Loading Resources");
  PbrPipeline pipeline = pbr_pipeline_create();

  PbrMaterial material = {};
  TextureCreateInfo info = texture_info_default();

  LOG("Materials:");
  LOG_RAW("1/4");
  material.albedo = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");

  LOG_RAW("\r2/4");
  material.normal = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga");

  LOG_RAW("\r3/4");
  material.metallic = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga");
  LOG_RAW("\r4/4");
  material.roughness = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga");
  LOG_NL;

  vec3 light_position = {0.0f, 0.0f, 10.0f};
  vec3 light_color = {150.0f, 150.0f, 150.0f};

  Texture env_texture = ibl();

  glViewport(0, 0, width, height);
  Shader frag = shader_from_file("shaders/skybox/frag.glsl", FRAGMENT_SHADER);
  Shader vert = shader_from_file("shaders/skybox/vert.glsl", VERTEX_SHADER);
  Shader shaders[2] = {frag, vert};
  PipelineCreateInfo create_info = {
      .shaders = shaders,
      .length = 2,
  };
  Pipeline skybox = pipeline_create(&create_info);
  pipeline_bind(&skybox);
  pipeline_set_int(&skybox, "environment_map", 0);

  VertexArray vao = vertex_array_create();

  LOG("loading model");
  Mesh *mesh = asset_load_mesh("assets/Cerberus/cerberus.obj");
  vertex_attributes();

  LOG("rendering begins");
  while (!glfwWindowShouldClose(window->window)) {
    process_input(window->window);
    process_camera_input(camera, window, &pipeline.pipeline);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pipeline_bind(&pipeline.pipeline);
    camera_bind(camera, &pipeline.pipeline);

    pipeline_set_vec3(&pipeline.pipeline, "light_position", &light_position);
    pipeline_set_vec3(&pipeline.pipeline, "light_color", &light_color);

    pbr_material_bind(&pipeline, &material);

    vertex_array_bind(&vao);
    mesh_draw(mesh, &pipeline.pipeline);

    float axis[3] = {0.0, 1.0, 0.0};
    glm_rotate(mesh->transform, 0.001, axis);

    glDepthFunc(GL_LEQUAL);
    pipeline_bind(&skybox);
    camera_bind(camera, &skybox);
    glActiveTexture(GL_TEXTURE0);
    texture_bind(&env_texture);
    renderCube();
    glDepthFunc(GL_LESS);
    (void)env_texture;

    glfwSwapBuffers(window->window);
    glfwPollEvents();
  }

  vertex_array_destroy(&vao);
  mesh_destroy(mesh);
  pbr_material_destroy(&material);
  pipeline_destroy(&pipeline.pipeline);
  free(camera);
  window_free(window);
  return 0;
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube() {
  // initialize (if necessary)
  if (cubeVAO == 0) {
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                            // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // render Cube
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
