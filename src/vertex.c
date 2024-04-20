#include "vertex.h"
#include "vertex_array.h"

void vertex_attributes() {
  // WARNING: be careful about the stride

  // position attribute
  vertex_array_set_attribute(0, 3, GL_FLOAT, 9 * sizeof(float), (void *)0);
  // color attribute
  vertex_array_set_attribute(1, 3, GL_FLOAT, 9 * sizeof(float),
                             (void *)(3 * sizeof(float)));

  vertex_array_set_attribute(2, 3, GL_FLOAT, 9 * sizeof(float),
                             (void *)(6 * sizeof(float)));
  // texture coord attribute
  //  vertex_array_set_attribute(2, 2, GL_FLOAT, 8 * sizeof(float),
  //                            (void *)(6 * sizeof(float)));
}

void vertex_from_positions(vec3 *src, Vertex *dst, uint32_t len, vec3 color) {
  for (uint32_t i = 0; i < len; i++) {
    Vertex v;
    glm_vec3_copy(src[i], v.pos);
    glm_vec3_copy(color, v.color);
    dst[i] = v;
  }
}

void vertex_from_positions_and_normal(float ***src, Vertex *dst, uint32_t len,
                                      vec3 color) {
  for (uint32_t i = 0; i < len; i++) {
    printf("start");
    Vertex v;
    for (int j = 0; j < 3; j++) {
      //  fprintf(stderr, "src[%d][pos][%d]: %f", i, j, v.pos[j]);
    }
    glm_vec3_copy(src[i][0], v.pos);
    // glm_vec3_copy(src[i][1], v.normal);
    glm_vec3_copy(color, v.color);
    for (int j = 0; j < 3; j++) {
      // fprintf(stderr, "src[%d][pos][%d]: %f", i, j, v.pos[j]);
    }
    dst[i] = v;
  }
}
