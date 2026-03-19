#include "Arduino_LED_Matrix.h"

#include "src/config.h"
#include "src/renderer.h"
#include "src/engine.h"
#include "src/paddle.h"
#include "src/ball.h"


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

ArduinoLEDMatrix matrix;

bool need_refresh= true;
uint8_t hits= 0;
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
// HumanPaddle p1(1, P1_BTN_UP, P1_BTN_BOTTOM);
// HumanPaddle p2(4, P2_BTN_UP, P2_BTN_BOTTOM);
BotPaddle p1(1, 0, 2);
BotPaddle p2(4, MATRIX_WIDTH-1, 2);
Engine engine(p1, p2, ball, INITIAL_BALL_DELAY);
Renderer renderer(p1, p2, ball, frame, matrix);

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
        renderer.render_timer(i);
        delay(1000);
      }
      game_status= RUN;
      // delay the first ball movement
      exec_t2= millis() + FIRST_START_BALL_DELAY;
      break;

    case RUN:
      // need_refresh= check_paddle_movements(p1, p2);
      need_refresh= engine.control_players(exec_t2);

      if (exec_t1 - exec_t2 > engine.ball_movement_delay()) {
        engine.run();
        if (engine.get_event() == P1SCORE || engine.get_event() == P2SCORE)
          game_status= SCORE;
        exec_t2= exec_t1;
        need_refresh= true;
      }
      // rerender matrix only if something is changed
      if (need_refresh) {
        renderer.render_matrix();
        need_refresh= false;
      }
      break;

    case SCORE:
      delay(300);
      renderer.render_score();
      engine.restart_ball();
      delay(1000);
      if (p1.get_score() >= MAX_POINTS || p2.get_score() >= MAX_POINTS)
        game_status= GAMEOVER;
      else {
        game_status= RUN;
        // before move again the ball wait a second
        renderer.render_matrix();
        exec_t2= millis() + FIRST_START_BALL_DELAY;
      }
      break;

    case GAMEOVER:
      renderer.render_winner();
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
