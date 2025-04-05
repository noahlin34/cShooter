#include "textures.h"

extern App app;

static SDL_Texture *getTexture(char *filename);
static void addTextureToCache(char *name, SDL_Texture *sdlTexture);

SDL_Texture *loadTexture(char *filename) {
  SDL_Texture *texture;
  texture = getTexture(filename);
  if (!texture) {
    printf("Loading %s \n", filename);
    texture = IMG_LoadTexture(app.renderer, filename);
    addTextureToCache(filename, texture);
  }
  return texture;
}

static SDL_Texture *getTexture(char *name) {
  Texture *texture;
  for (texture = app.textureHead.next; texture != NULL;
       texture = texture->next) {
    if (strcmp(texture->name, name) == 0) {
      return texture->texture;
    }
  }
  return NULL;
}

static void addTextureToCache(char *name, SDL_Texture *sdlTexture) {
  Texture *texture;
  texture = malloc(sizeof(Texture));
  if (!texture) {
    printf("error allocating memory for texture \n");
    exit(1);
  }
  memset(texture, 0, sizeof(Texture));
  app.textureTail->next = texture;
  app.textureTail = texture;
  strncpy(texture->name, name, MAX_NAME_LENGTH);
  texture->texture = sdlTexture;
}