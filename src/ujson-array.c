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
  * ujson-array.c
  *
  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ujson-values.h"
#include "ujson-array.h"

ujarray* array_allot(uint16_t len)
{
	ujarray* a;
	if (!len) return NULL;
	a = (ujarray*)calloc(1, sizeof(ujarray) + sizeof(ujvalue*) * len);
	a->size = len;
	a->start = 0;
	a->end = 0;
	return a;
}

uint16_t array_length(ujarray* a)
{
	return a->end - a->start;
}

ujarray* array_push(ujarray* a, ujvalue* v)
{
	a->values[a->end++ % a->size] = v;
	return a;
}

ujvalue* array_pop(ujarray* a)
{
	ujvalue* v;
	v = a->values[--a->end % a->size];
	a->values[a->end % a->size] = NULL;
	return v;
}

ujvalue* array_shift(ujarray* a)
{
	ujvalue* v;
	v = a->values[a->start++ % a->size];
	a->values[(a->start - 1) % a->size] = NULL;
	return v;
}

ujarray* array_each(ujarray* a, void(*f)(ujvalue** v))
{
	uint16_t i;
	for (i = a->start; i < a->end; i++) f(&a->values[i % a->size]);
	return a;
}

ujarray* array_map(ujarray* a, ujvalue*(*f)(ujvalue* v))
{
	uint16_t i;
	ujarray* a2;
	ujvalue* v2;
	a2 = array_allot(array_length(a));
	for (i = a->start; i < a->end; i++) {
		v2 = f(a->values[i % a->size]);
		array_push(a2, v2);
	}
	return a2;
}

static void _array_chain_release(ujvalue** v)
{
	switch((*v)->type) {
		case uj_string:
			str_release(&(*v)->data_as.string);
			break;
		case uj_array:
			array_release(&(*v)->data_as.array);
			break;
		case uj_object:
			// TODO object_release(&v->data_as.object);
			break;
	}
}

void array_release(ujarray** a)
{
	if (!(*a)) return;
	array_each(*a, _array_chain_release);
	free(*a);
	*a = NULL;
}

