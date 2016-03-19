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

#endif