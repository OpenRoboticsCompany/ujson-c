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
  * ujson-format.c
  * Format functions
  * Return a ujson schema describing sample data, suitable for use with extract
  *
  */

#include <stdint.h>
#include "ujson-endian.h"
#include "ujson-movebytes.h"
#include "ujson-value.h"
#include "schematags.h"
#include "ujson-format.h"

static void _format(uint8_t** buf, ujvalue* v, int i)
{
	uint16_t n;
	uint8_t* sizefield;
	switch(v->type) {
		case uj_true:
		case uj_false:
			*((*buf)++) = uj_bool_tag;
			break;
		case uj_null:
			*((*buf)++) = uj_null_tag;
			break;
		case uj_number:
			switch(v->numbertype) {
				case uj_uint8:
					*((*buf)++) = uj_uint8_tag;
					break;
				case uj_int8:
					*((*buf)++) = uj_int8_tag;
					break;
				case uj_uint16:
					*((*buf)++) = uj_uint16_tag;
					break;
				case uj_int16:
					*((*buf)++) = uj_int16_tag;
					break;
				case uj_uint32:
					*((*buf)++) = uj_uint32_tag;
					break;
				case uj_int32:
					*((*buf)++) = uj_int32_tag;
					break;
				case uj_uint64:
					*((*buf)++) = uj_uint64_tag;
					break;
				case uj_int64:
					*((*buf)++) = uj_int64_tag;
					break;
				case uj_float:
					*((*buf)++) = uj_float_tag;
					break;
				case uj_double:
					*((*buf)++) = uj_double_tag;
					break;
			}
			break;
		case uj_string:
			*((*buf)++) = uj_string_tag;
			break;
		case uj_array:
			*((*buf)++) = uj_array_tag;
			for (n = 0; n < array_length(v->data_as.array); n++) _format(buf, v->data_as.array->values[n], i+1);
			break;
		case uj_object:
			*((*buf)++) = uj_object_tag;
			for (n = 0; n < v->data_as.object->size; n++) _format(buf, v->data_as.object->data[n*2+1], i+1);
			break;
	}
	if (i==0) *((*buf)++) = 0;
}

uint16_t format(uint8_t* buf, ujvalue* v)
{
	uint8_t* bufstart;
	bufstart = buf;
	_format(&buf, v, 0);
	return (uint16_t)(buf - bufstart);
}
