#ifndef _DECK_
#define _DECK_

#include "Deck.h"

#endif

#ifndef _IO_
#define _IO_
#include <iostream>
#endif

#ifndef _STRING_
#define _STRING_
#include <string>
#endif

int main(int argc, char **argv){
  
  Deck deck;
  Card back_of_deck = deck.draw();
  std::cout << back_of_deck << std::endl;
  return 0;
}
