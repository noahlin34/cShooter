#include "common.h"
#include "structs.h"

static SDL_Color foreCol = {0xFF, 0xFF, 0xFF, 0};
extern App app;

TTF_Font *loadFont(char *fontName) {
  TTF_Font *font;
  font = TTF_OpenFont(fontName, 18);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  return font;
  
}

