#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

int SCREEN_HEIGHT = 800;
int SCREEN_WIDTH = 600;
int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("SDL Game", 0, 0, 
  SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_HIDDEN);
  SDL_ShowWindow(window);
  SDL_Event event;
  int running = 1;
  while(running) {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        running = 0;
      }
    }
    SDL_Delay( 32 );
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
