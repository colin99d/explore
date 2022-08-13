#include <stdlib.h>
#include "_helpers.h"

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

void create_user(Player *user) {
  user->x = 0;
  user->y = 0;
  user->health = 10;
  user->gold = 0;
}

int get_gold() {
  return rand() % 10;
}
