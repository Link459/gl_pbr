#include "asset.h"
#include "log.h"

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

FileResult asset_load_file(const char *file_path, const char *access) {
  FILE *file = fopen(file_path, access);
  FileResult res = {};
  if (file == NULL) {
    LOG_ERR("Failed to open file: %s", file_path);
    return res;
  }

  char *source = NULL;
  uint32_t lenght = 0;
  if (fseek(file, 0L, SEEK_END) == 0) {
    long bufsize = ftell(file);
    if (bufsize == -1) {
      return res;
    }

    source = malloc(sizeof(char) * (bufsize + 1));

    if (fseek(file, 0L, SEEK_SET) != 0) {
      exit(1);
    }

    size_t newLen = fread(source, sizeof(char), bufsize, file);
    if (ferror(file) != 0) {
      LOG_ERR("Error reading file: %s", file_path);
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
  (void)ctx;

  if (!filename) {
    LOG_ERR("null filename\n");
    (*data) = NULL;
    (*len) = 0;
    return;
  }

  FileResult file = asset_load_file(filename, "rb");
  (*data) = file.contents;
  (*len) = file.lenght;
}

static void calc_normal(vec3 N, vec3 v0, vec3 v1, vec3 v2) {
  float v10[3];
  float v20[3];
  float len2;

  v10[0] = v1[0] - v0[0];
  v10[1] = v1[1] - v0[1];
  v10[2] = v1[2] - v0[2];

  v20[0] = v2[0] - v0[0];
  v20[1] = v2[1] - v0[1];
  v20[2] = v2[2] - v0[2];

  N[0] = v20[1] * v10[2] - v20[2] * v10[1];
  N[1] = v20[2] * v10[0] - v20[0] * v10[2];
  N[2] = v20[0] * v10[1] - v20[1] * v10[0];

  len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
  if (len2 > 0.0f) {
    float len = (float)sqrt((double)len2);

    N[0] /= len;
    N[1] /= len;
  }
}

Mesh *asset_load_mesh(const char *file_path) {
  tinyobj_attrib_t attrib;
  tinyobj_shape_t *shapes = NULL;
  size_t num_shapes;
  tinyobj_material_t *materials = NULL;
  size_t num_materials;

  unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
  tinyobj_attrib_init(&attrib);

  int ret =
      tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                        &num_materials, file_path, get_file_data, NULL, flags);
  if (ret != TINYOBJ_SUCCESS) {
    PANIC("failed to load asset");
  }

  int vertex_count = attrib.num_faces;
  Vertex *vertices = malloc(sizeof(Vertex) * vertex_count);

  for (size_t i = 0; i < vertex_count; i++) {

    tinyobj_vertex_index_t idx = attrib.faces[i];
    Vertex *vertex = &vertices[i];
    // Position
    vertex->pos[0] = attrib.vertices[3 * idx.v_idx + 0];
    vertex->pos[1] = attrib.vertices[3 * idx.v_idx + 1];
    vertex->pos[2] = attrib.vertices[3 * idx.v_idx + 2];

    // Normal
    if (idx.vn_idx >= 0) {
      vertex->normal[0] = attrib.normals[3 * idx.vn_idx + 0];
      vertex->normal[1] = attrib.normals[3 * idx.vn_idx + 1];
      vertex->normal[2] = attrib.normals[3 * idx.vn_idx + 2];
    } else {
      calc_normal(vertex->normal, &attrib.vertices[idx.v_idx + 0],
                  &attrib.vertices[idx.v_idx + 1],
                  &attrib.vertices[idx.v_idx + 2]);
    }

    // Texture Coordinate
    if (idx.vt_idx >= 0) {
      vertex->tex_coord[0] = attrib.texcoords[2 * idx.vt_idx + 0];
      vertex->tex_coord[1] = 1 - attrib.texcoords[2 * idx.vt_idx + 1];
    } else {
      LOG_ERR("no texcoord");
      vertex->tex_coord[0] = 0.0f;
      vertex->tex_coord[1] = 0.0f;
    }
  }

  Mesh *mesh = mesh_create_prealloced(vertices, vertex_count, NULL, 0);

  // TODO: parse the material data

  tinyobj_attrib_free(&attrib);
  tinyobj_materials_free(materials, num_materials);
  tinyobj_shapes_free(shapes, num_shapes);

  return mesh;
}

Texture asset_load_texture(const TextureCreateInfo *info,
                           const char *file_path) {
  Texture texture = texture_create(info);
  int width, height, nr_channels;
  unsigned char *data = stbi_load(file_path, &width, &height, &nr_channels, 0);
  if (data) {

    GLenum format = GL_RED;
    if (nr_channels == 1)
      format = GL_RED;
    else if (nr_channels == 3)
      format = GL_RGB;
    else if (nr_channels == 4)
      format = GL_RGBA;
    /*switch (nr_channels) {
    case 1:
      format = GL_RED;
    case 3:
      format = GL_RGB;
    case 4:
      format = GL_RGBA;
    }*/

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LOG_ERR("failed to load image: %s\n", file_path);
  }
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

Texture asset_load_hdr(const char *file_path) {
  stbi_set_flip_vertically_on_load(true);
  TextureCreateInfo info = texture_info_default();
  info.filtering.mag_filtering = FILTERING_LINEAR;
  info.filtering.min_filtering = FILTERING_LINEAR;
  info.wrapping.wrap_s = WRAPPING_CLAMP_TO_EDGE;
  info.wrapping.wrap_t = WRAPPING_CLAMP_TO_EDGE;

  Texture texture = texture_create(&info);
  int width, height, nr_channels;
  float *data = stbi_loadf(file_path, &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, data);
  } else {
    LOG_ERR("failed to load image: %s\n", file_path);
  }
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_set_flip_vertically_on_load(false);
  return texture;
}
