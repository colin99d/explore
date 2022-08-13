#include <ncurses.h>

#define HEIGHT 30
#define WIDTH 60

typedef struct {
  int x;
  int y;
} Player;

WINDOW *create_newwin(Player *user) {
  WINDOW *local_win;

  int starty = (LINES - HEIGHT) / 2; /* Calculating for a center placement */
  int startx = (COLS - WIDTH) / 2;   /* of the window		*/
  local_win = newwin(HEIGHT, WIDTH, starty, startx);
  box(local_win, 0, 0);
  // The below draws the player
  mvaddch(starty + 1 + user->y, startx + 1 + user->x, 'A');
  wrefresh(local_win);

  return local_win;
}

void destroy_win(WINDOW *local_win) {
  /* box(local_win, ' ', ' '); : This won't produce the desired
   * result of erasing the window. It will leave it's four corners
   * and so an ugly remnant of window.
   */
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  /* The parameters taken are
   * 1. win: the window on which to operate
   * 2. ls: character to be used for the left side of the window
   * 3. rs: character to be used for the right side of the window
   * 4. ts: character to be used for the top side of the window
   * 5. bs: character to be used for the bottom side of the window
   * 6. tl: character to be used for the top left corner of the window
   * 7. tr: character to be used for the top right corner of the window
   * 8. bl: character to be used for the bottom left corner of the window
   * 9. br: character to be used for the bottom right corner of the window
   */
  wrefresh(local_win);
  delwin(local_win);
}

void move_user(Player *user, int y, int x) {
  int newy = user->y + y;
  int newx = user->x + x;
  if (newy < HEIGHT - 2 && newy >= 0) {
    user->y = newy;
  }
  if (newx < WIDTH - 2 && newx >= 0) {
    user->x = newx;
  }
}

int main(int argc, char *argv[]) {
  WINDOW *my_win;
  Player user;
  int ch;

  user.x = 0;
  user.y = 0;

  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);

  printw("Press F1 to exit");
  refresh();
  my_win = create_newwin(&user);

  while ((ch = getch()) != KEY_F(1)) {
    // clang-format off
	switch (ch) {
		case KEY_LEFT:
			destroy_win(my_win);
			move_user(&user, 0, -1);
			my_win = create_newwin(&user);
			break;
		case KEY_RIGHT:
			destroy_win(my_win);
			move_user(&user, 0, 1);
			my_win = create_newwin(&user);
			break;
		case KEY_UP:
			destroy_win(my_win);
			move_user(&user, -1, 0);
			my_win = create_newwin(&user);
			break;
		case KEY_DOWN:
			destroy_win(my_win);
			move_user(&user, 1, 0);
			my_win = create_newwin(&user);
			break;
    }
    // clang-format on
  }

  endwin();
  return 0;
}
