#include <Arduino.h>
#include "Arduino_LED_Matrix.h"
#include "config.h"
#include "font.h"

void clear_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH]) {
  for (int x=0; x < MATRIX_WIDTH; x++) {
    for (int y=0; y < MATRIX_HEIGHT; y++) {
      frame[y][x]= 0;
    }
  }
}

void render_matrix(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int players_coords[2], int ball_x, int ball_y) {
  clear_matrix(frame);
  int player_one= players_coords[0];
  int player_two= players_coords[1];
  // players coords
  for (int i= player_one; i < player_one+BAR_LENGTH; i++) {
    frame[i][0]= 1;
  }
  for (int i= player_two; i < player_two+BAR_LENGTH; i++) {
    frame[i][MATRIX_WIDTH-1]= 1;
  }
  
  // ball coords
  frame[ball_y][ball_x]= 1;
}

void render_score(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], int players_scores[2]) {
  clear_matrix(frame);
  int player_one= players_scores[0];
  int player_two= players_scores[1];
  if (player_one > 9) player_one = 9;
  if (player_two > 9) player_two = 9;
  
  // player score separator (-)
  frame[4][5]= 1;
  frame[4][6]= 1;

  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      frame[h][w+1]= font_pong[player_one][h][w];
    }
  }
  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      frame[h][w+8]= font_pong[player_two][h][w];
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

void render_winner(byte frame[MATRIX_HEIGHT][MATRIX_WIDTH], ArduinoLEDMatrix &matrix, int players_scores[2]) {
  clear_matrix(frame);
  // check winner 
  if (players_scores[0] > players_scores[1]) {
    Serial.println("Player 1 wins!!!");
    matrix.loadSequence(pone_wins);
  }
  else {
    Serial.println("Player 2 wins!!!");
    matrix.loadSequence(ptwo_wins);
  }
  matrix.play(true);
}
