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
#include <stdlib.h>

#include <libsocketaio.h>

void recv_cb(const int sockFD, void *arg)
{
	size_t len;

	unsigned char buffer[1024];

	struct sockaddr_in remote;

	socklen_t addr_len;

	addr_len = sizeof(remote);

	len = recvfrom(sockFD, buffer, 1024, 0, (struct sockaddr *)&remote, &addr_len);
	if(len <= 0)
	{
		return;
	}

	sendto(sockFD, "Hello ", sizeof("Hello ") - 1, 0, (struct sockaddr *)&remote, addr_len);
	sendto(sockFD, buffer, len, 0, (struct sockaddr *)&remote, addr_len);

	printf("Sent udp hello to %d.Param is %s\n", sockFD, (char *)arg);
	fflush(stdout);
}

void free_cb(void *arg)
{
	printf("Free %s\n", (char *)arg);
	free(arg);
}

int main(void)
{
	int sock;
	int res;
	
	char *arg;

	struct sockaddr_in addr;

	signal(SIGPIPE, SIG_IGN);

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0)
    {
		printf("can not create udp socket %d\n", sock);

		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8000);

	if(bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in))<0)
	{
		printf("can not bind to interface using socket %d\n", sock);
		close(sock);

		return 1;
	}

	listen(sock, 10);

	res = libsocketaio_initialize(8);
	if(res)
	{
		printf("libsocketaio_initialize failed");
		close(sock);

		return 1;
	}

	printf("Libsocketaio initialized. Version: %u\n", libsocketaio_version);
	fflush(stdout);

	arg = malloc(17);
	sprintf(arg, "arg%d", sock);

	libsocketaio_register_udp_socket(sock, &addr, arg, free_cb, recv_cb);

	while(1)
	{
		sleep(1);
	}
}