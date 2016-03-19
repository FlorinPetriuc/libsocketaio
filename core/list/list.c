#include "list.h"

static void *add_to_list_head(struct list_node *head, void *value)
{
	struct list_node *to_add;
	
	to_add = xmalloc(sizeof(struct list_node));
	
	to_add->val = value;
	to_add->next = head;
	
	if(head)
	{
		head->prev = to_add;
	}
	
	to_add->prev = NULL;
	
	return to_add;
}

void *new_concurrent_list()
{
	struct concurrent_list *ret;
	
	ret = xmalloc(sizeof(struct concurrent_list));
	
	INIT_MUTEX(ret->mtx);
	INIT_COND(ret->cond);

	ret->head = NULL;
	ret->tail = NULL;
	
	return ret;
}

void add_to_concurrent_list_head(struct concurrent_list *list, void *value)
{	
	TAKE_MUTEX(list->mtx);

	list->head = add_to_list_head(list->head, value);
	
	if(list->tail == NULL)
	{
		list->tail = list->head;
	}
	
	RELEASE_MUTEX(list->mtx);
	
	SIGNAL_COND(list->cond);
}

static int value_compare(struct socket_evt_bind *bind, struct socket_evt_bind *lookup)
{
	if(bind->sin_addr != lookup->sin_addr)
	{
		return 1;
	}
	
	if(bind->sin_port != lookup->sin_port)
	{
		return 1;
	}
	
	if(bind->sin_family != lookup->sin_family)
	{
		return 1;
	}
	
	if(bind->sin_type != lookup->sin_type)
	{
		return 1;
	}
	
	return 0;
}

void *list_lookup(struct concurrent_list *list, void *data)
{
	struct list_node *ret;
	
	TAKE_MUTEX(list->mtx);
	
	ret = list->head;
	
	while(ret)
	{
		if(value_compare(ret->val, data))
		{
			ret = ret->next;
			
			continue;
		}
		
		break;
	}
	
	RELEASE_MUTEX(list->mtx);
	
	if(ret)
	{
		return ret->val;
	}
	
	return NULL;
}

void *remove_from_concurrent_list_tail(struct concurrent_list *list)
{
	void *ret;
	
	struct list_node *to_delete;
	
	TAKE_MUTEX(list->mtx);
	
	if(list->tail == NULL)
	{
		RELEASE_MUTEX(list->mtx);
		
		return NULL;
	}
	
	ret = list->tail->val;
	
	to_delete = list->tail;
	
	list->tail = list->tail->prev;
	
	if(list->tail == NULL)
	{
		list->head = NULL;
	}
	
	RELEASE_MUTEX(list->mtx);
	
	free(to_delete);
	
	return ret;
}

void wait_for_element(struct concurrent_list *list)
{
	TAKE_MUTEX(list->mtx);
	
	while(list->tail == NULL)
	{
		WAIT_COND(list->cond, list->mtx);
	}
	
	RELEASE_MUTEX(list->mtx);
}