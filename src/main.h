#define HEIGHT 30
#define WIDTH 60

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
