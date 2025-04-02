//
//  structs.h
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//

#pragma once
#include "common.h"
#include "defs.h"

typedef struct {
    void (*logic)(void);
    void (*draw)(void);
} Delegate;


typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct Entity {
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload;
    SDL_Texture *texture;
    struct Entity *next;
    int side;
}Entity;



typedef struct {
    Entity fighterHead;
    Entity *fighterTail;
    Entity bulletHead;
    Entity *bulletTail;
    int score;
} Stage;



