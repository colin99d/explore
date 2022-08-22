#include "_menu.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "_helpers.h"
#include "main.h"

typedef struct {
  int cooldown;
  int damage;
  int health;
} Enemy;

int showmenu(SDL_Renderer* rend, Fonts* fonts, Menu* menu) {
  const int max_buttons = 10;
  const int NUMOPTIONS = menu->button_count;
  const int NUMMESSAGES = 2 + menu->button_count;
  const int button_width = 200;
  const int button_height = 200;
  int menuIsRunning = 1;
  int x, y, i, response = 0;
  SDL_Rect rect[max_buttons];
  SDL_Rect Message_rects[max_buttons];
  SDL_Texture* Messages[max_buttons];
  SDL_Surface* surfMessages[max_buttons];
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
  for (i = 0; i < NUMOPTIONS; ++i) {
    surfMessages[2 + i] =
        TTF_RenderText_Solid(fonts->medium, menu->buttons[i], Black);
    Messages[2 + i] = SDL_CreateTextureFromSurface(rend, surfMessages[2 + i]);
  }

  // Draw buttons for choices
  for (i = 0; i < NUMOPTIONS; i++) {
    rect[i].w = button_width;
    rect[i].h = button_height;
    rect[i].y = 600;
    rect[i].x = ((WIDTH / NUMOPTIONS) * i) +
                (((WIDTH / NUMOPTIONS) - button_width) / 2);
  }

  // Generate messages
  Message_rects[0].x = (WIDTH - surfMessages[0]->w) / 2;
  Message_rects[0].y = 20;
  Message_rects[1].x = (WIDTH - surfMessages[1]->w) / 2;
  Message_rects[1].y = 200;
  for (i = 0; i < NUMOPTIONS; i++) {
    Message_rects[i + 2].x =
        rect[i].x + ((rect[i].w - surfMessages[i + 2]->w) / 2);
    Message_rects[i + 2].y =
        rect[i].y + ((rect[i].h - surfMessages[i + 2]->h) / 2);
  }

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
            if (x >= rect[i].x && x <= rect[i].x + rect[i].w &&
                y >= rect[i].y && y <= rect[i].y + rect[i].h) {
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
      SDL_RenderFillRect(rend, &rect[i]);
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
  menu.buttons[0] = "Enter";
  menu.buttons[1] = "Flee";
  menu.button_count = 2;
  response = showmenu(rend, fonts, &menu);
  return response;
}

int main_menu(SDL_Renderer* rend, Fonts* fonts) {
  int response;
  Menu menu;
  strcpy(menu.title, "Explore");
  strcpy(menu.message, "Explore your surroundings.");
  menu.buttons[0] = "New Game";
  menu.buttons[1] = "Exit";
  menu.button_count = 2;
  response = showmenu(rend, fonts, &menu);
  return response;
}

int death_menu(SDL_Renderer* rend, Fonts* fonts) {
  Menu menu;
  int response;
  strcpy(menu.title, "You Died");
  strcpy(menu.message, "Press continue to go back to the main menu.");
  menu.buttons[0] = "Continue";
  menu.button_count = 1;
  response = showmenu(rend, fonts, &menu);
  return response;
}

int result_menu(Player* user, Location new_location, SDL_Renderer* rend,
                Fonts* fonts) {
  Menu menu;
  int response;
  int gold = get_gold();
  char message[200];
  sprintf(message, "We found %i pieces of gold.", gold);
  strcpy(menu.title, "Found Treasure");
  strcpy(menu.message, message);
  menu.buttons[0] = "Continue";
  menu.button_count = 1;
  response = showmenu(rend, fonts, &menu);
  user->gold = user->gold + gold;
  return response;
}
int fightmenu(SDL_Renderer* rend, Fonts* fonts, Player* user) {
  const int max_buttons = 10;
  const int NUMOPTIONS = 2;
  const int NUMMESSAGES = 2 + 2;
  const int button_width = 200;
  const int button_height = 100;
  float cooldown_timer = 0;
  SDL_Rect cooldown;
  char* labels[] = {"Strike", "Flee"};
  int menuIsRunning = 1;
  int x, y, i, response, timer = 0;
  SDL_Rect rect[max_buttons];
  SDL_Rect Message_rects[max_buttons];
  SDL_Rect Health_rects[2];
  SDL_Texture* Messages[max_buttons];
  SDL_Surface* surfMessages[max_buttons];
  SDL_Texture* Healths[2] = {0};
  SDL_Surface* surfHealths[2];
  SDL_Color Black = {0, 0, 0};
  SDL_Color Red = {255, 0, 0};
  int back_color[3] = {144, 238, 144};
  int button_color[3] = {188, 158, 130};
  int healths[2] = {0};
  char health_msg[5];

  Enemy enemy;
  enemy.cooldown = 120;
  enemy.damage = 1;
  enemy.health = 10;

  // Create title
  surfMessages[0] =
      TTF_RenderText_Solid(fonts->large, "Fight the enemy", Black);
  Messages[0] = SDL_CreateTextureFromSurface(rend, surfMessages[0]);

  // Create message
  surfMessages[1] =
      TTF_RenderText_Solid(fonts->medium, "A wild savage appears", Black);
  Messages[1] = SDL_CreateTextureFromSurface(rend, surfMessages[1]);

  // Create button text
  for (i = 0; i < NUMOPTIONS; ++i) {
    surfMessages[2 + i] = TTF_RenderText_Solid(fonts->medium, labels[i], Black);
    Messages[2 + i] = SDL_CreateTextureFromSurface(rend, surfMessages[2 + i]);
  }

  // Draw buttons for choices
  for (i = 0; i < NUMOPTIONS; i++) {
    rect[i].w = button_width;
    rect[i].h = button_height;
    rect[i].y = 800;
    rect[i].x = ((WIDTH / NUMOPTIONS) * i) +
                (((WIDTH / NUMOPTIONS) - button_width) / 2);
  }
  cooldown.w = button_width * (cooldown_timer / user->cooldown_time);
  cooldown.h = button_height / 8;
  cooldown.y = rect[0].y + rect[0].h - cooldown.h;
  cooldown.x = ((WIDTH / NUMOPTIONS) - button_width) / 2;
  // Generate messages
  Message_rects[0].x = (WIDTH - surfMessages[0]->w) / 2;
  Message_rects[0].y = 20;
  Message_rects[1].x = (WIDTH - surfMessages[1]->w) / 2;
  Message_rects[1].y = 200;
  Message_rects[2].x = rect[0].x + ((rect[0].w - surfMessages[2]->w) / 2);
  Message_rects[2].y = rect[0].y + ((rect[0].h - surfMessages[2]->h) / 2);
  Message_rects[3].x = rect[1].x + ((rect[1].w - surfMessages[3]->w) / 2);
  Message_rects[3].y = rect[1].y + ((rect[1].h - surfMessages[3]->h) / 2);

  for (i = 0; i < NUMMESSAGES; i++) {
    if (Messages[i] != 0) {
      Message_rects[i].w = surfMessages[i]->w;
      Message_rects[i].h = surfMessages[i]->h;
      SDL_FreeSurface(surfMessages[i]);
    }
  }

  while (menuIsRunning) {
    // Return 0 if the user died and 1 if the user killed
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
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_SPACE:
              if (cooldown_timer == 0) {
                cooldown_timer = user->cooldown_time;
                enemy.health -= user->damage;
                if (enemy.health <= 0) {
                  menuIsRunning = 0;
                  response = 1;
                }
              }
              break;
            default:
              break;
          }

        case SDL_MOUSEBUTTONDOWN:
          x = event.button.x;
          y = event.button.y;
          if (x >= rect[1].x && x <= rect[1].x + rect[1].w && y >= rect[1].y &&
              y <= rect[1].y + rect[1].h) {
            response = 2;
            menuIsRunning = 0;
          }
          break;
      }
    }
    // (2) Handle Updates
    if (cooldown_timer > 0) {
      cooldown_timer--;
    }
    cooldown.w = button_width * (cooldown_timer / user->cooldown_time);

    if (timer > 0 && timer % enemy.cooldown == 0) {
      user->health = user->health - enemy.damage;
      if (user->health == 0) {
        menuIsRunning = 0;
        response = 0;
      }
    }

    // (3) Clear and Draw the Screen
    // Gives us a clear "canvas"
    SDL_SetRenderDrawColor(rend, back_color[0], back_color[1], back_color[2],
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    // Do our drawing
    SDL_SetRenderDrawColor(rend, button_color[0], button_color[1],
                           button_color[2], SDL_ALPHA_OPAQUE);

    for (i = 0; i < NUMOPTIONS; i++) {
      SDL_RenderFillRect(rend, &rect[i]);
    }
    SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(rend, &cooldown);

    // Write health
    healths[0] = user->health;
    healths[1] = enemy.health;
    for (i = 0; i < 2; i++) {
      sprintf(health_msg, "%i", healths[i]);
      surfHealths[i] = TTF_RenderText_Solid(fonts->large, health_msg, Red);
      Healths[i] = SDL_CreateTextureFromSurface(rend, surfHealths[i]);
      Health_rects[i].x =
          ((WIDTH / 2) * i) + (((WIDTH / 2) - surfHealths[i]->w) / 2);
      Health_rects[i].y = 400;
      Health_rects[i].w = surfHealths[i]->w;
      Health_rects[i].h = surfHealths[i]->h;
      SDL_FreeSurface(surfHealths[i]);
      SDL_RenderCopy(rend, Healths[i], NULL, &Health_rects[i]);
    }

    // Add text
    for (i = 0; i < NUMMESSAGES; i++) {
      if (Messages[i] != 0) {
        SDL_RenderCopy(rend, Messages[i], NULL, &Message_rects[i]);
      }
    }

    // Finally show what we've drawn
    timer++;
    SDL_RenderPresent(rend);
    SDL_Delay(1000 / 60);
  }

  for (i = 0; i < NUMMESSAGES; i++) {
    if (Messages[i] != 0) {
      SDL_DestroyTexture(Messages[i]);
    }
  }
  for (i = 0; i < 2; i++) {
    if (Healths[i] != 0) {
      SDL_DestroyTexture(Healths[i]);
    }
  }
  return response;
}

