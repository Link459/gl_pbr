#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

typedef struct {
  GLFWwindow *window;
  uint32_t width;
  uint32_t height;
} Window;

Window *window_create();
void window_free(Window *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window,double xoffset, double yoffset);

#endif /* WINDOW_H */
