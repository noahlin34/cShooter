//
//  draw.h
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//

#pragma once
#include "common.h"

void prepareScene(void);
void presentScene(void);
SDL_Texture *loadTexture(char *);
void blit(SDL_Texture *, int , int );
void blitRect(SDL_Texture *, SDL_Rect *, int, int);
