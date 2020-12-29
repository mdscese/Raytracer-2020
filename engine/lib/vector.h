#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct {
  float pos[3];
  float dir[3];
} ray;

void norm(ray* curRay);
float* vecAdd(float fill[3], float a[3], float b[3]);
float* vecSub(float fill[3], float a[3], float b[3]);
float* vecMult(float fill[3], float a[3], float b);
float* vecDiv(float fill[3], float a[3], float b);
float dotProd(float a[3], float b[3]);
float* xProd(float fill[3], float a[3], float b[3]);

#endif
