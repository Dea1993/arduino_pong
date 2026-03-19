#include "paddle.h"

void Paddle::move_pad_up() {
  if (_position > 0) {
    _position -= 1;
  }
}
void Paddle::move_pad_down() {
  if (_position + _height < MATRIX_HEIGHT) {
    _position += 1;
  }
}

void run_paddle() {
}

uint8_t Paddle::get_position() {
  return _position;
}

bool Paddle::is_human() {
  return _human;
}

void Paddle::increase_score() {
  if (_score <= 9) _score += 1;
}

uint8_t Paddle::get_score() {
  return _score;
}

void Paddle::reset() {
  _score= 0;
}

bool Paddle::check_pad_movement() {
  // redefine me
  return false;
}

bool Paddle::check_pad_movement(Ball &ball) {
  // redefine me
  return false;
}

bool HumanPaddle::check_pad_movement() {
  bool need_refresh= false;
  if (digitalRead(_pin_btn_top) == LOW) {
    this -> move_pad_up();
    need_refresh= true;
  }
  else if (digitalRead(_pin_btn_bottom) == LOW) {
    this -> move_pad_down();
    need_refresh= true;
  }
  return need_refresh;
}

bool BotPaddle::check_pad_movement(Ball &ball) {
  uint8_t y= ball.get_y();
  // TODO BotPaddle movement logics
  // on higher difficult level i could also check the ball direction
  // or at lover difficulty level i could also check the distance from the pad and move only when the ball si near
  for (uint8_t _py= _position; _py < _position+PADDLE_LENGTH; _py++) {
    // don't move if ball is already centered to the pad
    if (_py == y) continue;
    else if (_position - y >= 0) this -> move_pad_up();
    else this -> move_pad_down();
  }
}
