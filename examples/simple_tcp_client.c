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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
 
#include <libsocketaio.h>

#define MAKE_IPv4(x, y, z, t) (x) | ((y) << 8) | ((z) << 16) | ((t) << 24)

void recv_cb(const int sockFD)
{
	int len;
	
	char buf[4096];
	
	len = recv(sockFD, buf, sizeof(buf) - 1, 0);

	buf[len] = '\0';
	
	printf("Got: %s\n", buf);
	fflush(stdout);
}

void close_cb(const int sockFD)
{
	printf("closed socket %d\n", sockFD);
	fflush(stdout);
}

int main(void)
{
	int sock;
	int res;
	
	struct sockaddr_in srv_addr;
	struct sockaddr_in cli_addr;
	
	socklen_t len;
	
	signal(SIGPIPE, SIG_IGN);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
    {
		printf("can not create tcp socket %d\n", sock);
		
        return 1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = MAKE_IPv4(127,0,0,1);
    srv_addr.sin_port = htons(80);
		
	res = libsocketaio_initialize(8);
	if(res)
	{
		printf("libsocketaio_initialize failed");
		close(sock);
		
		return 1;
	}
	
	printf("Libsocketaio initialized. Version: %u\n", libsocketaio_version);
	fflush(stdout);
	
	if(connect(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
	{
		printf("connect failed");
		close(sock);
		
		return 1;
	}
	
	len = sizeof(cli_addr);
	
	if(getsockname(sock, (struct sockaddr *)&cli_addr, &len))
	{
		printf("getsockname failed");
		close(sock);
		
		return 1;
	}
	
	if(libsocketaio_register_tcp_socket(sock, &cli_addr, &srv_addr, recv_cb, close_cb))
	{
		printf("can not register socket %d\n", sock);
		fflush(stdout);
		
		return 1;
	}
	
	send(sock, "World", sizeof("World") - 1, 0);
	
	while(1)
	{
		sleep(1);
	}
}