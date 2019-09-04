#include "BJPlayer.h"

Player::Player() { Player(DEFAULT_MONEY); };
// initialize remaining money
Player::Player(int start_money): money(start_money) {}
