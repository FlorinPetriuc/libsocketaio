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
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139,
# USA.
#
#

all:\
	./build/obj/socket.o		\
	./build/obj/engine.o		\
	./build/obj/memory.o		\
	./build/obj/hashmap.o		\
	./build/obj/list.o			\
	./build/obj/main.o			\
	
	$(CC) $(LDFLAGS)	\
	\
	./build/obj/socket.o		\
	./build/obj/engine.o		\
	./build/obj/memory.o		\
	./build/obj/hashmap.o		\
	./build/obj/list.o			\
	./build/obj/main.o			\
	\
	-o\
	\
	./build/bin/libsocketaio -lpthread

./build/obj/main.o:				./main.c
	$(CC) $(CFLAGS) -c -Werror	./main.c			-o 	./build/obj/main.o
	
./build/obj/socket.o:			./socket/socket.c
	$(CC) $(CFLAGS) -c -Werror	./socket/socket.c	-o 	./build/obj/socket.o
	
./build/obj/hashmap.o:			./hashmap/hashmap.c
	$(CC) $(CFLAGS) -c -Werror	./hashmap/hashmap.c	-o 	./build/obj/hashmap.o
	
./build/obj/list.o:				./list/list.c
	$(CC) $(CFLAGS) -c -Werror	./list/list.c		-o 	./build/obj/list.o
	
./build/obj/engine.o:			./engine/engine.c
	$(CC) $(CFLAGS) -c -Werror	./engine/engine.c	-o 	./build/obj/engine.o
	
./build/obj/memory.o:			./misc/memory.c
	$(CC) $(CFLAGS) -c -Werror	./misc/memory.c		-o 	./build/obj/memory.o

clean:
	rm ./build/obj/*
	rm ./build/bin/*