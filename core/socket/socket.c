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

#include "socket.h"

int create_monitor_socket()
{
	int ret;
	
	ret = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	return ret;
}

int register_socket(const int socket, const unsigned char socket_type, struct sockaddr_in *addr,
					callback_t accept_callback, callback_t recv_callback, callback_t rst_callback)
{
	struct socket_evt_bind *bind;
	
	int res;

	if(socket_type != SOCK_STREAM)
	{
		return -1;
	}
		
	if(addr->sin_family != AF_INET)
	{
		return -1;
	}
	
	bind = xmalloc(sizeof(struct socket_evt_bind));
	
	bind->sin_addr = addr->sin_addr.s_addr;
	bind->sin_port = addr->sin_port;
	bind->sin_family = addr->sin_family;
	bind->sin_type = socket_type;
	
	bind->sockFD = socket;
	
	bind->accept_callback = accept_callback;
	bind->recv_callback = recv_callback;
	bind->rst_callback = rst_callback;
	
	res = engine_register_bind_struct(bind);
	
	if(res)
	{
		free(bind);
		
		return 1;
	}
	
	return 0;
}