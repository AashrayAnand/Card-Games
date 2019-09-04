#ifndef _LISTEN_
#define _LISTEN_
#include "ServerListenHandle.h"
#endif

#ifndef _LISTEN_C_
#define _LISTEN_C_
#include "ClientListenHandle.h"
#endif

#ifndef _SERIAL_
#define _SERIAL_
#include "Serialize.h"
#endif

// small buffer, which is used to send serialized 
#ifndef SMALL_BUFFER_SIZE
#define SMALL_BUFFER_SIZE 2
#endif

// pre: must receive char string message, and socket file descriptor
// post: utility function to copy a message to a buffer, and send
//       it to the specified socket file descriptor
void sendMessage(const char *, int *);

// pre: must receive a pointer to a client socket file descriptor
// post: receives the client's balance from the client socket file descriptor
int getBalance(int *);
// pre: must receive a client object, and a Player object
// post: gets the balance from the player, and sends it to the server
int sendBalance(Client *, Player *);

// pre: must receive a server object and a client socket file descriptor
// post: game manager for server proc
void playGame(Server*, int *);
// pre: must receive a client object and a server socket file descriptor
// game manager for client proc
void playGame(Client*);

// pre: must receive client socket file descriptor, and deck pointer
// post: deals cards to user, and asks whether or not user wants card continuously
// , then does same for dealer and returns game response
int round(int *, Deck *);
int round(Client *, Player *);

// pre: must take a server object and a client socket file descriptor
// post: returns the bet placed by the client
int getBet(int *);

// pre: must take a client object, and an initial balance
// post: gets a bet from the player, and sends it to the server
int sendBet(Client *, int);

int draw(int *);

int getBest(std::vector<int>);

void printHand(std::vector<Card>);

int drawCard(int *, Deck *);

int checkDraw(int);

int takeCard(int, Player *, std::vector<int> *);

int updateBalance(int *, int);

int receiveBalance(int);
