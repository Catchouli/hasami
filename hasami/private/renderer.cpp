#include "renderer.hpp"
#include "SDL.h"

#include <stdio.h>

void test() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_CreateWindow("hello", 0, 0, 800, 600, 0);
}