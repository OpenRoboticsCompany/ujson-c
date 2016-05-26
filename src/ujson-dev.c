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
#define STACKSTR_SIZE 123
#include "str.h"
#include "ujson-types.h"
#include "schematags.h"
#include "ujson-value.h"
#include "ujson-array.h"
#include "ujson-dump.h"
#include "ujson-parse.h"

int main(int argc, char* argv[])
{
	printf("\n\n*********************\nRunning ujson dev rig.\n");
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

	ujarray* lp = 0;
	lp = array_allot(10);

	ujvalue* mainv;
	mainv = ujvalue_new();
	mainv->type = uj_array;
	mainv->data_as.array = lp;

	ujvalue* v;
	v = ujvalue_new();
	v->type = uj_number;
	v->numbertype = uj_uint64;
	v->data_as.uint64 = 0xffffffffffffffff;
	array_push(lp, v);

	v = ujvalue_new();
	v->type = uj_true;
	array_push(lp, v);

	v = ujvalue_new();
	v->type = uj_string;
	v->data_as.string = str_from((uint8_t*)"This is a test string");
	array_push(lp, v);

	v = ujvalue_new();
	v->type = uj_array;
	v->data_as.array = array_allot(3);
	ujvalue* v2;
	v2 = ujvalue_new(); v2->type = uj_string;
	v2->data_as.string = str_from((uint8_t*)"String One");
	array_push(v->data_as.array, v2); 
	v2 = ujvalue_new(); v2->type = uj_string;
	v2->data_as.string = str_from((uint8_t*)"String Two");
	array_push(v->data_as.array, v2); 
	v2 = ujvalue_new(); v2->type = uj_string;
	v2->data_as.string = str_from((uint8_t*)"String Three");
	array_push(v->data_as.array, v2); 
	array_push(lp, v);

	v = ujvalue_new();
	v->type = uj_number;
	v->numbertype = uj_uint32;
	v->data_as.uint32 = 0x12345678;
	array_push(lp, v);

	ujdump(mainv);

	return(0);
}




