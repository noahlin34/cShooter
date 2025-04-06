#include "sound.h"


 static Mix_Chunk *sounds[SND_MAX];
 static Mix_Music *music;

static void loadSounds(void);


void initSound(void) { 
  memset(sounds, 0, sizeof(Mix_Chunk *) * SND_MAX);
  music = NULL;
  loadSounds();
}

static void loadSounds(void) {
  sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("player_blaster.wav");
  sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("enemy_laser.wav");
  sounds[SND_PLAYER_DIE] = Mix_LoadWAV("player_explosion.wav");
  sounds[SND_ALIEN_DIE] = Mix_LoadWAV("enemy_explosion.wav");
  sounds[SND_POINT_GET] = Mix_LoadWAV("star.wav");
}

void loadMusic(char *filename) {
  if (music != NULL) {
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = NULL;
  }
  music = Mix_LoadMUS(filename);

  if (music == NULL) {
    printf("Failed to load music: %s\n", Mix_GetError());
  }
}

void playMusic(int loop) { 
  if (Mix_PlayMusic(music, (loop) ? -1 : 0) == -1) {
    printf("Failed to play music: %s\n", Mix_GetError());
  }
}

void playSound(int id, int channel) { Mix_PlayChannel(channel, sounds[id], 0); }

void cleanUpSound(void) {
  for (int i = 0; i < SND_MAX; i++) {
    if (sounds[i] != NULL) {
      Mix_FreeChunk(sounds[i]);
    }
  }
  if (music != NULL) {
    Mix_FreeMusic(music);
  }
  Mix_CloseAudio();
  Mix_Quit();
}
