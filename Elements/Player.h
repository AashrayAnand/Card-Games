#ifndef _CARD_
#define _CARD_
#include "Card.h"
#endif
#include <vector>
#define DEFAULT_MONEY 100

class Player {
public:
  // default is to have player start with $100
  Player();
  // initialize remaining money
  Player(int);

  int get_money() const { return this->money; }
private:
  // hand (cards the player currently has)
  std::vector<Card> deck;
  // amount of money player has left
  int money;
};
