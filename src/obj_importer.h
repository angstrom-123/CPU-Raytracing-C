#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "hittable.h"

#include <string.h>
#include <stdio.h>

typedef struct Token_Line {
	char** tokens;
	size_t length;
} Token_Line;

extern Obj_Object* parse_obj_file(char* file_name, double x, double y, 
								  double z, Material material);

#endif
