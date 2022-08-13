#include "_menu.h"
#include "_helpers.h"

#include <menu.h>
#include <stdlib.h>
#include <string.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color, Player *user, int menu_type) {
  int length, x, y, gold;
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
  wattroff(win, color);
  if (menu_type == 0) {
    mvwprintw(win, y + 1, startx + 1, "Health: %i", user->health);
  } else if (menu_type == 1) {
    gold = get_gold();
    mvwprintw(win, y + 1, startx + 1, "Found %i coins of gold.", gold);
    user->gold = user->gold + gold;
  }
  refresh();
}

void discovery_menu(Player *user, int width, char *string, char *selection) {
  char *disc_choices[] = {"Enter", "Leave", (char *)NULL};
  char title[100] = "Discovered a ";
  int height = 12;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;
  WINDOW *my_menu_win;
  MENU *my_menu;
  ITEM **my_items;
  int n_choices, i, c;

  strncat(title, string, 10);

  n_choices = ARRAY_SIZE(disc_choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) {
    my_items[i] = new_item(disc_choices[i], (char *)NULL);
  }
  my_menu = new_menu((ITEM **)my_items);

  my_menu_win = newwin(height, width, starty, startx);
  keypad(my_menu_win, TRUE);

  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 5, 1));
  set_menu_mark(my_menu, " * ");

  print_in_middle(my_menu_win, 1, 0, 40, title, COLOR_PAIR(4), user, 0);
  box(my_menu_win, 0, 0);
  mvwaddch(my_menu_win, 4, 0, ACS_LTEE);
  mvwhline(my_menu_win, 4, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 4, 39, ACS_RTEE);
  refresh();

  /* Post the menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  while ((c = wgetch(my_menu_win))) {
    switch (c) {
      case KEY_DOWN:
        menu_driver(my_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(my_menu, REQ_UP_ITEM);
        break;
      case 10: /* Enter */
        strcpy(selection, item_name(current_item(my_menu)));
        goto exit_loop;
    }
    wrefresh(my_menu_win);
  }
exit_loop:;

  unpost_menu(my_menu);
  free_menu(my_menu);
  for (i = 0; i < n_choices; ++i) free_item(my_items[i]);
}

void result_menu(Player *user, int width, int location) {
  char *disc_choices[] = {"Continue", (char *)NULL};
  int height = 12;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;
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

  my_menu_win = newwin(height, width, starty, startx);
  keypad(my_menu_win, TRUE);

  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 5, 1));
  set_menu_mark(my_menu, " * ");

  print_in_middle(my_menu_win, 1, 0, 40, "Exploration Results", COLOR_PAIR(4),
                  user, 1);
  box(my_menu_win, 0, 0);
  mvwaddch(my_menu_win, 4, 0, ACS_LTEE);
  mvwhline(my_menu_win, 4, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 4, 39, ACS_RTEE);
  refresh();

  /* Post the menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  while ((c = wgetch(my_menu_win))) {
    switch (c) {
      case 10: /* Enter */
        goto exit_loop;
    }
    wrefresh(my_menu_win);
  }
exit_loop:;

  unpost_menu(my_menu);
  free_menu(my_menu);
  for (i = 0; i < n_choices; ++i) free_item(my_items[i]);
}
