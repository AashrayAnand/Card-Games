#include "ServerListenHandle.h"

Server::Server(uint16_t port): port(port) {}

int Server::get_fd() const { return this->sockfd; }
int Server::get_family() const { return this->sock_family; }


// returns socket file descriptor on success, failure otherwise
int Server::createServer(){
  // create addrinfo structs and set appropriately to get
  // necessary info to make host server
  struct addrinfo hints, *res, *rp;
  int sockfd = 0;
  memset(&hints, 0, sizeof(hints)); // clear the hints structure
  hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
  hints.ai_flags = AI_PASSIVE; // use host address
  hints.ai_socktype = SOCK_STREAM; // use stream socket
  hints.ai_protocol = IPPROTO_TCP; // use TCP protocol
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_canonname = NULL;

  // create Server object with specified PORT
  std::string port = std::to_string(this->port);

  // store linked list of addrinfo for the host internet address
  // at the specified port, with the qualifications specified by hints
  if(getaddrinfo(NULL, port.c_str() , &hints, &res) < 0){
    std::cerr << "ERROR on getaddrinfo()" << std::endl;
    return -1;
  }

  // iterate through results and attempt to create socket file descriptor
  for(rp = res; rp != NULL; rp = rp->ai_next){
    if((sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0){
      std::cerr << "ERROR on socket()" << std::endl;
      continue;
    }

    // bind the socket file descriptor to the host address, break loop on success
    if(bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0){
      // store AI family (IPv4 or IPv6)
      std::cout << "AI_FAMILY: " << rp->ai_family << std::endl;
      this->sock_family = rp->ai_family;
      break;
    }

    std::cerr << "ERROR on bind()" << std::endl;
    close(sockfd);
    sockfd = -1;
  }

  // free the addrinfo structure as we no longer need it
  freeaddrinfo(res);

  if(sockfd == -1){
    std::cerr << "FAILED TO BIND" << std::endl;
    return sockfd;
  }

  // listen for incoming connections to the server socket fd
  if(listen(sockfd, BACKLOG) != 0){
    std::cerr << "ERROR on listen()" << std::endl;
    close(sockfd);
    return -1;
  }

  // at this point, we have successfully created and bound a server socket
  // file descriptor, which we can now return
  this->sockfd = sockfd;
  return sockfd;
}

int Server::acceptClient(struct sockaddr *client_addr, socklen_t *size){
  return accept(this->sockfd, client_addr, size);
}

int sendCard(int client_fd, Card card){
  return 0;
}
