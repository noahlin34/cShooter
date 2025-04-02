#include "common.h"
#include "defs.h"
#include "background.h"

static Star stars[MAX_STARS]; //array allowing for iterating stars

int doBackground(int backgroundX) {

  if (--backgroundX < -SCREEN_WIDTH) {
    backgroundX = 0;
    return backgroundX;
  }
  return backgroundX;
}

void drawBackground(SDL_Renderer *renderer, SDL_Texture *backgroundTexture, int backgroundX) {
  SDL_Rect dest;
  for (int x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH) {
    dest.x = x;
    dest.y = 0;
    dest.w = SCREEN_WIDTH;
    dest.h = SCREEN_HEIGHT;
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &dest);
  }
}

void initStarfield(void) {
  for (int i = 0; i < MAX_STARS; i++) {
    stars[i].x = rand() % SCREEN_WIDTH;
    stars[i].y = rand() % SCREEN_HEIGHT;
    stars[i].speed = 1 + rand() % 8;
  }
}

void doStarfield(void) {
  for (int i = 0; i < MAX_STARS; i++) {
    stars[i].x -= stars[i].speed;
    if (stars[i].x < 0) {
      stars[i].x = SCREEN_WIDTH + stars[i].x;
    }
  }
}

void drawStarfield(SDL_Renderer *renderer) {
  int c;
  for (int i = 0; i < MAX_STARS; i++) {
    c = 32 * stars[i].speed;
    SDL_SetRenderDrawColor(renderer, c, c, c, 255); //Set the color based on the speed
    SDL_RenderDrawLine(renderer, stars[i].x, stars[i].y, stars[i].x + 3,
                       stars[i].y);
  }
}