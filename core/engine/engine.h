#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <string.h>

#include "../misc/memory.h"
#include "../hashmap/hashmap.h"

typedef void (*callback_t)(const int);

struct socket_evt_bind
{
	int sockFD;
	
	in_addr_t sin_addr;
	
	unsigned short int sin_port;
	
	unsigned char sin_family;
	unsigned char sin_type;
	
	callback_t accept_callback;
	callback_t recv_callback;
	callback_t rst_callback;
};

struct eth_pck
{
	unsigned char *buf;
	
	int len;
};

int engine_init(const int workers_no);

int engine_register_bind_struct(struct socket_evt_bind *bind);

#endif