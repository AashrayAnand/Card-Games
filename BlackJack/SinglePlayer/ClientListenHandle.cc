#ifndef _LISTEN_C_
#define _LISTEN_C_
#include "ClientListenHandle.h"
#endif

int Client::get_fd() const { return this->sockfd; }

int Client::createClient(){
  int sockfd;
  struct addrinfo hint, *res, *rp;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_UNSPEC; // client will IPv-agnostic
  hint.ai_socktype = SOCK_STREAM; // client socket fd will be a stream socket

  // specify that we are trying to get info for local host at specified port
  if(getaddrinfo("127.0.0.1", PORT, &hint, &res) < 0){
    std::cerr << "ERROR on getaddrinfo()" << std::endl;
    return -1;
  }

  for(rp = res; rp != NULL; rp = rp->ai_next){
    if((sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0){
      std::cerr << "ERROR on socket()" << std::endl;
      continue;
    }
  }

  // if sockfd = -1, we iterated through all results of getaddrinfo and failed to
  // create a socket for any of them
  if(sockfd == -1){
    std::cerr << "FAILED to create socket" << std::endl;
    return sockfd;
  }

  // from getaddrinfo result, we have the sockaddr structure for the server,
  // and the length of this sockaddr structure (depending on IPv for server), we
  // can now use this information to connect our client socket fd to the server
  if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0){
    std::cerr << "ERROR on connect()" << std::endl;
    close(sockfd);
    return -1;
  }

  freeaddrinfo(res);

  // we can return our client socket file descriptor, which is now connected to the server
  this->sockfd = sockfd;
  return sockfd;
}
