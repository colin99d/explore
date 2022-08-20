#include "_menu.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "main.h"

int showmenu(SDL_Renderer* rend, Fonts* fonts, Menu* menu) {
  const int NUMOPTIONS = 2;
  const int NUMMESSAGES = 4;
  const int button_width = 200;
  const int button_height = 200;
  int menuIsRunning = 1;
  int x, y, i;
  SDL_Rect rectangle[NUMOPTIONS];
  SDL_Rect Message_rects[NUMMESSAGES] = {0};
  SDL_Texture* Messages[NUMMESSAGES] = {0};
  SDL_Surface* surfMessages[NUMMESSAGES] = {0};
  SDL_Color Black = {0, 0, 0};

  // Create title
  surfMessages[0] = TTF_RenderText_Solid(fonts->large, menu->title, Black);
  Messages[0] = SDL_CreateTextureFromSurface(rend, surfMessages[0]);

  // Create message
  surfMessages[1] = TTF_RenderText_Solid(fonts->medium, menu->message, Black);
  Messages[1] = SDL_CreateTextureFromSurface(rend, surfMessages[1]);

  // Draw buttons for choices
  for (i = 0; i < NUMOPTIONS; i++) {
    rectangle[i].w = button_width;
    rectangle[i].h = button_height;
    rectangle[i].y = 600;
    rectangle[i].x = ((WIDTH / NUMOPTIONS) * i) +
                     (((WIDTH / NUMOPTIONS) - button_width) / 2);
  }
  // Generate messages
  Message_rects[0].x = (WIDTH - surfMessages[0]->w) / 2;
  Message_rects[0].y = 20;
  Message_rects[1].x = (WIDTH - surfMessages[1]->w) / 2;
  Message_rects[1].y = 200;
  for (i = 0; i < NUMMESSAGES; i++) {
    if (Messages[i] != 0) {
      Message_rects[i].w = surfMessages[i]->w;
      Message_rects[i].h = surfMessages[i]->h;
      SDL_FreeSurface(surfMessages[i]);
    }
  }


  while (menuIsRunning) {
    SDL_Event event;

    // (1) Handle Input
    // Start our event loop
    while (SDL_PollEvent(&event)) {
      // Handle each specific event
      if (event.type == SDL_QUIT) {
        menuIsRunning = 0;
      }
      switch (event.type) {
        case SDL_QUIT:
          menuIsRunning = 0;
          return -1;
        case SDL_MOUSEBUTTONDOWN:
          x = event.button.x;
          y = event.button.y;
          for (i = 0; i < NUMOPTIONS; i++) {
            if (x >= rectangle[i].x && x <= rectangle[i].x + rectangle[i].w &&
                y >= rectangle[i].y && y <= rectangle[i].y + rectangle[i].h) {
              // SDL_FreeSurface(menus[0]);
              // SDL_FreeSurface(menus[1]);
              return i;
            }
          }
      }
    }
    // (2) Handle Updates

    // (3) Clear and Draw the Screen
    // Gives us a clear "canvas"
    SDL_SetRenderDrawColor(rend, 144, 238, 144, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    // Do our drawing
    SDL_SetRenderDrawColor(rend, 188, 158, 130, SDL_ALPHA_OPAQUE);
    for (i = 0; i < NUMOPTIONS; i++) {
      // SDL_RenderDrawRect(rend, &rectangle[i]);
      SDL_RenderFillRect(rend, &rectangle[i]);
    }

    // Add text
    for (i = 0; i < NUMMESSAGES; i++) {
      if (Messages[i] != 0) {
        SDL_RenderCopy(rend, Messages[i], NULL, &Message_rects[i]);
      }
    }

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
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            SDL_FreeSurface(menus[0]);
            SDL_FreeSurface(menus[1]);
            return 0;
          }
      }
    }
  }
  */
  for (i = 0; i < NUMMESSAGES; i++) {
    if (Messages[i] != 0) {
      SDL_DestroyTexture(Messages[i]);
    }
  }
  return 1;
}
