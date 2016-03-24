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

int register_socket(const int socket, const unsigned char socket_type, 
					struct sockaddr_in *local_endpoint, struct sockaddr_in *remote_endpoint,
					accept_callback_t accept_callback, recv_callback_t recv_callback, close_callback_t close_callback)
{
	struct socket_evt_bind *bind;
	
	int res;

	if(socket_type != SOCK_STREAM)
	{
		return -1;
	}
	
	if(local_endpoint == NULL)
	{
		return -1;
	}
	
	if(local_endpoint->sin_family != AF_INET)
	{
		return -1;
	}
	
	if(socket < 0)
	{
		return -1;
	}
	
	res = fcntl(socket, F_GETFL, 0);
	
	if(res < 0)
	{
		return -1;
	}
	
	res |= O_NONBLOCK;
	
	res = fcntl(socket, F_SETFL, res);
	
	if(res < 0)
	{
		return -1;
	}
	
	bind = xmalloc(sizeof(struct socket_evt_bind));
	
	bind->sockFD = socket;
	
	bind->sin_family = local_endpoint->sin_family;
	bind->sin_type = socket_type;
	
	bind->local_endpoint.sin_addr = local_endpoint->sin_addr.s_addr;
	bind->local_endpoint.sin_port = local_endpoint->sin_port;
	
	if(remote_endpoint)
	{
		if(local_endpoint->sin_family != remote_endpoint->sin_family)
		{
			free(bind);
			
			return -1;
		}
		
		bind->remote_endpoint_present = true;
		
		bind->remote_endpoint.sin_addr = remote_endpoint->sin_addr.s_addr;
		bind->remote_endpoint.sin_port = remote_endpoint->sin_port;
	}
	else
	{
		bind->remote_endpoint_present = false;
		
		bind->remote_endpoint.sin_addr = 0;
		bind->remote_endpoint.sin_port = 0;
	}
	
	bind->accept_callback = accept_callback;
	bind->recv_callback = recv_callback;
	bind->close_callback = close_callback;
	
	res = engine_register_bind_struct(bind);
	
	if(res)
	{
		free(bind);
		
		return 1;
	}
	
	return 0;
}