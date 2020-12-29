#include "scene.h"

scene makeScene(char* file)
{
  char temp;
  char line[200];
  int lightCount;
  int sphereCount;
  int triangleCount;

  FILE *fp;
  fp = fopen(file, "r");

  scene world;

  world.cameras = calloc(1, sizeof(camera));
  world.camCount = 1;
  world.cameras[0].orient = (ray){.pos = {0, 0, 0}, .dir = {0, 0, -1}};
  world.cameras[0].width = 512;
  world.cameras[0].height = 512;
  world.cameras[0].channels = 4;
  world.cameras[0].planeWidth = 2;
  world.cameras[0].planeWidthHalf = 1;
  world.cameras[0].planeHeight = 2;
  world.cameras[0].planeHeightHalf = 1;
  world.cameras[0].planeDist = 2;
  world.cameras[0].pixelWidth = 2.0 / 512;
  world.cameras[0].pixelHeight = 2.0 / 512;
  world.cameras[0].data = calloc(512 * 512 * 4, 1);

  fgets(line, 200, fp);
  sscanf(line, "%c,%d", &temp, &lightCount);

  world.lights = calloc(lightCount, sizeof(light));
  world.lightCount = lightCount;
  for (int i = 0; i < lightCount; i++) {
    fgets(line, 200, fp);
    sscanf(line, "%f %f %f, %f %f %f, %f", &world.lights[i].pos[0], &world.lights[i].pos[1], &world.lights[i].pos[2], &world.lights[i].color[0], &world.lights[i].color[1], &world.lights[i].color[2], &world.lights[i].bright);
  }

  fgets(line, 200, fp);
  sscanf(line, "%c,%d", &temp, &sphereCount);

  world.spheres = calloc(sphereCount, sizeof(sphere));
  world.sphereCount = sphereCount;
  for (int i = 0; i < sphereCount; i++) {
    fgets(line, 200, fp);
    sscanf(line, "%f %f %f, %f, ", &world.spheres[i].pos[0], &world.spheres[i].pos[1], &world.spheres[i].pos[2], &world.spheres[i].radius);
    sscanf(line, "%*f %*f %*f, %*d, %f %f %f, %f", &world.spheres[i].mat.color[0], &world.spheres[i].mat.color[1], &world.spheres[i].mat.color[2], &world.spheres[i].mat.reflective);
  }

  fgets(line, 200, fp);
  sscanf(line, "%c,%d", &temp, &triangleCount);

  world.triangles = calloc(triangleCount, sizeof(triangle));
  world.triangleCount = triangleCount;
  for (int i = 0; i < triangleCount; i++) {
    fgets(line, 200, fp);
    sscanf(line, "%f %f %f, ", &world.triangles[i].pos[0][0], &world.triangles[i].pos[0][1], &world.triangles[i].pos[0][2]);
    sscanf(line, "%*f %*f %*f, %f %f %f, ", &world.triangles[i].pos[1][0], &world.triangles[i].pos[1][1], &world.triangles[i].pos[1][2]);
    sscanf(line, "%*f %*f %*f, %*f %*f %*f, %f %f %f, ", &world.triangles[i].pos[2][0], &world.triangles[i].pos[2][1], &world.triangles[i].pos[2][2]);
    sscanf(line, "%*f %*f %*f, %*f %*f %*f, %*f %*f %*f, %f %f %f, %f\n", &world.triangles[i].mat.color[0], &world.triangles[i].mat.color[1], &world.triangles[i].mat.color[2], &world.triangles[i].mat.reflective);
  }

  return world;
}
