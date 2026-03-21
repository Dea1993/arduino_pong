#include "paddle.h"
#include <cstdio>

void Paddle::move_pad_up() {
  if (_pos_y > 0) {
    _pos_y -= 1;
  }
}
void Paddle::move_pad_down() {
  if (_pos_y + _height < MATRIX_HEIGHT) {
    _pos_y += 1;
  }
}

void run_paddle() {
}

uint8_t Paddle::get_position() {
  return _pos_y;
}

bool Paddle::is_human() {
  return _human;
}

void Paddle::increase_score() {
  if (_score <= 9) _score += 1;
}

uint8_t Paddle::get_score() {
  return _score;
}

void Paddle::reset() {
  _score= 0;
}

bool Paddle::check_pad_movement() {
  // redefine me
  return false;
}

bool Paddle::check_pad_movement(Ball &ball) {
  // redefine me
  return false;
}

uint8_t Paddle::get_skills() {
  return 0;
}

void Paddle::set_skills(uint8_t skills) {
}

bool HumanPaddle::check_pad_movement() {
  bool need_refresh= false;
  if (digitalRead(_pin_btn_top) == LOW) {
    this -> move_pad_up();
    need_refresh= true;
  }
  else if (digitalRead(_pin_btn_bottom) == LOW) {
    this -> move_pad_down();
    need_refresh= true;
  }
  return need_refresh;
}

bool BotPaddle::check_pad_movement(Ball &ball) {
  uint8_t ball_y= ball.get_y();
  int8_t ball_dir= ball.get_direction_x();
  int8_t ball_dir_ver= ball.get_direction_y();

  uint8_t skills= this -> get_skills();

  // ball is moving left and pad is on right, do not move
  if (ball_dir < 0 && _pos_x > MATRIX_WIDTH / 2) return false;
  // ball is moving right and pad is on left, do not move
  else if (ball_dir > 0 && _pos_x < MATRIX_WIDTH / 2) return false;
  
  uint8_t ball_x= ball.get_x();
  int8_t ball_distance= ball_x - _pos_x;
  if (ball_distance < 0) ball_distance *= -1;
  switch (skills) {
    case 1:
      if (ball_distance > 3) return false;
      break;
    case 2:
      if (ball_distance > 4) return false;
      break;
  }

  uint8_t move_chances= random(skills * 10) % 2;
  if (!move_chances) return false;

  enum Movement {NONE, UP, DOWN};
  Movement _movment= NONE;
  
  for (uint8_t py= _pos_y; py < _pos_y+PADDLE_LENGTH-1; py++) {
    if (_pos_y - ball_y >= 0 && ball_dir_ver < 0) {
      _movment= UP;
      break;
    }
    if (_pos_y - ball_y <= 0 && ball_dir_ver > 0) {
      _movment= DOWN;
      break;
    }
  }
  if (_movment == UP) {
    this -> move_pad_up();
    return true;
  }
  if (_movment == DOWN) {
    this -> move_pad_down();
    return true;
  }
  return false;
}

uint8_t BotPaddle::get_skills() {
  return _skills;
}

void BotPaddle::set_skills(uint8_t skills) {
  if (skills < 0) _skills= 0;
  else if (skills > 1) _skills= 1;
  else _skills= skills;
}
