#include "ball.h"

void Ball::_init_directions(int8_t &_direction) {
  if (random(2) == 0) _direction= 1;
  else _direction= -1;
}

void Ball::move() {
  if (!_direction_x) this -> _init_directions(_direction_x);
  if (!_direction_y) this -> _init_directions(_direction_y);

  // if (_x < 0 || _x > MATRIX_WIDTH-1 || _y < 0 || _y > MATRIX_HEIGHT-1) {
  //   this -> reset_position();
  // }

  if (_x + _direction_x >= 0 && _x + _direction_x < MATRIX_WIDTH)
    _x+= _direction_x;
  if (_y + _direction_y >= 0 && _y + _direction_y < MATRIX_HEIGHT)
    _y+= _direction_y;
}

void Ball::bounce_on_pad() {
  _direction_x *= -1;
}

void Ball::bounce_on_sides() {
  _direction_y *= -1;
}

int8_t Ball::get_direction_x() {
  return _direction_x;
}
int8_t Ball::get_direction_y() {
  return _direction_y;
}

void Ball::reset_position () {
  _x= BALL_RESET_X;
  int8_t delta= random(3);
  if (random(2) == 0) delta *= -1;
  _y= BALL_RESET_Y + delta;
  this -> _init_directions(_direction_x);
  this -> _init_directions(_direction_y);
}

uint8_t Ball::get_x() {
  return _x;
}

uint8_t Ball::get_y() {
  return _y;
}
