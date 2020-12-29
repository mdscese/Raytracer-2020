#include "render.h"

void render_init(RenderContext* ctx, int width, int height, int channels){
  RenderContext newCtx = {0};

  SDL_Init(SDL_INIT_VIDEO);

  newCtx.window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
  newCtx.renderer = SDL_CreateRenderer(newCtx.window, -1, SDL_RENDERER_ACCELERATED);
  newCtx.texture = SDL_CreateTexture(newCtx.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
  newCtx.pitch = width * channels;
  newCtx.captured = 1;
  SDL_SetRelativeMouseMode(newCtx.captured);

  ctx->input = (RenderInput) { 0 };

  *ctx = newCtx;
}

void render_exit(RenderContext* ctx){
  SDL_DestroyTexture(ctx->texture);
  SDL_DestroyRenderer(ctx->renderer);
  SDL_DestroyWindow(ctx->window);
  SDL_Quit();
}

static void do_key(RenderContext* ctx, int down, int key){
  switch(key) {
    case SDLK_ESCAPE:
      if (down == 0) {

      } else {
        ctx->captured = !ctx->captured;
        SDL_SetRelativeMouseMode(ctx->captured);
      }
      break;
    case SDLK_UP:
    case SDLK_w:
      ctx->input.move.y = down ?  1.0f : 0.0f;
      break;
    case SDLK_DOWN:
    case SDLK_s:
      ctx->input.move.y = down ? -1.0f : 0.0f;
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      ctx->input.move.x = down ?  1.0f : 0.0f;
      break;
    case SDLK_LEFT:
    case SDLK_a:
      ctx->input.move.x = down ? -1.0f : 0.0f;
      break;
    default:
      break;
  }
}

static void do_mouse(RenderInput *input, int x, int y){
  input->look.x = -x;
  input->look.y = -y;
}

int render_frame(RenderContext* ctx, unsigned char* img){
  // TO DO: make this so that we only clear pixels that need to change maybe
  SDL_RenderClear(ctx->renderer);

  SDL_UpdateTexture(ctx->texture, NULL, img, ctx->pitch);
  SDL_RenderCopy(ctx->renderer, ctx->texture, 0, 0);
  SDL_RenderPresent(ctx->renderer);

  SDL_Event ev;
  while(SDL_PollEvent(&ev)){
    switch(ev.type) {
      case SDL_QUIT:
        return 0;
      case SDL_KEYUP:
        do_key(ctx, 0, ev.key.keysym.sym);
        break;
      case SDL_KEYDOWN:
        do_key(ctx, 1, ev.key.keysym.sym);
        break;
      case SDL_MOUSEMOTION:
        do_mouse(&ctx->input, ev.motion.xrel, ev.motion.yrel);
        break;
      default:
        break;
    }
  }

  return 1;
}
