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
  * list.h
  *
  */

#ifndef _UJ_LIST_H
#define _UJ_LIST_H

#include <stdint.h>

typedef struct ujlist ujlist;
struct ujlist {
	uint16_t start;
	uint16_t end;
	uint16_t size;
	ujvalue values[0];
};

ujlist* list_allot(uint16_t len);
uint16_t list_length(ujlist* l);
void list_push(ujlist* l, ujvalue* v);
void list_pop(ujlist* l, ujvalue* v);
void list_each(ujlist* l, void(*f)(ujvalue* v));
ujlist* list_map(ujlist* l, ujvalue*(*f)(ujvalue* v));
static void _list_chain_release(ujvalue* v);
void list_release(ujlist** l);

#endif

