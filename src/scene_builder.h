#ifndef SCENE_BUILDER_H
#define SCENE_BUILDER_H

#include "obj_importer.h"
#include "scene.h"
#include "camera.h"

/*
 * Returns a pointer to a demo scene.
 * This scene consists of a simple imported 3d model of a car arranged with a 
 * few various spheres and a large reflective sphere to act as the ground.
 */
extern Hittable_List* build_demo_scene(Camera* cam);

 /*
  * Returns a pointer to a demo scene with an imported model.
 * This scene consists of a few spheres of various materials positioned on top 
 * of a larger sphere that acts as the ground.
 */
extern Hittable_List* build_model_scene(Camera* cam);

#endif
