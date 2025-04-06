#include "points.h"
#include "sound.h"
#include "text.h"

static char buf[100];
void resetPointsList(PointsList *pointsList) {
  Entity *e;
  while (pointsList->pointsHead.next) {
    e = pointsList->pointsHead.next;
    pointsList->pointsHead.next = e->next;
    free(e);
  }
  pointsList->pointsTail = &(pointsList->pointsHead);
}

void doPointsPods(PointsList *pointsList, Entity *player, int *score, 
                  int* highscore, TTF_Font *font, SDL_Texture **fontTexture, 
                   SDL_Renderer *renderer) {
  Entity *e;
  Entity *prev;
  prev = &(pointsList->pointsHead);
  for (e = pointsList->pointsHead.next; e != NULL; e = e->next) {
    if (e->x < 0) {
      e->x = 0;
      e->dx = -e->dx;
    }
    if (e->x + e->w > SCREEN_WIDTH) {
      e->x = SCREEN_WIDTH - e->w;
      e->dx = -e->dx;
    }
    if (e->y < 0) {
      e->y = 0;
      e->dy = -e->dy;
    }
    if (e->y + e->h > SCREEN_HEIGHT) {
      e->y = SCREEN_HEIGHT - e->h;
      e->dy = -e->dy;
    }
    e->x += e->dx;
    e->y += e->dy;

    if (player && collision(e->x, e->y, e->w, e->h, player->x, player->y,
                            player->w, player->h)) {
      e->health = 0;
      (*score)++;
      *highscore = MAX(*score, *highscore);
      sprintf(&buf[0], "Score: %d, HiScore: %d", *score, *highscore);
      *fontTexture = createFontTexture(font, renderer, &buf[0]);
      playSound(SND_POINT_GET, CH_POINT);
    }
    if (--e->health <= 0) {
      if (e == pointsList->pointsTail) {
        pointsList->pointsTail = prev;
      }
      prev->next = e->next;
      free(e);
      e = prev;
    }
    prev = e;
  }
}

void addPointsPod(PointsList *pointsList, SDL_Texture *pointsTexture, 
                  int x, int y) {
  Entity *e; 
  e = malloc(sizeof(Entity));
  if (!e) {
    printf("malloc error \n");
    exit(1);
  }
  memset(e, 0, sizeof(Entity));
  pointsList->pointsTail->next = e;
  pointsList->pointsTail = e;
  e->x = x;
  e->y = y;
  e->dx = -(rand() % 5);
  e->dy = (rand() % 5) - (rand() % 5);
  e->health = FPS * 10;
  e->texture = pointsTexture;
  SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);
  e->x -= e->w / 2;
  e->y -= e->h / 2;
}

void drawPointsPod(PointsList *pointsList) {
  Entity *e;
  for (e = pointsList->pointsHead.next; e != NULL; e = e->next) {
    blit(e->texture, e->x, e->y);
  }
}

