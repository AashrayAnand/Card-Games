#include "Game.h"

void sendMessage(const char *msg, int *sockfd){
  char buffer[BUFFER_SIZE];
  strncpy(buffer, msg, strlen(msg));
  buffer[strlen(msg)] = '\0';
  // send starting message to client
  if(send(*sockfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "ERROR on send()" << std::endl;
    return;
  }
}

// game manager from the perspective of a forked server proc,
// talking to a client socket file descriptor
void playGame(Server *server, int *clientfd){
  // initialize deck, and player representation for client
  Deck deck;
  char buffer[BUFFER_SIZE];

  // display starting message for user
  const char *start_msg = "Hello and welcome to Aashray's blackjack table\n";
  sendMessage(start_msg, clientfd);

  int currBet;
  // loop invariant:
  while(1){
    int balance;
    // check the user still has a positive balance before playing
    if((balance = getBalance(clientfd)) > 0){
      // get the round bet from the user, check if bet was received successfully
      if((currBet = getBet(clientfd)) == -1){
        std::cout << "server ERROR getting bet" << std::endl;
        break;
      }
      std::cout << "Current user bet is: " << currBet << std::endl;
      int res;
      if((res = round(clientfd, &deck)) == -1){
        std::cout << "server ERROR playing game" << std::endl;
        break;
      } else if(!res){
        std::cout << "you lost this round, lose $" << currBet << std::endl;
        // subtract bet from user's balance
        if(updateBalance(clientfd, -1 * currBet) == -1){
          std::cout << "server ERROR updating balance" << std::endl;
          break;
        }
      } else if(res == 1){
        std::cout << "Current user bet is: " << currBet << std::endl;
        std::cout << "you win this round, gain $" << currBet << std::endl;
        // add bet to user's balance
        if(updateBalance(clientfd, -1 * currBet) == -1){
          std::cout << "server ERROR updating balance" << std::endl;
          break;
        }
      } else {
          std::cout << "you got a blackjack" << std::endl;
          if(updateBalance(clientfd, -2 * currBet) == -1){
            std::cout << "server ERROR updating balance" << std::endl;
            break;
        }
      }
    } else if(balance == -1) {
      std::cout << "ERROR getting balance" << std::endl;
      break;
    } else {
      // balance is 0, game is over
      break;
    }
  }

  close(*clientfd);
  return;
}

void playGame(Client *client) {
  Player player(100);
  char buffer[BUFFER_SIZE];
  // send starting message to client
  if(recv(client->get_fd(), buffer, BUFFER_SIZE, 0) == -1){
    std::cout << "client ERROR on recv()";
    return;
  }

  std::cout << buffer << std::endl;

  while(1){
    int res;

    // send balance, returns 1 when positive
    if((res = sendBalance(client, &player)) == 1){
      // receive a message from the server, and output it to the client
      if(recv(client->get_fd(), buffer, BUFFER_SIZE, 0) == -1){
        std::cout << "client ERROR on recv()" << std::endl;
        return;
      }
      //std::cout << buffer << std::endl;
      // send the user's bet to the server
      if(sendBet(client, player.get_money()) == -1){
        std::cout << "client ERROR on send()" << std::endl;
        return;
      }

      if((res = round(client, &player)) == -1){
        std::cout << "client ERROR playing game" << std::endl;
        break;
      } else if(!res) {
        if((res = receiveBalance(client->get_fd(), &player, 1)) == -1){
          std::cout << "client ERROR on updating balance" << std::endl;
          break;
        }
      } else {
        if((res = receiveBalance(client->get_fd(), &player, 0)) == -1){
          std::cout << "client ERROR on updating balance" << std::endl;
          break;
        }
      }

    } else if(!res){
      std::cout << "Game over! you lose." << std::endl;
      break;
    } else {
      std::cout << "ERROR sending balance" << std::endl;
      break;
    }
  }
  // close client connection to server
  close(client->get_fd());
}

int round(Client *client, Player *player){
  player->clear_hand();
  std::vector<int> sums;
  sums.push_back(0);
  int res;

  // get first two cards
  if((res = takeCard(client->get_fd(), player, sums)) == -1){
    std::cout << "client ERROR taking card" << std::endl;
  }

  printVec(sums);

  if((res = takeCard(client->get_fd(), player, sums)) == -1){
    std::cout << "client ERROR taking card" << std::endl;
  }

  printVec(sums);

  printHand(player->get_hand());

  std::cout << "draw another card?" << std::endl;
  while((res = willDraw(client->get_fd(), getBest(sums))) == 1){
    if(takeCard(client->get_fd(), player, sums) == -1){
      std::cout << "client ERROR taking card" << std::endl;
    }
    printVec(sums);
    printHand(player->get_hand());
    // break loop, player either got blackjack or busted
    if(getBest(sums) == 21 || getMinSum(sums) > 21){
      if(sendStatus(client->get_fd(), 0) == -1){
        std::cout << "client ERROR sending status" << std::endl;
        return -1;
      }
      break;
    } else {
      if(sendStatus(client->get_fd(), 1) == -1){
        std::cout << "client ERROR sending status" << std::endl;
        return -1;
      }
    }
    std::cout << "draw another card?" << std::endl;
  }

  if(res == -1){
    std::cout << "client ERROR on draw" << std::endl;
    return -1;
  }

  int best = getBest(sums);
  // get best sum (greatest sum <= 21)
  if(res){
    if(best == 21){
      std::cout << "BLACKJACK!" << std::endl;
      if(sendStatus(client->get_fd(), 1) == -1){
        std::cout << "client ERROR sending status" << std::endl;
        return -1;
      }
      return 2;
    }

    if(getMinSum(sums) > 21){
      std::cout << "BUSTED" << std::endl;
      if(sendStatus(client->get_fd(), 0) == -1){
        std::cout << "client ERROR sending status" << std::endl;
        return -1;
      }
      return 0;
    }
  }

  if(sendStatus(client->get_fd(), best) == -1){
    std::cout << "client ERROR sending status" << std::endl;
    return -1;
  }

  res = getStatus(client->get_fd());
  if(res == -1){
    std::cout << "client ERROR receiving final result" << std::endl;
    return -1;
  } else if(!res){
    std::cout << "dealer win" << std::endl;
    return 0;
  }
  // check if user sum > 21, if so, return 0,
  // else if sum == 21, return 1, else if sum < 21,
  // let dealer draw cards

  // while none of the sums of the dealer's cards are > user's cards
  // and <= 21, and while the dealer still has at least one sum < 21,
  // keep drawing cards
  std::cout << "player win" << std::endl;
  return 1;
}

int round(int *clientfd, Deck *deck){
  // first two card are drawn no matter what
  deck->generate_deck();
  deck->shuffle();
  Player player(100);
  int res;
  if((res = drawCard(clientfd, deck)) == -1){
    std::cout << "server ERROR on drawing card" << std::endl;
    return -1;
  }

  if((res = drawCard(clientfd, deck)) == -1){
    std::cout << "server ERROR on drawing card" << std::endl;
    return -1;
  }
  // continually ask user if they want to draw another card

  while((res = checkDraw(clientfd)) == 1){
    if(drawCard(clientfd, deck) == -1){
      std::cout << "server ERROR on drawing card" << std::endl;
      return -1;
    }

    int status;
    if((status = getStatus(clientfd)) == -1){
      std::cout << "server ERROR getting status" << std::endl;
      return -1;
    // break loop on bust or blackjack
    } else if(!status){
        std::cout << "broke on loop condition (blackjack or bust)" << std::endl;
        break;
    }
  }

  if(res == -1){
    std::cout << "server ERROR on draw" << std::endl;
    return -1;
  }

  int sum;

  if(res == 1){
    if((res = getStatus(clientfd)) == -1){
      std::cout << "server ERROR getting blackjack/bust result" << std::endl;
    // user bust, exit program with return code 0
    } else if(!res){
      std::cout << "user busted" << std::endl;
      return 0;
    // user blackjack, exit program with return code 2
    } else if(res == 1){
      std::cout << "user blackjack" << std::endl;
      return 2;
    }
  }

  // user did not blackjack or bust, simply stopped
  // at a certain sum, store this sum
  if((sum = getStatus(clientfd)) == -1){
    std::cout << "server ERROR getting blackjack/bust result" << std::endl;
  }
  std::cout << "best received" << std::endl;

  // at this point, if we are still in method then the dealer will now
  // draw cards, create a sum vector
  std::vector<int> sums;
  sums.push_back(0);

  // dealer stops drawing when his sum is at least 17
  while(getMinSum(sums) < 17){
    std::cout << "DEALER DRAWING CARD" << std::endl;
    std::cout << "===================" << std::endl;

    Card card = deck->draw();
    player.add_card(card);
    updateSums(&card, sums);
    printVec(sums);
    printHand(player.get_hand());
  }

  // dealer did not bust, and has a better sum than player, wins the round
  if(getBest(sums) > res){
    std::cout << "dealer wins, scored " << getBest(sums) << " versus " << res << std::endl;
    if(sendStatus(*clientfd, 0) == -1){
      std::cout << "server ERROR sending status" << std::endl;
      return -1;
    }
    return 0;
  }

  if(sendStatus(*clientfd, 1) == -1){
    std::cout << "server ERROR sending status" << std::endl;
    return -1;
  }

  // check if user sum > 21, if so, return 0,
  // else if sum == 21, return 1, else if sum < 21,
  // let dealer draw cards

  // while none of the sums of the dealer's cards are > user's cards
  // and <= 21, and while the dealer still has at least one sum < 21,
  // keep drawing cards
  return 1;
}

int sendStatus(int clientfd, int status){
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%d", status);
  if(send(clientfd, &buffer, sizeof(buffer), 0) == -1){
    return -1;
  }

  return 0;
}

// get status values (used by server socket file descriptor pointer)
int getStatus(int *clientfd){
  char buffer[BUFFER_SIZE];
  if(recv(*clientfd, buffer, sizeof(buffer), 0) == -1){
    return -1;
  }

  int res = atoi(buffer);

  return res;
}

// get status values (used by client socket file descriptor value)
int getStatus(int clientfd){
  char buffer[BUFFER_SIZE];
  if(recv(clientfd, buffer, sizeof(buffer), 0) == -1){
    return -1;
  }

  int res = atoi(buffer);

  return res;
}

void printVec(std::vector<int> sums){
  std::cout << "sums: [";
  for(auto p: sums){
    std::cout << p << " ";
  }
  std::cout << "]" << std::endl;
}

int drawCard(int *clientfd, Deck *deck){
  char buffer[SMALL_BUFFER_SIZE];
  Card card = deck->draw();
  std::cout << "card drawn\n" << card << std::endl;
  // place card data in buffer, will be deserialized by user
  buffer[0] = card.getSuitChar();
  buffer[1] = card.getValue();

  // failed to send card to user, return -1
  if(send(*clientfd, buffer, SMALL_BUFFER_SIZE, 0) == -1){
    return -1;
  }

  return 0;
}

int takeCard(int clientfd, Player *player, std::vector<int> &sums){
  // get card from server and add to hand
  char buffer[SMALL_BUFFER_SIZE];
  if(recv(clientfd, buffer, SMALL_BUFFER_SIZE, 0) == -1){
    return -1;
  }
  Card card(buffer[0], buffer[1]);
  player->add_card(card);
  // update possible sums, either increment existing sum by value
  // of the card, or increment existing values and copy for the 2
  // possible ace values
  updateSums(&card, sums);
  return 0;
}

void updateSums(Card *card, std::vector<int> &sums){
  int i;
  int value = card->getValue();
  // set value for ace, will update sums for ace having
  // value of 1 or 11
  if(value == 14){
    value = 1;
  } else if(value > 10){
    value = 10;
  }
  // store number of existing sums, necessary for
  // case where we add additional values for each
  // existing sum, to account for ace having 2 values
  int len = sums.size();
  for(i = 0; i < len; i++){
    if(value == 1){
      // latest card is ace, push back updated value
      // where we increment sum by 11
      sums.push_back(sums[i] + 11);
    }
    // update sum for new card value
    sums[i] = sums[i] + value;
  }
}

int willDraw(int clientfd, int sum){
  char buffer[BUFFER_SIZE];
  std::string response;
  std::cin >> response;
  while(std::cin.fail() || (response.compare("yes") != 0 && response.compare("no") != 0)){
    std::cout << "RESPONSE: " << response << std::endl;
    std::cout << "response must be a yes or no (case-sensitive)" << std::endl;
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cin >> response;
  }

  int status;
  if(!response.compare("yes")){
    status = 1;
  } else {
    status = sum;
  }

  sprintf(buffer, "%d", status);

  if(send(clientfd, buffer, BUFFER_SIZE, 0) == -1){
    return -1;
  }

  return status;
}

int checkDraw(int *clientfd){
  char buffer[BUFFER_SIZE];
  if(recv(*clientfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "server ERROR on recv()" << std::endl;
    return -1;
  }

  // client serializes integer balance data, convert this string
  // representation of the balance to an int and return
  return atoi(buffer);
}

int getBalance(int *clientfd){
  char buffer[BUFFER_SIZE];
  if(recv(*clientfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "server ERROR on recv()" << std::endl;
    return -1;
  }

  // client serializes integer balance data, convert this string
  // representation of the balance to an int and return
  return atoi(buffer);
}

int sendBalance(Client *client, Player *player){
  char buffer[BUFFER_SIZE];
  std::cout << "player balance: " << player->get_money() << std::endl;
  sprintf(buffer, "%d", player->get_money());
  // send starting message to client
  if(send(client->get_fd(), buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "ERROR on send()" << std::endl;
    return -1;
  }

  if(player->get_money() > 0){
    return 1;
  }
  return 0;
}

int updateBalance(int *clientfd, int bet){
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%d", bet);
  if(send(*clientfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "ERROR on send()" << std::endl;
    return -1;
  }

  return 0;
}

int receiveBalance(int clientfd, Player *player, int plus){
  char buffer[BUFFER_SIZE];
  if(recv(clientfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cout << "client ERROR on recv()" << std::endl;
  }

  int diff = atoi(buffer);

  std::cout << "setting money, gaining $" << diff << std::endl;

  // add bet (or subtract bet) to/from balance
  if(plus){
    player->set_money(player->get_money() + diff);
  } else {
    player->set_money(player->get_money() - diff);
  }
  return 0;
}

int sendBet(Client *client, int balance){
  int bet;
  std::cout << "Enter your bet, current balance is $" << balance << std::endl;
  std::cin >> bet;
  while(std::cin.fail() || bet > balance){
    std::cout << "Balance must be a positive integer, and no greater than your current balance" << std::endl;
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cin >> bet;
  }

  char buffer[BUFFER_SIZE];
  sprintf(buffer, "%d", bet);
  if(send(client->get_fd(), buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "ERROR on send()" << std::endl;
    return -1;
  }

  return 0;
}

void printHand(std::vector<Card> hand){
  for(auto p: hand){
    std::cout << p << std::endl;
  }
}

int getMinSum(std::vector<int> sums){
  std::sort(sums.begin(), sums.end());
  return sums[0];
}

int getBest(std::vector<int> sums){
  int best = 0;
  std::sort(sums.begin(), sums.end());
  for(auto p: sums){
    if(p > 21){
      break;
    }
    best = p;
  }

  return best;
}

// server prompts user for a bet, which must be > 0 & <= user's remaining money
int getBet(int * clientfd){
  char buffer[BUFFER_SIZE];
  const char *bet_msg = "Place a bet, must be a positive amount which is no more than your remaining balance\n";
  sendMessage(bet_msg, clientfd);

  if(recv(*clientfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "server ERROR on recv()" << std::endl;
    return -1;
  }

  // client serializes integer balance data, convert this string
  // representation of the balance to an int and return
  return atoi(buffer);
}
