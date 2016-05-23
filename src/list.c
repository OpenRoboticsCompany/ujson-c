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
  * list.c
  *
  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ujson-types.h"
#include "ujson-values.h"
#include "list.h"

ujlist* list_allot(uint16_t len)
{
	ujlist* l;
	if (!len) return NULL;
	l = (ujlist*)calloc(1, sizeof(ujlist) + sizeof(ujvalue) * len);
	l->size = len;
	l->start = 0;
	l->end = 0;
	return l;
}

uint16_t list_length(ujlist* l)
{
	return l->end - l->start;
}

void list_push(ujlist* l, ujvalue* v)
{
	memcpy(&l->values[l->end++ % l->size], v, sizeof(ujvalue));
}

void list_pop(ujlist* l, ujvalue* v)
{
	memcpy(v, &l->values[--l->end % l->size], sizeof(ujvalue));
}

void list_each(ujlist* l, void(*f)(ujvalue* v))
{
	uint16_t i;
	for (i = l->start; i < l->end; i++) f(&l->values[i % l->size]);
}

ujlist* list_map(ujlist* l, ujvalue*(*f)(ujvalue* v))
{
	ujlist* l2;
	l2 = list_allot(list_length(l));
	ujvalue* v2;
	uint16_t i;
	for (i = l->start; i < l->end; i++) {
		v2 = f(&l->values[i % l->size]);
		list_push(l2, v2);
		ujvalue_release(&v2);
	}
	return l2;
}

static void _list_chain_release(ujvalue* v)
{
	switch(v->type) {
		case uj_string:
			str_release(&v->data_as.string);
			break;
		case uj_array:
			// TODO list_release(&v->data_as.array);
			break;
		case uj_object:
			// TODO object_release(&v->data_as.object);
			break;
	}
}

void list_release(ujlist** l)
{
	list_each(*l, _list_chain_release);
	free(*l);
	*l = NULL;
}

