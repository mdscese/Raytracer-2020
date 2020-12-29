#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

typedef struct {
  struct { float x,y; } look;
  struct { float x,y; } move;
} RenderInput;

typedef struct {
  RenderInput input;

  SDL_bool captured;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int pitch;
} RenderContext;

void render_init(RenderContext*, int width, int height, int channels);
int  render_frame(RenderContext*, unsigned char* img);
void render_exit(RenderContext*);

#endif
