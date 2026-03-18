#ifndef BALL_H
#define BALL_H

#include <Arduino.h>
#include "config.h"

class Ball {
  private:
    uint8_t _x, _y;
    int8_t _direction_x, _direction_y;

    void _init_directions(int8_t &_direction);

  public:
    Ball (uint8_t _x, uint8_t _y) : _x(_x), _y(_y) {}

    void move();
    void bounce_on_pad();
    void bounce_on_sides();
    int8_t get_direction_x();
    int8_t get_direction_y();
    void reset_position ();
    uint8_t get_x();
    uint8_t get_y();
};

#endif
