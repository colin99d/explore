#include <menu.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL) win = stdscr;
  getyx(win, y, x);
  if (startx != 0) x = startx;
  if (starty != 0) y = starty;
  if (width == 0) width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  mvwprintw(win, y+1, x, "%s", "Hello");
  wattroff(win, color);
  refresh();
}

void discovery_menu(int starty, int startx, int width, char *string) {
  char *disc_choices[] = {"Enter", "Leave", (char *)NULL};
  WINDOW *my_menu_win;
  MENU *my_menu;
  ITEM **my_items;
  int n_choices, i, c;

  n_choices = ARRAY_SIZE(disc_choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) {
    my_items[i] = new_item(disc_choices[i], (char *)NULL);
  }
  my_menu = new_menu((ITEM **)my_items);

  my_menu_win = newwin(10, 40, 4, 4);
  keypad(my_menu_win, TRUE);

  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

  set_menu_mark(my_menu, " * ");

  print_in_middle(my_menu_win, 1, 0, 40, string, COLOR_PAIR(1));
  box(my_menu_win, 0, 0);
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvprintw(LINES - 2, 0, "F1 to exit");
  refresh();

  /* Post the menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  while ((c = wgetch(my_menu_win)) != KEY_F(1)) {
    switch (c) {
      case KEY_DOWN:
        menu_driver(my_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(my_menu, REQ_UP_ITEM);
        break;
      case 10: /* Enter */
        move(20, 0);
        clrtoeol();
        mvprintw(20, 0, "Item selected is : %s",
                 item_name(current_item(my_menu)));
        pos_menu_cursor(my_menu);
        goto exit_loop;
    }
    wrefresh(my_menu_win);
  }
exit_loop:;

  /* Unpost and free all the memory taken up */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for (i = 0; i < n_choices; ++i) free_item(my_items[i]);
  endwin();
}
int main() {
  /* Initialize curses */
  initscr();
  start_color();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  discovery_menu(1, 0, 40, "My Menu");
}

