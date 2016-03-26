#
# Copyright (C) 2016 Florin Petriuc. All rights reserved.
# Initial release: Florin Petriuc <petriuc.florin@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version
# 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#

.PHONY: simple_tcp_server simple_tcp_client simple_udp_receiver clean

simple_tcp_server: simple_tcp_server.c
	$(CC) $(CFLAGS) $@.c -o$@ -lsocketaio $(LDFLAGS)
	
simple_tcp_client: simple_tcp_client.c
	$(CC) $(CFLAGS) $@.c -o$@ -lsocketaio $(LDFLAGS)

simple_udp_receiver: simple_udp_receiver.c
	$(CC) $(CFLAGS) $@.c -o$@ -lsocketaio $(LDFLAGS)

clean:
	rm -rf ./simple_tcp_server ./simple_tcp_client ./simple_udp_receiver