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