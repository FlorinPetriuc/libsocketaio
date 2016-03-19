#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "../engine/engine.h"
#include "../list/list.h"

struct socket_evt_bind;

typedef unsigned int (*hash_fct_t)(struct socket_evt_bind *);

struct hashmap
{
	struct concurrent_list **buckets;
	
	unsigned int buckets_no;
	
	hash_fct_t hash_fct;
};

void *new_hashmap(const unsigned int buckets_no, hash_fct_t hash_fct);

void add_to_hashmap(struct hashmap *map, struct socket_evt_bind *bind);

#endif