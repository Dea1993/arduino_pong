#include "Arduino_LED_Matrix.h"

#include "src/config.h"
#include "src/pong_render.h"

#include "src/paddle.h"
#include "src/ball.h"
#include "src/engine.h"

// create LED matrix object
ArduinoLEDMatrix matrix;

// initial pong frame matrix
byte frame[MATRIX_HEIGHT][MATRIX_WIDTH] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int need_refresh= true;
uint8_t hits= 0;
uint8_t ball_delay= INITIAL_BALL_DELAY;
long exec_t2= millis();

enum game_statuses : uint8_t {
  TIMER,
  RUN,
  SCORE,
  GAMEOVER,
  WAIT,
};
game_statuses game_status= TIMER;

Ball ball(4, 6);
Paddle p1(1);
Paddle p2(4);
Engine engine(p1, p2, ball);

void setup() {
  Serial.begin(9600);
  // start LED matrix
  matrix.begin();

  pinMode(P1_BTN_UP, INPUT_PULLUP);
  pinMode(P1_BTN_BOTTOM, INPUT_PULLUP);
  pinMode(P2_BTN_UP, INPUT_PULLUP);
  pinMode(P2_BTN_BOTTOM, INPUT_PULLUP);

  randomSeed(millis());
}

void loop() {
  long exec_t1= millis();

  switch (game_status) {

    case TIMER:
      for (int i = START_TIMER; i >= 0; i--) {
        render_timer(frame, i);
        matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
        delay(1000);
      }
      game_status= RUN;
      // delay the first ball movement
      exec_t2= millis() + FIRST_START_BALL_DELAY;
      break;

    case RUN:
      need_refresh= check_paddle_movements(p1, p2);

      if (exec_t1 - exec_t2 > ball_delay) {
        engine.run();
        if (engine.get_event() == P1SCORE || engine.get_event() == P2SCORE) {
          game_status= SCORE;
        }
        else if (engine.get_event() == P1_COLLISION || engine.get_event() == P2_COLLISION) {
          hits++;
          if (hits >= 6 && ball_delay >= 80) {
            hits= 0;
            ball_delay-= 20;
          }
        }
        exec_t2= exec_t1;
        need_refresh= true;
      }
      // rerender matrix only if something is changed
      if (need_refresh) {
        render_matrix(frame, p1, p2, ball);
        matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
        need_refresh= false;
      }
      break;

    case SCORE:
      delay(300); // small delay to let see the last ball position
      render_score(frame, p1, p2);
      matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
      delay(1000);
      ball.reset_position();
      ball_delay= INITIAL_BALL_DELAY;
      if (p1.get_score() >= MAX_POINTS || p2.get_score() >= MAX_POINTS) {
        game_status= GAMEOVER;
      }
      else {
        game_status= RUN;
        // before move again the ball wait a second
        render_matrix(frame, p1, p2, ball);
        matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
        delay(1000);
      }
      break;

    case GAMEOVER:
      render_winner(frame, matrix, p1, p2);
      game_status= WAIT;
      break;

    case WAIT:
      // keep showing the winner waiting for a restart
      // restart game once one button is pressed
      if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW || digitalRead(P2_BTN_UP) == LOW || digitalRead(P2_BTN_BOTTOM) == LOW) {
        game_status= TIMER;
        engine.reset();
      }
      break;
  }
  delay(50);
}
