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
 */

#include "engine.h"

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

	ret.accept_sequence = (buf[42] << 24) | (buf[43] << 16) | (buf[44] << 8) | (buf[45]);

	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_STREAM;	

	return ret;
}

static struct socket_evt_bind parse_tcp_accept_ack_request(const unsigned char *buf)
{
	struct socket_evt_bind ret;

	ret.local_endpoint.sin_addr = buf[30] | (buf[31] << 8) | (buf[32] << 16) | (buf[33] << 24);
	ret.local_endpoint.sin_port = buf[36] | (buf[37] << 8);

	ret.remote_endpoint_present = false;

	ret.remote_endpoint.sin_addr = 0;
	ret.remote_endpoint.sin_port = 0;

	ret.accept_sequence = (buf[38] << 24) | (buf[39] << 16) | (buf[40] << 8) | (buf[41]);

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

static struct socket_evt_bind parse_tcp_push_ack_request(const unsigned char *buf)
{
	struct socket_evt_bind ret;

	ret.remote_endpoint.sin_addr = buf[30] | (buf[31] << 8) | (buf[32] << 16) | (buf[33] << 24);
	ret.remote_endpoint.sin_port = buf[36] | (buf[37] << 8);

	ret.local_endpoint.sin_addr = buf[26] | (buf[27] << 8) | (buf[28] << 16) | (buf[29] << 24);
	ret.local_endpoint.sin_port = buf[34] | (buf[35] << 8);

	ret.remote_endpoint_present = true;

	ret.push_sequence = (buf[38] << 24) | (buf[39] << 16) | (buf[40] << 8) | (buf[41]);

	ret.sin_family = AF_INET;
	ret.sin_type = SOCK_STREAM;

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

	ret.push_sequence = (buf[42] << 24) | (buf[43] << 16) | (buf[44] << 8) | (buf[45]);

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
		libsocketaio_print("[FATAL] Unable to create monitor socket\n", 0);
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		len = recvfrom(sockFD, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);

		if(len < 0)
		{
			libsocketaio_print("[ERROR] Unable to read from monitor socket\n", 0);

			continue;
		}

		if(len < 38)
		{
			continue;
		}

		//not ipv4 protocol
		if(((buf[12] << 8) | buf[13]) != 0x0800 ||
			(buf[14] >> 4) != 4)
		{
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
	struct eth_init_process *eth_init;
	struct hashmap *lookup_map;

	pthread_mutex_t *process_mutex;

	eth_init = arg;

	eth_queue = eth_init->eth_queue;
	lookup_map = eth_init->lookup_map;
	process_mutex = eth_init->process_mutex;

	while(1)
	{
		if(eth_pck)
		{
			free(eth_pck->buf);
			free(eth_pck);

			eth_pck = NULL;
		}

		pthread_mutex_lock(process_mutex);

		wait_for_element(eth_queue);

		eth_pck = remove_from_concurrent_list_tail(eth_queue);

		if(eth_pck == NULL)
		{
			pthread_mutex_unlock(process_mutex);

			continue;
		}

		switch(eth_pck->buf[23])
		{
			case IPPROTO_TCP:
			{
				if(eth_pck->len < 47)
				{
					pthread_mutex_unlock(process_mutex);

					break;
				}

				if(!(eth_pck->buf[47] & 16))
				{
					//must contain ack

					pthread_mutex_unlock(process_mutex);

					break;
				}

				if(eth_pck->buf[47] & 2)
				{
					//SYN
					lookup = parse_tcp_accept_request(eth_pck->buf);

					map_lookup = hashmap_lookup(lookup_map, &lookup);

					if(map_lookup == NULL)
					{
						pthread_mutex_unlock(process_mutex);

						libsocketaio_print("[INFO] unmonitored accept\n", 0);

						break;
					}

					if(map_lookup->accept_callback == NULL)
					{
						pthread_mutex_unlock(process_mutex);

						libsocketaio_print("[INFO] accept with no callback\n", 0);

						break;
					}

					map_lookup->accept_sequence = lookup.accept_sequence;

					libsocketaio_print("[INFO] accept sequence for %d is %u\n", 2, map_lookup->sockFD, map_lookup->accept_sequence);

					pthread_mutex_unlock(process_mutex);

					break;
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
					}

					pthread_mutex_unlock(process_mutex);

					if(map_lookup == NULL)
					{
						libsocketaio_print("[INFO] unmonitored close\n", 0);

						break;
					}

					if(map_lookup->close_callback)
					{
						map_lookup->close_callback(map_lookup->sockFD, map_lookup->arg);
					}

					libsocketaio_print("[INFO] closing %d\n", 1, map_lookup->sockFD);

					if(map_lookup->free_callback)
					{
						map_lookup->free_callback(map_lookup->arg);
					}

					close(map_lookup->sockFD);
					free(map_lookup);

					break;
				}

				if(eth_pck->buf[47] & 8)
				{
					//PSH
					lookup = parse_tcp_push_request(eth_pck->buf);

					map_lookup = hashmap_lookup(lookup_map, &lookup);

					if(map_lookup == NULL)
					{
						pthread_mutex_unlock(process_mutex);

						libsocketaio_print("[INFO] got tcp data on unmonitored socket\n", 0);

						break;
					}

					if(map_lookup->recv_callback == NULL)
					{
						pthread_mutex_unlock(process_mutex);

						libsocketaio_print("[INFO] got tcp data on socket with no recv callback\n", 0);

						break;
					}

					map_lookup->push_sequence = lookup.push_sequence;

					libsocketaio_print("[INFO] push sequence for %d is %u\n", 2, map_lookup->sockFD, map_lookup->push_sequence);

					pthread_mutex_unlock(process_mutex);

					break;
				}

				if(eth_pck->buf[47] == 16)
				{
					lookup = parse_tcp_accept_ack_request(eth_pck->buf);

					map_lookup = hashmap_lookup(lookup_map, &lookup);

					if(map_lookup == NULL)
					{
						goto process_psh_ack;
					}

					if(map_lookup->accept_callback == NULL)
					{
						goto process_psh_ack;
					}

					if(map_lookup->accept_sequence == 0)
					{
						libsocketaio_print("[INFO] unexpected accept ack\n", 0);

						goto process_psh_ack;
					}

					if(map_lookup->accept_sequence != lookup.accept_sequence)
					{
						libsocketaio_print("[INFO] wrong accept sequence in ack\n", 0);

						goto process_psh_ack;
					}

					map_lookup->accept_sequence = 0;

					pthread_mutex_unlock(process_mutex);

					map_lookup->accept_callback(map_lookup->sockFD, map_lookup->arg);

					libsocketaio_print("[INFO] accept complete for %d\n", 1, map_lookup->sockFD);

					break;

process_psh_ack:

					lookup = parse_tcp_push_ack_request(eth_pck->buf);

					map_lookup = hashmap_lookup(lookup_map, &lookup);

					if(map_lookup == NULL)
					{
						pthread_mutex_unlock(process_mutex);

						break;
					}

					if(map_lookup->recv_callback == NULL)
					{
						pthread_mutex_unlock(process_mutex);

						break;
					}

					if(map_lookup->push_sequence == 0)
					{
						pthread_mutex_unlock(process_mutex);

						break;
					}

					if(map_lookup->push_sequence != lookup.push_sequence)
					{
						pthread_mutex_unlock(process_mutex);

						libsocketaio_print("[INFO] wrong push sequence in ack %u vs %u\n", 2, 	map_lookup->push_sequence, 
																								lookup.push_sequence);

						break;
					}

					map_lookup->push_sequence = 0;

					pthread_mutex_unlock(process_mutex);

					map_lookup->recv_callback(map_lookup->sockFD, map_lookup->arg);

					libsocketaio_print("[INFO] push complete for %d\n", 1, map_lookup->sockFD);

					break;
				}

				pthread_mutex_unlock(process_mutex);
			}
			break;

			case IPPROTO_UDP:
			{
				pthread_mutex_unlock(process_mutex);

				lookup = parse_udp_recv_request(eth_pck->buf);

				map_lookup = hashmap_lookup(lookup_map, &lookup);

				if(map_lookup == NULL)
				{
					libsocketaio_print("[INFO] got udp data on unmonitored socket\n", 0);

					break;
				}

				if(map_lookup->recv_callback == NULL)
				{
					libsocketaio_print("[INFO] got udp data on socket with no recv callback\n", 0);

					break;
				}

				libsocketaio_print("[INFO] got udp data on socket %d\n", 1, map_lookup->sockFD);

				map_lookup->recv_callback(map_lookup->sockFD, map_lookup->arg);
			}
			break;

			default:
			{
				pthread_mutex_unlock(process_mutex);
			}
			break;
		}
	}

