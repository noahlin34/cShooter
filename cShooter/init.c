//
//  init.c
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//

#include "common.h"
#include "structs.h"
#include "defs.h"
#include "sound.h"

extern App app;


extern sounds;

void initSDL(void) {
    int renderFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    }
    
    app.window  = SDL_CreateWindow("Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    
    
    if (!app.window) {
        printf("Failed to open. %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, renderFlags);
    
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    SDL_ShowCursor(0);


    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
      printf("Couldn't initialize SDL_Mixer: %s\n", Mix_GetError());
      exit(1);
    }
    Mix_Init(MIX_INIT_MP3);

    Mix_AllocateChannels(MAX_SND_CHANNELS);

}

void cleanup(void) {
    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
    
    cleanUpSound();
    SDL_Quit();
}

