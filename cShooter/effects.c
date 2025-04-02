#include "common.h"
#include "defs.h"
#include "effects.h"
#include "draw.h"
#include "structs.h"

void initExplosionList(ExplosionList* explosionList) {
  explosionList->explosionTail = &explosionList->explosionHead;
}

void addExplosions(ExplosionList *explosionList, int x, int y, int num) {
  Explosion *e;
  for (int i = 0; i < num; i++) {
    e = malloc(sizeof(Explosion));

    if (!e) {
      printf("Memmory failiure\n");
      exit(-2);
    }

    memset(e, 0, sizeof(Explosion));
    explosionList->explosionTail->next = e;
    explosionList->explosionTail = e;

    e->x = x + (rand() % 32) - (rand() % 32);
    e->y = y + (rand() % 32) - (rand() % 32);
    e->dx = (rand() % 10) - (rand() % 10);
    e->dy = (rand() % 10) - (rand() % 10);
    e->dx /= 10;
    e->dy /= 10;

    switch (rand() % 4) {
      case 0:
        e->r = 255;
        break;
      case 1:
        e->r = 255;
        e->g = 128;
        break;
      case 2:
        e->r = 255;
        e->g = 255;
        break;
      default:
        e->r = 255;
        e->g = 255;
        e->b = 255;
        break;
    }
    e->a = rand() % FPS * 3;
  }
}

void doExplosions(ExplosionList *explosionList) {
  Explosion *e;
  //need to create pointer to the head for traversal
  Explosion *prev = &(explosionList->explosionHead);
  for (e = explosionList->explosionHead.next; e != NULL; e = e->next) {
    e->x += e->dx;
    e->y += e->dy;

    if (--e->a <= 0) {
      if (e == explosionList->explosionTail) {
        explosionList->explosionTail = prev;
      }
      prev->next = e->next;
      free(e);
      e = prev;
    }
    prev = e;
  }
}

void resetExplosions(ExplosionList *explosionList) { 
  Explosion *ex;
  while (explosionList->explosionHead.next) {
    ex = explosionList->explosionHead.next;
    explosionList->explosionHead.next = ex->next;
    free(ex);
  }

  explosionList->explosionTail = &(explosionList->explosionHead);
}

void drawExplosions(SDL_Renderer *renderer, SDL_Texture *explosionTexture,
                    ExplosionList *explosionList) {
  Explosion *e;
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
  SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

  for (e = explosionList->explosionHead.next; e != NULL; e = e->next) {
    SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
    SDL_SetTextureAlphaMod(explosionTexture, e->a);
    blit(explosionTexture, e->x, e->y);
  }
}

void addDebris(DebrisList *debrisList, Entity *e) {
  Debris *d;
  int x;
  int y;
  int w;
  int h;
  w = e->w / 2;
  h = e->h / 2;

  for (y = 0; y <= h; y += h) {
    for (x = 0; x <= w; x += w) {
      d = malloc(sizeof(Debris));
      if (!d) {
        printf("Memory error \n");
        exit(1);
      }
      memset(d, 0, sizeof(Debris));
      debrisList->debrisTail->next = d;
      debrisList->debrisTail = d;
      d->x = e->x + e->w / 2;
      d->y = e->y + e->h / 2;
      d->dx = (rand() % 5) - (rand() % 5);
      d->dy = -(5 + (rand() % 12));
      d->life = FPS * 2;
      d->texture = e->texture;
      d->rect.x = x;
      d->rect.y = y;
      d->rect.w = w;
      d->rect.h = h;
    }
  }
}

void resetDebris(DebrisList *debrisList) { 
  Debris *d; 
  while (debrisList->debrisHead.next) {
    d = debrisList->debrisHead.next;
    debrisList->debrisHead.next = d->next;
    free(d);
  }
  debrisList->debrisTail = &(debrisList->debrisHead);
}

void doDebris(DebrisList *debrisList) {
  Debris *d;
  Debris *prev;
  prev = &(debrisList->debrisHead);
  for (d = debrisList->debrisHead.next; d != NULL; d = d->next) {
    d->x += d->dx;
    d->y += d->dy;
    d->dy += 0.5;
    if (--d->life <= 0) {
      if (d == debrisList->debrisTail) {
        debrisList->debrisTail = prev;
      }
      prev->next = d->next;
      free(d);
      d = prev;
    }
    prev = d;
  }
}

void drawDebris(SDL_Renderer *renderer, DebrisList *debrisList) {
  Debris *d;
  for (d = debrisList->debrisHead.next; d != NULL; d = d->next) {
    blitRect(d->texture, &d->rect, d->x, d->y);
  }
}