#include "scene.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>

Scene scene_create() {
  SceneNode *node = malloc(sizeof(SceneNode));
  node->data = NULL;
  node->index = 0;
  node->type = ROOT;
  node->children_lenght = 0;
  node->children_capacity = 16;
  node->children = malloc(node->children_capacity * sizeof(SceneNode));
  Scene scene = {
      .root = node,
      .current_index = 0,
  };
  return scene;
}

int scene_add_node(Scene *scene, SceneNodeType type, void *data) {
  scene->current_index++;
  scene_node_add_child(scene->root, scene->current_index, type, data);
  return scene->current_index;
}

void scene_add_child(Scene *scene, int index, SceneNodeType type, void *data) {
  SceneNode *node = scene_get_node(scene, index);
  scene_node_add_child(node, scene->current_index++, type, data);
}

SceneNode *scene_get_node(Scene *scene, int index) {
  return scene_node_get_child(scene->root, index);
}

void *scene_get_node_data(Scene *scene, int index) {
  return scene_get_node(scene, index)->data;
}

void scene_destroy(Scene *scene) { scene_node_destroy(scene->root); }

SceneNode *scene_node_create(int index, SceneNodeType type, void *data) {
  SceneNode *node = malloc(sizeof(SceneNode));
  node->type = type;
  node->data = data;
  node->index = index;
  node->children_capacity = 0;
  node->children_lenght = 0;
  node->children = NULL;
  return node;
}

void scene_node_add_child(SceneNode *node, int index, SceneNodeType type,
                          void *data) {
  if (node->children_lenght > node->children_capacity ||
      node->children == NULL) {
    scene_node_grow(node);
  }
  SceneNode *child_node = scene_node_create(index, type, data);
  node->children[node->children_lenght] = *child_node;
  node->children_lenght++;
}

SceneNode *scene_node_get_child(SceneNode *node, int index) {

  if (node->index == index) {
    return node;
  }

  for (int i = 0; i < node->children_lenght; i++) {

    SceneNode *found_node = scene_node_get_child(&node->children[i], index);
    if (found_node != NULL) {

      return found_node;
    }
  }

  return NULL;
}

void scene_node_draw(SceneNode *node, const Pipeline *pipeline) {
  pipeline_bind(pipeline);
  if (node->type == OBJECT) {
    Object *object = (Object *)node->data;
    object_draw(object, pipeline);
  }

  for (int i = 0; i < node->children_lenght; i++) {
    scene_node_draw(&node->children[i], pipeline);
  }
}

void scene_node_destroy(SceneNode *node) {
  for (int i = 0; i < node->children_lenght; i++) {
    scene_node_destroy(&node->children[i]);
  }

  if (node->children != NULL) {
    free(node->children);
    node->children = NULL;
  }

  free(node->data);
  node->data = NULL;
  free(node);
  node = NULL;
}

void scene_node_grow(SceneNode *node) {
  if (node->children_capacity == 0) {
    node->children_capacity = 2;
  }

  if (node->children == NULL) {
    node->children = malloc(node->children_capacity * sizeof(SceneNode));
  } else {
    node->children_capacity *= 2;
    SceneNode *children = realloc(node->children, node->children_capacity);
    if (children == NULL) {
      abort();
    }
    node->children = children;
  }
}
