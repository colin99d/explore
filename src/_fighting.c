#include "_fighting.h"

void move_character(SDL_Rect* character, int x, int y) {
  int speed = 10;
  int newx = character->x + (x * speed);
  int newy = character->y + (y * speed);
  if (newx > 0 && newx < WIDTH - character->w && newy > 0 &&
      newy < HEIGHT - character->h) {
    character->x = newx;
    character->y = newy;
  }
}

int fight(SDL_Renderer* rend, Textures* textures, Fonts* fonts) {
  char stat_text[10];
  int fightIsRunning = 1;
  int healths[2] = {10, 10};
  int i;
  SDL_Surface* surfInfos[2];
  SDL_Texture* Infos[2];
  SDL_Rect character;
  SDL_Rect enemy;
  SDL_Rect sword;
  SDL_Rect info_box[2];
  SDL_Rect Info_rects[2];
  SDL_Color Red = {255, 0, 0};

  character.x = 100;
  character.y = 100;
  character.w = 50;
  character.h = 100;

  sword.x = 400;
  sword.y = 400;
  sword.w = 20;
  sword.h = 35;

  enemy.x = 300;
  enemy.y = 300;
  enemy.w = 100;
  enemy.h = 100;

  for (i = 0; i < 2; i++) {
    info_box[i].w = 80;
    info_box[i].h = 80;
    info_box[i].x = i == 0 ? 0 : WIDTH - info_box[i].w;
    info_box[i].y = 0;
  }

  while (fightIsRunning) {
    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          fightIsRunning = 0;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
              move_character(&character, 0, -1);
              break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              move_character(&character, -1, 0);
              break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
              move_character(&character, 0, 1);
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              move_character(&character, 1, 0);
              break;
            default:
              break;
          }
      }
    }

    if (SDL_HasIntersection(&character, &enemy)) {
      healths[0] -= 1;
    }

    SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);

    SDL_RenderCopy(rend, textures->character, NULL, &character);
    SDL_RenderCopy(rend, textures->sword, NULL, &sword);

    SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(rend, &enemy);
    SDL_RenderDrawRect(rend, &character);
    SDL_RenderDrawRect(rend, &sword);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 130);
    for (i = 0; i < 2; i++) {
      SDL_RenderFillRect(rend, &info_box[i]);
      sprintf(stat_text, "%i", healths[i]);
      surfInfos[i] = TTF_RenderText_Solid(fonts->medium, stat_text, Red);
      Infos[+i] = SDL_CreateTextureFromSurface(rend, surfInfos[i]);
      Info_rects[i].w = surfInfos[i]->w;
      Info_rects[i].h = surfInfos[i]->h;
      Info_rects[i].x = info_box[i].x + ((info_box[i].w - Info_rects[i].w) / 2);
      Info_rects[i].y = info_box[i].y + ((info_box[i].h - Info_rects[i].h) / 2);
      SDL_FreeSurface(surfInfos[i]);
      SDL_RenderCopy(rend, Infos[i], NULL, &Info_rects[i]);
    }

    SDL_RenderPresent(rend);
    SDL_Delay(1000 / 60);
  }
  return 1;
}
