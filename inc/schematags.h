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
  * Defines for the various schema data type tags used in rendered ujson
  * and ujson schemas
  *
  */

#ifndef _UJSON_SCHEMATAGS_H
	#define _UJSON_SCHEMATAGS_H 1
#endif

#ifndef _STDINT_H
	#include <stdint.h>
#endif

#define UJ_BOOL_TRUE_TAG  ((uint8_t)'t')
#define UJ_BOOL_FALSE_TAG ((uint8_t)'f')
#define UJ_NULL_TAG       ((uint8_t)'n')
#define UJ_UINT8_TAG      ((uint8_t)'C')
#define UJ_INT8_TAG       ((uint8_t)'c')
#define UJ_UINT16_TAG     ((uint8_t)'W')
#define UJ_INT16_TAG      ((uint8_t)'w')
#define UJ_UINT32_TAG     ((uint8_t)'I')
#define UJ_INT32_TAG      ((uint8_t)'i')
#define UJ_UINT64_TAG     ((uint8_t)'Q')
#define UJ_INT64_TAG      ((uint8_t)'q')
#define UJ_STRING_TAG     ((uint8_t)'s')
#define UJ_FLOAT_TAG      ((uint8_t)'d')
#define UJ_DOUBLE_TAG     ((uint8_t)'D')
#define UJ_ARRAY_TAG      ((uint8_t)'a')
#define UJ_OBJECT_TAG     ((uint8_t)'o')


