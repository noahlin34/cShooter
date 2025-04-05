#pragma once
#include "structs.h"
#include "defs.h"

// Returns a SDL_Texture pointer for an img file found
// at 'filename.' Note that no modification is done to
// the provided filename, so it must be accurate,
// being either an absolute path or something visible
// to the exe from its working directory at runtime.

SDL_Texture *loadTexture(char *filename);
