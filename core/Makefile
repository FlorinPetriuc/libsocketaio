all:\
	./build/obj/socket.o		\
	./build/obj/main.o			\
	
	$(CC) $(LDFLAGS)	\
	\
	./build/obj/socket.o		\
	./build/obj/main.o			\
	\
	-o\
	\
	./build/bin/libsocketaio

./build/obj/main.o:				./main.c
	$(CC) $(CFLAGS) -c -Werror	./main.c			-o 	./build/obj/main.o
	
./build/obj/socket.o:			./socket/socket.c
	$(CC) $(CFLAGS) -c -Werror	./socket/socket.c	-o 	./build/obj/socket.o

clean:
	rm ./build/obj/*
	rm ./build/bin/*