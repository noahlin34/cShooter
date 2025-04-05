#pragma once
#include "common.h"

void initFonts(char *fontPath);
 // This function returns an SDL Texture of a rendered message in the given font.
// It creates a SDL_Surface, then creates the texture.
SDL_Texture *createFontTexture(TTF_Font *font, SDL_Renderer *renderer,
                               char *message);

void drawFont(SDL_Texture *fontTexture, int x, int y);