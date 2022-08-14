#include "main.h"

int get_location();
void create_user(Player* user);
int get_gold();
void load_textures(Textures* textures, SDL_Renderer* rend);
void move_user(Player* user, Location locations[rows][cols], int y, int x);
void handle_input(SDL_Event* event, Player* user, int positions[rows][cols]);
