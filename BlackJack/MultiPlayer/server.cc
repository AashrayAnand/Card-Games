#ifndef _LISTEN_
#define _LISTEN_
#include "ServerListenHandle.h"
#endif

#ifndef _SERIAL_
#define _SERIAL_
#include "Serialize.h"
#endif

#ifndef _GAME_
#define _GAME_
#include "Game.h"
#endif

int main(){

  int sockfd, client_fd, sock_family;

  // attempt to create and bind a socket, and listen for incoming connection
  if((sockfd = createServer(&sock_family)) == -1){
    std::cerr << "ERROR: failed to create/bind server socket" << std::endl;
    exit(1);
  }

  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);

  // continuously attempt to accept an incoming connection and play a game
  while(1){
    if((client_fd = acceptClient(sockfd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size)) == -1){
      std::cerr << "ERROR: failed to accept connection request" << '\n';
      continue;
    }

    std::cout << "connection initiation request accepted" << std::endl;
    playGame(sockfd, client_fd);
  }
}
