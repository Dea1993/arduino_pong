#include "Arduino_LED_Matrix.h"

#include "src/config.h"
#include "src/menu.h"
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
  MENU,
  MENU_BOT_SKILLS,
  TIMER,
  RUN,
  SCORE,
  GAMEOVER,
  WAIT,
};
game_statuses game_status= MENU;


Ball ball(4, 6);

Paddle* p1= nullptr;
Paddle* p2= nullptr;
HumanPaddle human_pad1(1, P1_BTN_UP, P1_BTN_BOTTOM);
HumanPaddle human_pad2(4, P2_BTN_UP, P2_BTN_BOTTOM);
BotPaddle bot_pad1(1, 0);
BotPaddle bot_pad2(4, MATRIX_WIDTH-1);
Menu menu;

// uint8_t current_gmode_idx= 0;
// bool update_menu= true;
// bool mode_selected= false;
// uint8_t current_bot_menu_idx= 0;
// bool update_menu_bot_skills= true;

Engine engine(ball, INITIAL_BALL_DELAY);
Renderer renderer(ball, frame, matrix);

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

    case MENU: {
      // switch modes
      if (digitalRead(P2_BTN_BOTTOM) == LOW) {
        menu.next_mode();
        const byte (*current_gmode)[12]= frame_gmodes[menu.get_mode()];
        matrix.loadPixels((uint8_t*)current_gmode, MATRIX_HEIGHT * MATRIX_WIDTH);
        delay(300);
      }
      else if (digitalRead(P2_BTN_UP) == LOW) {
        menu.prev_mode();
        const byte (*current_gmode)[12]= frame_gmodes[menu.get_mode()];
        matrix.loadPixels((uint8_t*)current_gmode, MATRIX_HEIGHT * MATRIX_WIDTH);
        delay(300);
      }

      // 1. P vs P
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW && menu.number_of_bots() == 0) {
        p1= &human_pad1;
        p2= &human_pad2;
        engine.set_players(p1, p2);
        renderer.set_players(p1, p2);
        game_status= TIMER;
      }
      // 2. P vs CPU
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW && menu.number_of_bots() == 1) {
        p1= &human_pad1;
        p2= &bot_pad2;
        engine.set_players(p1, p2);
        renderer.set_players(p1, p2);
        game_status= MENU_BOT_SKILLS;
        delay(300); // avoid accidental double click for next menu
      }
      // 3. CPU vs CPU
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW && menu.number_of_bots() == 2) {
        p1= &bot_pad1;
        p2= &bot_pad2;
        engine.set_players(p1, p2);
        renderer.set_players(p1, p2);
        game_status= MENU_BOT_SKILLS;
        delay(300); // avoid accidental double click for next menu
      }

      else {
        const byte (*current_gmode)[12]= frame_gmodes[menu.get_mode()];
        matrix.loadPixels((uint8_t*)current_gmode, MATRIX_HEIGHT * MATRIX_WIDTH);
      }

      break;
    }

    case MENU_BOT_SKILLS: {
      // switch difficulty level
      if (digitalRead(P2_BTN_BOTTOM) == LOW) {
        menu.increase_skills();
        const byte (*current_skill_frame)[12]= frame_bot_skills[menu.get_skill()];
        matrix.loadPixels((uint8_t*)current_skill_frame, MATRIX_HEIGHT * MATRIX_WIDTH);
        delay(300);
      }
      else if (digitalRead(P2_BTN_UP) == LOW) {
        menu.decrease_skills();
        const byte (*current_skill_frame)[12]= frame_bot_skills[menu.get_skill()];
        matrix.loadPixels((uint8_t*)current_skill_frame, MATRIX_HEIGHT * MATRIX_WIDTH);
        delay(300);
      }

      // choose difficulty level
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW) {
        if (!p1 -> is_human()) p1 -> set_skills(menu.get_skill() + 1);
        if (!p2 -> is_human()) p2 -> set_skills(menu.get_skill() + 1);
        game_status= TIMER;
      }

      else {
        const byte (*current_skill_frame)[12]= frame_bot_skills[menu.get_skill()];
        matrix.loadPixels((uint8_t*)current_skill_frame, MATRIX_HEIGHT * MATRIX_WIDTH);
      }
      break;
    }

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
      need_refresh= engine.control_players();

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
      if (p1 -> get_score() >= MAX_POINTS || p2 -> get_score() >= MAX_POINTS)
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
      if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW || 
          digitalRead(P2_BTN_UP) == LOW || digitalRead(P2_BTN_BOTTOM) == LOW) {
        engine.reset();
        game_status= MENU;
        delay(300);
      }
      break;
  }
  delay(50);
}
