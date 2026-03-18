#ifndef RENDERER_H
#define RENDERER_H

#include <Arduino.h>
#include "Arduino_LED_Matrix.h"
#include "config.h"
#include "font.h"
#include "paddle.h"
#include "ball.h"

class Renderer {

  private:
    // define player coordinates
    Paddle& _p1;
    Paddle& _p2;
    Ball& _ball;
    byte (&_frame)[MATRIX_HEIGHT][MATRIX_WIDTH];
    ArduinoLEDMatrix& _matrix;

    void _clear_matrix();

  public:
    Renderer (Paddle &p1, Paddle &p2, Ball &ball, byte (&frame)[MATRIX_HEIGHT][MATRIX_WIDTH], ArduinoLEDMatrix &matrix) 
            : _p1(p1), _p2(p2), _ball(ball), _frame(frame), _matrix(matrix) {}

    void render_timer(uint8_t seconds);
    void render_matrix();
    void render_score();
    void render_winner();
};

#endif
