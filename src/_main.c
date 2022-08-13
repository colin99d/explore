#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "_menu.h"
#include "_window.h"
#include "_helpers.h"


void move_user(Player *user, Location locations[HEIGHT][WIDTH], int y, int x) {
  char *selection = malloc(sizeof(char) * MAX_LEN + 1);
  Location new_location;
  int newy = user->y + y;
  int newx = user->x + x;
  if ((newy < HEIGHT - 2 && newy >= 0 && y != 0) ||
      (newx < WIDTH - 2 && newx >= 0 && x != 0)) {
    new_location = locations[newy][newx];
    if (new_location == 0) {
      new_location = get_location();
      locations[newy][newx] = new_location;
    }
    if (new_location == HOME) {
      discovery_menu(user, 40, "home", selection);
    } else if (new_location == CASTLE) {
      discovery_menu(user, 40, "castle", selection);
    }
    if (strcmp("Enter", selection) == 0) {
      result_menu(user, 40, new_location);
    }
    user->y = newy;
    user->x = newx;
  }
}


int main(int argc, char *argv[]) {
  Location locations[HEIGHT][WIDTH] = {0};
  WINDOW *my_win;
  Player user;
  int ch;

  locations[0][0] = 1;
  create_user(&user);

  srand(time(NULL));
  initscr();
  start_color();
  generate_color();
  curs_set(0);
  noecho();
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
