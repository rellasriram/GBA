//Mah Tej Rella

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

//Images
#include "images/cow_object.h"
#include "images/lose_screen.h"
#include "images/start_screen.h"
#include "images/win_screen.h"
#include "images/invinsible_cow.h"
#include "images/cowfound.h"

Finder f;
Position p;
Object o;
char steps[51];
char buffer[51];

int main(void) {

  REG_DISPCNT = MODE3 | BG2_ENABLE;
  videoBuffer[1208] = 0x7fff;

  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  gba_state state = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons
    waitForVBlank();

    switch (state) {
      case START:
        drawFullScreenImageDMA(start_screen);
        state = STARTGAME;
        break;
      case STARTGAME:
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          state = LOADGAME;
        }
        break;
      case LOADGAME:
        fillScreenDMA(BLACK);
        state = PLAY;
        break;
      case PLAY:
        playFind(&state);
        break;
      case WIN:
        drawFullScreenImageDMA(win_screen);
        state = STEPS;
        break;
      case COW:
        drawFullScreenImageDMA(cowfound);
        state = WIN;
        break;
      case LOSE:
        drawFullScreenImageDMA(lose_screen);
        state = OUTOFSTEPS;
        break;
      case STEPS:
        sprintf(buffer, "In: %d steps", f.steps);
        drawString(120, 150, buffer, BLACK);

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = LOADGAME;
        }
        break;
      case OUTOFSTEPS:
        sprintf(buffer, "Steps >= %d steps", f.steps);
        drawString(120, 110, buffer, BLACK);

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = LOADGAME;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}

Object createObject(void) {
  Object o;
  o.row = 10;
  o.col = 120;
  drawImageDMA(o.row, o.col, 40, 40, invinsible_cow);
  return o;
}

void createFinder(void) {
  f.col = 108;
  f.top = 140;
  f.bot = f.top + 5;
  f.left = 108;
  f.right = f.left + 25;

  drawRectDMA(140, f.col, 10, 10, WHITE);
}

void delay(int n) {
  volatile int x = 0;
  for (int i = 0; i < n*8000; i++) {
    x++;
  }
}

void trackSteps(void) {
  drawRectDMA(150, 5, 70, 10, BLACK);

  f.steps++;

  char buffer[51];
  sprintf(buffer, "Steps: %d", f.steps);
  drawString(150, 5, buffer, WHITE);
}

void playFind(gba_state *state) {
  o = createObject();

  u32 buttons;

  while (*state == PLAY) {
    waitForVBlank();
    buttons = BUTTONS;

    if (KEY_DOWN(BUTTON_LEFT, buttons)) {
      trackSteps();
      drawRectDMA(f.row, f.col, 10, 10, BLACK);

      f.col--;
      f.left--;
      f.right--;

      drawRectDMA(f.row, f.col, 10, 10, WHITE);
    }

    if (KEY_DOWN(BUTTON_RIGHT, buttons)) {
      trackSteps();
      drawRectDMA(f.row, f.col, 10, 10, BLACK);

      f.col++;
      f.left++;
      f.right++;

      drawRectDMA(f.row, f.col, 10, 10, WHITE);
    }

    if (KEY_DOWN(BUTTON_UP, buttons)) {
      trackSteps();
      drawRectDMA(f.row, f.col, 10, 10, BLACK);

      f.row--;

      drawRectDMA(f.row, f.col, 10, 10, WHITE);
    }

    if (KEY_DOWN(BUTTON_DOWN, buttons)) {
      trackSteps();
      drawRectDMA(f.row, f.col, 10, 10, BLACK);
      
      f.row++;

      drawRectDMA(f.row, f.col, 10, 10, WHITE);
    }

    for (int i = o.row; i <= o.row + 40; i++) {
      for (int j = o.col; j <= o.col + 40; j++) {
          if (f.row == i && f.col == j) {
            *state = WIN;
          }
      }
    }

    delay(1);

    if (f.steps == 500) {
      *state = LOSE;
    }

    if (KEY_DOWN(BUTTON_SELECT, buttons)) {
      f.row = 0;
      f.col = 0;
      f.steps = 0;
      fillScreenDMA(BLACK);
      *state = START;
    }
  }
}