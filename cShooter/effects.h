#pragma once
#include "common.h"
#include "structs.h"
typedef struct Explosion {
  float x;
  float y;
  float dx;
  float dy;
  int r;
  int g;
  int b;
  int a;
  struct Explosion *next;
} Explosion;

typedef struct Debris {
  float x;
  float y;
  float dx;
  float dy;
  SDL_Rect rect;
  SDL_Texture *texture;
  int life;
  struct Debris *next;
} Debris;

// linked list structure for explosions
typedef struct {
  Explosion explosionHead;
  Explosion *explosionTail;
} ExplosionList;

// same as above
typedef struct {
  Debris debrisHead;
  Debris *debrisTail;
} DebrisList;
// This initializes the explosion list. 
// Caller is responsible for ensuring this is invoked before using
// any other explosion methods.
void initExplosionList(ExplosionList *explosionList);

// This sets the number 'num' of explosions to be drawn at (x,y).
// This does not draw them. 
void addExplosions(ExplosionList *explosionList, int x, int y, int num);

// This function causes the explosion effect to fade out.
// This should be called every frame.
// TODO: Add the ability to adjust the lifespan loss coefficient.
void doExplosions(ExplosionList *explosionList);

// This clears out the explosionList. The caller is responsible for ensuring
// This function is called before addExplosions is invoked for a subsequent
// time.
void resetExplosions(ExplosionList *explosionList);

// This draws the explosions on the screen. Caller is responsible for ensuring
// *explosionList is not NULL and properly populated.
void drawExplosions(SDL_Renderer *renderer, SDL_Texture *explosionTexture,
                    ExplosionList *explosionList);

// Generates the debris for entity 'e.'
void addDebris(DebrisList *debrisList, Entity *e);

// this clears the provided 'debrisList.' Responsibility of the caller to 
// invoke this before generating new debris.
void resetDebris(DebrisList *debrisList);

// Animates the debris.
// Note that this function assumes drawDebris has already been called.
void doDebris(DebrisList *debrisList);

// Draws the debris onto the screen.
void drawDebris(SDL_Renderer *renderer, DebrisList *debrisList);