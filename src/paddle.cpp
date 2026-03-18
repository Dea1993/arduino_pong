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

bool check_paddle_movements(Paddle &p1, Paddle &p2) {
  bool need_refresh= false;
  if (digitalRead(P1_BTN_UP) == LOW) {
    p1.move_pad_up();
    need_refresh= true;
  }
  else if (digitalRead(P1_BTN_BOTTOM) == LOW) {
    p1.move_pad_down();
    need_refresh= true;
  }

  if (digitalRead(P2_BTN_UP) == LOW) {
    p2.move_pad_up();
    need_refresh= true;
  }
  else if (digitalRead(P2_BTN_BOTTOM) == LOW) {
    p2.move_pad_down();
    need_refresh= true;
  }
  return need_refresh;
}
