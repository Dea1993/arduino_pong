#include <Arduino.h>
#include "ball.h"
#include "paddle.h"
#include "config.h"
#include "engine.h"

bool Engine::_check_pad_ball_collision(Paddle &p) {
  uint8_t ppos= p.get_position();
  for (int p= ppos; p < ppos + PADDLE_LENGTH; p++) {
    if (_ball.get_y() == p) {
      return true;
    }
  }
  return false;
}

void Engine::_print_score() {
  Serial.print("P1: ");
  Serial.print(_p1.get_score());
  Serial.print(" - ");
  Serial.print("P2: ");
  Serial.print(_p2.get_score());
  Serial.println();
}

void Engine::run() {
  _event= NONE;
  _ball.move();
  uint8_t bx= _ball.get_x();
  uint8_t by= _ball.get_y();
  
  // pad is 1 pixel far from the edge, so i need to calc this delta
  if (bx <= 1) {
    // score the point only if ball reached the edge
    if (this -> _check_pad_ball_collision(_p1) && bx == 1) {
      _ball.bounce_on_pad();
      _event= P2_COLLISION;
    }
    else if (bx <= 0) {
      // p2 scores
      _p2.increase_score();
      Serial.println("Player 2 Scores");
      this -> _print_score();
      _event= P2SCORE;
      return;
    }
  }
  else if (bx >= MATRIX_WIDTH-2) {
    // score the point only if ball reached the edge
    if (this -> _check_pad_ball_collision(_p2) && bx == MATRIX_WIDTH-2) {
      _ball.bounce_on_pad();
      _event= P1_COLLISION;
    }
    else if (bx >= MATRIX_WIDTH-1) {
      // p1 scores
      _p1.increase_score();
      Serial.println("Player 1 Scores");
      this -> _print_score();
      _event= P1SCORE;
      return;
    }
  }
  
  if (by == 0 || by == MATRIX_HEIGHT-1) {
    _ball.bounce_on_sides();
    _event= WALL_COLLISION;
  }
}

EngineEvents Engine::get_event() {
  return _event;
}

void Engine::reset() {
  _p1.reset();
  _p2.reset();
  _ball.reset_position();
}
