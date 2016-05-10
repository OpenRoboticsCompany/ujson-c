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
  * ujson.c
  * Rendering functions, will likely split out to different files
  *
  */

#include <stdio.h>
#include <stdint.h>
#include "hexdump.h"

#include "endian.h"
#include "ujson.h"

void movebytes(uint8_t* to, uint8_t* from, uint16_t n)
{
	while ( n-- )
		to[n] = from[n];
}

void render_uint8(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = 'C';
	(*nextbuf)[1] = val;
	(*nextbuf) += 2;
}

void render_int8(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = 'c';
	(*nextbuf)[1] = val;
	(*nextbuf) += 2;
}

void render_uint16(uint8_t** nextbuf, uint16_t val)
{
	val = htoj16(val);
	(*nextbuf)[0] = 'W';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 2 );
	(*nextbuf) += 3;
}

void render_int16(uint8_t** nextbuf, int16_t val)
{
	val = htoj16(val);
	(*nextbuf)[0] = 'w';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 2 );
	(*nextbuf) += 3;
}

void render_uint32(uint8_t** nextbuf, uint32_t val)
{
	(*nextbuf)[0] = 'I';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 4 );
	(*nextbuf) += 5;
}

void render_int32(uint8_t** nextbuf, int32_t val)
{
	val = htoj32(val);
	(*nextbuf)[0] = 'i';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 4 );
	(*nextbuf) += 5;
}

void render_uint64(uint8_t** nextbuf, uint64_t val)
{
	val = htoj64(val);
	(*nextbuf)[0] = 'Q';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 8 );
	(*nextbuf) += 9;
}

void render_int64(uint8_t** nextbuf, int64_t val)
{
	val = htoj64(val);
	(*nextbuf)[0] = 'q';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 8 );
	(*nextbuf) += 9;
}

void render_string(uint8_t** nextbuf, char* str)
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

void render_float(uint8_t** nextbuf, float val)
{
	val = htojf(val);
	(*nextbuf)[0] = 'd';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 4 );
	(*nextbuf) += 5;
}

void render_double(uint8_t** nextbuf, double val)
{
	val = htojd(val);
	(*nextbuf)[0] = 'D';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 8 );
	(*nextbuf) += 9;
}


