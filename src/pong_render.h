#ifndef PONG_RENDER_H
#define PONG_RENDER_H

#include <Arduino.h>
#include "Arduino_LED_Matrix.h"
#include "config.h"

void render_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int players_coords[2], int ball_x, int ball_y);
void render_score(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int players_scores[2]);
void render_timer(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int seconds);
void render_winner(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], ArduinoLEDMatrix &matrix, int players_scores[2]);

#endif
