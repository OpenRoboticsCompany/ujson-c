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
  * ujson-tojson.c
  * prints out a parsed ujson object (ujvalue tree)
  *
  */

#include <stdio.h>
#include <stdint.h>
#include "ujson-value.h"
#include "ujson-tojson.h"

static void _tojson(ujvalue* v, int i, uint8_t opts)
{
	int n;
	switch(v->type) {
		case uj_true:
			printf("true");
			break;
		case uj_false:
			printf("false");
			break;
		case uj_null:
			printf("null");
			break;
		case uj_number:
			switch(v->numbertype) {
				case uj_uint8:
					if (opts & uj_showtypes) printf("uint8/");
					printf("%u",v->data_as.uint8); break;
				case uj_int8:
					if (opts & uj_showtypes) printf("int8/");
					printf("%d",v->data_as.int8); break;
				case uj_uint16:
					if (opts & uj_showtypes) printf("uint16/");
					printf("%u",v->data_as.uint16); break;
				case uj_int16:
					if (opts & uj_showtypes) printf("int16/");
					printf("%d",v->data_as.int16); break;
				case uj_uint32:
					if (opts & uj_showtypes) printf("uint32/");
					printf("%u",v->data_as.uint32); break;
				case uj_int32:
					if (opts & uj_showtypes) printf("int32/");
					printf("%d",v->data_as.int32); break;
				case uj_uint64:
					if (opts & uj_showtypes) printf("uint64/");
					printf("%llu",v->data_as.uint64); break;
				case uj_int64:
					if (opts & uj_showtypes) printf("int64/");
					printf("%lld",v->data_as.int64); break;
				case uj_float:
					if (opts & uj_showtypes) printf("float/");
					printf("%f",v->data_as.f); break;
				case uj_double:
					if (opts & uj_showtypes) printf("double/");
					printf("%f",v->data_as.d); break;
			}
			break;
		case uj_string:
			printf("\"%s\"",(char*)v->data_as.string->data);
			break;
		case uj_array:
			printf("[");
			for (n = 0; n < array_length(v->data_as.array); n++) {
				_tojson(v->data_as.array->values[n], i + 1, opts);
				if (n < array_length(v->data_as.array) - 1) printf(",");
			}
			printf("]");
			break;
		case uj_object:
			printf("{");
			for (n = 0; n < v->data_as.object->size; n++) {
				printf("\"%s\":", ((ujstring*)v->data_as.object->data[n*2])->data);
				_tojson((ujvalue*)v->data_as.object->data[n*2+1], i+1, opts);
				if (n < v->data_as.object->size - 1) printf(",");
			}
			printf("}");
			break;
	}
	if (i == 0) printf("\n");
}

// TODO convert this to print into a buffer, buflen setup
// TODO add string " escaping - get rid of printf and use a byte moverator with slash inserterator
// TODO redo dumper
void tojson(ujvalue* v)
{
	_tojson(v, 0, uj_plainjson);
}

void tojson_with_types(ujvalue* v)
{
	_tojson(v, 0, uj_showtypes);
}
