#include <Arduino.h>
#include "config.h"
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
