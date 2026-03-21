#ifndef FONT_H
#define FONT_H

#include <Arduino.h>
#include "config.h"

extern const uint32_t pone_wins[5][4];
extern const uint32_t ptwo_wins[5][4];
extern const byte font_pong[10][8][3];

extern const byte pvp_frame[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte pvc_frame[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte cvc_frame[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte (*game_modes[3])[MATRIX_WIDTH];

#endif
