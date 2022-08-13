#include <ncurses.h>
#include <stdlib.h>

#include "_window.h"

#define HEIGHT 30
#define WIDTH 60

typedef struct {
  int x;
  int y;
} Player;

typedef enum {
  UNKNOWN = 0,
  GRASS = 1,
  HOME = 2,
  CASTLE = 3,
} Location;

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

WINDOW *create_newwin(Player *user, Location locations[HEIGHT][WIDTH]) {
  WINDOW *local_win;
  int starty = (LINES - HEIGHT) / 2;
  int startx = (COLS - WIDTH) / 2;
  int i, j;

  local_win = newwin(HEIGHT, WIDTH, starty, startx);
  box(local_win, 0, 0);
  // Draw the field
  for (i = 0; i < HEIGHT - 2; i++) {
    for (j = 0; j < WIDTH - 2; j++) {
      // Adding '0' only works for numbers below 10
      attron(COLOR_PAIR(locations[i][j]));
      mvaddch(starty + 1 + i, startx + 1 + j, locations[i][j] + '0');
      attroff(COLOR_PAIR(locations[i][j]));
    }
  }
  // The below draws the player
  attron(COLOR_PAIR(10));
  mvaddch(starty + 1 + user->y, startx + 1 + user->x, 'A');
  attroff(COLOR_PAIR(10));
  wrefresh(local_win);

  return local_win;
}

void move_user(Player *user, Location locations[HEIGHT][WIDTH], int y, int x) {
  int newy = user->y + y;
  int newx = user->x + x;
  if (newy < HEIGHT - 2 && newy >= 0 && y != 0) {
    locations[newy][newx] = get_location();
    user->y = newy;
  }
  if (newx < WIDTH - 2 && newx >= 0 && x != 0) {
    locations[newy][newx] = get_location();
    user->x = newx;
  }
}

int main(int argc, char *argv[]) {
  Location locations[HEIGHT][WIDTH] = {0};
  WINDOW *my_win;
  Player user;
  int ch;

  locations[0][0] = 1;
  user.x = 0;
  user.y = 0;

  initscr();
  start_color();
  generate_color();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);

  printw("Press F1 to exit");
  refresh();
  my_win = create_newwin(&user, locations);

  while ((ch = getch()) != KEY_F(1)) {
    // clang-format off
	switch (ch) {
		case KEY_LEFT:
			destroy_win(my_win);
			move_user(&user, locations, 0, -1);
			my_win = create_newwin(&user, locations);
			break;
		case KEY_RIGHT:
			destroy_win(my_win);
			move_user(&user, locations, 0, 1);
			my_win = create_newwin(&user, locations);
			break;
		case KEY_UP:
			destroy_win(my_win);
			move_user(&user, locations, -1, 0);
			my_win = create_newwin(&user, locations);
			break;
		case KEY_DOWN:
			destroy_win(my_win);
			move_user(&user, locations, 1, 0);
			my_win = create_newwin(&user, locations);
			break;
    }
    // clang-format on
  }
  endwin();
  return 0;
}
