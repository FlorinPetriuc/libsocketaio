#include "memory.h"

void *xmalloc(const size_t size)
{
	void *ret;
	
	ret = malloc(size);
	
	if(ret == NULL)
	{
		perror("[FATAL] Out of memory");
		exit(EXIT_FAILURE);
	}
	
	return ret;
}