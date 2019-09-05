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
  struct hostent *host_info;
  char host[255];
  if(gethostname(host, sizeof(host)) < 0){
    std::cerr << "ERROR: failed to get host name" << std::endl;
  } else {
    if((host_info = gethostbyname(host)) == NULL){
      std::cerr << "ERROR: failed to get host info by name" << std::endl;
    } else {
      std::cout << "host name: " << host_info->h_name << std::endl;
      std::cout << "host address type: " << host_info->h_addrtype << std::endl;
      int i = 0;
    }
  }


  Server server(PORT);

  // attempt to create and bind a socket, and listen for incoming connection
  if((sockfd = server.createServer()) == -1){
    std::cerr << "ERROR: failed to create/bind server socket" << std::endl;
    exit(1);
  }

  struct sockaddr_storage client_addr;
  socklen_t addr_size = sizeof(client_addr);

  // continuously attempt to accept an incoming connection and play a game
  while(1){
    if((client_fd = server.acceptClient(reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size)) == -1){
      std::cerr << "ERROR: failed to accept connection request" << '\n';
      continue;
    }

    std::cout << "connection initiation request accepted" << std::endl;
    // initiate child process to play game with client, will eventually manage games with
    // multithreading instead
    if(!fork()){
      playGame(&server, &client_fd);
      exit(0);
    }
  }
}
