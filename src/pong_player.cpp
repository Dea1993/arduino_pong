#include <Arduino.h>
#include "config.h"

int ball_player_collision(int player, int ball_y) {
  for (int p= player; p < player + BAR_LENGTH; p++) {
    if (ball_y == p) {
      return 1;
    }
  }
  return 0;
}

int pong_move_p1(int &p1_start, int &need_refresh) {
  if (digitalRead(P1_BTN_UP) == LOW && p1_start > 0) {
    p1_start -= 1;
    need_refresh= true;
  }
  else if (digitalRead(P1_BTN_BOTTOM) == LOW && p1_start < 5) {
    p1_start += 1;
    need_refresh= true;
  }
}

int pong_move_p2(int &p2_start, int &need_refresh) {
  if (digitalRead(P2_BTN_UP) == LOW && p2_start > 0) {
    p2_start -= 1;
    need_refresh= true;
  }
  else if (digitalRead(P2_BTN_BOTTOM) == LOW && p2_start < 5) {
    p2_start += 1;
    need_refresh= true;
  }
}
