#ifndef PONG_RENDER_H
#define PONG_RENDER_H
#include <Arduino.h>
#include "Arduino_LED_Matrix.h"

void render_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int p1_start, int p2_start, int &need_refresh, int ball_x, int ball_y);

#endif
