#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"

#pragma once
typedef struct {
  TTF_Font *large;
  TTF_Font *medium;
  TTF_Font *small;
} Fonts;

#pragma once
typedef struct {
  char title[200];
  char message[200];
  char *buttons[200];
  int button_count;
} Menu;

int discover_menu(Location new_location, SDL_Renderer *rend, Fonts *fonts);
int result_menu(Player *user, Location new_location, SDL_Renderer *rend,
                Fonts *fonts);
int fightmenu(SDL_Renderer* rend, Fonts* font, Player *users);
int death_menu(SDL_Renderer* rend, Fonts* fonts);
int main_menu(SDL_Renderer* rend, Fonts* fonts);
