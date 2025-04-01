//
//  util.c
//  shooterTest
//
//  Created by Noah Lin  on 2025-03-29.
//
#include "util.h"
#include "common.h"
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))


int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
    int steps = MAX(abs(x1 - x2), abs(y1 - y2));

    if (steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2);
    *dx /= steps;

    *dy = (y1 - y2);
    *dy /= steps;
}

char *concat(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}
