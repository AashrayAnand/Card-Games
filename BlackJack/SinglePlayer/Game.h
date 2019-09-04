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

// pre: must receive char string message, and socket file descriptor
// post: utility function to copy a message to a buffer, and send
//       it to the specified socket file descriptor
void sendMessage(const char *, int *);

// pre: must receive a pointer to a server object
int getBalance(Server *, int *);
int sendBalance(Client *, Player *);


// pre: must receive a server object and a client socket file descriptor
// post: game manager for server proc
void playGame(Server*, int *);
// pre: must receive a client object and a server socket file descriptor
// game manager for client proc
void playGame(Client*);

// pre: must take a server object, a client socket file descriptor,
//      and a current client balance
// post: returns the bet placed by the client
int placeBet(Server *, int *, int);
