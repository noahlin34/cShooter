//
//  util.h
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//
#pragma once
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

int collision(int, int, int, int, int, int, int, int);
void calcSlope(int, int, int, int, float *, float *);
char *concat(const char *, const char *);
