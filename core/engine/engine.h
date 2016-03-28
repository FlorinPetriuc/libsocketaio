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

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <string.h>
#include <stdbool.h>

typedef void (*close_callback_t)(const int, void *);
typedef void (*recv_callback_t)(const int, void *);
typedef void (*accept_callback_t)(const int, void *);
typedef void (*free_callback_t)(void *);

#include "../debug/debug.h"
#include "../socket/socket.h"
#include "../misc/memory.h"
#include "../hashmap/hashmap.h"

enum engine_operation_t
{
	LIBSOCKETAIO_ENGINE_INIT,
	LIBSOCKETAIO_ENGINE_REGISTER,
	LIBSOCKETAIO_ENGINE_UNREGISTER
};

struct enpoint
{
	in_addr_t sin_addr;

	unsigned short int sin_port;
};

struct socket_evt_bind
{
	int sockFD;

	struct enpoint local_endpoint;
	struct enpoint remote_endpoint;	

	bool remote_endpoint_present;

	unsigned char sin_family;
	unsigned char sin_type;

	unsigned int accept_sequence;
	unsigned int push_sequence;

	void *arg;

	accept_callback_t accept_callback;
	recv_callback_t recv_callback;
	close_callback_t close_callback;
	free_callback_t free_callback;
};

struct eth_init_process
{
	struct hashmap *lookup_map;

	struct concurrent_list *eth_queue;

	pthread_mutex_t *process_mutex;
};

struct eth_pck
{
	unsigned char *buf;

	int len;
};

int libsocketaio_engine_init(int workers_no);
int libsocketaio_engine_register_bind_struct(struct socket_evt_bind *bind);
int libsocketaio_engine_unregister_socket(int socket);

#endif