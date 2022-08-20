#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "_helpers.h"
int showmenu(SDL_Renderer* rend, TTF_Font* font) {
  int x, y;
  const int NUMMENU = 2;
  const char* labels[NUMMENU] = {"Continue", "Exit"};
  SDL_Surface* menus[NUMMENU];
  SDL_Color color[NUMMENU] = {{255, 255, 255}, {255, 0, 0}};
  SDL_Rect rectangle;
  int gameIsRunning = 1;
  SDL_Rect Message_rect;
  SDL_Color White = {255, 255, 255};
  SDL_Surface* surfaceMessage =
  TTF_RenderText_Solid(font, "put your text here", White); 
  SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);

  menus[0] = TTF_RenderText_Solid(font, labels[0], color[0]);
  menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
  // SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, menus[0]);
  rectangle.x = 100;
  rectangle.y = 200;
  rectangle.w = 200;
  rectangle.h = 200;

  Message_rect.x = 250;
  Message_rect.y = 20;
  Message_rect.w = surfaceMessage->w;
  Message_rect.h = surfaceMessage->h;

  while (gameIsRunning) {
    SDL_Event event;

    // (1) Handle Input
    // Start our event loop
    while (SDL_PollEvent(&event)) {
      // Handle each specific event
      if (event.type == SDL_QUIT) {
        gameIsRunning = 0;
      }
      switch (event.type) {
        case SDL_QUIT:
          gameIsRunning = 0;
          break;
        case SDL_MOUSEBUTTONDOWN:
          x = event.button.x;
          y = event.button.y;
          for (int i = 0; i < NUMMENU; i += 1) {
            if (x >= rectangle.x && x <= rectangle.x + rectangle.w &&
                y >= rectangle.y && y <= rectangle.y + rectangle.h) {
              SDL_FreeSurface(menus[0]);
              SDL_FreeSurface(menus[1]);
              return i;
            }
          }
      }
    }
    // (2) Handle Updates

    // (3) Clear and Draw the Screen
    // Gives us a clear "canvas"
    SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    // Do our drawing
    SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(rend, &rectangle);

    // Add text
    SDL_RenderCopy(rend, Message, NULL, &Message_rect);

    // Finally show what we've drawn
    SDL_RenderPresent(rend);
  }

  /*
  SDL_Event event;
  while (1) {
    time = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          SDL_FreeSurface(menus[0]);
          SDL_FreeSurface(menus[1]);
          return 1;
        case SDL_MOUSEMOTION:
          x = event.motion.x;
          y = event.motion.y;
          for (int i = 0; i < NUMMENU; i += 1) {
            if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y &&
                y <= pos[i].y + pos[i].h) {
              if (!selected[i]) {
                selected[i] = 1;
                SDL_FreeSurface(menus[i]);
                menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
              }
            } else {
              if (selected[i]) {
                selected[i] = 0;
                SDL_FreeSurface(menus[i]);
                menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
              }
            }
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          x = event.button.x;
          y = event.button.y;
          for (int i = 0; i < NUMMENU; i += 1) {
            if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y &&
                y <= pos[i].y + pos[i].h) {
              SDL_FreeSurface(menus[0]);
              SDL_FreeSurface(menus[1]);
              return i;
            }
          }
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            SDL_FreeSurface(menus[0]);
            SDL_FreeSurface(menus[1]);
            return 0;
          }
      }
    }
    for (int i = 0; i < NUMMENU; i += 1) {
      // SDL_BlitSurface(menus[i], NULL, rend, &pos[i]);
    }
    SDL_RenderPresent(rend);
    if (1000 / 30 > (SDL_GetTicks() - time))
      SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
  }
  */
  SDL_DestroyTexture(Message);
  return 1;
}

int main(int argc, char* argv[]) {
  SDL_Rect destinations[rows][cols] = {0};
  Player user;
  int positions[rows][cols] = {0};
  int i, j, response;
  Location position;
  Textures textures;

  positions[0][0] = 1;

  create_user(&user);
  // returns zero on success else non-zero
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }

  if (TTF_Init() != 0) {
    printf("error initializing TTF: %s\n", TTF_GetError()); 
  }
  TTF_Font* font = TTF_OpenFont("images/bodoni-roman.ttf", 80);
  if (font == 0) {
    printf("error opening font: %s\n", TTF_GetError());
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

  showmenu(rend, font);
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

    // calculates to 60 fps
    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
