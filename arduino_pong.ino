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
  MENU,
  MENU_BOT_SKILLS,
  TIMER,
  RUN,
  SCORE,
  GAMEOVER,
  WAIT,
};
game_statuses game_status= MENU;

enum game_modes : uint8_t {PVP, PVC, CVC};
game_modes game_mode = PVP;


Ball ball(4, 6);

Paddle* p1= nullptr;
Paddle* p2= nullptr;
HumanPaddle human_pad1(1, P1_BTN_UP, P1_BTN_BOTTOM);
HumanPaddle human_pad2(4, P2_BTN_UP, P2_BTN_BOTTOM);
BotPaddle bot_pad1(1, 0);
BotPaddle bot_pad2(4, MATRIX_WIDTH-1);

uint8_t current_gmode_idx= 0;
bool update_menu= true;
bool mode_selected= false;
uint8_t current_bot_menu_idx= 0;
bool update_menu_bot_skills= true;

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
      if (digitalRead(P2_BTN_BOTTOM) == LOW && current_gmode_idx < sizeof(frame_gmodes)/sizeof(frame_gmodes[0]) -1) {
        current_gmode_idx += 1;
        update_menu= true;
      }
      else if (digitalRead(P2_BTN_UP) == LOW && current_gmode_idx > 0) {
        update_menu= true;
        current_gmode_idx -= 1;
        game_mode= PVP;
      }

      // 1. P vs P
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW && game_modes(current_gmode_idx) == PVP) {
        p1= &human_pad1;
        p2= &human_pad2;
        mode_selected= true;
      }
      // 2. P vs CPU
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW && game_modes(current_gmode_idx) == PVC) {
        p1= &human_pad1;
        p2= &bot_pad2;
        mode_selected= true;
        update_menu= false;
        game_mode= PVC;
      }
      // 3. CPU vs CPU
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW && game_modes(current_gmode_idx) == CVC) {
        p1= &bot_pad1;
        p2= &bot_pad2;
        mode_selected= true;
        update_menu= false;
        game_mode= CVC;
      }

      if (update_menu) {
        // show menu on the matrix
        const byte (*current_gmode)[12]= frame_gmodes[current_gmode_idx];
        matrix.loadPixels((uint8_t*)current_gmode, MATRIX_HEIGHT * MATRIX_WIDTH);
        update_menu= false;
        delay(300);
      }
      else if (mode_selected) {
        engine.set_players(p1, p2);
        renderer.set_players(p1, p2);
        if (game_mode == PVC || game_mode == CVC) {
          game_status= MENU_BOT_SKILLS;
          delay(300); // avoid accidental double click for next menu
        }
        else game_status= TIMER;
      }
      break;
    }

    case MENU_BOT_SKILLS: {
      if (digitalRead(P2_BTN_BOTTOM) == LOW && current_bot_menu_idx < sizeof(frame_bot_skills)/sizeof(frame_bot_skills[0]) -1) {
        current_bot_menu_idx += 1;
        update_menu_bot_skills= true;
      }
      else if (digitalRead(P2_BTN_UP) == LOW && current_bot_menu_idx > 0) {
        current_bot_menu_idx -= 1;
        update_menu_bot_skills= true;
      }
      else if (digitalRead(P1_BTN_UP) == LOW || digitalRead(P1_BTN_BOTTOM) == LOW) {
        if (!p1 -> is_human()) p1 -> set_skills(current_bot_menu_idx + 1);
        if (!p2 -> is_human()) p2 -> set_skills(current_bot_menu_idx + 1);
        game_status= TIMER;
        update_menu_bot_skills= false;
      } else update_menu_bot_skills= false;

      if (update_menu_bot_skills) {
        const byte (*current_skill_frame)[12]= frame_bot_skills[current_bot_menu_idx];
        matrix.loadPixels((uint8_t*)current_skill_frame, MATRIX_HEIGHT * MATRIX_WIDTH);
        update_menu= false;
        delay(300);
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
        game_status= MENU;
        engine.reset();
      }
      break;
  }
  delay(50);
}
