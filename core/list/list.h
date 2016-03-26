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

#ifndef _LIST_H_
#define _LIST_H_

#include <pthread.h>

#include "../engine/engine.h"
#include "../misc/memory.h"
#include "../misc/mutex.h"

struct list_node;

struct list_node
{
	void *val;

	struct list_node *next;
	struct list_node *prev;
};

struct concurrent_list
{
	pthread_mutex_t mtx;

	pthread_cond_t cond;

	struct list_node *head;
	struct list_node *tail;
};

void *new_concurrent_list();

void add_to_concurrent_list_head(struct concurrent_list *list, void *value);
void *remove_from_concurrent_list_tail(struct concurrent_list *list);

void wait_for_element(struct concurrent_list *list);

void *concurrent_list_lookup(struct concurrent_list *list, void *data);
void *concurrent_list_remove(struct concurrent_list *list, void *data);

#endif