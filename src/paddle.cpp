#include <stdint.h>
#include "config.h"

class Paddle {

  private:
    // define player coordinates
    uint8_t _position, _height;
    bool _human;

  public:
    void move_pad_up(bool &need_refresh) {
      if (_position > 0) {
        _position -= 1;
        need_refresh= true;
      }
    }
    void move_pad_down(bool &need_refresh) {
      if (_position + _height <= MATRIX_HEIGHT) {
        _position += 1;
        need_refresh= true;
      }
    }

    uint8_t get_position() {
      return _position;
    }

    bool is_human() {
      return _human;
    }
};
