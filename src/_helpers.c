#include "_helpers.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>

#include "_menu.h"

int get_location() {
  double random_number;
  random_number = (double)rand() / (double)RAND_MAX;
  if (random_number > 0.05) {
    return 1;
  } else if (random_number > 0.01) {
    return 2;
  } else {
    return 3;
  }
}

void create_user(Player* user) {
  user->x = 0;
  user->y = 0;
  user->health = 10;
  user->max_health = 10;
  user->damage = 1;
  user->gold = 0;
  user->cooldown_time = 60;
  user->food = 0;
}

int get_gold() { return rand() % 10; }

void load_textures(Textures* textures, SDL_Renderer* rend) {
  SDL_Surface* grass = IMG_Load("images/undiscovered.png");
  textures->grass = SDL_CreateTextureFromSurface(rend, grass);
  SDL_FreeSurface(grass);

  SDL_Surface* cleared = IMG_Load("images/discovered--.png");
  textures->cleared = SDL_CreateTextureFromSurface(rend, cleared);
  SDL_FreeSurface(cleared);

  SDL_Surface* cleared_active = IMG_Load("images/discovered--character.png");
  textures->cleared_active = SDL_CreateTextureFromSurface(rend, cleared_active);
  SDL_FreeSurface(cleared_active);

  SDL_Surface* home = IMG_Load("images/discovered-house-.png");
  textures->home = SDL_CreateTextureFromSurface(rend, home);
  SDL_FreeSurface(home);

  SDL_Surface* home_active = IMG_Load("images/discovered-house-character.png");
  textures->home_active = SDL_CreateTextureFromSurface(rend, home_active);
  SDL_FreeSurface(home_active);

  SDL_Surface* castle = IMG_Load("images/discovered-castle-.png");
  textures->castle = SDL_CreateTextureFromSurface(rend, castle);
  SDL_FreeSurface(castle);

  SDL_Surface* castle_active = IMG_Load("images/discovered-castle-character.png");
  textures->castle_active = SDL_CreateTextureFromSurface(rend, castle_active);
  SDL_FreeSurface(castle_active);
}

GameStatus move_user(Player* user, Location locations[ROWS][COLS], int y, int x,
                     SDL_Renderer* rend, Fonts* fonts) {
  Location new_location;
  int newy = user->y + y;
  int newx = user->x + x;
  GameStatus response = CONTINUE;
  int result = 1;
  if ((newy < ROWS && newy >= 0 && y != 0) ||
      (newx < COLS && newx >= 0 && x != 0)) {
    new_location = locations[newy][newx];
    if (new_location == UNKNOWN) {
      new_location = get_location();
      locations[newy][newx] = new_location;
    }
    if (new_location == HOME || new_location == CASTLE) {
      response = discover_menu(new_location, rend, fonts);
      if (response == -1) {
        return -1;
      }
      if (response == CONTINUE) {
        result = fightmenu(rend, fonts, user);
        if (result == -1) {
          return -1;
        } else if (result == 0) {
          response = GAMEOVER;
        } else if (result == 1) {
          response = result_menu(user, new_location, rend, fonts);
        } else if (result == 2) {
          // add fleeing logic
        }
        // Currently fleeing returns a 1 for gameover, so we have to change
        // it to a 0 (continue) at the end. this is janky
      } else if (result == 1) {
        response = CONTINUE;
      }
    }
    user->y = newy;
    user->x = newx;
  }
  return response;
}

GameStatus handle_input(SDL_Event* event, Player* user,
                        int positions[ROWS][COLS], SDL_Renderer* rend,
                        Fonts* fonts) {
  GameStatus response = CONTINUE;
  switch (event->key.keysym.scancode) {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
      response = move_user(user, positions, -1, 0, rend, fonts);
      break;
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
      response = move_user(user, positions, 0, -1, rend, fonts);
      break;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
      response = move_user(user, positions, 1, 0, rend, fonts);
      break;
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
      response = move_user(user, positions, 0, 1, rend, fonts);
      break;
    case SDL_SCANCODE_M:
      response = market_menu(rend, fonts, user);
      if (response != EXIT) {
        response = CONTINUE;
      }
      break;
    case SDL_SCANCODE_E:
      if (user->food > 0){
        user->food--;
        user->health = user->max_health;
      }
      break;
    default:
      break;
  }
  return response;
}

void destroy_textures(Textures* textures) {
  SDL_DestroyTexture(textures->grass);
  SDL_DestroyTexture(textures->cleared);
  SDL_DestroyTexture(textures->cleared_active);
  SDL_DestroyTexture(textures->home);
  SDL_DestroyTexture(textures->home_active);
  SDL_DestroyTexture(textures->castle);
  SDL_DestroyTexture(textures->castle_active);
}

int min(int a, int b) { return (a > b) ? b : a; }
int max(int a, int b) { return (a > b) ? a : b; }

TTF_Font* load_font(char* path, int size) {
  TTF_Font* value;
  value = TTF_OpenFont(path, size);
  if (value == 0) {
    printf("error opening font: %s\n", TTF_GetError());
  }
  return value;
}

void start_game(int positions[ROWS][COLS], Player* user) {
  int i, j;
  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      positions[i][j] = 0;
    }
  }
  positions[0][0] = 1;
  create_user(user);
}
