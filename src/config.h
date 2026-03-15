#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #define SECRET_SSID "Fallback_SSID"
  #define SECRET_PASS "Fallback_PASS"
  #warning "⚠️ secrets.h not found"
#endif

#define P1_BTN_UP 12
#define P1_BTN_BOTTOM 11
#define P2_BTN_UP 10
#define P2_BTN_BOTTOM 9

#define MATRIX_WIDTH 12
#define MATRIX_HEIGHT 8
#define BALL_RESET_X (MATRIX_WIDTH / 2)
#define BALL_RESET_Y (MATRIX_HEIGHT / 2)
#define BAR_LENGTH 3
#define INITIAL_BALL_DELAY 200
