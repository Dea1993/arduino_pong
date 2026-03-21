#include "renderer.h"


void Renderer::_clear_matrix() {
  for (int x=0; x < MATRIX_WIDTH; x++) {
    for (int y=0; y < MATRIX_HEIGHT; y++) {
      _frame[y][x]= 0;
    }
  }
}

void Renderer::set_players(Paddle *p1, Paddle *p2) {
  _p1= p1;
  _p2= p2;
}

void Renderer::render_timer(uint8_t seconds) {
  this -> _clear_matrix();

  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      _frame[h][w+5]= font_pong[seconds][h][w];
    }
  }
  _matrix.renderBitmap(_frame, MATRIX_HEIGHT, MATRIX_WIDTH);
}

void Renderer::render_matrix() {
  this -> _clear_matrix();
  uint8_t p1pos= _p1 -> get_position();
  uint8_t p2pos= _p2 -> get_position();
  // players coords
  for (int i= p1pos; i < p1pos+PADDLE_LENGTH; i++) {
    _frame[i][0]= 1;
  }
  for (int i= p2pos; i < p2pos+PADDLE_LENGTH; i++) {
    _frame[i][MATRIX_WIDTH-1]= 1;
  }
  
  // ball coords
  uint8_t bx= _ball.get_x();
  uint8_t by= _ball.get_y();
  _frame[by][bx]= 1;
  _matrix.renderBitmap(_frame, MATRIX_HEIGHT, MATRIX_WIDTH);
}

void Renderer::render_score() {
  this -> _clear_matrix();
  _frame[4][5]= 1;
  _frame[4][6]= 1;

  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      _frame[h][w+1]= font_pong[_p1 -> get_score()][h][w];
    }
  }
  for (int h=0; h < 8; h++) {
    for (int w=0; w < 3; w++) {
      _frame[h][w+8]= font_pong[_p2 -> get_score()][h][w];
    }
  }
  _matrix.renderBitmap(_frame, MATRIX_HEIGHT, MATRIX_WIDTH);
}

void Renderer::render_winner() {
  this -> _clear_matrix();
  // check winner 
  if (_p1 -> get_score() > _p2 -> get_score())
    _matrix.loadSequence(pone_wins);
  else 
    _matrix.loadSequence(ptwo_wins);
  _matrix.play(true);
}
