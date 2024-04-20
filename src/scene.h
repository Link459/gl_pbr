#ifndef SCENE_H
#define SCENE_H

#include "pipeline.h"
typedef enum {
  ROOT,
  OBJECT,
  LIGHT,
} SceneNodeType;

typedef struct SceneNode {
  int index;
  void *data;
  SceneNodeType type;
  int children_lenght;
  int children_capacity;
  struct SceneNode *children;
} SceneNode;

typedef struct {
  SceneNode *root;
  int current_index;
} Scene;

Scene scene_create();
int scene_add_node(Scene *scene, SceneNodeType type, void *data);
void scene_add_child(Scene *scene, int index, SceneNodeType type, void *data);
SceneNode *scene_get_node(Scene *scene, int index);
void *scene_get_node_data(Scene *scene, int index);
void scene_draw(Scene *scene);
void scene_destroy(Scene *scene);

SceneNode *scene_node_create(int index, SceneNodeType type, void *data);
void scene_node_add_child(SceneNode *node, int index, SceneNodeType type,
                          void *data);
void scene_node_remove_child(SceneNode *node, int index);
SceneNode *scene_node_get_child(SceneNode *node, int index);
void scene_node_draw(SceneNode *node, const Pipeline *pipeline);
void scene_node_destroy(SceneNode *node);
void scene_node_grow(SceneNode *node);

#endif /* SCENE_H */
