#include "Game.h"

void playGame(int sockfd, int client_fd){
  Deck deck;
  BJPlayer player;
  char buffer[BUFFER_SIZE];

  // display starting message for user
  const char *start_msg = "Hello and welcome to Aashray's blackjack table, you have $100 dollars\n";
  strncpy(buffer, start_msg, strlen(start_msg));
  buffer[strlen(start_msg)] = '\0';

  // send starting message
  if(send(client_fd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "ERROR on send()" << std::endl;
    return;
  }
}
