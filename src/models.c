#include "models.h"

Mesh *model_flat_plane() {
  Vertex vertices[] = {
      {
          {0.5f, 0.5f, 0.0f},
          {1.0f, 0.0f, 0.0f},
      }, // top right
      {
          {0.5f, -0.5f, 0.0f},
          {0.0f, 1.0f, 0.0f},
      }, // bottom right
      {
          {-0.5f, -0.5f, 0.0f},
          {0.0f, 0.0f, 1.0f},
      }, // bottom left
      {
          {-0.5f, 0.5f, 0.0f},
          {1.0f, 1.0f, 0.0f},
      }, // top left
  };
  uint32_t indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  return mesh_create(vertices, 4, indices, 6);
}

Mesh *model_cube(vec3 color) {
  vec3 vertices_pos[] = {
      {-0.5f, -0.5f, -0.5f},
      {0.5f, -0.5f, -0.5f},
      {0.5f, 0.5f, -0.5f},
      {0.5f, 0.5f, -0.5f},
      {-0.5f, 0.5f, -0.5f},
      {-0.5f, -0.5f, -0.5f},

      {-0.5f, -0.5f, 0.5f},
      {0.5f, -0.5f, 0.5f},
      {0.5f, 0.5f, 0.5f},
      {
          0.5f,
          0.5f,
          0.5f,
      },
      {
          -0.5f,
          0.5f,
          0.5f,
      },
      {
          -0.5f,
          -0.5f,
          0.5f,
      },

      {
          -0.5f,
          0.5f,
          0.5f,
      },
      {
          -0.5f,
          0.5f,
          -0.5f,
      },
      {
          -0.5f,
          -0.5f,
          -0.5f,
      },
      {
          -0.5f,
          -0.5f,
          -0.5f,
      },
      {
          -0.5f,
          -0.5f,
          0.5f,
      },
      {
          -0.5f,
          0.5f,
          0.5f,
      },

      {
          0.5f,
          0.5f,
          0.5f,
      },
      {
          0.5f,
          0.5f,
          -0.5f,
      },
      {
          0.5f,
          -0.5f,
          -0.5f,
      },
      {
          0.5f,
          -0.5f,
          -0.5f,
      },
      {
          0.5f,
          -0.5f,
          0.5f,
      },
      {
          0.5f,
          0.5f,
          0.5f,
      },

      {
          -0.5f,
          -0.5f,
          -0.5f,
      },
      {
          0.5f,
          -0.5f,
          -0.5f,
      },
      {
          0.5f,
          -0.5f,
          0.5f,
      },
      {
          0.5f,
          -0.5f,
          0.5f,
      },
      {
          -0.5f,
          -0.5f,
          0.5f,
      },
      {
          -0.5f,
          -0.5f,
          -0.5f,
      },

      {
          -0.5f,
          0.5f,
          -0.5f,
      },
      {
          0.5f,
          0.5f,
          -0.5f,
      },
      {
          0.5f,
          0.5f,
          0.5f,
      },
      {
          0.5f,
          0.5f,
          0.5f,
      },
      {
          -0.5f,
          0.5f,
          0.5f,
      },
      {
          -0.5f,
          0.5f,
          -0.5f,
      },
  };

  Vertex vertices[36];
  vertex_from_positions(vertices_pos, vertices, 36, color);
  return mesh_create(vertices, 36, NULL, 0);
}

Mesh *model_cube_with_normals(vec3 color) {
  Vertex vertices[36] = {
      {{
           -0.5f,
           -0.5f,
           -0.5f,
       },
       {
           0.0f,
           0.0f,
           -1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           -0.5f,
       },
       {
           0.0f,
           0.0f,
           -1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           -0.5f,
       },
       {
           0.0f,
           0.0f,
           -1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           -0.5f,
       },
       {
           0.0f,
           0.0f,
           -1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           0.5f,
           -0.5f,
       },
       {
           0.0f,
           0.0f,
           -1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           -0.5f,
       },
       {
           0.0f,
           0.0f,
           -1.0f,
       },
       {color[0], color[1], color[2]}},

      {{
           -0.5f,
           -0.5f,
           0.5f,
       },
       {
           0.0f,
           0.0f,
           1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           0.5f,
       },
       {
           0.0f,
           0.0f,
           1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           0.5f,
       },
       {
           0.0f,
           0.0f,
           1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           0.5f,
       },
       {
           0.0f,
           0.0f,
           1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           0.5f,
           0.5f,
       },
       {
           0.0f,
           0.0f,
           1.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           0.5f,
       },
       {
           0.0f,
           0.0f,
           1.0f,
       },
       {color[0], color[1], color[2]}},

      {{
           -0.5f,
           0.5f,
           0.5f,
       },
       {
           -1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           0.5f,
           -0.5f,
       },
       {
           -1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           -0.5f,
       },
       {
           -1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           -0.5f,
       },
       {
           -1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           0.5f,
       },
       {
           -1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           0.5f,
           0.5f,
       },
       {
           -1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},

      {{
           0.5f,
           0.5f,
           0.5f,
       },
       {
           1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           -0.5f,
       },
       {
           1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           -0.5f,
       },
       {
           1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           -0.5f,
       },
       {
           1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           0.5f,
       },
       {
           1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           0.5f,
       },
       {
           1.0f,
           0.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},

      {{
           -0.5f,
           -0.5f,
           -0.5f,
       },
       {
           0.0f,
           -1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           -0.5f,
       },
       {
           0.0f,
           -1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           0.5f,
       },
       {
           0.0f,
           -1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           -0.5f,
           0.5f,
       },
       {
           0.0f,
           -1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           0.5f,
       },
       {
           0.0f,
           -1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           -0.5f,
           -0.5f,
       },
       {
           0.0f,
           -1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},

      {{
           -0.5f,
           0.5f,
           -0.5f,
       },
       {
           0.0f,
           1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           -0.5f,
       },
       {
           0.0f,
           1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           0.5f,
       },
       {
           0.0f,
           1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           0.5f,
           0.5f,
           0.5f,
       },
       {
           0.0f,
           1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           0.5f,
           0.5f,
       },
       {
           0.0f,
           1.0f,
           0.0f,
       },
       {color[0], color[1], color[2]}},
      {{
           -0.5f,
           0.5f,
           -0.5f,
       },
       {0.0f, 1.0f, 0.0f},
       {color[0], color[1], color[2]}},
  };

  return mesh_create(vertices, 36, NULL, 0);
}
