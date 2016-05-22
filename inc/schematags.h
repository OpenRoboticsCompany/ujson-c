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
  * schematags.h
  * Defines for the various schema data type tags used in encoded ujson
  * and ujson schemas
  *
  */

#ifndef _UJSON_SCHEMATAGS_H
#define _UJSON_SCHEMATAGS_H

enum schematags {
	uj_bool_true_tag  = 't',
	uj_bool_false_tag = 'f',
	uj_null_tag       = 'n',
	uj_uint8_tag      = 'C',
	uj_int8_tag       = 'c',
	uj_uint16_tag     = 'W',
	uj_int16_tag      = 'w',
	uj_uint32_tag     = 'I',
	uj_int32_tag      = 'i',
	uj_uint64_tag     = 'Q',
	uj_int64_tag      = 'q',
	uj_string_tag     = 's',
	uj_float_tag      = 'd',
	uj_double_tag     = 'D',
	uj_array_tag      = 'a',
	uj_object_tag     = 'o'
};

typedef enum schematags schematag;

#endif
