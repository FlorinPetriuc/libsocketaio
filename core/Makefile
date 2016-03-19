all:\
	./build/obj/socket.o		\
	./build/obj/engine.o		\
	./build/obj/memory.o		\
	./build/obj/main.o			\
	
	$(CC) $(LDFLAGS)	\
	\
	./build/obj/socket.o		\
	./build/obj/engine.o		\
	./build/obj/memory.o		\
	./build/obj/main.o			\
	\
	-o\
	\
	./build/bin/libsocketaio

./build/obj/main.o:				./main.c
	$(CC) $(CFLAGS) -c -Werror	./main.c			-o 	./build/obj/main.o
	
./build/obj/socket.o:			./socket/socket.c
	$(CC) $(CFLAGS) -c -Werror	./socket/socket.c	-o 	./build/obj/socket.o
	
./build/obj/engine.o:			./engine/engine.c
	$(CC) $(CFLAGS) -c -Werror	./engine/engine.c	-o 	./build/obj/engine.o
	
./build/obj/memory.o:			./misc/memory.c
	$(CC) $(CFLAGS) -c -Werror	./misc/memory.c		-o 	./build/obj/memory.o

clean:
	rm ./build/obj/*
	rm ./build/bin/*