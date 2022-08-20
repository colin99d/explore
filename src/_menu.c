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
  int x, y, i, response = 0;
  SDL_Rect rectangle[NUMOPTIONS];
  SDL_Rect Message_rects[NUMMESSAGES] = {0};
  SDL_Texture* Messages[NUMMESSAGES] = {0};
  SDL_Surface* surfMessages[NUMMESSAGES] = {0};
  SDL_Color Black = {0, 0, 0};
  int back_color[3] = {144, 238, 144};
  int button_color[3] = {188, 158, 130};

  // Create title
  surfMessages[0] = TTF_RenderText_Solid(fonts->large, menu->title, Black);
  Messages[0] = SDL_CreateTextureFromSurface(rend, surfMessages[0]);

  // Create message
  surfMessages[1] = TTF_RenderText_Solid(fonts->medium, menu->message, Black);
  Messages[1] = SDL_CreateTextureFromSurface(rend, surfMessages[1]);

  // Create button text
  surfMessages[2] = TTF_RenderText_Solid(fonts->medium, menu->button1, Black);
  Messages[2] = SDL_CreateTextureFromSurface(rend, surfMessages[2]);
  surfMessages[3] = TTF_RenderText_Solid(fonts->medium, menu->button2, Black);
  Messages[3] = SDL_CreateTextureFromSurface(rend, surfMessages[3]);

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
  Message_rects[2].x =
      rectangle[0].x + ((rectangle[0].w - surfMessages[2]->w) / 2);
  Message_rects[2].y =
      rectangle[0].y + ((rectangle[0].h - surfMessages[2]->h) / 2);
  Message_rects[3].x =
      rectangle[1].x + ((rectangle[1].w - surfMessages[3]->w) / 2);
  Message_rects[3].y =
      rectangle[1].y + ((rectangle[1].h - surfMessages[3]->h) / 2);

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
      switch (event.type) {
        case SDL_QUIT:
          menuIsRunning = 0;
          response = -1;
          break;
        case SDL_MOUSEBUTTONDOWN:
          x = event.button.x;
          y = event.button.y;
          for (i = 0; i < NUMOPTIONS; i++) {
            if (x >= rectangle[i].x && x <= rectangle[i].x + rectangle[i].w &&
                y >= rectangle[i].y && y <= rectangle[i].y + rectangle[i].h) {
              response = i;
              menuIsRunning = 0;
            }
          }
          break;
      }
    }
    // (2) Handle Updates

    // (3) Clear and Draw the Screen
    // Gives us a clear "canvas"
    SDL_SetRenderDrawColor(rend, back_color[0], back_color[1], back_color[2],
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    // Do our drawing
    SDL_SetRenderDrawColor(rend, button_color[0], button_color[1],
                           button_color[2], SDL_ALPHA_OPAQUE);

    for (i = 0; i < NUMOPTIONS; i++) {
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
    SDL_Delay(1000 / 60);
  }

  for (i = 0; i < NUMMESSAGES; i++) {
    if (Messages[i] != 0) {
      SDL_DestroyTexture(Messages[i]);
    }
  }
  return response;
}

int discover_menu(Location new_location, SDL_Renderer* rend, Fonts* fonts) {
  int response;
  Menu menu;
  if (new_location == HOME) {
    strcpy(menu.title, "Discovered a home");
    strcpy(menu.message, "A loney home in the woods. Will you enter or flee?");
  } else if (new_location == CASTLE) {
    strcpy(menu.title, "Discovered a castle");
    strcpy(menu.message, "A large fortress. Will you go inside?");
  }
  strcpy(menu.button1, "Enter");
  strcpy(menu.button2, "Flee");
  response = showmenu(rend, fonts, &menu);
  return response;
}

int result_menu(Player *user, Location new_location, SDL_Renderer* rend, Fonts* fonts) {
  int response;
  Menu menu;
  if (new_location == HOME) {
    strcpy(menu.title, "Found Treasure");
    strcpy(menu.message, "A loney home in the woods. Will you enter or flee?");
  } else if (new_location == CASTLE) {
    strcpy(menu.title, "Discovered a castle");
    strcpy(menu.message, "A large fortress. Will you go inside?");
  }
  strcpy(menu.button1, "Enter");
  strcpy(menu.button2, "Flee");
  response = showmenu(rend, fonts, &menu);
  return response;
}
