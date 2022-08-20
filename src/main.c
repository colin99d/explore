#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "_helpers.h"
#include "_menu.h"

TTF_Font* load_font(char* path, int size) {
  TTF_Font* value;
  value = TTF_OpenFont(path, size);
  if (value == 0) {
    printf("error opening font: %s\n", TTF_GetError());
  }
  return value;
}

int main(int argc, char* argv[]) {
  SDL_Rect destinations[ROWS][COLS] = {0};
  int positions[ROWS][COLS] = {0};
  int gameIsRunning = 1;
  Location position;
  Textures textures;
  Fonts fonts;
  Player user;
  int i, j;

  positions[0][0] = 1;

  create_user(&user);
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }

  if (TTF_Init() != 0) {
    printf("error initializing TTF: %s\n", TTF_GetError());
  }
  fonts.large = load_font("fonts/bodoni-roman.ttf", 60);
  fonts.medium = load_font("fonts/bodoni-roman.ttf", 45);
  fonts.small = load_font("fonts/bodoni-roman.ttf", 30);
  SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

  /*
  if (SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN)) {
    printf("error going fullscreen: %s\n", SDL_GetError());
  }
  */

  // triggers the program that controls
  // your graphics hardware and sets flags
  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  // creates a renderer to render our images
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
  load_textures(&textures, rend);

  // creates a surface to load an image into the main memory

  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      destinations[i][j].w = OBJ_WIDTH;
      destinations[i][j].h = OBJ_HEIGHT;
      destinations[i][j].x = j * OBJ_WIDTH;
      destinations[i][j].y = i * OBJ_HEIGHT;
    }
  }
  fightmenu(rend, &fonts);
  gameIsRunning = 0;

  while (gameIsRunning) {
    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          // handling of close button
          gameIsRunning = 0;
          break;
        case SDL_KEYDOWN:
          if (handle_input(&event, &user, positions, rend, &fonts) == -1) {
            gameIsRunning = 0;
          }
          break;
      }
    }

    SDL_RenderClear(rend);

    // triggers the double buffers
    // for multiple rendering
    for (i = 0; i < ROWS; i++) {
      for (j = 0; j < COLS; j++) {
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
      SDL_RenderCopy(rend, textures.cleared_active, NULL,
                     &destinations[user.y][user.x]);
    } else if (position == HOME) {
      SDL_RenderCopy(rend, textures.home_active, NULL,
                     &destinations[user.y][user.x]);
    } else if (position == CASTLE) {
      SDL_RenderCopy(rend, textures.castle_active, NULL,
                     &destinations[user.y][user.x]);
    }
    SDL_RenderPresent(rend);
    SDL_Delay(1000 / 60);
  }

  destroy_textures(&textures);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
