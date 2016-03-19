#ifndef _LIST_H_
#define _LIST_H_

#include <pthread.h>

#include "../misc/memory.h"
#include "../misc/mutex.h"

struct list_node;

struct list_node
{
	void *val;
	
	struct list_node *next;
};

struct concurrent_list
{
	pthread_mutex_t mtx;
	
	struct list_node *head;
};

void *new_concurrent_list();

void add_to_concurrent_list_head(struct concurrent_list *list, void *value);

#endif