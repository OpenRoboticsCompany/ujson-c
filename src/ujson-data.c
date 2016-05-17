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

#include "endian.h"
#include "movebytes.h"
#include "ujson-data.h"

// you'll probably never use these bool_foo; included for structural symmetry
// perhaps to be removed later.
void data_bool_true(uint8_t** nextbuf, uint8_t val)
{
	**nextbuf = (uint8_t)'t';
	(*nextbuf) += 1;
}

void data_bool_false(uint8_t** nextbuf, uint8_t val)
{
	**nextbuf = (uint8_t)'f';
	(*nextbuf) += 1;
}

void data_bool(uint8_t** nextbuf, uint8_t val)
{
	**nextbuf = (uint8_t)(val ? 't' : 'f');
	(*nextbuf) += 1;
}

void data_null(uint8_t** nextbuf, uint8_t val)
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

/*
void data(uint8_t* destbuf, uint16_t destsize, uint8_t** srcbuf, uint16_t srclen)
{
	//TODO

	uint8_t derp;

	switch(derp) {
		case UJ_BOOL_TRUE_TAG:
		case UJ_BOOL_FALSE_TAG:
		case UJ_NULL_TAG:
		case UJ_UINT8_TAG:
		case UJ_INT8_TAG:
		case UJ_UINT16_TAG:
		case UJ_INT16_TAG:
		case UJ_UINT32_TAG:
		case UJ_INT32_TAG:
		case UJ_UINT64_TAG:
		case UJ_INT64_TAG:
		case UJ_STRING_TAG:
		case UJ_FLOAT_TAG:
		case UJ_DOUBLE_TAG:
		case UJ_ARRAY_TAG:
		case UJ_OBJECT_TAG:
		default:
	}

}
*/





