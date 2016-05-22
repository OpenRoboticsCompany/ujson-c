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
  * array helpers
  *
  */

#include <stdint.h>

#include "schematags.h"
#include "ujson-array.h"

uint16_t uj_arraylen(uint8_t** buf)
{
	// buf should have the 'a' schematag trimmed
	uint8_t* b;
	b = *buf;
	uint16_t size = 0, l = 0;
	extract_uint16(&b, &size);
	uint16_t n = 0;
	schematag t;
	while (b < *buf + size) {
		t = (schematag)*b;
		b++;
		n++;
		switch(t) {
			case uj_bool_true_tag:
			case uj_bool_false_tag:
			case uj_null_tag:
				break;
			case uj_uint8_tag:
			case uj_int8_tag:
				b += 1;
				break;
			case uj_uint16_tag:
			case uj_int16_tag:
				b += 2;
				break;
			case uj_uint32_tag:
			case uj_int32_tag:
			case uj_float_tag:
				b += 4;
				break;
			case uj_uint64_tag:
			case uj_int64_tag:
			case uj_double_tag:
				b += 8;
				break;
			case uj_string_tag:
			case uj_array_tag:
			case uj_object_tag:
				extract_uint16(&b, &l);
				b += l;
				break;
			default:
				break;
		}
	}
	return n;
}


