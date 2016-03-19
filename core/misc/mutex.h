#ifndef _MUTEX_H_
#define _MUTEX_H_

#define INIT_MUTEX(x) \
		if(pthread_mutex_init(&x, NULL) != 0) \
		{ \
			exit(EXIT_FAILURE); \
		}

#define TAKE_MUTEX(x) \
		if(pthread_mutex_lock(&x) != 0) \
		{ \
			exit(EXIT_FAILURE); \
		}

#define RELEASE_MUTEX(x) \
		if(pthread_mutex_unlock(&x) != 0) \
		{ \
			exit(EXIT_FAILURE); \
		}
		
#define INIT_COND(x)\
		if(pthread_cond_init(&x, NULL) != 0)\
		{\
			exit(EXIT_FAILURE); \
		}
		
#define WAIT_COND(x, y)\
		pthread_cond_wait(&x, &y)
		
#define SIGNAL_COND(x)\
		pthread_cond_signal(&x)

#endif