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

void Engine::run(uint8_t &ball_delay) {
  _event= NONE;
  _ball.move();
  uint8_t bx= _ball.get_x();
  uint8_t by= _ball.get_y();

  if (bx == 0) {
    if (!this -> _check_pad_ball_collision(_p1)) {
      // p2 scores
      _p2.increase_score();
      _ball.reset_position();
      ball_delay= INITIAL_BALL_DELAY;
      Serial.println("Player 2 Scores");
      this -> _print_score();
      _event= P2SCORE;
      return;
    }
    else {
      _hits += 1;
      _ball.bounce_on_pad();
      _event= P2_COLLISION;
    }
  }
  else if (bx >= MATRIX_WIDTH-1) {
    if (!this -> _check_pad_ball_collision(_p2)) {
      // p1 scores
      _p1.increase_score();
      _ball.reset_position();
      ball_delay= INITIAL_BALL_DELAY;
      Serial.println("Player 1 Scores");
      this -> _print_score();
      _event= P1SCORE;
      return;
    }
    else {
      _hits += 1;
      _ball.bounce_on_pad();
      _event= P1_COLLISION;
    }
  }
  
  if (by == 0 || by == MATRIX_HEIGHT-1) {
    _ball.bounce_on_sides();
    _event= WALL_COLLISION;
  }

  // increase ball speed every 6 hits on pads
  // if ball is not at max speed
  if (_hits >= 6 && ball_delay >= 80) {
    _hits= 0;
    ball_delay-= 20;
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
