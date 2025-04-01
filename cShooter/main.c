//
//  main.c
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//

#include "common.h"
#include "structs.h"
#include "init.h"
#include "input.h"
#include "draw.h"
#include "defs.h"
#include "stage.h"
#include "sound.h"
#include "text.h"

App app;

Stage stage;


static void capFrameRate(long *, float *);

int main (int argc, char *argv[]) {
    
    long then;
    float remainder;
    
    memset(&app, 0, sizeof(App));
    
    initSDL();
    
    atexit(cleanup);
    
    initStage();
    
    then = SDL_GetTicks();
    
    remainder = 0;

    initSound();
  //  playMusic(-1);

    

    while(1) {
        prepareScene();
        
        doInput();
        
        app.delegate.logic();
        
        app.delegate.draw();
        
        presentScene();
        
        capFrameRate(&then, &remainder);
    }
}

static void capFrameRate(long *then, float *remainder) {
    long wait;
    long frameTime;
    
    wait = 16 + *remainder;
    
    *remainder -= (int)*remainder;
    frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;
    
    if (wait < 1) {
        wait = 1;
    }
    
    SDL_Delay(wait);
    *remainder += 0.667;
    *then = SDL_GetTicks();
    
    
}

    

