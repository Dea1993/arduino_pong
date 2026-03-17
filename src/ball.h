#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "config.h"

class Ball {
  private:
    uint8_t _x, _y;
    int8_t _direction_x, _direction_y;

  public:
    void move();
    void bounce_on_pad();
    void bounce_on_sides();
    void reset_position ();
    uint8_t get_x();
    uint8_t get_y();
};

#endif
