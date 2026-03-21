#ifndef FONT_H
#define FONT_H

#include <Arduino.h>
#include "config.h"

extern const uint32_t pone_wins[5][4];
extern const uint32_t ptwo_wins[5][4];
extern const byte font_pong[10][8][3];

extern const byte frame_pvp[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte frame_pvc[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte frame_cvc[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte (*frame_gmodes[3])[12];

extern const byte frame_bot_skill_easy[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte frame_bot_skill_hard[MATRIX_HEIGHT][MATRIX_WIDTH];
extern const byte (*frame_bot_skills[3])[12];

#endif
