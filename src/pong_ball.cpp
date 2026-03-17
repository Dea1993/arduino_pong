#include <Arduino.h>
#include "config.h"
#include "pong_player.h"

// used to increase speed when game is too easy
int hits= 0;

// initially ball has no movements
// once game/round starts, balls gets random x and y movements
int ball_move_x= 0;
int ball_move_y= 0;

void random_ball_movement(int &ball_move_x, int &ball_move_y) {
  if (random(2) == 0) ball_move_x= 1;
  else ball_move_x= -1;
  if (random(2) == 0) ball_move_y= 1;
  else ball_move_y= -1;
}

void point_scored(int &ball_x, int &ball_y, int &ball_delay, int players_scores[2], int &ball_move_x, int &ball_move_y) {
  ball_x= BALL_RESET_X;
  ball_y= BALL_RESET_Y;
  random_ball_movement(ball_move_x, ball_move_y);

  Serial.print("P1: ");
  Serial.print(players_scores[0]);
  Serial.print(" - ");
  Serial.print("P2: ");
  Serial.print(players_scores[1]);
  Serial.println();

  hits= 0;
  ball_delay= INITIAL_BALL_DELAY;
}

bool move_ball(int &ball_x, int &ball_y, int &ball_delay, int players_coords[2], int players_scores[2]) {
  if (ball_x < 0 || ball_x > MATRIX_WIDTH-1 || ball_y < 0 || ball_y > MATRIX_HEIGHT-1) {
    // ball out of matrix limits
    ball_x= BALL_RESET_X;
    ball_y= BALL_RESET_Y;
    return false;
  }

  bool scored= false;
  
  // if ball is not moving, get random direction
  // this is the initial position
  if (ball_move_x == 0 || ball_move_y == 0) {
    // extract random number between 0 or 1 to select the directions
    random_ball_movement(ball_move_x, ball_move_y);
  }

  else if (ball_x == 0) {
    // if p1 collision: reverse x, go left
    if (!ball_player_collision(players_coords[0], ball_y)) {
      // else p2 score, reset board
      players_scores[1] += 1;
      scored= true;
      Serial.println("Player 2 Scores");
      point_scored(ball_x, ball_y, ball_delay, players_scores, ball_move_x, ball_move_y);
      return true;
    }
    else {
      hits += 1;
      ball_move_x= ball_move_x * -1;
    }
  }
  else if (ball_x == MATRIX_WIDTH-1) {
    if (!ball_player_collision(players_coords[1], ball_y)) {
      // else p1 score, reset board
      players_scores[0] += 1;
      scored= true;
      Serial.println("Player 1 Scores");
      point_scored(ball_x, ball_y, ball_delay, players_scores, ball_move_x, ball_move_y);
      return true;
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

  if (hits >= 6 && ball_delay >= 80) {
    // increase ball speed
    hits= 0;
    ball_delay-= 20;
  }

  ball_x+= ball_move_x;
  ball_y+= ball_move_y;
  return scored;
}
