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
#include "ujson-object.h"
#include "ujson-hash.h"

ujobject* object_allot(uint16_t len)
{
	ujobject* o;
	if (!len) return NULL;
	o = (ujobject*)calloc(1, sizeof(ujobject) + sizeof(ujstring*) * len + sizeof(ujvalue*) * len);
	o->size = len;
	o->used = 0;
	return o;
}

uint16_t object_length(ujobject* o)
{
}

ujobject* object_set(ujobject* o, ujstring* k, ujvalue* v)
{
	// TODO simplify the string handling first, then fixup the hash, then implement here
	uint16_t i;
	uint16_t h;
	//h = hash_buffer(k->data
}

ujvalue* object_get(ujobject* o, uint8_t* k)
{
}

ujarray* object_keys(ujobject* o)
{
}

ujobject* object_each(ujobject* o, void(*f)(ujstring** k, ujvalue** v))
{
}

static void _object_chain_release(ujstring** k, ujvalue** v)
{
	str_release(k);
	ujvalue_release(v);
}

void object_release(ujobject** o)
{
	if (!(*o)) return;
	object_each(*o, _object_chain_release);
	free(*o);
	*o = NULL;
}

