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

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <string.h>

#include "../misc/memory.h"
#include "../hashmap/hashmap.h"

typedef void (*callback_t)(const int);

struct socket_evt_bind
{
	int sockFD;
	
	in_addr_t sin_addr;
	
	unsigned short int sin_port;
	
	unsigned char sin_family;
	unsigned char sin_type;
	
	callback_t accept_callback;
	callback_t recv_callback;
	callback_t rst_callback;
};

struct eth_pck
{
	unsigned char *buf;
	
	int len;
};

int engine_init(const int workers_no);

int engine_register_bind_struct(struct socket_evt_bind *bind);

#endif