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

int main() {
  Window *window = window_create();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERR("failed to load opengl");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

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

  PbrPipeline pipeline = pbr_pipeline_create();

  PbrMaterial material = {};
  TextureCreateInfo info = texture_info_default();

  material.albedo = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");

  material.normal = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga");

  material.metallic = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga");
  material.roughness = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga");

  vec3 light_position = {0.0f, 0.0f, 10.0f};
  vec3 light_color = {150.0f, 150.0f, 150.0f};

  VertexArray vao = vertex_array_create();

  Mesh *mesh = asset_load_mesh("assets/Cerberus/cerberus.obj");
  vertex_attributes();

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

    pipeline_bind(&pipeline.pipeline);
    vertex_array_bind(&vao);

    float axis[3] = {0.0, 1.0, 0.0};
    glm_rotate(mesh->transform, 0.001, axis);

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
