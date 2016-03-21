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

#include "main.h"

void recv_cb(const int sockFD)
{
	int len;
	
	char buf[4096];
	
	len = recv(sockFD, buf, sizeof(buf) - 1, 0);
	
	if(len < 0)
	{
		printf("can not read from connection\n");
		fflush(stdout);
		
		return;
	}
	
	buf[len] = '\0';
	
	printf("GOT: %s\n", buf);
	fflush(stdout);
}

void close_cb(const int sockFD)
{
	printf("rst on %d\n", sockFD);
	fflush(stdout);
}

void accept_cb(const int sockFD, struct sockaddr_in *accept_addr)
{
	int newSocket;
	
	struct sockaddr_in cli_addr;
	
	socklen_t clilen = sizeof(cli_addr);
	
	newSocket = accept(sockFD, (struct sockaddr *) &cli_addr, &clilen);
	
	if(newSocket < 0)
	{
		printf("can not accept connection\n");
		fflush(stdout);
		
		return;
	}

	register_socket(newSocket, SOCK_STREAM, accept_addr, &cli_addr, NULL, recv_cb, close_cb);

	printf("registered new connection\n");
	fflush(stdout);
}

int main(void)
{
	int sock;
	int res;
	
	struct sockaddr_in addr;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
    {
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(80);

    if(bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in))<0)
    {
        return 1;
    }
	
	listen(sock, 10);
	
	res = engine_init(8);
	if(res)
	{
		printf("engine init failed");
		return 1;
	}
	
	register_socket(sock, SOCK_STREAM, &addr, NULL, accept_cb, NULL, NULL);
	
	while(1)
	{
		sleep(1);
	}
}