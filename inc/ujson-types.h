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

#endif

