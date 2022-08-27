#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "_helpers.h"
#include "_menu.h"

// WASD or arrow keys to move
// m: market, e: eat

int main(int argc, char* argv[]) {
  SDL_Rect destinations[ROWS][COLS] = {0};
  int positions[ROWS][COLS] = {0};
  int gameIsRunning = 1;
  const int NUMSTATS = 2;
  int stats[NUMSTATS] = {0};
  char info_labels[NUMSTATS][10] = {"Health", "Food"};
  char stat_text[15];
  int i, j, response;
  SDL_Rect Info_rects[NUMSTATS];
  SDL_Texture* Infos[NUMSTATS];
  SDL_Surface* surfInfos[NUMSTATS];
  SDL_Color Black = {0, 0, 0};
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
  info_box.w = 150;
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
        switch (positions[i][j]) {
          case UNKNOWN:
            SDL_RenderCopy(rend, textures.undiscovered, NULL,
                           &destinations[i][j]);
            break;
          case GRASS:
            SDL_RenderCopy(rend, textures.discovered, NULL,
                           &destinations[i][j]);
            break;
          case HOME:
            SDL_RenderCopy(rend, textures.home, NULL, &destinations[i][j]);
            break;
          case EXPLORED_HOME:
            SDL_RenderCopy(rend, textures.cleared_home, NULL,
                           &destinations[i][j]);
            break;
          case CASTLE:
            SDL_RenderCopy(rend, textures.castle, NULL, &destinations[i][j]);
            break;
          case EXPLORED_CASTLE:
            SDL_RenderCopy(rend, textures.cleared_castle, NULL, &destinations[i][j]);
            break;
        }
      }
      // draw current user position
      position = positions[user.y][user.x];
      switch (position) {
        case GRASS:
          SDL_RenderCopy(rend, textures.discovered_active, NULL,
                         &destinations[user.y][user.x]);
          break;
        case HOME:
          SDL_RenderCopy(rend, textures.home_active, NULL,
                         &destinations[user.y][user.x]);
          break;
        case EXPLORED_HOME:
          SDL_RenderCopy(rend, textures.cleared_home_active, NULL,
                         &destinations[user.y][user.x]);
          break;
        case CASTLE:
          SDL_RenderCopy(rend, textures.castle_active, NULL,
                         &destinations[user.y][user.x]);
          break;
        case EXPLORED_CASTLE:
          SDL_RenderCopy(rend, textures.cleared_castle_active, NULL,
                         &destinations[user.y][user.x]);
          break;
        default:
          break;
      }
    }

    // create info box
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 130);
    SDL_RenderFillRect(rend, &info_box);
    stats[0] = user.health;
    stats[1] = user.food;
    for (i = 0; i < NUMSTATS; i++) {
      sprintf(stat_text, "%s: %i", info_labels[i], stats[i]);
      surfInfos[i] = TTF_RenderText_Solid(fonts.small, stat_text, Black);
      Infos[+i] = SDL_CreateTextureFromSurface(rend, surfInfos[i]);
      Info_rects[i].x = WIDTH - info_box.w;
      Info_rects[i].y = 10 + (i * 30);
      Info_rects[i].w = surfInfos[i]->w;
      Info_rects[i].h = surfInfos[i]->h;
      SDL_FreeSurface(surfInfos[i]);
      SDL_RenderCopy(rend, Infos[i], NULL, &Info_rects[i]);
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
