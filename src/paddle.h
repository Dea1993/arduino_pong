#ifndef PADDLE_H
#define PADDLE_H

#include <stdint.h>
#include "config.h"

class Paddle {

  private:
    // define player coordinates
    uint8_t position, height;
    bool human;

  public:
    void move_pad_up(bool &need_refresh);
    void move_pad_down(bool &need_refresh);
    uint8_t get_position();
    bool is_human();
};

#endif
