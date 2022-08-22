#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "_helpers.h"
#include "_menu.h"

// WASD or arrow keys to move
// m: market, e: eat

int main(int argc, char* argv[]) {
  SDL_Rect destinations[ROWS][COLS] = {0};
  int positions[ROWS][COLS] = {0};
  int gameIsRunning = 1;
  int i, j, response;
  SDL_Rect info_box;
  GameStatus status;
  Location position;
  Textures textures;
  Fonts fonts;
  Player user;

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

  Uint32 render_flags = SDL_RENDERER_ACCELERATED;
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
  if (SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND) != 0) {
    printf("error initializing blend mode: %s\n", SDL_GetError());
  }
  load_textures(&textures, rend);
  // creates a surface to load an image into the main memory
  info_box.w = 100;
  info_box.h = 100;
  info_box.x = WIDTH - info_box.w;
  info_box.y = 0;

  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      destinations[i][j].w = OBJ_WIDTH;
      destinations[i][j].h = OBJ_HEIGHT;
      destinations[i][j].x = j * OBJ_WIDTH;
      destinations[i][j].y = i * OBJ_HEIGHT;
    }
  }

  response = main_menu(rend, &fonts);
  if (response == 0) {
    start_game(positions, &user);
  } else {
    exit(1);
  }

  while (gameIsRunning) {
    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          gameIsRunning = 0;
          break;
        case SDL_KEYDOWN:
          status = handle_input(&event, &user, positions, rend, &fonts);
          if (status == EXIT) {
            gameIsRunning = 0;
          } else if (status == GAMEOVER) {
            death_menu(rend, &fonts);
            response = main_menu(rend, &fonts);
            if (response == 0) {
              start_game(positions, &user);
            } else {
              exit(1);
            }
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

    // create info box
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 130);
    SDL_RenderFillRect(rend, &info_box);

    SDL_RenderPresent(rend);
    SDL_Delay(1000 / 60);
  }

  destroy_textures(&textures);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
