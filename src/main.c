#include <glad/glad.h>
// needs this ordering to stop glad from complaining
// #define STB_IMAGE_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "asset.h"
#include "buffer.h"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "models.h"
#include "pipeline/pipeline.h"
#include "pipeline/shader.h"
#include "scene.h"
#include "texture.h"
#include "vertex_array.h"
#include "window.h"

void process_input(GLFWwindow *window);

int main() {
  Window *window = window_create();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to load opengl");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  int width, height;
  glfwGetWindowSize(window->window, &width, &height);
  CameraCreateInfo camera_info = {
      .type = PERSPECTIVE,
      .fov = 45.0f,
      .aspect = window->width / window->height,
      .near_z = 0.1f,
      .far_z = 100.0f,
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

  Shader vertex_shader = shader_from_file("shaders/vert.glsl", VERTEX_SHADER);
  Shader fragment_shader =
      shader_from_file("shaders/frag.glsl", FRAGMENT_SHADER);

  Shader shaders[2] = {vertex_shader, fragment_shader};
  PipelineCreateInfo pipeline_info = {.shaders = shaders, .length = 2};
  Pipeline pipeline = pipeline_create(&pipeline_info);
  shader_destroy(vertex_shader);
  shader_destroy(fragment_shader);

  Shader light_vertex_shader =
      shader_from_file("shaders/light_vert.glsl", VERTEX_SHADER);
  Shader light_fragment_shader =
      shader_from_file("shaders/light_frag.glsl", FRAGMENT_SHADER);
  Shader light_shaders[2] = {light_vertex_shader, light_fragment_shader};
  PipelineCreateInfo light_pipeline_info = {.shaders = light_shaders,
                                            .length = 2};
  Pipeline light_pipeline = pipeline_create(&light_pipeline_info);

  Material material = {
      .ambient = {1.0f, 0.5f, 0.31f},
      .diffuse = {1.0f, 0.5f, 0.31f},
      .specular = {0.5f, 0.5f, 0.5f},
      .shininess = 32.0f,
  };

  Light light = {
      .position = {1.2f, 1.0f, 2.0f},
      .ambient = {0.2f, 0.2f, 0.2f},
      .diffuse = {0.5f, 0.5f, 0.5f},
      .specular = {1.0f, 1.0f, 1.0f},
  };

  VertexArray vao = vertex_array_create();

  Mesh *mesh = model_cube_with_normals((vec3){1.0f, 0.5f, 0.31f});
  vertex_attributes();

  VertexArray light_vao = vertex_array_create();
  Mesh *light_mesh = model_cube_with_normals((vec3){1.0, 1.0, 1.0});
  vertex_attributes();

  glm_translate(light_mesh->transform, light.position);
  glm_scale(light_mesh->transform, (vec3){0.2, 0.2, 0.2});

  while (!glfwWindowShouldClose(window->window)) {
    printf("loop");
    process_input(window->window);
    process_camera_input(camera, window, &pipeline);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pipeline_bind(&pipeline);
    camera_bind(camera, &pipeline);

    vec3 light_color;
    light_color[0] = sin(glfwGetTime() * 2.0f);
    light_color[1] = sin(glfwGetTime() * 0.7f);
    light_color[2] = sin(glfwGetTime() * 1.3f);

    // light.diffuse
    glm_vec3_mul(light_color, (vec3){0.5f, 0.5f, 0.5f}, light.diffuse);
    glm_vec3_mul(light_color, (vec3){0.2f, 0.2f, 0.2f}, light.ambient);

    pipeline_set_vec3(&pipeline, "view_pos", &camera->position);
    material_bind(&material, &pipeline);
    light_bind(&light, &pipeline);
    vertex_array_bind(&vao);
    mesh_draw(mesh, &pipeline);

    pipeline_bind(&light_pipeline);
    camera_bind(camera, &light_pipeline);
    vertex_array_bind(&light_vao);
    mesh_draw(light_mesh, &light_pipeline);

    glfwSwapBuffers(window->window);
    glfwPollEvents();
  }

  vertex_array_destroy(&vao);
  vertex_array_destroy(&light_vao);
  mesh_destroy(mesh);
  mesh_destroy(light_mesh);
  pipeline_destroy(&pipeline);
  pipeline_destroy(&light_pipeline);
  free(camera);
  window_free(window);
  return 0;
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}
