#include "Arduino_LED_Matrix.h"

#define P1_BTN_UP 13
#define P1_BTN_BOTTOM 12
#define P2_BTN_UP 11
#define P2_BTN_BOTTOM 10

// create LED matrix object
ArduinoLEDMatrix matrix;

// initial snake frame
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int p1_start= 1;
int p1_end= 3;

int p2_start= 4;
int p2_end= 6;

const uint32_t frame_game_over[] = {
  0x20410809,
  0x600600,
  0x90108204,
};


int p1_score= 0;
int p2_score= 0;

int loop_delay= 300;
long exec_t2= millis();
bool running= true;

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

void pong_controls() {
  if      (digitalRead(P1_BTN_UP) == LOW) Serial.println("P1 up");
  else if (digitalRead(P1_BTN_BOTTOM) == LOW) Serial.println("P1 down");
  else if (digitalRead(P2_BTN_UP) == LOW) Serial.println("P2 up");
  else if (digitalRead(P2_BTN_BOTTOM) == LOW) Serial.println("P2 down");
}

void loop() {
  if (!running) return;
  long exec_t1= millis();
  pong_controls();
  if (exec_t1 - exec_t2 > loop_delay) {
    matrix.renderBitmap(frame, 8, 12);
    exec_t2= exec_t1;
  }
  delay(10);
}