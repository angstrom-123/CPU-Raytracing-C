#ifndef OBJ_IMPORTER_H
#define OBJ_IMPORTER_H

#include "hittable.h"

#include <string.h>
#include <stdio.h>


typedef struct Token_Array {
	char** tokens;
	uint8_t len;
} Token_Array;

typedef struct Obj_Object {
	Hittable* tris[1000];
	uint16_t len;
	Material mat;
} Obj_Object;

extern Obj_Object* parse_obj(char* file_name);

#endif
