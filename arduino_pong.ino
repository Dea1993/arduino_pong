#include "Arduino_LED_Matrix.h"

#include "config.h"
#include "pong_render.h"
#include "pong_player.h"
#include "pong_ball.h"

// create LED matrix object
ArduinoLEDMatrix matrix;

// initial pong frame
byte frame[MATRIX_HEIGHT][MATRIX_WIDTH] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// players coordinates
int p1_start= 1;
int p2_start= 4;

// initials balls coordinates
int ball_x= BALL_RESET_X;
int ball_y= BALL_RESET_Y;

int need_refresh= 1;

int ball_delay= INITIAL_BALL_DELAY;

long exec_t2= millis();

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  // stard LED matrix
  matrix.begin();
  pinMode(P1_BTN_UP, INPUT_PULLUP);
  pinMode(P1_BTN_BOTTOM, INPUT_PULLUP);
  pinMode(P2_BTN_UP, INPUT_PULLUP);
  pinMode(P2_BTN_BOTTOM, INPUT_PULLUP);

  randomSeed(analogRead(0));
}

void loop() {
  long exec_t1= millis();
  pong_move_p1(p1_start, need_refresh);
  pong_move_p2(p2_start, need_refresh);
  if (exec_t1 - exec_t2 > ball_delay) {
    move_ball(ball_x, ball_y, ball_delay, p1_start, p2_start, need_refresh);
    exec_t2= exec_t1;
  }
  if (need_refresh) {
    render_matrix(frame, p1_start, p2_start, ball_x, ball_y);
    matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
    need_refresh= 0;
  }
  delay(50);
}
