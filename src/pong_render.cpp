#include <Arduino.h>
#include "Arduino_LED_Matrix.h"
#include "config.h"
#include "paddle.h"
#include "ball.h"
#include "font.h"

void clear_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH]) {
  for (int x=0; x < MATRIX_WIDTH; x++) {
    for (int y=0; y < MATRIX_HEIGHT; y++) {
      frame[y][x]= 0;
    }
  }
}

void render_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], Paddle &p1, Paddle &p2, Ball &ball) {
  clear_matrix(frame);
  uint8_t p1pos= p1.get_position();
  uint8_t p2pos= p2.get_position();
  // players coords
  for (int i= p1pos; i < p1pos+PADDLE_LENGTH; i++) {
    frame[i][0]= 1;
  }
  for (int i= p2pos; i < p2pos+PADDLE_LENGTH; i++) {
    frame[i][MATRIX_WIDTH-1]= 1;
  }
  
  // ball coords
  uint8_t bx= ball.get_x();
  uint8_t by= ball.get_y();
  frame[by][bx]= 1;
}

void render_score(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], Paddle &p1, Paddle &p2) {
  clear_matrix(frame);
  
  // player score separator (-)
  frame[4][5]= 1;
  frame[4][6]= 1;

  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      frame[h][w+1]= font_pong[p1.get_score()][h][w];
    }
  }
  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      frame[h][w+8]= font_pong[p2.get_score()][h][w];
    }
  }
}

void render_timer(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int seconds) {
  clear_matrix(frame);

  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      frame[h][w+5]= font_pong[seconds][h][w];
    }
  }
}

void render_winner(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], ArduinoLEDMatrix &matrix, Paddle &p1, Paddle &p2) {
  clear_matrix(frame);
  // check winner 
  if (p1.get_score() > p2.get_score()) {
    Serial.println("Player 1 wins!!!");
    matrix.loadSequence(pone_wins);
  }
  else {
    Serial.println("Player 2 wins!!!");
    matrix.loadSequence(ptwo_wins);
  }
  matrix.play(true);
}
