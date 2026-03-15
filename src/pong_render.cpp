#include <Arduino.h>
#include "config.h"

void render_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int p1_start, int p2_start, int ball_x, int ball_y) {
  // clear
  for (int x=0; x < MATRIX_WIDTH; x++) {
    for (int y=0; y < MATRIX_HEIGHT; y++) {
      frame[y][x]= 0;
    }
  }
  
  // players coords
  for (int i= p1_start; i < p1_start+BAR_LENGTH; i++) {
    frame[i][0]= 1;
  }
  for (int i= p2_start; i < p2_start+BAR_LENGTH; i++) {
    frame[i][MATRIX_WIDTH-1]= 1;
  }
  
  // ball coords
  frame[ball_y][ball_x]= 1;
}
