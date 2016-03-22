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

#include "hashmap.h"

void *new_hashmap(const unsigned int buckets_no, hash_fct_t hash_fct)
{
	struct hashmap *ret;
	
	unsigned int i;
	
	ret = xmalloc(sizeof(struct hashmap));
	
	ret->buckets_no = buckets_no;
	ret->hash_fct = hash_fct;
	
	ret->buckets = xmalloc(buckets_no * sizeof(struct concurrent_list *));
	
	for(i = 0; i < buckets_no; ++i)
	{
		ret->buckets[i] = new_concurrent_list();
	}
	
	return ret;
}

void add_to_hashmap(struct hashmap *map, struct socket_evt_bind *bind)
{
	unsigned int hash;
	
	struct concurrent_list *bucket;
	
	hash = map->hash_fct(bind);
	
	bucket = map->buckets[hash];

	add_to_concurrent_list_head(bucket, bind);
}

struct socket_evt_bind *hashmap_lookup(struct hashmap *map, struct socket_evt_bind *lookup)
{
	struct socket_evt_bind *ret;
	
	unsigned int hash;
	
	struct concurrent_list *bucket;
	
TRY_AGAIN:
	hash = map->hash_fct(lookup);
		
	bucket = map->buckets[hash];

	ret = concurrent_list_lookup(bucket, lookup);
	
	if(ret)
	{
		return ret;
	}
	
	if(lookup->remote_endpoint_present)
	{
		return NULL;
	}
	
	//accept lookup
	if(lookup->local_endpoint.sin_addr != INADDR_ANY)
	{
		lookup->local_endpoint.sin_addr = INADDR_ANY;

		goto TRY_AGAIN;
	}
	
	return NULL;
}

struct socket_evt_bind *hashmap_remove(struct hashmap *map, struct socket_evt_bind *lookup)
{
	struct socket_evt_bind *ret;
	
	unsigned int hash;
	
	struct concurrent_list *bucket;
	
	hash = map->hash_fct(lookup);

	bucket = map->buckets[hash];

	ret = concurrent_list_remove(bucket, lookup);
	
	return ret;
}