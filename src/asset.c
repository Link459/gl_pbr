#include "asset.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

FileResult asset_load_file(const char *file_path, const char *access) {
  FILE *file = fopen(file_path, access);
  FileResult res = {};
  if (file == NULL) {
    fprintf(stderr, "Failed to open file: %s", file_path);
    return res;
  }

  char *source = NULL;
  uint32_t lenght = 0;
  /* Go to the end of the file. */
  if (fseek(file, 0L, SEEK_END) == 0) {
    /* Get the size of the file. */
    long bufsize = ftell(file);
    if (bufsize == -1) { /* Error */
      return res;
    }

    /* Allocate our buffer to that size. */
    source = malloc(sizeof(char) * (bufsize + 1));

    /* Go back to the start of the file. */
    if (fseek(file, 0L, SEEK_SET) != 0) { /* Error */
      exit(1);
    }

    /* Read the entire file into memory. */
    size_t newLen = fread(source, sizeof(char), bufsize, file);
    if (ferror(file) != 0) {
      fprintf(stderr, "Error reading file: %s", file_path);
    } else {
      source[newLen++] = '\0'; /* Just to be safe. */
    }
    lenght = bufsize;
  }
  fclose(file);
  res.contents = source;
  res.lenght = lenght;
  return res;
}

static void get_file_data(void *ctx, const char *filename, const int is_mtl,
                          const char *obj_filename, char **data, size_t *len) {
  // NOTE: If you allocate the buffer with malloc(),
  // You can define your own memory management struct and pass it through `ctx`
  // to store the pointer and free memories at clean up stage(when you quit an
  // app)
  // This example uses mmap(), so no free() required.
  (void)ctx;

  if (!filename) {
    fprintf(stderr, "null filename\n");
    (*data) = NULL;
    (*len) = 0;
    return;
  }

  FileResult file = asset_load_file(filename, "rb");
  (*data) = file.contents;
  (*len) = file.lenght;
}

Mesh *asset_load_mesh(const char *file_path) {
  tinyobj_attrib_t attrib;
  tinyobj_shape_t *shapes = NULL;
  size_t num_shapes;
  tinyobj_material_t *materials = NULL;
  size_t num_materials;

  unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
  char *content = NULL;
  int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                              &num_materials, file_path, get_file_data, content,
                              flags);

  if (ret == -1) {
    return NULL;
  }

  Vertex *vertices = malloc(sizeof(Vertex) * attrib.num_vertices);
  for (int j = 0; j < attrib.num_vertices; j++) {
    Vertex *vertex = &vertices[j];
    tinyobj_vertex_index_t *v = &attrib.faces[j];
    vertex->pos[0] = attrib.vertices[v->v_idx + 0];
    vertex->pos[1] = attrib.vertices[v->v_idx + 1];
    vertex->pos[2] = attrib.vertices[v->v_idx + 2];
    glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, vertex->color);
  }

  for (int j = 0; j < attrib.num_normals; j++) {
    Vertex *vertex = &vertices[j];
    tinyobj_vertex_index_t *v = &attrib.faces[j];

    vertex->normal[0] = attrib.normals[v->vn_idx + 0];
    vertex->normal[1] = attrib.normals[v->vn_idx + 1];
    vertex->normal[2] = attrib.normals[v->vn_idx + 2];
  }


  /*Mesh *mesh = mesh_create_prealloced(
      attrib.num_vertices / *attrib.face_num_verts, attrib.num_vertices);*/

  Mesh *mesh = mesh_create_prealloced(vertices, attrib.num_vertices, NULL, 0);
  tinyobj_attrib_free(&attrib);
  tinyobj_materials_free(materials, num_materials);
  tinyobj_shapes_free(shapes, num_shapes);

  if (content != NULL) {
    free(content);
  }

  return mesh;
}

Texture asset_load_texture(const TextureCreateInfo *info,
                           const char *file_path) {
  Texture texture = texture_create(info);
  int width, height, nr_channels;
  unsigned char *data = stbi_load(file_path, &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    fprintf(stderr, "failed to load image: %s\n", file_path);
  }
  stbi_image_free(data);

  return texture;
}
