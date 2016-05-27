/*
 * Copyright (c) 2016 Aaron Kondziela <aaron@aaronkondziela.com> 
 *
 * This file is part of ujson-c
 *
 * ujson-c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ujson-c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
 /*
  * Part of ujson-c - Implements microjson in C - see ujson.org
  * and https://github.com/aaronkondziela/ujson-c/
  *
  * ujson-dev.c
  *
  * This is only used during development of the ujson-c code, and will
  * most likely be deleted from the final release. Just various internal
  * probing and testing and whatnot. Nothing to see here, move along.
  *
  */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ujson-encode.h"
#include "hexdump.h"
#include "endian.h"
#include "udp.h"
#include "ujson-hash.h"
#include "ujson-string.h"
#include "ujson-types.h"
#include "schematags.h"
#include "ujson-value.h"
#include "ujson-array.h"
#include "ujson-dump.h"
#include "ujson-parse.h"
#include "ujson-hash.h"
#include "ujson-object.h"
#include <assert.h>

int main(int argc, char* argv[])
{
	printf("\n\n*********************\nRunning ujson dev rig.\n");

	printf("parse_objectlen()\n");
	char* a = "\x00\x0e\x00\x03\x41\x41\x41\x63\x2a\x00\x03\x42\x42\x42\x63\x40";
	printf("len is %d\n", parse_objectlen(&a));
	assert( parse_objectlen(&a) == 2 );

	printf("Listening for udp...\n");
	int n;
	unsigned char buffer[65536];
	n = rec(buffer, 65536);
	printf("recevied %u bytes\n",n);
	ujvalue* parsedv;
	uint8_t* buf = (uint8_t*)buffer;
	parsedv = parse(&buf);
	ujdump(parsedv);
	return 0;
}




