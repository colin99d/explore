#include "_helpers.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>

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
  user->gold = 0;
}

int get_gold() { return rand() % 10; }

void load_textures(Textures* textures, SDL_Renderer* rend) {
  SDL_Surface* grass = IMG_Load("images/grass.jpeg");
  textures->grass = SDL_CreateTextureFromSurface(rend, grass);
  SDL_FreeSurface(grass);

  SDL_Surface* cleared = IMG_Load("images/cleared.jpeg");
  textures->cleared = SDL_CreateTextureFromSurface(rend, cleared);
  SDL_FreeSurface(cleared);

  SDL_Surface* cleared_active = IMG_Load("images/cleared_active.jpeg");
  textures->cleared_active = SDL_CreateTextureFromSurface(rend, cleared_active);
  SDL_FreeSurface(cleared_active);

  SDL_Surface* home = IMG_Load("images/home.jpeg");
  textures->home = SDL_CreateTextureFromSurface(rend, home);
  SDL_FreeSurface(home);

  SDL_Surface* home_active = IMG_Load("images/home_active.jpeg");
  textures->home_active = SDL_CreateTextureFromSurface(rend, home_active);
  SDL_FreeSurface(home_active);

  SDL_Surface* castle = IMG_Load("images/castle.jpeg");
  textures->castle = SDL_CreateTextureFromSurface(rend, castle);
  SDL_FreeSurface(castle);

  SDL_Surface* castle_active = IMG_Load("images/castle_active.jpeg");
  textures->castle_active = SDL_CreateTextureFromSurface(rend, castle_active);
  SDL_FreeSurface(castle_active);
}

void move_user(Player* user, Location locations[rows][cols], int y, int x) {
  // char* selection = malloc(sizeof(char) * MAX_LEN + 1);
  Location new_location;
  int newy = user->y + y;
  int newx = user->x + x;
  if ((newy < rows && newy >= 0 && y != 0) ||
      (newx < cols && newx >= 0 && x != 0)) {
    new_location = locations[newy][newx];
    if (new_location == UNKNOWN) {
      new_location = get_location();
      locations[newy][newx] = new_location;
    }
    /*
    if (new_location == HOME) {
      discovery_menu(user, 40, "home", selection);
    } else if (new_location == CASTLE) {
      discovery_menu(user, 40, "castle", selection);
    }
    if (strcmp("Enter", selection) == 0) {
      result_menu(user, 40, new_location);
    }
    */
    user->y = newy;
    user->x = newx;
  }
}

void handle_input(SDL_Event* event, Player* user, int positions[rows][cols]) {
  switch (event->key.keysym.scancode) {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
      move_user(user, positions, -1, 0);
      break;
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
      move_user(user, positions, 0, -1);
      break;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
      move_user(user, positions, 1, 0);
      break;
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
      move_user(user, positions, 0, 1);
      break;
    default:
      break;
  }
}

void destroy_textures(Textures *textures) {
  SDL_DestroyTexture(textures->grass);
  SDL_DestroyTexture(textures->cleared);
  SDL_DestroyTexture(textures->cleared_active);
  SDL_DestroyTexture(textures->home);
  SDL_DestroyTexture(textures->home_active);
  SDL_DestroyTexture(textures->castle);
  SDL_DestroyTexture(textures->castle_active);
}
