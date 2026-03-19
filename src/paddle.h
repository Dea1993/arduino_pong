#ifndef PADDLE_H
#define PADDLE_H

#include <Arduino.h>
#include "config.h"
#include "ball.h"

class Paddle {

  protected:
    // define player coordinates
    uint8_t _position;
    uint8_t _height= PADDLE_LENGTH;
    uint8_t _score= 0;
    bool _human;

  public:
    Paddle (uint8_t position, bool human) : _position(position), _human(human) {}
    void move_pad_up();
    void move_pad_down();
    uint8_t get_position();
    bool is_human();
    void increase_score();
    uint8_t get_score();
    void reset();
    virtual bool check_pad_movement();
    virtual bool check_pad_movement(Ball &ball);
};

class HumanPaddle : public Paddle {
  private:
    uint8_t _pin_btn_top;
    uint8_t _pin_btn_bottom;
  public:
    HumanPaddle(uint8_t position, uint8_t pin_btn_top, uint8_t pin_btn_bottom) 
              : Paddle(position, true), _pin_btn_top(pin_btn_top), _pin_btn_bottom(pin_btn_bottom) {}
    bool check_pad_movement();
};

class BotPaddle : public Paddle {
  private:
    uint8_t _level; // this is the difficulty level

  public:
    BotPaddle(uint8_t position, uint8_t level) 
              : Paddle(position, false), _level(level) {
                if (_level < 1) _level= 1;
                if (_level > 3) _level= 3;
              }
    bool check_pad_movement(Ball &ball);
};

#endif
