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
      } /*else if(!res){
        std::cout << "you lost this round" << std::endl;
        // subtract bet from user's balance
        if(updateBalance(clientfd, -1 * currBet) == -1){
          std::cout << "server ERROR updating balance" << std::endl;
          break;
        }
      } else {
        std::cout << "you win this round" << std::endl;
        // add bet to user's balance
        if(updateBalance(clientfd, currBet) == -1){
          std::cout << "server ERROR updating balance" << std::endl;
          break;
        }
      }*/
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
      std::cout << buffer << std::endl;
      // send the user's bet to the server
      if(sendBet(client, player.get_money()) == -1){
        std::cout << "client ERROR on send()" << std::endl;
        return;
      }

      if((res = round(client, &player)) == -1){
        std::cout << "client ERROR playing game" << std::endl;
        break;
      } /*else {
        if((res = receiveBalance(client->get_fd())) == -1){
          std::cout << "client ERROR on updating balance" << std::endl;
          break;
        }
      }*/

    } else if(res == -1){
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
  std::vector<int> sums;
  int res;

  // get first two cards
  if((res = takeCard(client->get_fd(), player, &sums)) == -1){
    std::cout << "client ERROR taking card" << std::endl;
  }

  if((res = takeCard(client->get_fd(), player, &sums)) == -1){
    std::cout << "client ERROR taking card" << std::endl;
  }

  printHand(player->get_hand());

  /*std::cout << "draw another card?" << std::endl;
  while((res = checkDraw(client->get_fd())) == 1 && *std::min(sums.begin(), sums.end()) < 21){

  }

  if(res == -1){
    std::cout << "server ERROR on draw" << std::endl;
    return -1;
  }

  // get best sum (greatest sum <= 21)
  int best = getBest(sums);

  // check if user sum > 21, if so, return 0,
  // else if sum == 21, return 1, else if sum < 21,
  // let dealer draw cards

  // while none of the sums of the dealer's cards are > user's cards
  // and <= 21, and while the dealer still has at least one sum < 21,
  // keep drawing cards
  */
  return 0;
}

int round(int *clientfd, Deck *deck){
  // first two card are drawn no matter what
  int res;
  if((res = drawCard(clientfd, deck)) == -1){
    std::cout << "server ERROR on drawing card" << std::endl;
    return -1;
  }

  if((res = drawCard(clientfd, deck)) == -1){
    std::cout << "server ERROR on drawing card" << std::endl;
    return -1;
  }
  // cotinually ask user if they want to draw another card
  /*while((res = draw(clientfd)) == 1){
    if((res = drawCard(clientfd, deck)) == -1){
      std::cout << "server ERROR on drawing card" << std::endl;
      return -1;
    }
  }

  if(res == -1){
    std::cout << "server ERROR on draw" << std::endl;
    return -1;
  }
  // check if user sum > 21, if so, return 0,
  // else if sum == 21, return 1, else if sum < 21,
  // let dealer draw cards

  // while none of the sums of the dealer's cards are > user's cards
  // and <= 21, and while the dealer still has at least one sum < 21,
  // keep drawing cards
  */
  return 0;
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

int takeCard(int clientfd, Player *player, std::vector<int> *sums){
  char buffer[SMALL_BUFFER_SIZE];
  if(recv(clientfd, buffer, SMALL_BUFFER_SIZE, 0) == -1){
    return -1;
  }
  Card card(buffer[0], buffer[1]);
  //std::cout << "card received\n" << card << std::endl;
  player->add_card(card);
  return 0;
}

int checkDraw(int clientfd){
  return 0;
}

int draw(int *clientfd){
  char buffer[BUFFER_SIZE];
  if(read(*clientfd, buffer, BUFFER_SIZE) == -1){
    std::cerr << "server ERROR on recv()" << std::endl;
    return -1;
  }

  // client serializes integer balance data, convert this string
  // representation of the balance to an int and return
  return atoi(buffer);
}

int getBalance(int *clientfd){
  char buffer[BUFFER_SIZE];
  if(read(*clientfd, buffer, BUFFER_SIZE) == -1){
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

int receiveBalance(int clientfd){
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

  if(read(*clientfd, buffer, BUFFER_SIZE) == -1){
    std::cerr << "server ERROR on recv()" << std::endl;
    return -1;
  }

  // client serializes integer balance data, convert this string
  // representation of the balance to an int and return
  return atoi(buffer);
}
