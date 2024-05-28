#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/types.h"
#include "pipeline/pipeline.h"
#include "window.h"

typedef enum {
  ORTHOGRAPHIC,
  PERSPECTIVE,
} CameraType;

typedef struct {
  CameraType type;
  float left;
  float right;
  float bottom;
  float top;
  float near_z;
  float far_z;
  float fov;
  float aspect;

  Window *window;
} CameraCreateInfo;

typedef struct {
  mat4 view;
  mat4 projection;
  vec3 position;
  vec3 front;
  vec3 up;
} Camera;

typedef struct {
  int first_mouse;
  float width;
  float height;
  float last_x;
  float last_y;
  float yaw;
  float pitch;
  float fov;
  Camera *camera;
} WindowInfo;

Camera *camera_create(const CameraCreateInfo *create_info);
void camera_bind(const Camera *camera, const Pipeline *pipeline);

void process_camera_input(Camera *camera, const Window *window,
                          const Pipeline *pipeline);
#endif /* CAMERA_H */
