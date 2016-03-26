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

#ifndef _LIBSOCKETAIO_H_
#define _LIBSOCKETAIO_H_

/*
 * The version of the running library
 */
extern const unsigned int libsocketaio_version;

/*
 * Free argument callback function
 *
 * Params:
 * - the arg pointer passed when the socket was registered
 */
typedef void (*free_callback_t)(void *);

/*
 * Socket close callback function
 *
 * Params:
 * - the socket descriptor: int
 *
 * Warning
 * the socket will be closed by the library after this call
 */
typedef void (*close_callback_t)(const int, void *);

/*
 * Socket receive callback function
 *
 * Params:
 * - the socket descriptor: int
 */
typedef void (*recv_callback_t)(const int, void *);

/*
 * Socket accept callback function
 *
 * Params:
 * - the socket descriptor: int
 */
typedef void (*accept_callback_t)(const int, void *);

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
 * - arg - argument that will be passed in all callbacks as void *
 * - free_cb - callback to free the arg pointer
 * - recv_cb - callback for handling udp receive
 */
int libsocketaio_register_udp_socket(const int socketFD, struct sockaddr_in *bind_addr, void *arg,
													free_callback_t free_cb, recv_callback_t recv_cb);

/*
 * libsocketaio_register_tcp_server_socket
 *
 * Adds a server socket to the pool of monitored sockets
 * 
 * Params:
 * - socketFD - the server socket that has to be monitored
 * - addr - the address that the socket is bound to
 * - arg - argument that will be passed in all callbacks as void *
 * - free_cb - callback to free the arg pointer
 * - accept_cb - callback for handling tcp accept
 *
 * Returns:
 * 0 - success
 * not 0 - failure
 */
int libsocketaio_register_tcp_server_socket(const int socketFD, struct sockaddr_in *addr, void *arg,
												free_callback_t free_cb, accept_callback_t accept_cb);

/*
 * libsocketaio_register_tcp_client_socket
 *
 * Params:
 * - socketFD - the tcp socket that has to be monitored
 * - remote_addr - the remote address where the socket communicates to
 * - arg - argument that will be passed in all callbacks as void *
 * - free_cb - callback to free the arg pointer
 * - recv_cb - callback for handling tcp receive
 * - close_cb - callback that is called when the socket is closing
 *
 * Returns:
 * 0 - success
 * not 0 - failure
 */
int libsocketaio_register_tcp_client_socket(const int socketFD, struct sockaddr_in *remote_addr, void *arg,
									free_callback_t free_cb, recv_callback_t recv_cb, close_callback_t close_cb);

/*
 * libsocketaio_unregister_socket
 *
 * Params:
 * - socketFD - the socket that has to be removed monitored sockets
 *
 * Warning:
 * - this does not close the socket
 * - this calls the free callback on argument
 */
int libsocketaio_unregister_socket(const int socketFD);

#endif