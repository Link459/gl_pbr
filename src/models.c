#include "models.h"
#include "asset.h"
#include "stb_image.h"

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
      {
          {
              -0.5f,
              -0.5f,
              -0.5f,
          },
          {
              0.0f,
              0.0f,
              -1.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              -0.5f,
          },
          {
              0.0f,
              0.0f,
              -1.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              -0.5f,
          },
          {
              0.0f,
              0.0f,
              -1.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              -0.5f,
          },
          {
              0.0f,
              0.0f,
              -1.0f,
          },
      },
      {
          {
              -0.5f,
              0.5f,
              -0.5f,
          },
          {
              0.0f,
              0.0f,
              -1.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              -0.5f,
          },
          {
              0.0f,
              0.0f,
              -1.0f,
          },
      },

      {
          {
              -0.5f,
              -0.5f,
              0.5f,
          },
          {
              0.0f,
              0.0f,
              1.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              0.5f,
          },
          {
              0.0f,
              0.0f,
              1.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              0.5f,
          },
          {
              0.0f,
              0.0f,
              1.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              0.5f,
          },
          {
              0.0f,
              0.0f,
              1.0f,
          },
      },
      {
          {
              -0.5f,
              0.5f,
              0.5f,
          },
          {
              0.0f,
              0.0f,
              1.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              0.5f,
          },
          {
              0.0f,
              0.0f,
              1.0f,
          },
      },

      {
          {
              -0.5f,
              0.5f,
              0.5f,
          },
          {
              -1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              0.5f,
              -0.5f,
          },
          {
              -1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              -0.5f,
          },
          {
              -1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              -0.5f,
          },
          {
              -1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              0.5f,
          },
          {
              -1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              0.5f,
              0.5f,
          },
          {
              -1.0f,
              0.0f,
              0.0f,
          },
      },

      {
          {
              0.5f,
              0.5f,
              0.5f,
          },
          {
              1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              -0.5f,
          },
          {
              1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              -0.5f,
          },
          {
              1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              -0.5f,
          },
          {
              1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              0.5f,
          },
          {
              1.0f,
              0.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              0.5f,
          },
          {
              1.0f,
              0.0f,
              0.0f,
          },
      },

      {
          {
              -0.5f,
              -0.5f,
              -0.5f,
          },
          {
              0.0f,
              -1.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              -0.5f,
          },
          {
              0.0f,
              -1.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              0.5f,
          },
          {
              0.0f,
              -1.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              -0.5f,
              0.5f,
          },
          {
              0.0f,
              -1.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              0.5f,
          },
          {
              0.0f,
              -1.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              -0.5f,
              -0.5f,
          },
          {
              0.0f,
              -1.0f,
              0.0f,
          },
      },

      {
          {
              -0.5f,
              0.5f,
              -0.5f,
          },
          {
              0.0f,
              1.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              -0.5f,
          },
          {
              0.0f,
              1.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              0.5f,
          },
          {
              0.0f,
              1.0f,
              0.0f,
          },
      },
      {
          {
              0.5f,
              0.5f,
              0.5f,
          },
          {
              0.0f,
              1.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              0.5f,
              0.5f,
          },
          {
              0.0f,
              1.0f,
              0.0f,
          },
      },
      {
          {
              -0.5f,
              0.5f,
              -0.5f,
          },
          {0.0f, 1.0f, 0.0f},
      },
  };

  return mesh_create(vertices, 36, NULL, 0);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void render_cube() {
  // initialize (if necessary)
  if (cubeVAO == 0) {
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                            // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // render Cube
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void render_quad() {
  if (quadVAO == 0) {
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
  }
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

Mesh *model_cerberus(PbrMaterial *material) {
  TextureCreateInfo info = texture_info_default();
  material->albedo_texture = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");

  material->normal_texture = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga");

  material->metallic_texture = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga");
  material->roughness_texture = asset_load_texture(
      &info, "assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga");

  material->ao_texture = pbr_material_texture((vec3){0.3, 0.3, 0.3});

  Mesh *mesh = asset_load_mesh("assets/Cerberus/cerberus.obj", true);
  return mesh;
}

Mesh *model_damaged_helmet(PbrMaterial *material) {
  {
    glm_vec4_copy((vec4){1.0, 1.0, 1.0, 1.0}, material->emissive_factor);
    material->emissive_strength = 1;

    stbi_set_flip_vertically_on_load(false);
    TextureCreateInfo info = texture_info_default();
    material->albedo_texture =
        asset_load_texture(&info, "assets/DamagedHelmet/Default_albedo.jpg");

    material->normal_texture =
        asset_load_texture(&info, "assets/DamagedHelmet/Default_normal.jpg");

    material->metallic_texture = asset_load_texture(
        &info, "assets/DamagedHelmet/Default_metalRoughness.jpg");
    material->roughness_texture = asset_load_texture(
        &info, "assets/DamagedHelmet/Default_metalRoughness.jpg");

    material->ao_texture =
        asset_load_texture(&info, "assets/DamagedHelmet/Default_AO.jpg");

    material->emissive_texture =
        asset_load_texture(&info, "assets/DamagedHelmet/Default_emissive.jpg");

    Mesh *mesh =
        asset_load_mesh("assets/DamagedHelmet/DamagedHelmet.obj", false);
    return mesh;
  }
}
