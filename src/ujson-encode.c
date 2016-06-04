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
  * ujson-encode.c
  * Encode functions - serialize elements into ujson format
  *
  */

#include "ujson-endian.h"
#include "movebytes.h"
#include "ujson-encode.h"

void encode_bool(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = val ? (uint8_t)'t' : (uint8_t)'f';
	(*nextbuf) += 1;
}

void encode_null(uint8_t** nextbuf)
{
	(*nextbuf)[0] = (uint8_t)'n';
	(*nextbuf) += 1;
}

void encode_uint8(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = 'C';
	(*nextbuf)[1] = val;
	(*nextbuf) += 2;
}

void encode_int8(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = 'c';
	(*nextbuf)[1] = val;
	(*nextbuf) += 2;
}

void encode_uint16(uint8_t** nextbuf, uint16_t val)
{
	val = htoj16(val);
	(*nextbuf)[0] = 'W';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 2 );
	(*nextbuf) += 3;
}

void encode_int16(uint8_t** nextbuf, int16_t val)
{
	val = htoj16(val);
	(*nextbuf)[0] = 'w';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 2 );
	(*nextbuf) += 3;
}

void encode_uint32(uint8_t** nextbuf, uint32_t val)
{
	val = htoj32(val);
	(*nextbuf)[0] = 'I';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 4 );
	(*nextbuf) += 5;
}

void encode_int32(uint8_t** nextbuf, int32_t val)
{
	val = htoj32(val);
	(*nextbuf)[0] = 'i';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 4 );
	(*nextbuf) += 5;
}

void encode_uint64(uint8_t** nextbuf, uint64_t val)
{
	val = htoj64(val);
	(*nextbuf)[0] = 'Q';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 8 );
	(*nextbuf) += 9;
}

void encode_int64(uint8_t** nextbuf, int64_t val)
{
	val = htoj64(val);
	(*nextbuf)[0] = 'q';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 8 );
	(*nextbuf) += 9;
}

void encode_string(uint8_t** nextbuf, char* str)
{
	uint16_t len = 0, len2 = 0;
	while (str[len++]);
	len--;
	len2 = len;
	(*nextbuf)[0] = 's';
	movebytes( &((*nextbuf)[3]), (uint8_t*)str, len );
	len = htoj16(len);
	(*nextbuf)[1] = ((uint8_t*)&len)[0];
	(*nextbuf)[2] = ((uint8_t*)&len)[1];
	(*nextbuf) += 3 + len2;
}

void encode_float(uint8_t** nextbuf, float val)
{
	val = htojf(val);
	(*nextbuf)[0] = 'd';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 4 );
	(*nextbuf) += 5;
}

void encode_double(uint8_t** nextbuf, double val)
{
	val = htojd(val);
	(*nextbuf)[0] = 'D';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 8 );
	(*nextbuf) += 9;
}

// _encode_size and _encode_string could be used from the data_* funcs, but
// having them here avoids the need to bring in that whole file which you
// probably aren't using (either inlining schemas, or not)

static void _encode_size(uint8_t* buf, uint16_t val)
{
	val = htoj16(val);
	movebytes(buf, (uint8_t*)&val, 2);
}

static void _encode_string(uint8_t** buf, ujstring* str)
{
	_encode_size(*buf, str->length);
	(*buf) += 2;
	movebytes(*buf, str->data, str->length);
	(*buf) += str->length;
}

static void _encode(uint8_t** buf, ujvalue* v, int i)
{
	uint16_t n;
	uint8_t* sizefield;
	switch(v->type) {
		case uj_true:
			encode_bool(buf, 1);
			break;
		case uj_false:
			encode_bool(buf, 0);
			break;
		case uj_null:
			encode_null(buf);
			break;
		case uj_number:
			switch(v->numbertype) {
				case uj_uint8:
					encode_uint8(buf, v->data_as.uint8);
					break;
				case uj_int8:
					encode_int8(buf, v->data_as.int8);
					break;
				case uj_uint16:
					encode_uint16(buf, v->data_as.uint16);
					break;
				case uj_int16:
					encode_int16(buf, v->data_as.int16);
					break;
				case uj_uint32:
					encode_uint32(buf, v->data_as.uint32);
					break;
				case uj_int32:
					encode_int32(buf, v->data_as.int32);
					break;
				case uj_uint64:
					encode_uint64(buf, v->data_as.uint64);
					break;
				case uj_int64:
					encode_int64(buf, v->data_as.int64);
					break;
				case uj_float:
					encode_float(buf, v->data_as.f);
					break;
				case uj_double:
					encode_double(buf, v->data_as.d);
					break;
			}
			break;
		case uj_string:
			encode_string(buf, (char*)v->data_as.string->data);
			break;
		case uj_array:
			*((*buf)++) = 'a';
			sizefield = *buf;
			(*buf) += 2;
			for (n = 0; n < array_length(v->data_as.array); n++) _encode(buf, v->data_as.array->values[n], i+1);
			_encode_size(sizefield, (uint16_t)((*buf) - sizefield - 2));
			break;
		case uj_object:
			*((*buf)++) = 'o';
			sizefield = *buf;
			(*buf) += 2;
			for (n = 0; n < v->data_as.object->size; n++) {
				_encode_string(buf, (ujstring*)v->data_as.object->data[n*2]);
				_encode(buf, v->data_as.object->data[n*2+1], i+1);
			}
			_encode_size(sizefield, (uint16_t)((*buf) - sizefield - 2));
			break;
	}
}

uint16_t encode(uint8_t* buf, ujvalue* v)
{
	uint8_t* bufstart;
	bufstart = buf;
	_encode(&buf, v, 0);
	return (uint16_t)(buf - bufstart);
}
