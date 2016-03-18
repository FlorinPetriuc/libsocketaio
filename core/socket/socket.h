#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

int register_socket(const int socket, const unsigned char socket_type);

#endif