#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "hittable.h"

#include <string.h>
#include <stdio.h>

typedef struct Token_Line {
	char** tokens;
	size_t length;
} Token_Line;

typedef struct Obj_Object {
	Hittable* tris[1000];
	size_t length;
	Material mat;
} Obj_Object;

extern Obj_Object* parse_obj_file(char* file_name);

#endif
