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
  * ujson-parse.c
  * Parsing functions - de-serialize elements from ujson format
  *
  */

#include "endian.h"
#include "ujson-parse.h"
#include "ujson-extract.h"
#include "schematags.h"
#include "ujsizes.h"

static void movebytes(uint8_t* to, uint8_t* from, uint16_t n)
{
	while ( n-- ) to[n] = from[n];
}

void parse(uint8_t** destbuf, uint16_t destsize, uint8_t** srcbuf, uint16_t srclen)
{
	uint8_t* srcbuf_start = *srcbuf;

	while (*srcbuf - (*srcbuf_start + len)) {
		// not at end yet

		switch(derp) {
			//case UJ_BOOL_TRUE_TAG:
			//case UJ_BOOL_FALSE_TAG:
			//case UJ_NULL_TAG:
			//case UJ_UINT8_TAG:
			//case UJ_INT8_TAG:
			//case UJ_UINT16_TAG:
			//case UJ_INT16_TAG:
			//case UJ_UINT32_TAG:
			//case UJ_INT32_TAG:
			//case UJ_UINT64_TAG:
			//case UJ_INT64_TAG:
			//case UJ_STRING_TAG:
			//case UJ_FLOAT_TAG:
			//case UJ_DOUBLE_TAG:
			//case UJ_ARRAY_TAG: //parse can call itself here, srcbuf** will step along. just pass new srclen
			//case UJ_OBJECT_TAG:
			default:
		}
	}

}






