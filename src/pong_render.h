#ifndef PONG_RENDER_H
#define PONG_RENDER_H

#include <Arduino.h>
#include "Arduino_LED_Matrix.h"
#include "config.h"
#include "paddle.h"
#include "ball.h"

void render_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], Paddle &p1, Paddle &p2, Ball &ball);
void render_score(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], Paddle &p1, Paddle &p2);
void render_timer(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int seconds);
void render_winner(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], ArduinoLEDMatrix &matrix, Paddle &p1, Paddle &p2);

#endif
