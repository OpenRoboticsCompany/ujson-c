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
  * ujson-types.h
  *
  */

#ifndef _UJ_TYPES_H
#define _UJ_TYPES_H

#include <stdint.h>

#include "str.h"

enum ujtype {
	uj_string = 0,
	uj_number,
	uj_object,
	uj_array,
	uj_true,
	uj_false,
	uj_null
};

enum ujnumbertype {
	uj_uint8 = 0,
	uj_int8,
	uj_uint16,
	uj_int16,
	uj_uint32,
	uj_int32,
	uj_uint64,
	uj_int64,
	uj_float,
	uj_double
};

typedef struct ujvalue ujvalue;
typedef struct ujobject ujobject;
typedef struct ujarray ujarray;

// Tune for your application if needed. e.g.:
// - Make type and numbertype 4-bit-wide bitfields to shave a byte off
// - Remove packed attribute to get 64-bit alignment
struct __attribute__ ((__packed__)) ujvalue {
	union {
		ujobject* object;
		ujarray* array;
		str* string;
		uint8_t uint8;
		int8_t int8;
		uint16_t uint16;
		int16_t int16;
		uint32_t uint32;
		int32_t int32;
		uint64_t uint64;
		int64_t int64;
		float f;
		double d;
	} data_as;
	uint8_t type;
	uint8_t numbertype;
};

struct ujobject {
	int placeholder; // dict here
};

struct ujarray {
	int placeholder; // list here
};


#endif



