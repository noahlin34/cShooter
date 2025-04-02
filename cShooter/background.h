#pragma once
#include "common.h"

// This function takes an integer representing the X coordinate of the background
// It decrements it, and checks if it is off screen.
// If it is off screen, it returns 0. Otherwise, it returns the new decremented value.
int doBackground(int backGroundX);


void drawBackground(SDL_Renderer *renderer, SDL_Texture *backgroundTexture,
                    int backgroundX);

typedef struct {
  int x;
  int y;
  int speed;
} Star;

void initStarfield(void);

void doStarfield(void);

void drawStarfield(SDL_Renderer *renderer);
