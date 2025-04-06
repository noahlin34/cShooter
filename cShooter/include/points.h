#pragma once
#include "common.h"
#include"structs.h"
#include "util.h"
#include "draw.h"

typedef struct {
  Entity pointsHead;
  Entity *pointsTail;
} PointsList;

void resetPointsList(PointsList *pointsList);

void doPointsPods(PointsList *pointsList, Entity *player, int *score,
                  int *highscore, TTF_Font *font, SDL_Texture **fontTexture,
                  SDL_Renderer *renderer);

void addPointsPod(PointsList *pointsList, SDL_Texture *pointsTexture, 
                  int x, int y);

void drawPointsPod(PointsList *pointsList);
