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
  * ujson-hash.c
  * for hashmaps
  *
  */

#include <stdint.h>
#include "ujson-hash.h"
#include "ujson-string.h"

uint16_t hash_buffer(const uint8_t* x, const uint16_t len)
{
	uint16_t h = 0;
	uint16_t i;
	for (i = 0; i < len; i++) {
		h += ((x[i] << 9) + (x[i] << 4) + x[i]) ^ (h << 3);
	}
	return h;
}

uint16_t hash(const str* s)
{
	return hash_buffer(s->data, s->length);
}
