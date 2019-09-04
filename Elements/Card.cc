#ifndef _CARD_
#define _CARD_
#include "Card.h"
#endif

Card::Card(char suit, char value) {
  this->suit = suit;
  this->value = value;
}

std::string Card::getSuit() const {
  switch(this->suit){
    case 0:
      return "Clubs";
    case 1:
      return "Diamonds";
    case 2:
      return "Spades";
  }
  return "Hearts";
}

char Card::getValue() const { return this->value; }
char Card::getSuitChar() const { return this->suit; }

bool Card::operator>(const Card &rCard) const { return this->value > rCard.getValue(); }
bool Card::operator<(const Card &rCard) const { return this->value < rCard.getValue(); }
bool Card::operator==(const Card &rCard) const { return this->value == rCard.getValue(); }

Card & Card::operator=(Card card) {
  // swaps members with a copy of the Card we are assigning to,
  // will copy old values to location of the copy, which will be
  // destructed after assignment is complete
  card.swap(*this);
  return *this;
}

void Card::swap(Card &other) {
  using std::swap;
  swap(suit, other.suit);
  swap(value, other.value);
}
