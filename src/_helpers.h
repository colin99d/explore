#include <SDL2/SDL.h>

#include "main.h"
#include "_menu.h"

int get_location();
void create_user(Player* user);
int get_gold();
void load_textures(Textures* textures, SDL_Renderer* rend);
int move_user(Player* user, Location locations[ROWS][COLS], int y, int x,
              SDL_Renderer* rend, Fonts* fonts);
int handle_input(SDL_Event* event, Player* user, int positions[ROWS][COLS], SDL_Renderer* rend, Fonts* fonts);
