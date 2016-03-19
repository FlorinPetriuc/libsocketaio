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