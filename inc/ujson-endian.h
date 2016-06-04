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
  * ujson-endian.h
  * Conversion macros and helper functions for juggling endianness
  * Assumes gcc for some things. Note, if you have to deal with actual
  * floats on ARM architectures, read the ARM docs and tread carefully.
  *
  */

/*
Functions below who's names begin with and underscore (_) should not be
used directly. Instead, use these htons-esque swap macros. Their naming
convention is:

	<func_name> ::= <SOURCE> to <DEST> <SIZE>
	<SOURCE>    ::= h | j
	<DEST>      ::= h | j
	<SIZE>      ::= 16 | 32 | 64 | f | d

Where:

	h  -> host-native endianness
	j  -> ujson-native endianness
	16 -> two-byte integer types
	32 -> four-byte integer types
	64 -> eight-byte integer types
	f  -> four-byte float type
	d  -> eight-byte double type
*/

#ifndef _UJ_ENDIAN_H
#define _UJ_ENDIAN_H

#include <stdint.h>

uint16_t _swap_16(uint16_t x);
uint32_t _swap_32(uint32_t x);
uint64_t _swap_64(uint64_t x);

uint64_t _swap_fpa_64(uint64_t x);

float _bytewise_uint32_to_float(uint32_t u);
uint32_t _bytewise_float_to_uint32(float f);
double _bytewise_uint64_to_double(uint64_t u);
uint64_t _bytewise_double_to_uint64(double d);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define htoj16(x) (_swap_16((x)))
	#define htoj32(x) (_swap_32((x)))
	#define htoj64(x) (_swap_64((x)))
	#define jtoh16(x) (_swap_16((x)))
	#define jtoh32(x) (_swap_32((x)))
	#define jtoh64(x) (_swap_64((x)))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define htoj16(x) ((x))
	#define htoj32(x) ((x))
	#define htoj64(x) ((x))
	#define jtoh16(x) ((x))
	#define jtoh32(x) ((x))
	#define jtoh64(x) ((x))
#else
	#error "Macro __BYTE_ORDER__ has unexpected value or is undefined"
#endif

#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define _swap_word_endians(x) ((x))
#elif __FLOAT_WORD_ORDER == __ORDER_BIG_ENDIAN__
	#define _swap_word_endians(x) (_swap_fpa_64((x)))
#elif !defined(__FLOAT_WORD_ORDER)
	// Assume not hybrid
	#warning "Macro __FLOAT_WORD_ORDER__ not defined, assuming non-hybrid"
	#define _swap_word_endians(x) ((x))
#endif

// Only big-endian word order on little-endian byte order ARM FPA needs special handling
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define htojf(x) (_bytewise_uint32_to_float(_swap_32(_bytewise_float_to_uint32((x)))))
	#define htojd(x) (_bytewise_uint64_to_double(_swap_64(_swap_word_endians(_bytewise_double_to_uint64((x))))))
	#define jtohf(x) (_bytewise_uint32_to_float(_swap_32(_bytewise_float_to_uint32((x)))))
	#define jtohd(x) (_bytewise_uint64_to_double(_swap_word_endians(_swap_64(_bytewise_double_to_uint64((x))))))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define htojf(x) ((x))
	#define htojd(x) ((x))
	#define jtohf(x) ((x))
	#define jtohd(x) ((x))
#else
	#error "Macro __BYTE_ORDER__ has unexpected value or is undefined"
#endif

#endif
