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
  * ujson-dump.c
  * prints out a parsed ujson object (ujvalue tree)
  *
  */

#include <stdio.h>
#include <stdint.h>
#include "ujson-value.h"

static void indent(int i)
{
	while (i--) printf("\t");
}

static void ujdumpval(ujvalue* v, int i)
{
	int n;
	indent(i);
	switch(v->type) {
		case uj_true:
			printf("true\n");
			break;
		case uj_false:
			printf("false\n");
			break;
		case uj_null:
			printf("null\n");
			break;
		case uj_number:
			switch(v->numbertype) {
				case uj_uint8:
					printf("uint8/%u\n",v->data_as.uint8); break;
				case uj_int8:
					printf("int8/%d\n",v->data_as.int8); break;
				case uj_uint16:
					printf("uint16/%u\n",v->data_as.uint16); break;
				case uj_int16:
					printf("int16/%d\n",v->data_as.int16); break;
				case uj_uint32:
					printf("uint32/%u\n",v->data_as.uint32); break;
				case uj_int32:
					printf("int32/%d\n",v->data_as.int32); break;
				case uj_uint64:
					printf("uint64/%llu\n",v->data_as.uint64); break;
				case uj_int64:
					printf("int64/%lld\n",v->data_as.int64); break;
				case uj_float:
					printf("float/%f\n",v->data_as.f); break;
				case uj_double:
					printf("double/%f\n",v->data_as.d); break;
			}
			break;
		case uj_string:
			printf("\"%s\"\n",(char*)v->data_as.string->data);
			break;
		case uj_array:
			printf("[\n");
			for (n = 0; n < array_length(v->data_as.array); n++) {
				ujdumpval(v->data_as.array->values[n], i + 1);
			}
			indent(i);
			printf("]\n");
			break;
		case uj_object:
			// TODO implement this after objects done
			printf("{Object (not yet implemented)}\n");
			break;
	}
}

void ujdump(ujvalue* v)
{
	ujdumpval(v, 0);
}
