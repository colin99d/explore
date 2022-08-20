#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "_menu.h"
#include "main.h"


int showmenu(SDL_Renderer* rend, Fonts *fonts, Menu *menu) {
  int x, y, i;
  const int NUMOPTIONS = 2;
  const int button_width = 200;
  const int button_height = 200;
  SDL_Surface* options[NUMOPTIONS];
  SDL_Rect rectangle[NUMOPTIONS];
  int menuIsRunning = 1;
  SDL_Rect Message_rect;
  SDL_Color White = {255, 255, 255};

  // Create title
  SDL_Surface* surfaceMessage =
  TTF_RenderText_Solid(fonts->large, menu->title, White);
  SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);

  for(i=0; i<NUMOPTIONS; i++) {
      rectangle[i].w = button_width;
      rectangle[i].h = button_height;
      rectangle[i].y = 600;
      rectangle[i].x = ((WIDTH / NUMOPTIONS) * i) + (((WIDTH / NUMOPTIONS) - button_width) / 2);

}
  Message_rect.x = 250;
  Message_rect.y = 20;
  Message_rect.w = surfaceMessage->w;
  Message_rect.h = surfaceMessage->h;

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
              //SDL_FreeSurface(menus[0]);
              //SDL_FreeSurface(menus[1]);
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
    for (i=0; i <NUMOPTIONS; i++) {
        // SDL_RenderDrawRect(rend, &rectangle[i]);
        SDL_RenderFillRect(rend, &rectangle[i]);
    }

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
          for (int i = 0; i < NUMOPTIONS; i += 1) {
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
          for (int i = 0; i < NUMOPTIONS; i += 1) {
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
    for (int i = 0; i < NUMOPTIONS; i += 1) {
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
