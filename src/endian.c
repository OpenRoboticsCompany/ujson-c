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
  * endian.c
  * Conversion and helper functions for juggling endianness.
  * Assumes gcc for some things. Note, floats on ARM are tricksy. RTFM.
  *
  */

#include "endian.h"

#ifdef __GNUC__
	#define _NOWARN_UNUSED __attribute__ ((unused))
#else
	#define _NOWARN_UNUSED
#endif

uint16_t _NOWARN_UNUSED _swap_16(uint16_t x)
{
	return
		( ((x) & 0xff00u) >> 8 \
		| ((x) & 0x00ffu) << 8 )
	;
}

uint32_t _NOWARN_UNUSED _swap_32(uint32_t x)
{
	return
		( ((x) & 0xff000000u) >> 24 \
		| ((x) & 0x00ff0000u) >> 8  \
		| ((x) & 0x0000ff00u) << 8  \
		| ((x) & 0x000000ffu) << 24 )
	;
}

uint64_t _NOWARN_UNUSED _swap_64(uint64_t x)
{
	return
		( ((x) & 0xff00000000000000ull) >> 56 \
		| ((x) & 0x00ff000000000000ull) >> 40 \
		| ((x) & 0x0000ff0000000000ull) >> 24 \
		| ((x) & 0x000000ff00000000ull) >> 8  \
		| ((x) & 0x00000000ff000000ull) << 8  \
		| ((x) & 0x0000000000ff0000ull) << 24 \
		| ((x) & 0x000000000000ff00ull) << 40 \
		| ((x) & 0x00000000000000ffull) << 56 )
	;
}

// This is for wacky ARM FPA doubles, which have both byte- and word-order considerations.
// Recommend RTFM if you are working with doubles on an FPA-flavoured ARM core.
// TODO: test this floating-point stuff thoroughly
uint64_t _NOWARN_UNUSED _swap_fpa_64(uint64_t x)
{
	return
		( ((x) & 0xffffffff00000000ull) >> 32 \
		| ((x) & 0x00000000ffffffffull) << 32 )
	;
}

// These are helpers to convince C to treat a series of bytes that were once a float or uint
// as the other type, while avoiding the conversion inherent in C's typecasting. This can be
// somewhat tricky at the machine code level as well due to separate integer and floating-point
// registers, possibly needing a trip in and out of memory, or some funky cpu instructions.

float _NOWARN_UNUSED _bytewise_uint32_to_float(uint32_t u)
{
	return *(float*)&u;
}

uint32_t _NOWARN_UNUSED _bytewise_float_to_uint32(float f)
{
	return *(uint32_t*)&f;
}

double _NOWARN_UNUSED _bytewise_uint64_to_double(uint64_t u)
{
	return *(double*)&u;
}

uint64_t _NOWARN_UNUSED _bytewise_double_to_uint64(double d)
{
	return *(uint64_t*)&d;
}

