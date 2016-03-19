#include "list.h"

static void *add_to_list_head(struct list_node *head, void *value)
{
	struct list_node *to_add;
	
	to_add = xmalloc(sizeof(struct list_node));
	
	to_add->val = value;
	to_add->next = head;
	
	return to_add;
}

void *new_concurrent_list()
{
	struct concurrent_list *ret;
	
	ret = xmalloc(sizeof(struct concurrent_list));
	
	INIT_MUTEX(ret->mtx);
	ret->head = NULL;
	
	return ret;
}

void add_to_concurrent_list_head(struct concurrent_list *list, void *value)
{	
	TAKE_MUTEX(list->mtx);

	list->head = add_to_list_head(list->head, value);
	
	RELEASE_MUTEX(list->mtx);
}