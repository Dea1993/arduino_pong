#ifndef PADDLE_H
#define PADDLE_H

#include <Arduino.h>
#include "config.h"

class Paddle {

  private:
    // define player coordinates
    uint8_t _position;
    uint8_t _height= PADDLE_LENGTH;
    uint8_t _score= 0;
    bool _human= true;

  public:
    Paddle (uint8_t _position) : _position(_position) {}
    void move_pad_up();
    void move_pad_down();
    uint8_t get_position();
    bool is_human();
    void increase_score();
    uint8_t get_score();
    void reset();
};

bool check_paddle_movements(Paddle &p1, Paddle &p2);

#endif
