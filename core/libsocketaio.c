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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139,
 * USA.
 *
 */
 
#include "./engine/engine.h"
#include "./socket/socket.h"

unsigned int libsocketaio_version = 100;
 
int libsocketaio_initialize(const int number_of_workers)
{	
	return engine_init(number_of_workers);
}

int libsocketaio_register_tcp_server_socket(const int socketFD, struct sockaddr_in *srv_addr, 
																	accept_callback_t accept_cb)
{
	return register_socket(socketFD, SOCK_STREAM, srv_addr, NULL, accept_cb, NULL, NULL);
}

int libsocketaio_register_tcp_socket(const int socketFD, struct sockaddr_in *local_addr, struct sockaddr_in *remote_addr, 
																recv_callback_t recv_cb, close_callback_t close_cb)
{
	return register_socket(socketFD, SOCK_STREAM, local_addr, remote_addr, NULL, recv_cb, close_cb);
}