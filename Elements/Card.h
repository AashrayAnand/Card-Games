#ifndef _IO_STR_
#define _IO_STR_
#include <iostream>
#include <string>
#endif

class Card {
  typedef std::string Suit;

public:

  // generate default 0-arg constructor and copy constructor
  Card() = default;
  Card(const Card &) = default;

  // simple constructor, since game will be managing
  // all instantation of cards, we can provide only the
  // fully-defined constructor that we use
  Card(char, char);

  // get string representation of card suit
  Suit getSuit() const;

  // get card value
  int getValue() const;

  // card comparison operators, will be needed to sort a deck of
  // cards, and a player's hand
  bool operator>(const Card &) const;
  bool operator<(const Card &) const;
  bool operator==(const Card &) const;

  // assignment operator override (pass-by-value to allow for swap)
  Card &operator=(Card);

  void swap(Card &);
private:
  // e.g. 10 of hearts: suit = hearts, value = 10;
  char suit;
  char value;
};

// << operator override for card class, outputs card as
// <Value> of <Suit> e.g. 3 of clubs
inline std::ostream &operator<<(std::ostream &os, const Card &card){
  int value = card.getValue();
  switch(value){
    case 14:
      os << "Ace";
      break;
    case 13:
      os << "King";
      break;
    case 12:
      os << "Queen";
      break;
    case 11:
      os << "Jack";
      break;
    default:
      os << value;
    }
  os << " of " << card.getSuit();
  return os;
}
