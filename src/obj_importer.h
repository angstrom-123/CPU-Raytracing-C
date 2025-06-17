#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "hittable.h"

#include <string.h>
#include <stdio.h>

/*
 * Imports a given obj file as an object that can be added to a scene. Puts the 
 * object at the given coordinates and gives it the given material.
 */
extern Obj_Object* parse_obj_file(char* file_name, double x, double y, 
								  double z, Material material);

#endif
