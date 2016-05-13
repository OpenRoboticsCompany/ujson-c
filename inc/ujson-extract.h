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
  * ujson-extract.h
  *
  */

#ifndef _STDINT_H
	#include <stdint.h>
#endif

void extract_bool_true(uint8_t** nextbuf, uint8_t* val);
void extract_bool_false(uint8_t** nextbuf, uint8_t* val);
void extract_null(uint8_t** nextbuf, uint8_t* val);

void extract_uint8(uint8_t** nextbuf, uint8_t* val);
void extract_int8(uint8_t** nextbuf, int8_t* val);
void extract_uint16(uint8_t** nextbuf, uint16_t* val);
void extract_int16(uint8_t** nextbuf, int16_t* val);
void extract_uint32(uint8_t** nextbuf, uint32_t* val);
void extract_int32(uint8_t** nextbuf, int32_t* val);
void extract_uint64(uint8_t** nextbuf, uint64_t* val);
void extract_int64(uint8_t** nextbuf, int64_t* val);

void extract_string(uint8_t** nextbuf, char* str);

void extract_float(uint8_t** nextbuf, float* val);
void extract_double(uint8_t** nextbuf, double* val);



