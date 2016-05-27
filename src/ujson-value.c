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
  * ujson-value.c
  *
  */

#include <stdint.h>
#include <stdlib.h>
#include "ujson-value.h"

ujvalue* ujvalue_new()
{
	ujvalue* v = calloc(1, sizeof(ujvalue));
	v->type = uj_null;
	return v;
}

void ujvalue_release(ujvalue** v)
{
	if (!(*v)) return;
	if ((*v)->type == uj_string && (*v)->data_as.string) string_release(&(*v)->data_as.string);
	if ((*v)->type == uj_array && (*v)->data_as.array) array_release(&(*v)->data_as.array);
	if ((*v)->type == uj_object && (*v)->data_as.object) object_release(&(*v)->data_as.object);
	free(*v);
	*v = NULL;
}
