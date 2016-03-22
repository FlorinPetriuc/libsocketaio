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
struct socket_evt_bind *hashmap_lookup(struct hashmap *map, struct socket_evt_bind *lookup);
struct socket_evt_bind *hashmap_remove(struct hashmap *map, struct socket_evt_bind *lookup);

#endif