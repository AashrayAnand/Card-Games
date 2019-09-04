#ifndef _LISTEN_C_
#define _LISTEN_C_
#include "ClientListenHandle.h"
#endif

#ifndef _SERIAL_
#define _SERIAL_
#include "Serialize.h"
#endif

#ifndef _GAME_
#define _GAME_
#include "Game.h"
#endif

#define PORT "8080"

int main(){
  // client socket file descriptor
  int sockfd;
  char buffer[BUFFER_SIZE];

  // class which encapsulates client functionality
  Client client;

  // create Client and connect to server at appropriate address/port
  if((sockfd = client.createClient()) == -1){
    std::cerr << "ERROR: failed to create/bind server socket" << std::endl;
  }

  playGame(&client);
}
