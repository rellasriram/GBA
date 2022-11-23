#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

void startScreen(void);

typedef enum {
  START,
  STARTGAME,
  LOADGAME,
  PLAY,
  WIN,
  COW,
  LOSE,
  STEPS,
  OUTOFSTEPS,
} gba_state;

typedef struct finder {
  int row;
  int col;
  int top;
  int bot;
  int left;
  int right;
  int steps;
} Finder;

typedef struct object {
  int row;
  int col;
  int top;
  int bot;
  int left;
  int right;
} Object;

typedef struct position {
  int row;
  int col;
} Position;

void delay(int i);
void trackSteps(void);
void createFinder(void);
Object createObject(void);
void playFind(gba_state *state);

#endif
