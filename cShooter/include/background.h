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

// This function randomly initializes the array holding
// the star data. This needs to be called at least once prior to
// calling doStarfield().
// It can be called as many times as necessary.
void initStarfield(void);

// This decrements every star's x position by its speed.
void doStarfield(void);

// This draws a line on the screen for every star in the array.
void drawStarfield(SDL_Renderer *renderer);
