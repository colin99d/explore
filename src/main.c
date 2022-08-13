#include <ncurses.h>
#include <stdlib.h>

#include "_menu.h"
#include "_window.h"
#include "_helpers.h"


void move_user(Player *user, Location locations[HEIGHT][WIDTH], int y, int x) {
  char *selection = malloc(sizeof(char) * MAX_LEN + 1);
  int newy = user->y + y;
  int newx = user->x + x;
  if ((newy < HEIGHT - 2 && newy >= 0 && y != 0) ||
      (newx < WIDTH - 2 && newx >= 0 && x != 0)) {
    int new_location = get_location();
    locations[newy][newx] = new_location;
    if (new_location == 2) {
      discovery_menu(user, 40, "home", selection);
    } else if (new_location == 3) {
      discovery_menu(user, 40, "castle", selection);
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
