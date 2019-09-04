#ifndef _CARD_
#define _CARD_
#include "Card.h"
#endif

#ifndef _CONST_
#define _CONST_

// suit macros
#define HEARTS (1 << 1) + 1

#define SPADES 1 << 1

#define DIAMONDS 1

#define CLUBS 0

#define NUM_CARDS 52
#define NUM_VALUES 13
#endif

#ifndef _LIBS_
#define _LIBS_
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#endif

class Deck {
public:
  Deck(){
    // iterate through all the numerical card values
    this->generate_deck();
    // shuffle the deck
    this->shuffle();
  }

  // populate deck
  void generate_deck();
  // shuffle the deck
  void shuffle();
  // draw card from the deck
  Card draw();

private:
  // card deck (drawn in reverse order)
  std::vector<Card> deck;

  // clear the deck vector, used when generating
  // new deck
  void clearDeck();
};
