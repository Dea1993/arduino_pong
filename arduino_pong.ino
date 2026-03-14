#include "Arduino_LED_Matrix.h"

#define P1_BTN_UP 13
#define P1_BTN_BOTTOM 12
#define P2_BTN_UP 11
#define P2_BTN_BOTTOM 10

// create LED matrix object
ArduinoLEDMatrix matrix;

// initial pong frame
byte frame[8][12] = {
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
int p1_end= 3;

int p2_start= 4;
int p2_end= 6;

// initials balls coordinates
int ball_reset_x=6;
int ball_reset_y=3;
int ball_x= ball_reset_x;
int ball_y= ball_reset_y;

// initially ball has no movements
// once game/round starts, balls gets random x and y movements
int ball_move_x= 0;
int ball_move_y= 0;

int bar_length= 3;
int p1_score= 0;
int p2_score= 0;

int need_refresh= 1;

int loop_delay= 220;
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

void render_matrix() {
  if (!need_refresh) return;
  need_refresh= 0;
  // clear
  for (int x=0; x < 12; x++) {
    for (int y=0; y < 8; y++) {
      frame[y][x]= 0;
      frame[y][x]= 0;
    }
  }
  
  // players coords
  for (int i= p1_start; i < p1_start+bar_length; i++) {
    frame[i][0]= 1;
  }
  for (int i= p2_start; i < p2_start+bar_length; i++) {
    frame[i][11]= 1;
  }
  
  // ball coords
  frame[ball_y][ball_x]= 1;

  matrix.renderBitmap(frame, 8, 12);
}

void pong_move_p1() {
  if (digitalRead(P1_BTN_UP) == LOW && p1_start > 0) {
    p1_start -= 1;
    need_refresh= 1;
  }
  else if (digitalRead(P1_BTN_BOTTOM) == LOW && p1_start < 5) {
    p1_start += 1;
    need_refresh= 1;
  }
}

void pong_move_p2() {
  if (digitalRead(P2_BTN_UP) == LOW && p2_start > 0) {
    p2_start -= 1;
    need_refresh= 1;
  }
  else if (digitalRead(P2_BTN_BOTTOM) == LOW && p2_start < 5) {
    p2_start += 1;
    need_refresh= 1;
  }
}

void move_ball() {
  need_refresh= 1;
  if (ball_move_x == 0 || ball_move_y == 0) {
    // extract random number between 0 or 1 to select the directions
    if (random(2) == 0) ball_move_x= 1;
    else ball_move_x= -1;
    if (random(2) == 0) ball_move_y= 1;
    else ball_move_y= -1;
  }

  else if (ball_x == 0) {
    // if p1 collision: reverse x, go left
    int hit= 0;
    for (int p1= p1_start; p1 < p1_start + bar_length; p1++) {
      if (ball_y == p1) {
        ball_move_x= ball_move_x * -1;
        hit= 1;
        break;
      }
    }

    if (!hit) {
      // else p2 score, reset board
      ball_x= ball_reset_x;
      ball_y= ball_reset_y;
      p2_score += 1;

      Serial.print("Player 2 score: ");
      Serial.println(p2_score);
      Serial.print("Player 1 score: ");
      Serial.println(p1_score);
    }
  }
  else if (ball_x == 11) {
    int hit= 0;
    for (int p2= p2_start; p2 < p2_start + bar_length; p2++) {
    // if p2 collision: reverse x, go left
      if (ball_y == p2) {
        ball_move_x= ball_move_x * -1;
        hit= 1;
        break;
      }
    }
    
    if (!hit) {
      // else p1 score, reset board
      ball_x= ball_reset_x;
      ball_y= ball_reset_y;
      p1_score += 1;
      Serial.print("Player 2: ");
      Serial.println(p2_score);
      Serial.print("Player 1: ");
      Serial.println(p1_score);
    }
  }

  else if (ball_y == 0 || ball_y == 7) {
    // reverse y, go down
    ball_move_y= ball_move_y * -1;
  }

  ball_x+= ball_move_x;
  ball_y+= ball_move_y;
}

void loop() {
  long exec_t1= millis();
  pong_move_p1();
  pong_move_p2();
  render_matrix();
  if (exec_t1 - exec_t2 > loop_delay) {
    move_ball();
    exec_t2= exec_t1;
  }
  delay(10);
}
