#include "vector.h"

void norm(ray* curRay)
{
  float magnitude = sqrt(pow(curRay->dir[0], 2) + pow(curRay->dir[1], 2) + pow(curRay->dir[2], 2));
	vecDiv(curRay->dir, curRay->dir, magnitude);
}

float* vecAdd(float fill[3], float a[3], float b[3])
{
  for(int i = 0; i < 3; i++)
		fill[i] = a[i] + b[i];
	return fill;
}

float* vecSub(float fill[3], float a[3], float b[3])
{
  for(int i = 0; i < 3; i++)
		fill[i] = a[i] - b[i];
	return fill;
}

float* vecMult(float fill[3], float a[3], float b)
{
  for(int i = 0; i < 3; i++)
		fill[i] = a[i] * b;
	return fill;
}

float* vecDiv(float fill[3], float a[3], float b)
{
  for(int i = 0; i < 3; i++)
		fill[i] = a[i] / b;
	return fill;
}

float dotProd(float a[3], float b[3])
{
  float sum = 0;
	for(int i = 0; i < 3; i++)
		sum += a[i] * b[i];
	return sum;
}

float* xProd(float fill[3], float a[3], float b[3])
{
  fill[0] = (a[1] * b[2]) - (a[2] * b[1]);
	fill[1] = (a[2] * b[0]) - (a[0] * b[2]);
	fill[2] = (a[0] * b[1]) - (a[1] * b[0]);
	return fill;
}
