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

SDL_Texture* get_texture(SDL_Renderer* rend, char* path) {
  SDL_Texture* temp_text;
  SDL_Surface* undiscovered = IMG_Load(path);
  temp_text = SDL_CreateTextureFromSurface(rend, undiscovered);
  SDL_FreeSurface(undiscovered);
  return temp_text;
}

void load_textures(Textures* textures, SDL_Renderer* rend) {
  textures->undiscovered = get_texture(rend, "images/undiscovered.png");
  textures->discovered = get_texture(rend, "images/discovered--.png");
  textures->discovered_active =
      get_texture(rend, "images/discovered--character.png");
  textures->home = get_texture(rend, "images/discovered-house-.png");
  textures->home_active =
      get_texture(rend, "images/discovered-house-character.png");
  textures->cleared_home = get_texture(rend, "images/cleared-house-.png");
  textures->cleared_home_active =
      get_texture(rend, "images/cleared-house-character.png");
  textures->castle = get_texture(rend, "images/discovered-castle-.png");
  textures->cleared_castle = get_texture(rend, "images/cleared-castle-.png");
  textures->castle_active =
      get_texture(rend, "images/discovered-castle-character.png");
  textures->cleared_castle_active =
      get_texture(rend, "images/cleared-castle-character.png");
  textures->character = get_texture(rend, "images/character.png");
  textures->sword = get_texture(rend, "images/sword.png");
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
          // Mark the location as cleared
          locations[newy][newx] *= -1;
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
      if (user->food > 0) {
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
  SDL_DestroyTexture(textures->undiscovered);
  SDL_DestroyTexture(textures->discovered);
  SDL_DestroyTexture(textures->discovered_active);
  SDL_DestroyTexture(textures->home);
  SDL_DestroyTexture(textures->cleared_home);
  SDL_DestroyTexture(textures->home_active);
  SDL_DestroyTexture(textures->cleared_home_active);
  SDL_DestroyTexture(textures->castle);
  SDL_DestroyTexture(textures->cleared_castle);
  SDL_DestroyTexture(textures->castle_active);
  SDL_DestroyTexture(textures->cleared_castle_active);
  SDL_DestroyTexture(textures->character);
  SDL_DestroyTexture(textures->sword);
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
