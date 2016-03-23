# libsocketaio

Libsocketaio is a C event-driven socket library.

## Dependencies

The library depends on libpthread.

## Installation

1. Download the source<br />
	git clone https://github.com/FlorinPetriuc/libsocketaio.git
	
2. Compile<br />
	make
	
3. Install<br />
	sudo make install
	
## Usage

Include libsocketaio.h in your aplication and link to -lsocketaio.
Follow the examples. 

## Examples

simple_tcp_server:<br />
	Echoes "Hello " + <client data> back to the sender
	
	Build & run: 
	cd examples
	make simple_tcp_server
	./simple_tcp_server
