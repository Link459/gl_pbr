#ifndef VERTEX_H
#define VERTEX_H

#include <cglm/struct.h>

typedef struct {
  vec3 pos;
  vec3 normal;
  //vec3 color;
  vec2 tex_coord;
} Vertex;

void vertex_attributes();
void set_attribute();
void vertex_from_positions(vec3 *src, Vertex *dst, uint32_t len, vec3 color);
void vertex_from_positions_and_normal(float ***src, Vertex *dst, uint32_t len,
                                      vec3 color);

#endif /* VERTEX_H */
