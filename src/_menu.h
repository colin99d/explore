#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
  TTF_Font *large;
  TTF_Font *medium;
  TTF_Font *small;
} Fonts;

typedef struct {
  char title[200];
  char message[200];
  char button1[200];
  char button2[200];
} Menu;

int showmenu(SDL_Renderer *rend, Fonts *fonts, Menu *menu);
