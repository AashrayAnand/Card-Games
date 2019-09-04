#ifndef _DECK_
#define _DECK_

#include "Deck.h"

#endif

Card Deck::draw() {
  Card card = this->deck.back();
  this->deck.pop_back();
  return card;
}

void Deck::clearDeck(){
  this->deck.clear();
}

void Deck::generate_deck(){
  this->clearDeck();
  int i, j;
  for(i = 0; i < NUM_VALUES; i++){
    // cards values go from 2-14, add 2 to i to correct range
    int value = i + 2;
    // add 1 card for the current number for each suit
    deck.push_back(Card(HEARTS, value));
    deck.push_back(Card(SPADES, value));
    deck.push_back(Card(CLUBS, value));
    deck.push_back(Card(DIAMONDS, value));
  }
}

void Deck::shuffle() {
  std::mt19937 g(static_cast<uint32_t>(time(0)));
  std::shuffle(deck.begin(), deck.end(), g);
}
