#ifndef _IO_STR_
#define _IO_STR_
#include <iostream>
#include <string>
#endif

#define ACE "Ace"

#define KING "King"

#define QUEEN "Queen"

#define JACK "Jack"

#define TEN "10"

#define WHITESPACE 13

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

  // get card value
  char getValue() const;
  // get suit char value
  char getSuitChar() const;
  // get string representation of card suit
  Suit getSuit() const;

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
  os << "===============\n";
  int i;
  std::string suit = card.getSuit();
  int suit_length = suit.length();
  os << "| " << card.getSuit();
  for(i = suit_length; i < WHITESPACE; i++){
    os << " ";
  }
  os << "|\n";
  os << "|              |\n";
  os << "|              |\n";
  os << "|              |\n";
  os << "|              |\n";
  int value = card.getValue();
  os << "| ";

  switch(value){
    case 14:
      os << ACE;
      for(i = strlen(ACE); i < WHITESPACE; i++){
        os << " ";
      }
      break;
    case 13:
      os << KING;
      for(i = strlen(KING); i < WHITESPACE; i++){
        os << " ";
      }
      break;
    case 12:
      os << QUEEN;
      for(i = strlen(QUEEN); i < WHITESPACE; i++){
        os << " ";
      }
      break;
    case 11:
      os << JACK;
      for(i = strlen(JACK); i < WHITESPACE; i++){
        os << " ";
      }
      break;
    case 10:
      os << TEN;
      for(i = strlen(TEN); i < WHITESPACE; i++){
        os << " ";
      }
      break;
    default:
      os << value;
      for(i = 1; i < WHITESPACE; i++){
        os << " ";
      }
    }
  os << "|\n";
  os << "===============\n";
  return os;
}
