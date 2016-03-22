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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>
 
#ifdef LIBSOCKETAIO_DEBUG

#define libsocketaio_print debugPrint

void debugPrint(const char *template, const unsigned int n, ...);

#else

#define libsocketaio_print(x, y, ...)

#endif
 
#endif