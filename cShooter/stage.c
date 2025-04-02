//
//  stage.c
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//
#include "common.h"
#include "defs.h"
#include "draw.h"
#include "sound.h"
#include "structs.h"
#include "text.h"
#include "util.h"
#include "background.h"


extern App app;
extern Stage stage;

static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *alienBulletTexture;
static SDL_Texture *playerTexture;
static SDL_Texture *background;
static SDL_Texture *explosionTexture;
static SDL_Texture *arialTexture;
static char *buffer[100];
static TTF_Font *arialFont;
static int backgroundX;

static int enemySpawnTimer;
static int stageResetTimer;
int highscore;
Entity *player;

static void initPlayer(void);
static void logic(void);
static void doPlayer(void);
static void drawFighters(void);
static void doBullets(void);
static void fireBullet(void);
static void draw(void);
static void drawBullets(void);
static void doFighters(void);
static void spawnEnemies(void);
static int bulletHitFighter(Entity *b);
static void resetStage(void);
static void clipPlayer(void);
static void doEnemies(void);
static void fireAlienBullet(Entity *e);
static void doExplosions(void);
static void doDebris(void);
static void addExplosions(int, int, int);
static void addDebris(Entity *);
static void drawDebris(void);
static void drawExplosions(void);





static void fireBullet(void) {
  Entity *bullet;
  bullet = malloc(sizeof(Entity));
  memset(bullet, 0, sizeof(Entity));
  stage.bulletTail->next = bullet;
  stage.bulletTail = bullet;

  bullet->x = player->x;
  bullet->y = player->y;
  bullet->dx = PLAYER_BULLET_SPEED;
  bullet->health = 1;
  bullet->texture = bulletTexture;
  SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

  bullet->y += (player->h / 2) - (bullet->h / 2);
  player->reload = 8;
  bullet->side = SIDE_PLAYER;
}

static void doPlayer(void) {
  if (player != NULL) {
    player->dx = player->dy = 0;

    if (player->reload > 0) {
      player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_UP]) {
      player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_DOWN]) {
      player->dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LEFT]) {
      player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT]) {
      player->dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0) {
      playSound(SND_PLAYER_FIRE, CH_PLAYER);
      fireBullet();
    }
  }
}

static void doEnemies(void) {
  Entity *e;

  for (e = stage.fighterHead.next; e != NULL; e = e->next) {
    if (e != player && player != NULL && --e->reload <= 0) {
      fireAlienBullet(e);
      playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
    }
  }
}

static void doFighters(void) {
  Entity *e;
  Entity *prev;

  prev = &stage.fighterHead;

  for (e = stage.fighterHead.next; e != NULL; e = e->next) {
    e->x += e->dx;
    e->y += e->dy;

    if (e != player && e->x < -e->w) {
      e->health = 0;
    }

    if (e->health == 0) {
      if (e == player) {
        player = NULL;
      }

      if (e == stage.fighterTail) {
        stage.fighterTail = prev;
      }

      prev->next = e->next;
      free(e);
      e = prev;
    }
    prev = e;
  }
}

static void doBullets(void) {
  Entity *b;
  Entity *prev;

  prev = &stage.bulletHead;

  for (b = stage.bulletHead.next; b != NULL; b = b->next) {
    b->x += b->dx;
    b->y += b->dy;

    if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h ||
        b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT) {
      if (b == stage.bulletTail) {
        stage.bulletTail = prev;
      }

      prev->next = b->next;
      free(b);
      b = prev;
    }
    prev = b;
  }
}

