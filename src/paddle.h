#ifndef PADDLE_H
#define PADDLE_H

#include <Arduino.h>
#include <cstdint>
#include "config.h"
#include "ball.h"

class Paddle {

  protected:
    // define player coordinates
    uint8_t _pos_y;
    uint8_t _height= PADDLE_LENGTH;
    uint8_t _score= 0;
    bool _human;

  public:
    Paddle (uint8_t pos_y, bool human) : _pos_y(pos_y), _human(human) {}
    void move_pad_up();
    void move_pad_down();
    uint8_t get_position();
    bool is_human();
    void increase_score();
    uint8_t get_score();
    void reset();
    virtual bool check_pad_movement();
    virtual bool check_pad_movement(Ball &ball);
    virtual uint8_t get_skills();
    virtual void set_skills(uint8_t skills);
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
    uint8_t _pos_x;
    uint8_t _skills; // this is the difficulty level

  public:
    BotPaddle(uint8_t position, uint8_t pos_x) 
              : Paddle(position, false), _pos_x(pos_x) {}
    bool check_pad_movement(Ball &ball);
    uint8_t get_skills();
    void set_skills(uint8_t skills);
};

#endif
