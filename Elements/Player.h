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

  void set_money(int balance) { this->money = balance; }
  int get_money() const { return this->money; }
  std::vector<Card> get_hand() const { return this->hand; }
  void add_card(Card card) { this->hand.push_back(card); }
private:
  // hand (cards the player currently has)
  std::vector<Card> hand;
  // amount of money player has left
  int money;
};
