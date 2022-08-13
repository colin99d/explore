#include <ncurses.h>
#include "main.h"

WINDOW *create_newwin(Player *user, Location locations[HEIGHT][WIDTH]);
void destroy_win(WINDOW *local_win);
void generate_color();
