#include <SDL2/SDL_image.h>

#define WIDTH 1000
#define HEIGHT 1000
#define OBJ_WIDTH 50
#define OBJ_HEIGHT 50
#define ROWS 20
#define COLS 20

#pragma once
typedef enum {
  EXIT = -1,
  CONTINUE = 0,
  GAMEOVER = 1,
} GameStatus;

#pragma once
typedef struct {
  float cooldown_time;
  int health;
  int max_health;
  int damage;
  int gold;
  int food;
  int x;
  int y;
} Player;

#pragma once
typedef enum {
  UNKNOWN = 0,
  GRASS = 1,
  HOME = 2,
  EXPLORED_HOME = -2,
  CASTLE = 3,
  EXPLORED_CASTLE = -3,
} Location;

typedef struct {
  SDL_Texture* undiscovered;
  SDL_Texture* discovered;
  SDL_Texture* discovered_active;
  SDL_Texture* home;
  SDL_Texture* home_active;
  SDL_Texture* cleared_home;
  SDL_Texture* cleared_home_active;
  SDL_Texture* castle;
  SDL_Texture* cleared_castle;
  SDL_Texture* castle_active;
  SDL_Texture* cleared_castle_active;
  SDL_Texture* character;
  SDL_Texture* sword;
} Textures;
