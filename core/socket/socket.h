/*
 * Copyright (C) 2016 Florin Petriuc. All rights reserved.
 * Initial release: Florin Petriuc <petriuc.florin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include "../misc/memory.h"
#include "../engine/engine.h"

struct socket_evt_bind;

int libsocketaio_unregister_socket_fd(int socket);

int libsocketaio_register_socket(const int socket, const unsigned char socket_type,
								struct sockaddr_in *local_endpoint, struct sockaddr_in *remote_endpoint,
								void *arg, free_callback_t free_callback,
								accept_callback_t accept_callback, recv_callback_t recv_callback, close_callback_t close_callback);

int create_monitor_socket();

#endif