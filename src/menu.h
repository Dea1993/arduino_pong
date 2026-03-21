#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "font.h"

class Menu {

  private:
    enum _game_modes : uint8_t {PVP, PVC, CVC, COUNT_MODES};
    _game_modes _game_mode = PVP;
    enum _bot_skill_levels : uint8_t {EASY, HARD, COUNT_SKILLS};
    _bot_skill_levels _bot_skill = EASY;

  public:
    void next_mode();
    void prev_mode();
    uint8_t get_mode();
    uint8_t number_of_bots();
    void increase_skills();
    void decrease_skills();
    uint8_t get_skill();
    void reset_menu();
};

#endif
