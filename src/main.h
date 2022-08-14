#include <SDL2/SDL_image.h>

#define width 1000
#define height 1000
#define obj_width 50
#define obj_height 50
#define rows 20
#define cols 20

#pragma once
typedef struct {
  int health;
  int gold;
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
  SDL_Texture* grass;
  SDL_Texture* cleared;
  SDL_Texture* cleared_active;
  SDL_Texture* home;
  SDL_Texture* home_active;
  SDL_Texture* castle;
  SDL_Texture* castle_active;
} Textures;
