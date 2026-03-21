#include "engine.h"

bool Engine::_check_pad_ball_collision(Paddle *p) {
  uint8_t ppos= p -> get_position();
  for (int p= ppos; p < ppos + PADDLE_LENGTH; p++) {
    if (_ball.get_y() == p) {
      return true;
    }
  }
  return false;
}

void Engine::run() {
  // if (_event == P1SCORE || _event == P2SCORE) this -> _soft_reset();

  _event= NONE;
  _ball.move();
  uint8_t bx= _ball.get_x();
  uint8_t by= _ball.get_y();
  int8_t ball_dir= _ball.get_direction_x(); 
  // pad is 1 pixel far from the edge, so i need to calc this delta
  // check also if the ball is already moving away, to skip useless checks
  if (bx <= 1 && ball_dir < 0) {
    // score the point only if ball reached the edge
    if (this -> _check_pad_ball_collision(_p1) && bx <= 1) {
      _ball.bounce_on_pad();
      _event= P1_COLLISION;
      _hits++;
    }
    else if (bx <= 0) {
      // p2 scores
      _p2 -> increase_score();
      _event= P2SCORE;
      return;
    }
  }
  else if (bx >= MATRIX_WIDTH-2 && ball_dir > 0) {
    // score the point only if ball reached the edge
    if (this -> _check_pad_ball_collision(_p2) && bx >= MATRIX_WIDTH-2) {
      _ball.bounce_on_pad();
      _event= P2_COLLISION;
      _hits++;
    }
    else if (bx >= MATRIX_WIDTH-1) {
      // p1 scores
      _p1 -> increase_score();
      _event= P1SCORE;
      return;
    }
  }
  
  if (by == 0 || by == MATRIX_HEIGHT-1) {
    _ball.bounce_on_sides();
    _event= WALL_COLLISION;
  }

  if (_hits >= 6 && _ball_mv_delay >= 80) {
    _hits= 0;
    _ball_mv_delay -= 20;
  }
}

void Engine::set_players(Paddle *p1, Paddle *p2) {
  _p1= p1;
  _p2= p2;
}

bool Engine::control_players() {
  bool need_refresh= false;

  if (_p1 -> is_human()) need_refresh |= _p1 -> check_pad_movement();
  else need_refresh |= _p1 -> check_pad_movement(_ball);
  if (_p2 -> is_human()) need_refresh |= _p2 -> check_pad_movement();
  else need_refresh |= _p2 -> check_pad_movement(_ball);
  return need_refresh;
}

uint8_t Engine::ball_movement_delay() {
  return _ball_mv_delay;
}


EngineEvents Engine::get_event() {
  return _event;
}

void Engine::restart_ball() {
  _ball.reset_position();
  _ball_mv_delay= INITIAL_BALL_DELAY;
}

void Engine::reset() {
  this -> restart_ball();
  _p1 -> reset();
  _p2 -> reset();
}
