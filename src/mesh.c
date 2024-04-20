#include "mesh.h"

#include <string.h>
#include <sys/mman.h>

/*#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader.h>*/

/*static void get_file_data(void *ctx, const char *filename, const int is_mtl,
                          const char *obj_filename, char **data, size_t *len) {

  if (!filename) {
    fprintf(stderr, "null filename\n");
    (*data) = NULL;
    (*len) = 0;
    return;
  }

  FileResult res = file_load(obj_filename, "r");
  *data = res.contents;
  *len = res.lenght;

  char *content = (char *)ctx;
  content = res.contents;
  // to shut up clang/gcc
  (void)content;
}*/

Mesh *mesh_create(const Vertex *vertices, uint32_t vertex_count,
                  const uint32_t *indices, uint32_t index_count) {
  Mesh *mesh = malloc(sizeof(Mesh));
  if (indices != NULL) {
    mesh->indices_length = index_count;
    mesh->indices = malloc(sizeof(uint32_t) * index_count);
    memcpy(mesh->indices, indices, sizeof(uint32_t) * index_count);
    mesh->index_buffer =
        buffer_create_data(ELEMENT_ARRAY_BUFFER, STATIC_DRAW,
                           sizeof(uint32_t) * index_count, mesh->indices);
  } else {
    mesh->indices = NULL;
    mesh->indices_length = 0;
    mesh->index_buffer.buffer_id = 0;
  }

  if (vertices != NULL) {
    mesh->vertices_length = vertex_count;
    mesh->vertices = malloc(sizeof(Vertex) * vertex_count);
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * vertex_count);
    mesh->vertex_buffer =
        buffer_create_data(ARRAY_BUFFER, STATIC_DRAW,
                           sizeof(Vertex) * vertex_count, mesh->vertices);
  } else {
    mesh->vertices = NULL;
    mesh->vertices_length = 0;
    mesh->vertex_buffer.buffer_id = 0;
  }

  glm_mat4_identity(mesh->transform);
  return mesh;
}

Mesh *mesh_create_prealloced(Vertex *vertices, uint32_t vertex_count,
                             uint32_t *indices, uint32_t index_count) {

  Mesh *mesh = malloc(sizeof(Mesh));
  if (indices != NULL) {
    mesh->indices_length = index_count;
    mesh->indices = indices;
    mesh->index_buffer =
        buffer_create_data(ELEMENT_ARRAY_BUFFER, STATIC_DRAW,
                           sizeof(uint32_t) * index_count, mesh->indices);
  } else {
    mesh->indices = NULL;
    mesh->indices_length = 0;
    mesh->index_buffer.buffer_id = 0;
  }

  if (vertices != NULL) {
    mesh->vertices_length = vertex_count;
    mesh->vertices = vertices;
    mesh->vertex_buffer =
        buffer_create_data(ARRAY_BUFFER, STATIC_DRAW,
                           sizeof(Vertex) * vertex_count, mesh->vertices);
  } else {
    mesh->vertices = NULL;
    mesh->vertices_length = 0;
    mesh->vertex_buffer.buffer_id = 0;
  }

  glm_mat4_identity(mesh->transform);

  return mesh;
}

Mesh *mesh_create_empty(const uint32_t vertex_count, uint32_t index_count) {

  Mesh *mesh = malloc(sizeof(Mesh));
  mesh->indices_length = index_count;
  mesh->indices = malloc(sizeof(uint32_t) * index_count);

  mesh->vertices_length = vertex_count;
  mesh->vertices = malloc(sizeof(Vertex) * vertex_count);

  glm_mat4_identity(mesh->transform);
  return mesh;
}

void mesh_draw(const Mesh *mesh, const Pipeline *pipeline) {
  pipeline_set_mat4(pipeline, "transform", &mesh->transform);
  if (mesh->indices_length == 0) {
    buffer_bind(&mesh->vertex_buffer);
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertices_length);
  } else {
    buffer_bind(&mesh->vertex_buffer);
    buffer_bind(&mesh->index_buffer);
    glDrawElements(GL_TRIANGLES, mesh->indices_length, GL_UNSIGNED_INT, 0);
  }
}

void mesh_draw_instanced(const Mesh *mesh, uint32_t count,
                         const Pipeline *pipeline) {

  pipeline_set_mat4(pipeline, "transform", &mesh->transform);
  if (mesh->indices == 0) {
    glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->vertices_length, count);
  } else {
    glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_length, GL_UNSIGNED_INT,
                            0, count);
  }
}

void mesh_destroy(Mesh *mesh) {
  if (mesh != NULL) {
    if (mesh->indices != NULL) {
      free(mesh->indices);
    }
    if (mesh->vertices != NULL) {
      free(mesh->vertices);
    }

    if (mesh->index_buffer.buffer_id != 0) {
      buffer_destroy(&mesh->index_buffer);
    }
    if (mesh->vertex_buffer.buffer_id != 0) {
      buffer_destroy(&mesh->vertex_buffer);
    }

    free(mesh);
  }
}
