#include "engine.h"

static struct hashmap *lookup_map = NULL;

static unsigned int hashGenerator(struct socket_evt_bind *bind)
{
	unsigned int hash = 0;
	
	hash = (bind->sin_addr & 0xFF000000) >> 16;
	hash |= (bind->sin_addr & 0x00FF0000) >> 16;
	hash ^= bind->sin_port;
	
	hash &= 0xFFFF;
		
	return hash;
}

void engine_init(const int workers_no)
{
	lookup_map = new_hashmap(64 * 1024, hashGenerator);
}

int engine_register_bind_struct(struct socket_evt_bind *bind)
{	
	add_to_hashmap(lookup_map, bind);

	return 0;
}