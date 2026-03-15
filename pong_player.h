#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H

int ball_player_collision(int player, int ball_y);
int pong_move_p1(int &p1_start, int &need_refresh);
int pong_move_p2(int &p2_start, int &need_refresh);

#endif
