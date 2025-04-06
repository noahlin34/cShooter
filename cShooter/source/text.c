#include "common.h"
#include "structs.h"
#include "draw.h"
#include "text.h"

static SDL_Color white = {0xFF, 0xFF, 0xFF, 255};

extern App app; 

static TTF_Font *loadFont(char *fontPath);

void initFonts(char *fontPath) { 
  TTF_Init(); 
  app.font = loadFont(fontPath);
}


static TTF_Font *loadFont(char *fontPath) {
  TTF_Font *font;
  font = TTF_OpenFont(fontPath, 18);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);  
  return font;
}

SDL_Texture *createFontTexture(TTF_Font *font, SDL_Renderer *renderer, char *message) {
  SDL_Texture *fontTexture;
  SDL_Surface *fSurface;
  fSurface = TTF_RenderText_Blended(font, message, white);
  fontTexture = SDL_CreateTextureFromSurface(renderer, fSurface);
  SDL_FreeSurface(fSurface);
  return fontTexture;
}


void drawFont(SDL_Texture *fontTexture, int x, int y) {
  blit(fontTexture, x, y);
}
