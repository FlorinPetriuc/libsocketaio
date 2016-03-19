#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include "../misc/memory.h"
#include "../engine/engine.h"

int register_socket(const int socket, const unsigned char socket_type, struct sockaddr_in *addr,
					callback_t accept_callback, callback_t recv_callback, callback_t rst_callback);
					
int create_monitor_socket();

#endif