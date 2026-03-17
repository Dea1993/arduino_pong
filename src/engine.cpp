#include <Arduino.h>
#include "ball.h"
#include "paddle.h"
#include "config.h"

class Engine {

  private:
    Paddle& _p1;
    Paddle& _p2;
    Ball& _ball;
    uint8_t _p1_score, _p2_score;
    uint8_t _hits;

    bool _check_pad_ball_collision(Paddle &p1, Ball &ball) {
      return false;
    }

    void point_scored(Ball &ball) {
      ball.reset_position();

      Serial.print("P1: ");
      Serial.print(_p1_score);
      Serial.print(" - ");
      Serial.print("P2: ");
      Serial.print(_p2_score);
      Serial.println();

      _hits= 0;
      // ball_delay= INITIAL_BALL_DELAY;
    }

  public:
    // inizialize Engine constructor, linking all args with private args
    Engine(Paddle &p_one, Paddle &p_two, Ball &ball): _p1(p_one), _p2(p_two), _ball(ball) {}

    void run(uint8_t &ball_delay) {
      _ball.move();
      uint8_t bx= _ball.get_x();
      uint8_t by= _ball.get_y();

      if (bx == 0) {
        if (!_check_pad_ball_collision(_p1, _ball)) {
          // p2 scores
          _p2_score +=1;
          Serial.println("Player 2 Scores");
        }
        else {
          _hits += 1;
          _ball.bounce_on_pad();
        }
      }
      else if (bx >= MATRIX_WIDTH-1) {
        if (!_check_pad_ball_collision(_p2, _ball)) {
          // p1 scores
          _p1_score += 1;
          Serial.println("Player 1 Scores");
        }
      }

      if (by == 0 || by == MATRIX_HEIGHT-1) _ball.bounce_on_sides();

      if (_hits >= 6 && ball_delay >= 80) {
        // increase ball speed
        _hits= 0;
        ball_delay-= 20;
      }
    }
};
