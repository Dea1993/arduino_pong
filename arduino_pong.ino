#include "Arduino_LED_Matrix.h"

#include "src/config.h"
#include "src/pong_render.h"
#include "src/pong_player.h"
#include "src/pong_ball.h"

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

// players coordinates
int players_coords[2]= {1, 4};
int players_scores[2]= {0, 0};

// initials balls coordinates
int ball_x= BALL_RESET_X;
int ball_y= BALL_RESET_Y;

int need_refresh= true;

int ball_delay= INITIAL_BALL_DELAY;

long exec_t2= millis();

enum game_statuses : uint8_t {
  TIMER,
  RUN,
  SCORE,
  GAMEOVER,
  WAIT,
};
game_statuses game_status= TIMER;


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
        delay(1000);
        matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
      }
      game_status= RUN;
      // delay the first ball movement
      exec_t2= millis() + FIRST_START_BALL_DELAY;
      break;

    case RUN:
      pong_move_p1(players_coords[0], need_refresh);
      pong_move_p2(players_coords[1], need_refresh);
      if (exec_t1 - exec_t2 > ball_delay) {
        need_refresh= true;
        bool scored= move_ball(ball_x, ball_y, ball_delay, players_coords, players_scores);
        if (scored) {
          game_status= SCORE;
          // delay the ball movement after score
          exec_t2= millis() + FIRST_START_BALL_DELAY;
        } else exec_t2= exec_t1;
      }
      // rerender matrix only if something is changed
      if (need_refresh) {
        render_matrix(frame, players_coords, ball_x, ball_y);
        matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
        need_refresh= 0;
      }
      delay(50);
      break;

    case SCORE:
      render_score(frame, players_scores);
      matrix.renderBitmap(frame, MATRIX_HEIGHT, MATRIX_WIDTH);
      delay(1000);
      if (players_scores[0] >= MAX_POINTS || players_scores[1] >= MAX_POINTS) {
        game_status= GAMEOVER;
      }
      else game_status= RUN;
      break;

    case GAMEOVER:
      render_winner(frame, matrix, players_scores);
      game_status= WAIT;
      break;

    case WAIT:
      // keep showing the winner waiting for a restart
      // restart game once one button is pressed
      if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW || digitalRead(P2_BTN_UP) == LOW || digitalRead(P2_BTN_BOTTOM) == LOW) {
        game_status= TIMER;
        players_scores[0]= 0;
        players_scores[1]= 0;
      }
      delay(100);
      break;
  }
}
