#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
#include "ball.h"
#include "paddle.h"
#include "config.h"

enum EngineEvents : uint8_t {NONE, P1SCORE, P2SCORE, P1_COLLISION, P2_COLLISION, WALL_COLLISION};

class Engine {

  private:
    Paddle& _p1;
    Paddle& _p2;
    Ball& _ball;
    EngineEvents _event= NONE;

    bool _check_pad_ball_collision(Paddle &p);
    void _print_score();

  public:
    // inizialize Engine constructor, linking all args with private args
    Engine(Paddle &p_one, Paddle &p_two, Ball &ball)
        : _p1(p_one), _p2(p_two), _ball(ball) {}

    void run();
    EngineEvents get_event();
    void reset();
};

#endif