static int bulletHitFighter(Entity *b) {
  Entity *e;

  for (e = stage.fighterHead.next; e != NULL; e = e->next) {
    if (e->side != b->side &&
        collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h)) {
      b->health = 0;
      e->health = 0;

      if (e == player) {
        playSound(SND_PLAYER_DIE, CH_PLAYER);
      } else {
        playSound(SND_ALIEN_DIE, CH_ANY);
        stage.score++;
        highscore = max(stage.score, highscore);
        sprintf(buffer, "Score: %d, HiScore: %d", stage.score, highscore);
        arialTexture = createFontTexture(arialFont, app.renderer, buffer);
      }
      addDebris(e);
      addExplosions(e->x, e->y, 16);

      return 1;
    }
  }
  return 0;
}

static void drawBullets(void) {
  Entity *b;
  for (b = stage.bulletHead.next; b != NULL; b = b->next) {
    blit(b->texture, b->x, b->y);
  }
}




static void drawDebris(void) {
  Debris *d;
  for (d = stage.debrisHead.next; d != NULL; d = d->next) {
    blitRect(d->texture, &d->rect, d->x, d->y);
  }
}

static void drawExplosions(void) {
  Explosion *e;
  SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
  SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

  for (e = stage.explosionHead.next; e != NULL; e = e->next) {
    SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
    SDL_SetTextureAlphaMod(explosionTexture, e->a);
    blit(explosionTexture, e->x, e->y);
  }
}

static void draw(void) {
  drawBackground(app.renderer, background, backgroundX);
  drawStarfield(app.renderer);
  drawDebris();
  drawExplosions();
  drawFighters();
  drawBullets();
  drawFont(arialTexture, 20, 20);
}


