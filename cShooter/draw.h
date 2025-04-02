//
//  draw.h
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//

#pragma once
#include "common.h"

// This function is only used to set the renderdraw color
// which is under the background anyways, and also 
// calls SDL_RenderClear to prep the app.renderer.
void prepareScene(void);

// This function invokes SDL_RenderPresent on app.renderer to 
// Actually draw our current renderer's contents to the screen.
void presentScene(void);

// Returns a SDL_Texture pointer for an img file found 
// at 'filename.' Note that no modification is done to
// the provided filename, so it must be accurate,
// being either an absolute path or something visible
// to the exe from its working directory at runtime.
SDL_Texture *loadTexture(char *filename);

// Renders the specified 'texture' at (x,y). 
// This function queries the texture's width and height to create a 
// SDL_Rect destination.
// Then uses SDL_RenderCopy to render the texture to the rect.
void blit(SDL_Texture *texture, int x, int y);

// Renders the specified texture to the screen at the location
// (x,y). Destination width and height are derived from the 
// provided SDL_Rect, not the texture.
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
