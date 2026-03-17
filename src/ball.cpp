#include <Arduino.h>
#include <stdint.h>
#include "config.h"

class Ball {

  private:

    uint8_t _x, _y;
    int8_t _direction_x, _direction_y;

    void _init_directions(int8_t &_direction) {
      if (random(2) == 0) _direction= 1;
      else _direction= -1;
    }

  public:

    void move() {
      if (!_direction_x) _init_directions(_direction_x);
      if (!_direction_y) _init_directions(_direction_y);

      if (_x + _direction_x >= 0 && _x + _direction_x < MATRIX_WIDTH)
        _x+= _direction_x;
      if (_y + _direction_y >= 0 && _y + _direction_y < MATRIX_HEIGHT)
        _y+= _direction_y;
    }

    void bounce_on_pad() {
      _direction_x *= -1;
    }

    void bounce_on_sides() {
      _direction_y *= -1;
    }

    void reset_position () {
      _x= BALL_RESET_X;
      _y= BALL_RESET_Y;
      _init_directions(_direction_x);
      _init_directions(_direction_y);
    }

    uint8_t get_x() {
      return _x;
    }

    uint8_t get_y() {
      return _y;
    }
};
