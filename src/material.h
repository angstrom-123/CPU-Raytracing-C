#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

extern void scatter_diffuse(Ray* r, Hit_Record hit_rec);
extern void scatter_specular(Ray* r, Hit_Record hit_rec);

#endif
