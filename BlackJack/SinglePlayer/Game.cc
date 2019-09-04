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
    if((balance = getBalance(server, clientfd)) > 0){
      currBet = getBet(server, clientfd, balance);
    } else if(balance == -1) {
      std::cout << "ERROR getting balance" << std::endl;
      return;
    } else {
      // balance is 0, game is over
      break;
    }
  }

  close(*clientfd);
  return;
}

void playGame(Client *client) {
  BJPlayer player(100);
  char buffer[BUFFER_SIZE];
  // send starting message to client
  if(recv(client->get_fd(), buffer, BUFFER_SIZE, 0) == -1){
    return;
  }

  std::cout << buffer << std::endl;

  while(1){
    int res;

    // send balance, returns 1 when positive
    if((res = sendBalance(client, &player)) == 1){
      // PLAY

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

int getBalance(Server *server, int *clientfd){
  char buffer[BUFFER_SIZE];
  if(read(*clientfd, buffer, BUFFER_SIZE) == -1){
    std::cerr << "server ERROR on recv()" << std::endl;
    return -1;
  }

  // client serializes integer balance data, convert this string
  // representation of the balance to an int and return
  return atoi(buffer);
}

int sendBalance(Client *client, BJPlayer *player){
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

// server prompts user for a bet, which must be > 0 & <= user's remaining money
int placeBet(Server *server, int * clientfd, int balance){
  const char *bet_msg = "Place a bet, must be a positive amount which is no more than your remaining balance\n";
  sendMessage(bet_msg, clientfd);
}
