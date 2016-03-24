/*
 * Copyright (C) 2016 Florin Petriuc. All rights reserved.
 * Initial release: Florin Petriuc <petriuc.florin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139,
 * USA.
 *
 */

#include "engine.h"

static struct hashmap *lookup_map = NULL;

static unsigned int hashGenerator(struct socket_evt_bind *bind)
{
	unsigned int hash = 0;
	
	hash = bind->local_endpoint.sin_addr >> 16;
	hash ^= bind->local_endpoint.sin_port;
	
	hash ^= bind->remote_endpoint.sin_addr >> 16;
	hash ^= bind->remote_endpoint.sin_port;
	
	hash &= 0xFFFF;
	
	return hash;
}

static struct socket_evt_bind parse_tcp_accept_request(const unsigned char *buf)
{
	struct socket_evt_bind ret;
	
	ret.local_endpoint.sin_addr = buf[26] | (buf[27] << 8) | (buf[28] << 16) | (buf[29] << 24);
	ret.local_endpoint.sin_port = buf[34] | (buf[35] << 8);
	
	ret.remote_endpoint_present = false;
	
	ret.remote_endpoint.sin_addr = 0;
	ret.remote_endpoint.sin_port = 0;
	
	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_STREAM;	
	
	return ret;
}

static struct socket_evt_bind parse_udp_recv_request(const unsigned char *buf)
{
	struct socket_evt_bind ret;
	
	ret.local_endpoint.sin_addr = buf[30] | (buf[31] << 8) | (buf[32] << 16) | (buf[33] << 24);
	ret.local_endpoint.sin_port = buf[36] | (buf[37] << 8);
	
	ret.remote_endpoint.sin_addr = 0;
	ret.remote_endpoint.sin_port = 0;
	
	ret.remote_endpoint_present = false;
	
	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_DGRAM;
	
	return ret;
}

static struct socket_evt_bind parse_tcp_push_request(const unsigned char *buf)
{
	struct socket_evt_bind ret;
	
	ret.local_endpoint.sin_addr = buf[30] | (buf[31] << 8) | (buf[32] << 16) | (buf[33] << 24);
	ret.local_endpoint.sin_port = buf[36] | (buf[37] << 8);
	
	ret.remote_endpoint.sin_addr = buf[26] | (buf[27] << 8) | (buf[28] << 16) | (buf[29] << 24);
	ret.remote_endpoint.sin_port = buf[34] | (buf[35] << 8);
	
	ret.remote_endpoint_present = true;
	
	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_STREAM;
	
	return ret;
}

static struct socket_evt_bind parse_tcp_close_request(const unsigned char *buf)
{
	struct socket_evt_bind ret;
	
	ret.local_endpoint.sin_addr = buf[30] | (buf[31] << 8) | (buf[32] << 16) | (buf[33] << 24);
	ret.local_endpoint.sin_port = buf[36] | (buf[37] << 8);
	
	ret.remote_endpoint.sin_addr = buf[26] | (buf[27] << 8) | (buf[28] << 16) | (buf[29] << 24);
	ret.remote_endpoint.sin_port = buf[34] | (buf[35] << 8);
	
	ret.remote_endpoint_present = true;
	
	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_STREAM;
	
	return ret;
}

static struct socket_evt_bind parse_tcp_close_request_reverse(const unsigned char *buf)
{
	struct socket_evt_bind ret;
	
	ret.remote_endpoint.sin_addr = buf[30] | (buf[31] << 8) | (buf[32] << 16) | (buf[33] << 24);
	ret.remote_endpoint.sin_port = buf[36] | (buf[37] << 8);
	
	ret.local_endpoint.sin_addr = buf[26] | (buf[27] << 8) | (buf[28] << 16) | (buf[29] << 24);
	ret.local_endpoint.sin_port = buf[34] | (buf[35] << 8);
	
	ret.remote_endpoint_present = true;
	
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
		libsocketaio_print("[FATAL] Unable to create monitor socket", 0);
		exit(EXIT_FAILURE);
	}
	
	while(1)
	{
		len = recvfrom(sockFD, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
		
		if(len < 0)
		{
			libsocketaio_print("[ERROR] Unable to read from monitor socket", 0);
			
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
	struct socket_evt_bind lookup;
	struct socket_evt_bind *map_lookup;
	
	struct sockaddr_in accept_addr;
	
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
		
		//ipv4
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
				
				if(!(eth_pck->buf[47] & 16))
				{
					//must contain ack
					continue;
				}
				
				if(eth_pck->buf[47] & 2)
				{
					//SYN
					lookup = parse_tcp_accept_request(eth_pck->buf);
					
					accept_addr.sin_addr.s_addr = lookup.local_endpoint.sin_addr;
					accept_addr.sin_port = lookup.local_endpoint.sin_port;
					accept_addr.sin_family = AF_INET;

					map_lookup = hashmap_lookup(lookup_map, &lookup);
					
					if(map_lookup == NULL)
					{						
						continue;
					}
					
					if(map_lookup->accept_callback == NULL)
					{
						continue;
					}
					
					map_lookup->accept_callback(map_lookup->sockFD, &accept_addr);
					
					continue;
				}
				
				if((eth_pck->buf[47] & 4) || (eth_pck->buf[47] & 1))
				{
					//RST or FIN
					lookup = parse_tcp_close_request(eth_pck->buf);
					
					map_lookup = hashmap_remove(lookup_map, &lookup);
					
					if(map_lookup == NULL)
					{						
						lookup = parse_tcp_close_request_reverse(eth_pck->buf);
					
						map_lookup = hashmap_remove(lookup_map, &lookup);
						
						if(map_lookup == NULL)
						{						
							continue;
						}
					}
					
					if(map_lookup->close_callback == NULL)
					{
						continue;
					}
					
					map_lookup->close_callback(map_lookup->sockFD);
					
					close(map_lookup->sockFD);
					free(map_lookup);
					
					continue;
				}
				
				if(eth_pck->buf[47] & 8)
				{
					//PSH
					lookup = parse_tcp_push_request(eth_pck->buf);
					
					map_lookup = hashmap_lookup(lookup_map, &lookup);
					
					if(map_lookup == NULL)
					{						
						continue;
					}
					
					if(map_lookup->recv_callback == NULL)
					{
						continue;
					}
					
					map_lookup->recv_callback(map_lookup->sockFD);
					
					continue;
				}
			}
			break;
			
			case IPPROTO_UDP:
			{
				lookup = parse_udp_recv_request(eth_pck->buf);
				
				map_lookup = hashmap_lookup(lookup_map, &lookup);
					
				if(map_lookup == NULL)
				{						
					continue;
				}
					
				if(map_lookup->recv_callback == NULL)
				{
					continue;
				}
					
				map_lookup->recv_callback(map_lookup->sockFD);
					
				continue;
			}
			break;
			
			default: break;
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
	
	libsocketaio_print("[INFO] libsocketaio initialized with %d workers: ", 1, workers_no);
	
out:
	return ret;
}

int engine_register_bind_struct(struct socket_evt_bind *bind)
{	
	add_to_hashmap(lookup_map, bind);

	return 0;
}