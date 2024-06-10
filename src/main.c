#include <glad/glad.h>

#include <unistd.h>

#include "camera.h"
#include "ibl.h"
#include "log.h"
#include "mesh.h"
#include "models.h"
#include "pbr/pbr_material.h"
#include "pbr/pbr_pipeline.h"
#include "pipeline/pipeline.h"
#include "vertex_array.h"
#include "window.h"

void process_input(GLFWwindow *window);
void render_cube();

int main() {
  LOG("creating window");
  Window *window = window_create();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERR("failed to load opengl");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_MULTISAMPLE);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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

  pipeline_set_int(&pipeline.pipeline, "irradiance_map", 5);

  vec3 light_position = {0.0f, 0.0f, 10.0f};
  vec3 light_color = {150.0f, 150.0f, 150.0f};

  Texture env_texture = {};
  Texture prefilter_texture = {};
  Texture brdf_texture = {};
  Texture irradiance_map =
      ibl_generate_irradiance("assets/new_port_loft.hdr", 512, &env_texture,
                              &prefilter_texture, &brdf_texture);

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

  PbrMaterial material = {};
  Mesh *mesh = model_damaged_helmet(&material);
  vertex_attributes();

  float axis[3] = {0.0, 1.0, 0.0};
  glm_rotate(mesh->transform, glm_rad(70), axis);

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
    glActiveTexture(GL_TEXTURE6);
    texture_bind(&irradiance_map);
    glActiveTexture(GL_TEXTURE7);
    texture_bind(&prefilter_texture);
    glActiveTexture(GL_TEXTURE8);
    texture_bind(&brdf_texture);

    vertex_array_bind(&vao);
    mesh_draw(mesh, &pipeline.pipeline);

    glDepthFunc(GL_LEQUAL);
    pipeline_bind(&skybox);
    camera_bind(camera, &skybox);
    glActiveTexture(GL_TEXTURE0);
    texture_bind(&env_texture);
    render_cube();
    glDepthFunc(GL_LESS);

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