	return NULL;
}

static int libsocketaio_engine_do(const enum engine_operation_t op, void *arg)
{
	static struct hashmap *lookup_map;
	static bool initialized = false;
	static pthread_mutex_t *process_mutex;
	static struct concurrent_list *eth_queue;

	pthread_t worker;

	struct eth_init_process *eth_init;

	struct socket_evt_bind *bind;

	int ret;
	int i;
	int workers_no;
	int socket;

	switch(op)
	{
		case LIBSOCKETAIO_ENGINE_INIT:
		{
			workers_no = *(int *)arg;

			if(workers_no == 0)
			{
				return 1;
			}

			if(initialized)
			{
				return 1;
			}

			process_mutex = xmalloc(sizeof(pthread_mutex_t));

			pthread_mutex_init(process_mutex, NULL);

			eth_queue = new_concurrent_list();

			lookup_map = new_hashmap(64 * 1024, hashGenerator);

			eth_init = xmalloc(sizeof(struct eth_init_process));
			eth_init->lookup_map = lookup_map;
			eth_init->eth_queue = eth_queue;
			eth_init->process_mutex = process_mutex;

			ret = pthread_create(&worker, NULL, eth_listen, eth_queue);
			if(ret)
			{
				goto out;
			}

			for(i = 0; i < workers_no; ++i)
			{
				ret = pthread_create(&worker, NULL, eth_process, eth_init);

				if(ret)
				{
					goto out;
				}
			}

			libsocketaio_print("[INFO] libsocketaio initialized with %d workers\n", 1, workers_no);
			initialized = true;
		}

out:
		return ret;

		case LIBSOCKETAIO_ENGINE_REGISTER:
		{
			bind = arg;

			add_to_hashmap(lookup_map, bind);
		}
		return 0;

		case LIBSOCKETAIO_ENGINE_UNREGISTER: 
		{
			socket = *(int *)arg;
		}
		return hashmap_remove_fd(lookup_map, socket);

		default: return 1;
	}
}

int libsocketaio_engine_init(int workers_no)
{
	return libsocketaio_engine_do(LIBSOCKETAIO_ENGINE_INIT, &workers_no);
}

int libsocketaio_engine_unregister_socket(int socket)
{
	return libsocketaio_engine_do(LIBSOCKETAIO_ENGINE_UNREGISTER, &socket);
}

int libsocketaio_engine_register_bind_struct(struct socket_evt_bind *bind)
{
	return libsocketaio_engine_do(LIBSOCKETAIO_ENGINE_REGISTER, bind);
}