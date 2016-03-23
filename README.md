# libsocketaio

Libsocketaio is a C event-driven library for easy socket manipulation.

## Dependencies

The library depends on libpthread.

## Installation

1. Download the source:<br />
	```bash
	$ git clone https://github.com/FlorinPetriuc/libsocketaio.git
	```
	
2. Compile:<br />
	```bash
	$ make
	```
	
3. Install:<br />
	```bash
	sudo make 
	```
	
## Usage

Include libsocketaio.h in your aplication and link to -lsocketaio.
Follow the examples. 

## Examples

simple_tcp_server:<br />
	Echoes "Hello " + [client message] back to the sender
		
	```bash
	$ cd examples
	$ make simple_tcp_server
	$ ./simple_tcp_server
	```
