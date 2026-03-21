#include "menu.h"

void Menu::next_mode() {
  if (_game_mode < _game_modes(COUNT_MODES) - 1)
    _game_mode= _game_modes(_game_mode+1);
}

void Menu::prev_mode() {
  if (_game_mode > 0) {
    _game_mode= _game_modes(_game_mode-1);
  }
}

uint8_t Menu::get_mode() {
  return _game_mode;
}

uint8_t Menu::number_of_bots() {
  if (_game_mode == PVC) return 1;
  else if (_game_mode == CVC) return 2;
  return 0;
}

void Menu::increase_skills() {
  if (_bot_skill < _bot_skill_levels(COUNT_SKILLS) - 1)
    _bot_skill= _bot_skill_levels(_bot_skill+1);
}

void Menu::decrease_skills() {
  if (_bot_skill > 0) {
    _bot_skill= _bot_skill_levels(_bot_skill-1);
  }
}

uint8_t Menu::get_skill() {
  return _bot_skill;
}

void Menu::reset_menu() {
  _game_mode= PVP;
  _bot_skill= EASY;
}
