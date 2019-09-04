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
#define BACKLOG 5

#ifndef PORT
#define PORT 8080
#endif

// function prototypes in this header file manage the creation and set up of server
// process, and the handlers for sending and receiving data from the player

// need to be able to:
// 1. set up server (create socket fd, bind and listen to port) -> [createServer(...)]
// 2. accept client (accept and store client socket fd) -> [acceptClient(...)]
// 3. start game loop:
//  a. ask user if they would like to receive a card: -> [askToDraw() && user.sum < 21]
//    i. yes: deal card (send 2 bytes, representing suit and value) -> sendCard()
//      - if user_sum == 21, user wins -> [handled on client side]
//      - if user_sum > 21, user loses -> [handled on client side]
//      - while user_sum < 21, repeat a.
//    ii. no: user turn is over, tally up and store max score -> [readScore()]
//      - dealer (server) deals cards while max sum is less than
//        sum of user -> [OUTSIDE OF ListenHandle scope]
//      - if sum > user_sum and sum <= 21 then dealer wins [OUTSIDE OF ListenHandle scope]
//      - else, user wins [OUTSIDE OF ListenHandle scope]
//    iii. update user's account based on win or loss -> [updateBalance()]

class Server {
public:
  explicit Server(uint16_t);

  // pre: requires a pointer to an integer
  // post: returns a socket file descriptor for the server process, or -1 if fails to do so
  //       also stores the ai_family of the port in the provided int pointer

  // will need to getaddrinfo(), call:
  // i. socket()
  // ii. bind()
  // iii. listen()

  // using info from getaddrinfo
  int createServer();

  // pre: requires a server socket file descriptor, which is listening for an incoming connection
  //      as well as a pointer to a sockaddr structure
  // post: accepts an incoming connection, and returns the socket file descriptor created
  //       for the client, stores client address information in sockaddr pointer parameter
  int acceptClient(struct sockaddr *, socklen_t *);

  // getters;
  int get_fd() const;
  int get_family() const;
private:
  uint16_t port;
  int sockfd; // server socket file descriptor
  int sock_family; // server socket family
};

// pre: requires a socket file descriptor (representing the client) to send data to,
//      as well as a card which has been drawn from the deck
// post: sends a serialized representation of the card drawn by the user
//       to the user socket file descriptor, returns 0 on successful send to client
int sendCard(int, Card);

// pre: requires a socket file descriptor (representing the client) to read and write to
// post: returns a bool representing whether or not the player wishes to draw another card
bool askToDraw(int);

// pre: requires a socket file descriptor (representing the client) to read data from
// post: reads the user's final score, and stores it
void readScore(int *);
