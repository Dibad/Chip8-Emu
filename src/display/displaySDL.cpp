#include <iostream>

#include "displaySDL.h"
#include "memory/memory.h"

DisplaySDL::DisplaySDL(Memory &memory) : memory_(memory) {
  std::cout << "- Loading DisplaySDL..." << std::endl;

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    throw SDL_GetError();

  window_ =
      SDL_CreateWindow("Chip-8 emulator", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN);

  if (!window_)
    throw SDL_GetError();

  renderer_ = SDL_CreateRenderer(
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer_)
    throw SDL_GetError();

  // Sprites are automatically scaled to fit windows size
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
  SDL_RenderSetLogicalSize(renderer_, 64, 32);

  SDL_SetRenderDrawColor(renderer_, 0x33, 0x33, 0x33, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  // Colors for active and inactive pixels
  bg_.r = bg_.g = bg_.b = 0x00;
  bg_.a = 0xFF;
  fg_.r = fg_.g = fg_.b = fg_.a = 0xFF;

  std::cout << "- DisplaySDL loaded." << std::endl;
}

DisplaySDL::~DisplaySDL() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  std::cout << "- DisplaySDL closed." << std::endl;
}

void DisplaySDL::toggleFullScreen() {
  if(!isFullScreen_)
    SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
  else
    SDL_SetWindowFullscreen(window_, 0);

  isFullScreen_ = !isFullScreen_;
}

void DisplaySDL::saveScreenshot() const {
  const uint32_t format = SDL_PIXELFORMAT_ARGB8888;

  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width_, height_, 32,
      format);
  SDL_RenderReadPixels(renderer_, NULL, format, surface->pixels,
      surface->pitch);
  SDL_SaveBMP(surface, "screenshot.bmp");
  SDL_FreeSurface(surface);
}

void DisplaySDL::render() {
  SDL_SetRenderDrawColor(renderer_, 0xAA, 0xAA, 0xAA, 0xFF);
  SDL_RenderClear(renderer_);

  for (size_t i = 0; i < 64 * 32; ++i) {
    if (memory_.readFromDisplay(i) != 0) {
      SDL_SetRenderDrawColor(renderer_, fg_.r, fg_.g, fg_.b, fg_.a);
      SDL_RenderDrawPoint(renderer_, i % 64, i / 64);
    } else {
      SDL_SetRenderDrawColor(renderer_, bg_.r, bg_.g, bg_.b, bg_.a);
      SDL_RenderDrawPoint(renderer_, i % 64, i / 64);
    }
  }
  SDL_RenderPresent(renderer_);
}
