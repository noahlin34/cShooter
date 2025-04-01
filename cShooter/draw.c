//
//  draw.c
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//
#include "common.h"
#include "structs.h"


extern App app;



void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}


SDL_Texture *loadTexture(char *filename) {
    SDL_Texture *texture;

    texture = IMG_LoadTexture(app.renderer, filename);
    return texture;
}

void blit(SDL_Texture *texture, int x, int y) {
    SDL_Rect dest;
    dest.x = x;
    dest. y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y) {
    SDL_Rect dest;
    
    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;
    
    SDL_RenderCopy(app.renderer, texture, src, &dest);
}
