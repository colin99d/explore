#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "_helpers.h"
int showmenu(SDL_Surface *screen, TTF_Font *font) {
  double time;


}

int main(int argc, char* argv[]) {
  SDL_Rect destinations[rows][cols] = {0};
  Player user;
  int positions[rows][cols] = {0};
  int i, j;
  Location position;
  Textures textures;

  positions[0][0] = 1;

  create_user(&user);
  // returns zero on success else non-zero
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }
  SDL_Window* win = SDL_CreateWindow("GAME",  // creates a window
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, width, height, 0);

  // triggers the program that controls
  // your graphics hardware and sets flags
  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  // creates a renderer to render our images
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
  load_textures(&textures, rend);

  // creates a surface to load an image into the main memory

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      destinations[i][j].w = obj_width;
      destinations[i][j].h = obj_height;
      destinations[i][j].x = j * obj_width;
      destinations[i][j].y = i * obj_height;
    }
  }
  SDL_RenderPresent(rend);

  int close = 0;
  while (!close) {
    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          // handling of close button
          close = 1;
          break;
        case SDL_KEYDOWN:
          handle_input(&event, &user, positions);
          break;
      }
    }

    // clears the screen
    SDL_RenderClear(rend);

    // triggers the double buffers
    // for multiple rendering
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        // SDL_RenderCopy(rend, texgrass, NULL, &destinations[i][j]);
        position = positions[i][j];
        if (position == UNKNOWN) {
          SDL_RenderCopy(rend, textures.grass, NULL, &destinations[i][j]);
        } else if (position == GRASS) {
          SDL_RenderCopy(rend, textures.cleared, NULL, &destinations[i][j]);
        } else if (position == HOME) {
          SDL_RenderCopy(rend, textures.home, NULL, &destinations[i][j]);
        } else if (position == CASTLE) {
          SDL_RenderCopy(rend, textures.castle, NULL, &destinations[i][j]);
        }
      }
    }
    // draw current user position
    position = positions[user.y][user.x];
    if (position == GRASS) {
      SDL_RenderCopy(rend, textures.cleared_active, NULL, &destinations[user.y][user.x]);
    } else if (position == HOME) {
      SDL_RenderCopy(rend, textures.home_active, NULL, &destinations[user.y][user.x]);
    } else if (position == CASTLE) {
      SDL_RenderCopy(rend, textures.castle_active, NULL, &destinations[user.y][user.x]);
    }

    SDL_RenderPresent(rend);

    // calculates to 60 fps
    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
