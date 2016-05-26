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
  * ujson-object.h
  *
  */

#ifndef _UJ_OBJECT_H
#define _UJ_OBJECT_H

#include <stdint.h>
#include "ujson-value.h"
#include "ujson-array.h"

typedef struct ujvalue ujvalue;
typedef struct ujarray ujarray;
typedef struct ujobject ujobject;

struct ujobject {
	uint16_t size;
	uint16_t used;
	void* data[0];
};

ujobject* object_allot(uint16_t len);
uint16_t object_length(ujobject* o);
ujobject* object_set(ujobject* o, ujstring* k, ujvalue* v);
ujvalue* object_get(ujobject* o, uint8_t* k);
ujarray* object_keys(ujobject* o);
ujobject* object_each(ujobject* o, void(*f)(ujstring** k, ujvalue** v));
void object_release(ujobject** o);

#endif













