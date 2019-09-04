#ifndef _PLAYER_
#define _PLAYER_
#include "Player.h"
#endif
// BJPlayer is a dervied class of Player, which extends
// the basic Player class to specifically handle games of
// blackjack

// includes a vector which represents all of the possible hand sums
// (as they can vary due to aces)
class BJPlayer : public Player {
private:
  std::vector<int> hand_sums;
};
