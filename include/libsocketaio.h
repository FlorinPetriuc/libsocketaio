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

#ifndef _LIBSOCKETAIO_H_
#define _LIBSOCKETAIO_H_

/*
 * The version of the running library
 */
extern const unsigned int libsocketaio_version;

/*
 * Socket close callback function
 *
 * Params:
 * the socket descriptor: int
 */
typedef void (*close_callback_t)(const int);

/*
 * Socket receive callback function
 *
 * Params:
 * the socket descriptor: int
 */
typedef void (*recv_callback_t)(const int);

/*
 * Socket accept callback function
 *
 * Params:
 * the socket descriptor: int
 * the server address that the accepted the connection: sockaddr_in *
 */
typedef void (*accept_callback_t)(const int, struct sockaddr_in *);

/*
 * libsocketaio_initialize
 *
 * Initializes the library's threadpool and internal data structures
 *
 * Params:
 * - number_of_workers: the number of threads in the threadpool
 *
 * Returns:
 * 0 - success
 * not 0 - failure
 */
int libsocketaio_initialize(const int number_of_workers);

/*
 * libsocketaio_register_udp_socket
 *
 * Adds a udp socket to the pool of monitored sockets
 *
 * Params:
 * - socketFD - the udp socket descriptor
 * - bind_addr - the address that the socket is bound to
 * - recv_cb - callback for handling udp receive
 */
int libsocketaio_register_udp_socket(const int socketFD, struct sockaddr_in *bind_addr,
																	recv_callback_t recv_cb);

/*
 * libsocketaio_register_tcp_server_socket
 *
 * Adds a server socket to the pool of monitored sockets
 * 
 * Params:
 * - socketFD - the server socket that has to be monitored
 * - addr - the address that the socket is bound to
 * - accept_cd - callback for handling tcp accept
 *
 * Returns:
 * 0 - success
 * not 0 - failure
 */
int libsocketaio_register_tcp_server_socket(const int socketFD, struct sockaddr_in *addr, 
																accept_callback_t accept_cb);
																
/*
 * libsocketaio_register_tcp_socket
 *
 * Params:
 * - socketFD - the tcp socket that has to be monitored
 * - local_addr - the local address that the socket is bound to
 * - remote_addr - the remote address that the socket communicates to
 * - recv_cb - callback for handling tcp receive
 * - close_cb - callback that is called when the socket is closing
 *
 * Returns:
 * 0 - success
 * not 0 - failure
 */
int libsocketaio_register_tcp_socket(const int socketFD, struct sockaddr_in *local_addr, struct sockaddr_in *remote_addr, 
																recv_callback_t recv_cb, close_callback_t close_cb);

#endif