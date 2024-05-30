#include "camera.h"
#include "cglm/cam.h"
#include <stdlib.h>

Camera *camera_create(const CameraCreateInfo *info) {
  Camera *camera = malloc(sizeof(Camera));

  if (info->type == ORTHOGRAPHIC) {
    glm_ortho(info->left, info->right, info->bottom, info->top, info->near_z,
              info->far_z, camera->projection);
  } else if (info->type == PERSPECTIVE) {
    glm_perspective(glm_rad(info->fov), info->aspect, info->near_z, info->far_z,
                    camera->projection);
  }

  glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, camera->position);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);

  vec3 center;
  glm_vec3_add(camera->position, camera->front, center);
  glm_lookat(camera->position, center, camera->up, camera->view);

  WindowInfo window_info = {
      .last_x = 400.0f,
      .last_y = 300.0f,
      .yaw = -90.0f,
      .pitch = 0.0f,
      .first_mouse = 1.0f,
      .fov = 45.0f,
      .camera = camera,
  };

  glfwSetWindowUserPointer(info->window->window, &window_info);
  /*WindowInfo window_info = {
      .last_x = 400.0f,
      .last_y = 300.0f,
      .yaw = -90.0f,
      .pitch = 0.0f,
      .first_mouse = 1.0f,
      .fov = 45.0f,
      .camera = camera,
  };

  glfwSetWindowUserPointer(info->window->window, &window_info);*/
  return camera;
}

void camera_bind(const Camera *camera, const Pipeline *pipeline) {
  pipeline_set_mat4(pipeline, "projection", &camera->projection);
  pipeline_set_mat4(pipeline, "view", &camera->view);
  pipeline_set_vec3(pipeline, "camera_pos", &camera->position);
}

void process_camera_input(Camera *camera, const Window *window,
                          const Pipeline *pipeline) {
  const float speed = 0.05f;
  if (glfwGetKey(window->window, GLFW_KEY_UP) == GLFW_PRESS)

    glm_vec3_muladds(camera->front, speed, camera->position);
  if (glfwGetKey(window->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    vec3 cross;
    glm_vec3_cross(camera->front, camera->up, cross);
    glm_vec3_normalize(cross);
    glm_vec3_muladds(cross, speed, camera->position);
  }
  if (glfwGetKey(window->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    vec3 tmp;
    vec3 cross;
    glm_vec3_cross(camera->front, camera->up, cross);
    glm_vec3_normalize(cross);
    glm_vec3_scale(cross, speed, tmp);
    glm_vec3_sub(camera->position, tmp, camera->position);
  }
  if (glfwGetKey(window->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    vec3 tmp;
    glm_vec3_scale(camera->front, speed, tmp);
    glm_vec3_sub(camera->position, tmp, camera->position);
  }

  vec3 center;
  glm_vec3_add(camera->position, camera->front, center);
  glm_lookat(camera->position, center, camera->up, camera->view);
  camera_bind(camera, pipeline);
}