int market_menu(SDL_Renderer* rend, Fonts* fonts, Player* user) {
  const int max_buttons = 10;
  const int NUMOPTIONS = 3;
  const int NONINTMESSAGES = 3;
  const int NUMMESSAGES = NONINTMESSAGES + NUMOPTIONS;
  const int button_width = 200;
  const int button_height = 200;
  int menuIsRunning = 1;
  int x, y, i, response = 0;
  SDL_Rect rect[max_buttons];
  SDL_Rect Message_rects[max_buttons];
  SDL_Texture* Messages[max_buttons];
  SDL_Surface* surfMessages[max_buttons];
  SDL_Rect Gold_rects[NUMOPTIONS];
  SDL_Surface* surfGolds[NUMOPTIONS];
  SDL_Texture* Golds[2] = {0};
  char buttons[3][10] = {"Sword", "Armour", "Food"};
  SDL_Color Black = {0, 0, 0};
  SDL_Color Red = {255, 0, 0};
  int back_color[3] = {144, 238, 144};
  int button_color[3] = {188, 158, 130};
  char gold_msg[10];
  int prices[NUMOPTIONS] = {2, 2, 2};

  // Create title
  surfMessages[0] = TTF_RenderText_Solid(fonts->large, "Market", Black);
  Messages[0] = SDL_CreateTextureFromSurface(rend, surfMessages[0]);

  // Create message
  surfMessages[1] =
      TTF_RenderText_Solid(fonts->medium, "Buy items for your journey", Black);
  Messages[1] = SDL_CreateTextureFromSurface(rend, surfMessages[1]);

  // Create button text
  for (i = 0; i < NUMOPTIONS; ++i) {
    surfMessages[NONINTMESSAGES + i] =
        TTF_RenderText_Solid(fonts->medium, buttons[i], Black);
    Messages[NONINTMESSAGES + i] =
        SDL_CreateTextureFromSurface(rend, surfMessages[NONINTMESSAGES + i]);
  }

  // Draw buttons for choices
  for (i = 0; i < NUMOPTIONS; i++) {
    rect[i].w = button_width;
    rect[i].h = button_height;
    rect[i].y = 600;
    rect[i].x = ((WIDTH / NUMOPTIONS) * i) +
                (((WIDTH / NUMOPTIONS) - button_width) / 2);
  }

  // Generate messages
  Message_rects[0].x = (WIDTH - surfMessages[0]->w) / 2;
  Message_rects[0].y = 20;
  Message_rects[1].x = (WIDTH - surfMessages[1]->w) / 2;
  Message_rects[1].y = 200;
  for (i = 0; i < NUMOPTIONS; i++) {
    Message_rects[i + NONINTMESSAGES].x =
        rect[i].x + ((rect[i].w - surfMessages[i + NONINTMESSAGES]->w) / 2);
    Message_rects[i + NONINTMESSAGES].y =
        rect[i].y + ((rect[i].h - surfMessages[i + NONINTMESSAGES]->h) / 2);
  }

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
            if (x >= rect[i].x && x <= rect[i].x + rect[i].w &&
                y >= rect[i].y && y <= rect[i].y + rect[i].h) {
              if (user->gold >= prices[i]) {
                user->gold -= prices[i];
                if(i == 0) {
                  user->damage += 1;
                } else if (i == 1) {
                  user->max_health += 2;
                } else if (i == 2) {
                  user->food += 1;
                }
              }
            }
          }
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_M:
              menuIsRunning = 0;
              break;
            default:
              break;
          }
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
      SDL_RenderFillRect(rend, &rect[i]);
    }

    // Write gold
    sprintf(gold_msg, "Gold: %i", user->gold);
    surfGolds[0] = TTF_RenderText_Solid(fonts->large, gold_msg, Red);
    Golds[0] = SDL_CreateTextureFromSurface(rend, surfGolds[0]);
    Gold_rects[0].x = (WIDTH - surfGolds[0]->w) / 2;
    Gold_rects[0].y = 400;
    Gold_rects[0].w = surfGolds[0]->w;
    Gold_rects[0].h = surfGolds[0]->h;
    SDL_FreeSurface(surfGolds[0]);
    SDL_RenderCopy(rend, Golds[0], NULL, &Gold_rects[0]);

    // Write Prices
    for (i = 0; i < NUMOPTIONS; i++) {
      sprintf(gold_msg, "%i", prices[i]);
      surfGolds[1 + i] = TTF_RenderText_Solid(fonts->large, gold_msg, Black);
      Golds[1 + i] = SDL_CreateTextureFromSurface(rend, surfGolds[1 + i]);
      Gold_rects[1 + i].x =
          ((WIDTH / 3) * i) + (((WIDTH / 3) - surfGolds[1 + i]->w) / 3);
      Gold_rects[1 + i].y = 720;
      Gold_rects[1 + i].w = surfGolds[1 + i]->w;
      Gold_rects[1 + i].h = surfGolds[1 + i]->h;
      SDL_FreeSurface(surfGolds[1 + i]);
      SDL_RenderCopy(rend, Golds[1 + i], NULL, &Gold_rects[1 + i]);
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
