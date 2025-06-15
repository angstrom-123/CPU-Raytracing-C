#include "math_utils.h"

extern Vector scatter_diffuse(Vector surf_norm);
extern Vector scatter_metallic(Vector incoming, Vector surf_norm);
extern Vector scatter_glass(Vector incoming, Vector surf_norm, 
							bool front_face, double constant);
