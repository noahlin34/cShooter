#pragma once
#include "common.h"
#include "defs.h"

void initSound(void);
void loadMusic(char *);
void playMusic(int);
void playSound(int id, int channel);
void cleanUpSound(void);