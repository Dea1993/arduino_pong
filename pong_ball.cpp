#include <Arduino.h>
#include "config.h"
#include "pong_player.h"

// used to increase speed when game is too easy
int hits= 0;

int p1_score= 0;
int p2_score= 0;

// initially ball has no movements
// once game/round starts, balls gets random x and y movements
int ball_move_x= 0;
int ball_move_y= 0;

void point_scored(int &ball_x, int &ball_y, int &loop_delay) {
  ball_x= BALL_RESET_X;
  ball_y= BALL_RESET_Y;
  Serial.print("P1: ");
  Serial.print(p1_score);
  Serial.print(" - ");
  Serial.print("P2: ");
  Serial.print(p2_score);
  Serial.println();

  hits= 0;
  loop_delay= INITIAL_LOOP_DELAY;
}

void move_ball(int &ball_x, int &ball_y, int &loop_delay, int p1_start, int p2_start, int &need_refresh) {
  need_refresh= 1;
  if (ball_x < 0 || ball_x > MATRIX_WIDTH-1 || ball_y < 0 || ball_y > MATRIX_HEIGHT-1) {
    // ball out of matrix limits
    ball_x= BALL_RESET_X;
    ball_y= BALL_RESET_Y;
    return;
  }
  
  // if ball is not moving, get random direction
  // this is the initial position
  if (ball_move_x == 0 || ball_move_y == 0) {
    // extract random number between 0 or 1 to select the directions
    if (random(2) == 0) ball_move_x= 1;
    else ball_move_x= -1;
    if (random(2) == 0) ball_move_y= 1;
    else ball_move_y= -1;
  }

  else if (ball_x == 0) {
    // if p1 collision: reverse x, go left
    if (!ball_player_collision(p1_start, ball_y)) {
      // else p2 score, reset board
      p2_score += 1;
      Serial.println("Player 2 Scores");
      point_scored(ball_x, ball_y, loop_delay);
    }
    else {
      hits += 1;
      ball_move_x= ball_move_x * -1;
    }
  }
  else if (ball_x == MATRIX_WIDTH-1) {
    if (!ball_player_collision(p2_start, ball_y)) {
      // else p1 score, reset board
      p1_score += 1;
      Serial.println("Player 1 Scores");
      point_scored(ball_x, ball_y, loop_delay);
    }
    else {
      hits += 1;
      ball_move_x= ball_move_x * -1;
    }
  }

  if (ball_y == 0 || ball_y == MATRIX_HEIGHT-1) {
    // reverse y, go down
    ball_move_y= ball_move_y * -1;
  }

  if (hits >= 6 && loop_delay >= 80) {
    // increase ball speed
    hits = 0;
    loop_delay -= 20;
  }

  ball_x+= ball_move_x;
  ball_y+= ball_move_y;
}
