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
  * ujson-object.c
  *
  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ujson-value.h"
#include "ujson-string.h"
#include "ujson-array.h"
#include "ujson-hash.h"
#include "ujson-object.h"

ujobject* object_allot(uint16_t len)
{
	ujobject* o;
	if (!len) return NULL;
	o = (ujobject*)calloc(1, sizeof(ujobject) + sizeof(ujstring*) * len + sizeof(ujvalue*) * len);
	o->size = len;
	o->used = 0;
	return o;
}

ujobject* object_set(ujobject* o, ujstring* k, ujvalue* v)
{
	uint16_t i;
	ujhash h;
	h = hash_string(k);
	for (i = 0; i < o->size; i++) {
		if (o->data[((h+i) % o->size) * 2] && !string_eq(k, (ujstring*)o->data[((h+i) % o->size) * 2])) continue;
		printf("\n");
		o->data[((h+i) % o->size) * 2] = k;
		o->data[((h+i) % o->size) * 2 + 1] = v;
		return o;
	}
	return NULL;
}

ujvalue* object_get(ujobject* o, ujstring* k)
{
	uint16_t i;
	ujhash h;
	h = hash_string(k);
	for (i = 0; i < o->size; i++) {
		if (o->data[((h+i) % o->size) * 2] && !string_eq(k, (ujstring*)o->data[((h+i) % o->size) * 2])) continue;
		return o->data[((h+i) % o->size) * 2 + 1];
	}
	return NULL;
}

ujvalue* object_keys(ujobject* o)
{
	uint16_t i;
	ujvalue* v;
	ujvalue* k;
	ujstring* s;
	v = ujvalue_new();
	v->type = uj_array;
	v->data_as.array = array_allot(o->size);
	for (i = 0; i < o->size; i++) {
		if (o->data[i*2]) {
			k = ujvalue_new();
			k->type = uj_string;
			// TODO Don't like duplictaing data here, but need to or else breaks chain-release
			// with duplicated pointers to same object. Make be less dumb.
			s = string_from( ((ujstring*)o->data[i*2])->data );
			k->data_as.string = s;
			array_push(v->data_as.array, k);
		}
	}
	return v;
}

ujobject* object_each(ujobject* o, void(*f)(ujstring** k, ujvalue** v))
{
	uint16_t i;
	for (i = 0; i < o->size; i++) {
		f((ujstring**)&o->data[i*2], (ujvalue**)&o->data[i*2+1]);
	}
	return o;
}

static void _object_chain_release(ujstring** k, ujvalue** v)
{
	string_release(k);
	ujvalue_release(v);
}

void object_release(ujobject** o)
{
	if (!(*o)) return;
	object_each(*o, _object_chain_release);
	free(*o);
	*o = NULL;
}

