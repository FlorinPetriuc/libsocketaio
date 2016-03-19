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

static struct socket_lookup parse_accept_request(const unsigned char *buf)
{
	struct socket_lookup ret;
	
	ret.sin_addr = buf[26] | (buf[27] << 8) | (buf[28] << 16) | (buf[29] << 24);
	ret.sin_port = buf[34] | (buf[35] << 8);
	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_STREAM;
	
	return ret;
}

static void *eth_listen(void *arg)
{
	struct concurrent_list *eth_queue;
	struct eth_pck *to_push;
	
	int sockFD;
	int len;
	
	unsigned char buf[64 * 1024];
	
	socklen_t addr_len;
	
	struct sockaddr_ll addr;
	
	eth_queue = arg;
	
	sockFD = create_monitor_socket();
	
	if(sockFD < 0)
	{
		perror("[FATAL] Unable to create monitor socket");
		exit(EXIT_FAILURE);
	}
	
	while(1)
	{
		len = recvfrom(sockFD, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
		
		if(len < 0)
		{
			perror("[ERROR] Unable to read from monitor socket");
			
			continue;
		}
		
		to_push = xmalloc(sizeof(struct eth_pck));
		
		to_push->buf = xmalloc(len);
		memcpy(to_push->buf, buf, len);
		
		to_push->len = len;
		
		add_to_concurrent_list_head(eth_queue, to_push);
	}
	
	return NULL;
}

static void *eth_process(void *arg)
{
	struct concurrent_list *eth_queue;
	struct eth_pck *eth_pck = NULL;
	struct socket_lookup lookup;
	
	unsigned short int pck_type;
	
	unsigned char sin_proto;
	
	eth_queue = arg;
	
	while(1)
	{
		if(eth_pck)
		{
			free(eth_pck->buf);
			free(eth_pck);
			
			eth_pck = NULL;
		}
		
		wait_for_element(eth_queue);
		
		printf("Thread %lu is up\n", pthread_self());
		fflush(stdout);
		
		eth_pck = remove_from_concurrent_list_tail(eth_queue);
		
		if(eth_pck == NULL)
		{
			continue;
		}
		
		if(eth_pck->len < 13)
		{
			continue;
		}
		
		pck_type = (eth_pck->buf[12] << 8) | eth_pck->buf[13];
		
		//ip protocol
		if(pck_type != 0x0800)
		{
			continue;
		}
		
		if(eth_pck->len < 23)
		{
			continue;
		}
		
		if(eth_pck->buf[14] >> 4 != 4)
		{
			continue;
		}
		
		sin_proto = eth_pck->buf[23];
		
		switch(sin_proto)
		{
			case IPPROTO_TCP:
			{
				if(eth_pck->len < 47)
				{
					continue;
				}
				
				if((eth_pck->buf[47] & 2) && (eth_pck->buf[47] & 16))
				{
					//SYN && ACK
					lookup = parse_accept_request(eth_pck->buf);
					
					printf("lookup ip: %u\n", lookup.sin_addr);
					printf("lookup port: %u\n", lookup.sin_port);
					printf("lookup family: %u\n", lookup.sin_family);
					printf("lookup type: %u\n", lookup.sin_type);
					fflush(stdout);
				}
				
				if(eth_pck->buf[47] & 4)
				{
					//RST
					printf("GOT CONNECTION RST\n");
					fflush(stdout);
				}
				
				if(eth_pck->buf[47] & 8)
				{
					//PSH
					printf("GOT CONNECTION PSH\n");
					fflush(stdout);				
				}
			}
			break;
			
			default: continue;
		}
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