static void addExplosions(int x, int y, int num) {
  Explosion *e;

  for (int i = 0; i < num; i++) {
    e = malloc(sizeof(Explosion));
    memset(e, 0, sizeof(Explosion));

    stage.explosionTail->next = e;
    stage.explosionTail = e;

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

static void addDebris(Entity *e) {
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
      memset(d, 0, sizeof(Debris));
      stage.debrisTail->next = d;
      stage.debrisTail = d;
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

static void logic(void) {
  backgroundX = doBackground(backgroundX);
  doStarfield();
  doPlayer();
  doFighters();
  doBullets();
  spawnEnemies();
  doEnemies();
  clipPlayer();

  if (player == NULL && --stageResetTimer <= 0) {
    resetStage();
  }

  doExplosions();
  doDebris();
}




static void doExplosions(void) {
  Explosion *e;
  Explosion *prev = &stage.explosionHead;

  for (e = stage.explosionHead.next; e != NULL; e = e->next) {
    e->x += e->dx;
    e->y += e->dy;

    if (--e->a <= 0) {
      if (e == stage.explosionTail) {
        stage.explosionTail = prev;
      }

      prev->next = e->next;
      free(e);
      e = prev;
    }
    prev = e;
  }
}

static void doDebris(void) {
  Debris *d;
  Debris *prev;
  prev = &stage.debrisHead;

  for (d = stage.debrisHead.next; d != NULL; d = d->next) {
    d->x += d->dx;
    d->y += d->dy;

    d->dy += 0.5;

    if (--d->life <= 0) {
      if (d == stage.debrisTail) {
        stage.debrisTail = prev;
      }
      prev->next = d->next;
      free(d);
      d = prev;
    }
    prev = d;
  }
}

static void clipPlayer(void) {
  if (player != NULL) {
    if (player->x < 0) {
      player->x = 0;
    }
    if (player->y < 0) {
      player->y = 0;
    }
    if (player->x > SCREEN_WIDTH / 2) {
      player->x = SCREEN_WIDTH / 2;
    }
    if (player->y > SCREEN_HEIGHT - player->h) {
      player->y = SCREEN_HEIGHT - player->h;
    }
  }
}

static void spawnEnemies(void) {
  Entity *enemy;

  if (--enemySpawnTimer <= 0) {
    enemy = malloc(sizeof(Entity));
    memset(enemy, 0, sizeof(Entity));
    stage.fighterTail->next = enemy;
    stage.fighterTail = enemy;

    enemy->x = SCREEN_WIDTH;
    enemy->y = rand() % SCREEN_HEIGHT;
    enemy->texture = enemyTexture;
    SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
    enemy->dx = -(2 + (rand() % 4));
    enemySpawnTimer = 30 + (rand() % 60);
    enemy->side = SIDE_ALIEN;
    enemy->health = 1;
    enemy->reload = FPS * (1 + (rand() % 3));
  }
}

static void drawFighters(void) {
  Entity *e;
  for (e = stage.fighterHead.next; e != NULL; e = e->next) {
    blit(e->texture, e->x, e->y);
  }
}

static void initPlayer(void) {
  player = malloc(sizeof(Entity));
  memset(player, 0, sizeof(Entity));

  stage.fighterTail->next = player;
  stage.fighterTail = player;

  player->x = 100;
  player->y = 100;

  player->texture = playerTexture;
  player->side = SIDE_PLAYER;
  player->health = 100;

  SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

void initStage(void) {
  app.delegate.logic = logic;
  app.delegate.draw = draw;
  stage.explosionTail = &stage.explosionHead;
  stage.debrisTail = &stage.debrisHead;
  bulletTexture = loadTexture(".\\assets\\laserBlue12.png");
  enemyTexture = loadTexture(".\\assets\\enemyBlack1.png");
  alienBulletTexture = loadTexture(".\\assets\\laserRed12.png");
  playerTexture = loadTexture(".\\assets\\playerShip3_red.png");
  background = loadTexture(".\\assets\\background.png");
  explosionTexture = loadTexture(".\\assets\\explosion00.png");
  arialFont = loadFont(".\\assets\\arial.ttf");

  sprintf(buffer, "Score: %d, HiScore: %d", stage.score, highscore);
  arialTexture = createFontTexture(arialFont, app.renderer, buffer);
  resetStage();
}



static void fireAlienBullet(Entity *e) {
  Entity *bullet;

  bullet = malloc(sizeof(Entity));
  memset(bullet, 0, sizeof(Entity));
  stage.bulletTail->next = bullet;
  stage.bulletTail = bullet;

  bullet->x = e->x;
  bullet->y = e->y;
  bullet->health = 1;
  bullet->texture = alienBulletTexture;
  bullet->side = SIDE_ALIEN;
  SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

  bullet->x += (e->w / 2) - (bullet->w / 2);
  bullet->y += (e->h / 2) - (bullet->h / 2);

  calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x,
            e->y, &bullet->dx, &bullet->dy);

  bullet->dx *= ALIEN_BULLET_SPEED;
  bullet->dy *= ALIEN_BULLET_SPEED;

  e->reload = (rand() % FPS * 2);
}

static void resetStage(void) {
  Entity *e;
  Explosion *ex;
  Debris *d;

  while (stage.fighterHead.next) {
    e = stage.fighterHead.next;
    stage.fighterHead.next = e->next;
    free(e);
  }

  while (stage.bulletHead.next) {
    e = stage.bulletHead.next;
    stage.bulletHead.next = e->next;
    free(e);
  }

  while (stage.explosionHead.next) {
    ex = stage.explosionHead.next;
    stage.explosionHead.next = ex->next;
    free(ex);
  }

  while (stage.debrisHead.next) {
    d = stage.debrisHead.next;
    stage.debrisHead.next = d->next;
    free(d);
  }

  memset(&stage, 0, sizeof(Stage));

  stage.fighterTail = &stage.fighterHead;
  stage.bulletTail = &stage.bulletHead;
  stage.explosionTail = &stage.explosionHead;
  stage.debrisTail = &stage.debrisHead;

  initPlayer();
  initStarfield();

  enemySpawnTimer = 0;

  stageResetTimer = FPS * 3;
  stage.score = 0;
  sprintf(buffer, "Score: %d, HiScore: %d", stage.score, highscore);
  arialTexture = createFontTexture(arialFont, app.renderer, buffer);

  drawFont(arialTexture, 20, 20);
}
