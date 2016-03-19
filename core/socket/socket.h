#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include "../misc/memory.h"
#include "../engine/engine.h"

int libsocketaio_register_socket(const int socket, const unsigned char socket_type, 
								callback_t accept_callback, callback_t recv_callback, 
								callback_t rst_callback);



#endif