#
# Copyright (C) 2016 Florin Petriuc. All rights reserved.
# Initial release: Florin Petriuc <petriuc.florin@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version
# 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#

all:
	mkdir -p ./core/build
	mkdir -p ./core/build/obj
	mkdir -p ./core/build/lib
	make -f release.mk

debug:
	mkdir -p ./core/build
	mkdir -p ./core/build/obj
	mkdir -p ./core/build/lib
	make -f debug.mk

install:
	cp ./core/build/lib/libsocketaio.so /usr/lib/libsocketaio.so
	cp ./include/libsocketaio.h /usr/include/libsocketaio.h

clean:
	rm ./core/build/obj/*
	rm ./core/build/lib/*