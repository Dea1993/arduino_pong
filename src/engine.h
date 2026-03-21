#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
#include "ball.h"
#include "paddle.h"
#include "config.h"

enum EngineEvents : uint8_t {NONE, P1SCORE, P2SCORE, P1_COLLISION, P2_COLLISION, WALL_COLLISION};

class Engine {

  private:
    Paddle* _p1;
    Paddle* _p2;
    Ball& _ball;
    EngineEvents _event= NONE;
    uint8_t _ball_mv_delay;
    uint8_t _hits = 0;

    bool _check_pad_ball_collision(Paddle *p);

  public:
    // inizialize Engine constructor, linking all args with private args
    Engine(Ball &ball, uint8_t ball_mv_delay)
        : _ball(ball), _ball_mv_delay(ball_mv_delay) {}

    void run();
    void set_players(Paddle *p_one, Paddle *p_two);
    bool control_players();
    uint8_t ball_movement_delay();
    EngineEvents get_event();
    void restart_ball();
    void reset();
};

#endif
