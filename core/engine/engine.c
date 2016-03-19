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

static void *eth_listen(void *arg)
{
	struct concurrent_list *eth_queue = arg;
	
	while(1)
	{
		
	}
	
	return NULL;
}

static void *eth_process(void *arg)
{
	struct concurrent_list *eth_queue = arg;
	
	while(1)
	{
		
	}
		
	return NULL;
}

int engine_init(const int workers_no)
{
	pthread_t worker;
	
	struct concurrent_list *eth_queue;
	
	int ret;
	int i;
	
	if(workers_no == 0)
	{
		return 1;
	}
	
	eth_queue = new_concurrent_list();
	
	lookup_map = new_hashmap(64 * 1024, hashGenerator);
	
	ret = pthread_create(&worker, NULL, eth_listen, eth_queue);
	if(ret)
	{
		goto out;
	}
	
	for(i = 0; i < workers_no; ++i)
	{
		ret = pthread_create(&worker, NULL, eth_process, eth_queue);
		
		if(ret)
		{
			goto out;
		}
	}
	
out:
	return ret;
}

int engine_register_bind_struct(struct socket_evt_bind *bind)
{	
	add_to_hashmap(lookup_map, bind);

	return 0;
}