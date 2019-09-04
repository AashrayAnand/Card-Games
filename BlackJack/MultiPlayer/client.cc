#include "ClientListenHandle.h"

#define PORT "8080"

int main(){
  // client socket file descriptor
  int sockfd;
  char buffer[BUFFER_SIZE];

  if((sockfd = createClient()) == -1){
    std::cerr << "ERROR: failed to create/bind server socket" << std::endl;
  }

  if(recv(sockfd, buffer, BUFFER_SIZE, 0) == -1){
    std::cerr << "ERROR on recv()" << std::endl;
  }

  std::cout << buffer << std::endl;
}
