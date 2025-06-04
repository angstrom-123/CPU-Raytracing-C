#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct VEC_Vector {
	double x, y, z;
} Vector;

extern Vector VEC_Add(Vector u, Vector v);
extern Vector VEC_Sub(Vector u, Vector v);
extern Vector VEC_Mul(Vector u, double t);
extern Vector VEC_Div(Vector u, double t);
extern Vector VEC_Cross(Vector u, Vector v);
extern double VEC_Dot(Vector u, Vector v);

#endif
