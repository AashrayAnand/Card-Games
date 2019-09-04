#ifndef _IO_STR_
#define _IO_STR_

#include <iostream>
#include <string>

#endif

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

// function prototypes in this header file manage the serialization of card structures,
// which is needed to send cards and populate the client's card set appropriately

// flow:
// 1. create a 2-byte buffer, with one byte being used to represent the card's suit,
// and one byte being used to represent the card's value
// 2. populate the buffer with the card's suit and value
