#include "camera.h"
// needs this order
#include "log.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

Window *window_create() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *glfw_window =
      glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (glfw_window == NULL) {
    LOG_ERR("Failed to create GLFW window");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(glfw_window);
  glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
  glfwSetCursorPosCallback(glfw_window, mouse_callback);
  glfwSetScrollCallback(glfw_window, scroll_callback);
  Window *window = malloc(sizeof(Window));
  window->window = glfw_window;
  window->width = 800;
  window->height = 600;
  return window;
}

void window_free(Window *window) {
  glfwDestroyWindow(window->window);
  glfwTerminate();
  free(window);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  WindowInfo *info = glfwGetWindowUserPointer(window);
  info->width = width;
  info->height = height;
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  WindowInfo *info = glfwGetWindowUserPointer(window);
  if (info->first_mouse) {
    info->last_x = xpos;
    info->last_y = ypos;
    info->first_mouse = 0;
  }

  float xoffset = xpos - info->last_x;
  float yoffset = info->last_y - ypos;
  info->last_x = xpos;
  info->last_y = ypos;

  float sensitivity = 0.3f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  info->yaw += xoffset;
  info->pitch += yoffset;

  if (info->pitch > 89.0f)
    info->pitch = 89.0f;
  if (info->pitch < -89.0f)
    info->pitch = -89.0f;

  vec3 direction;
  direction[0] = cos(glm_rad(info->yaw)) * cos(glm_rad(info->pitch));
  direction[1] = sin(glm_rad(info->pitch));
  direction[2] = sin(glm_rad(info->yaw)) * cos(glm_rad(info->pitch));
  glm_normalize(direction);
  if (info->camera != NULL) {
    glm_vec3_copy(direction, info->camera->front);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  WindowInfo *info = glfwGetWindowUserPointer(window);
  info->fov -= (float)yoffset;
  if (info->fov < 1.0f)
    info->fov = 1.0f;
  if (info->fov > 45.0f)
    info->fov = 45.0f;
  if (info->camera != NULL) {
    glm_perspective(glm_rad(info->fov), info->width / info->height, 0.1f,
                    100.0f, info->camera->projection);
  }
}
