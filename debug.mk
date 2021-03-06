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

CFLAGS = -c -Wall -Werror -fpic -DLIBSOCKETAIO_DEBUG
LDFLAGS = -shared -o
FILES = ./core/build/obj/socket.o ./core/build/obj/engine.o ./core/build/obj/memory.o \
		./core/build/obj/hashmap.o ./core/build/obj/list.o ./core/build/obj/debug.o \
		./core/build/obj/libsocketaio.o

all:$(FILES)
	$(CC) $(LDFLAGS) ./core/build/lib/libsocketaio.so $(FILES) -lpthread

./core/build/obj/libsocketaio.o:./core/libsocketaio.c
	$(CC) $(CFLAGS)				./core/libsocketaio.c		-o 	./core/build/obj/libsocketaio.o
	
./core/build/obj/socket.o:		./core/socket/socket.c
	$(CC) $(CFLAGS)				./core/socket/socket.c		-o 	./core/build/obj/socket.o
	
./core/build/obj/hashmap.o:		./core/hashmap/hashmap.c
	$(CC) $(CFLAGS)				./core/hashmap/hashmap.c	-o 	./core/build/obj/hashmap.o
	
./core/build/obj/list.o:		./core/list/list.c
	$(CC) $(CFLAGS)				./core/list/list.c			-o 	./core/build/obj/list.o
	
./core/build/obj/engine.o:		./core/engine/engine.c
	$(CC) $(CFLAGS)				./core/engine/engine.c		-o 	./core/build/obj/engine.o
	
./core/build/obj/memory.o:		./core/misc/memory.c
	$(CC) $(CFLAGS)				./core/misc/memory.c		-o 	./core/build/obj/memory.o
	
./core/build/obj/debug.o:		./core/debug/debug.c
	$(CC) $(CFLAGS)				./core/debug/debug.c		-o 	./core/build/obj/debug.o
