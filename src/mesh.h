#ifndef MESH_H
#define MESH_H

#include "buffer.h"
#include "pipeline.h"
#include "vertex.h"
#include <cglm/struct.h>

typedef struct {
  Vertex *vertices;
  uint32_t vertices_length;
  uint32_t *indices;
  uint32_t indices_length;
  Buffer vertex_buffer;
  Buffer index_buffer;
  mat4 transform;
} Mesh;

Mesh *mesh_create(const Vertex *vertices, uint32_t vertex_count,
                  const uint32_t *indices, uint32_t index_count);

Mesh *mesh_create_prealloced(Vertex *vertices, uint32_t vertex_count,
                             uint32_t *indices, uint32_t index_count);
Mesh *mesh_create_empty(const uint32_t vertex_count, uint32_t index_count);
void mesh_update();

void mesh_draw(const Mesh *mesh, const Pipeline *pipeline);
void mesh_draw_instanced(const Mesh *mesh, uint32_t count,
                         const Pipeline *pipeline);
void mesh_destroy(Mesh *mesh);

#endif /* MESH_H */
