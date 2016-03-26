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