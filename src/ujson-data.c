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
  * ujson-data.c
  * Returns a packed data representation suitable for use with a schema
  *
  */

#include "ujson-endian.h"
#include "ujson-movebytes.h"
#include "ujson-value.h"
#include "ujson-data.h"

void data_bool(uint8_t** nextbuf, uint8_t val)
{
	**nextbuf = (uint8_t)(val ? 't' : 'f');
	(*nextbuf) += 1;
}

void data_null(uint8_t** nextbuf)
{
	**nextbuf = (uint8_t)'n';
	(*nextbuf) += 1;
}

void data_uint8(uint8_t** nextbuf, uint8_t val)
{
	**nextbuf = val;
	(*nextbuf) += 1;
}

void data_int8(uint8_t** nextbuf, int8_t val)
{
	**nextbuf = (uint8_t)val;
	(*nextbuf) += 1;
}

void data_uint16(uint8_t** nextbuf, uint16_t val)
{
	val = htoj16(val);
	movebytes( *nextbuf, (uint8_t*)&val, 2 );
	(*nextbuf) += 2;
}

void data_int16(uint8_t** nextbuf, int16_t val)
{
	val = htoj16(val);
	movebytes( *nextbuf, (uint8_t*)&val, 2 );
	(*nextbuf) += 2;
}

void data_uint32(uint8_t** nextbuf, uint32_t val)
{
	val = htoj32(val);
	movebytes( *nextbuf, (uint8_t*)&val, 4 );
	(*nextbuf) += 4;
}

void data_int32(uint8_t** nextbuf, int32_t val)
{
	val = htoj32(val);
	movebytes( *nextbuf, (uint8_t*)&val, 4 );
	(*nextbuf) += 4;
}

void data_uint64(uint8_t** nextbuf, uint64_t val)
{
	val = htoj64(val);
	movebytes( *nextbuf, (uint8_t*)&val, 8 );
	(*nextbuf) += 8;
}

void data_int64(uint8_t** nextbuf, int64_t val)
{
	val = htoj64(val);
	movebytes( *nextbuf, (uint8_t*)&val, 8 );
	(*nextbuf) += 8;
}

// TODO clean up
void data_string(uint8_t** nextbuf, char* str)
{
	uint16_t len = 0, len2 = 0;
	while (str[len++]);
	len--;
	len2 = len;
	movebytes( &((*nextbuf)[2]), (uint8_t*)str, len );
	len = htoj16(len);
	(*nextbuf)[0] = ((uint8_t*)&len)[0];
	(*nextbuf)[1] = ((uint8_t*)&len)[1];
	(*nextbuf) += 3 + len2;
}

void data_float(uint8_t** nextbuf, float val)
{
	val = htojf(val);
	movebytes( *nextbuf, (uint8_t*)&val, 4 );
	(*nextbuf) += 4;
}

void data_double(uint8_t** nextbuf, double val)
{
	val = htojd(val);
	movebytes( *nextbuf, (uint8_t*)&val, 8 );
	(*nextbuf) += 8;
}


static void _data_size(uint8_t* buf, uint16_t val)
{
	val = htoj16(val);
	movebytes(buf, (uint8_t*)&val, 2);
}

static void _data_string(uint8_t** buf, ujstring* str)
{
	_data_size(*buf, str->length);
	(*buf) += 2;
	movebytes(*buf, str->data, str->length);
	(*buf) += str->length;
}

static void _data(uint8_t** buf, ujvalue* v, int i)
{
	uint16_t n;
	uint8_t* sizefield;
	switch(v->type) {
		case uj_true:
			data_bool(buf, 1);
			break;
		case uj_false:
			data_bool(buf, 0);
			break;
		case uj_null:
			data_null(buf);
			break;
		case uj_number:
			switch(v->numbertype) {
				case uj_uint8:
					data_uint8(buf, v->data_as.uint8);
					break;
				case uj_int8:
					data_int8(buf, v->data_as.int8);
					break;
				case uj_uint16:
					data_uint16(buf, v->data_as.uint16);
					break;
				case uj_int16:
					data_int16(buf, v->data_as.int16);
					break;
				case uj_uint32:
					data_uint32(buf, v->data_as.uint32);
					break;
				case uj_int32:
					data_int32(buf, v->data_as.int32);
					break;
				case uj_uint64:
					data_uint64(buf, v->data_as.uint64);
					break;
				case uj_int64:
					data_int64(buf, v->data_as.int64);
					break;
				case uj_float:
					data_float(buf, v->data_as.f);
					break;
				case uj_double:
					data_double(buf, v->data_as.d);
					break;
			}
			break;
		case uj_string:
			data_string(buf, (char*)v->data_as.string->data);
			break;
		case uj_array:
			sizefield = *buf;
			(*buf) += 2;
			for (n = 0; n < array_length(v->data_as.array); n++) _data(buf, v->data_as.array->values[n], i+1);
			_data_size(sizefield, (uint16_t)((*buf) - sizefield - 2));
			break;
		case uj_object:
			sizefield = *buf;
			(*buf) += 2;
			for (n = 0; n < v->data_as.object->size; n++) {
				_data_string(buf, (ujstring*)v->data_as.object->data[n*2]);
				_data(buf, v->data_as.object->data[n*2+1], i+1);
			}
			_data_size(sizefield, (uint16_t)((*buf) - sizefield - 2));
			break;
	}
}

uint16_t data(uint8_t* buf, ujvalue* v)
{
	uint8_t* bufstart;
	bufstart = buf;
	_data(&buf, v, 0);
	return (uint16_t)(buf - bufstart);
}



