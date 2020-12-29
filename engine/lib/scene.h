#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

typedef struct {
  ray orient;
  int width;
  int height;
  int channels;
  float planeWidth;
  float planeWidthHalf;
  float planeHeight;
  float planeHeightHalf;
  float planeDist;
  float pixelHeight;
  float pixelWidth;
  unsigned char *data;
} camera;

typedef struct {
  float color[3];
	float reflective;
} material;

typedef struct {
  float pos[3];
  float radius;
  material mat;
} sphere;

typedef struct {
  float pos[3][3];
  material mat;
} triangle;

typedef struct {
  float pos[3];
  float color[3];
  float bright;
} light;

typedef struct {
  camera* cameras;
  int camCount;
  sphere* spheres;
  int sphereCount;
  triangle* triangles;
  int triangleCount;
  light* lights;
  int lightCount;
} scene;

scene makeScene(char* file);

#endif
