#ifndef _DEF_
#define _DEF_

#include <unistd.h>

// sockets
#include <sys/socket.h>

#include <stdlib.h>

// for inet_pton() function, converts character string IP address
// to network address structure for the specified address family
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#endif

#ifndef _IO_STR_
#define _IO_STR_
#include <iostream>
#include <string>
#endif

#ifndef _CARD_
#define _CARD_
#include "../../Elements/Card.h"
#endif

#ifndef _DECK_
#define _DECK_
#include "../../Elements/Deck.h"
#endif

#ifndef _PLAYER_
#define _PLAYER_
#include "../../Elements/Player.h"
#endif

#ifndef _BJ_PLAYER_
#define _BJ_PLAYER_
#include "../../Elements/BJPlayer.h"
#endif

#define BUFFER_SIZE 1024

#ifndef PORT_STR
#define PORT_STR "8080"
#endif


// post: returns a client socket file descriptor, which connects to a localhost
// server at a specified port
class Client {
public:
  int get_fd() const;
  int createClient();
private:
  int sockfd;
};
