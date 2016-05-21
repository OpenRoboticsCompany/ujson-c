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
  * ujsizes.h
  * Sizes for each of the types in a ujson schema
  *
  */

#ifndef _UJ_SIZES_H
#define _UJ_SIZES_H

#include <stdint.h>

#define UJ_BOOL_TRUE_SIZE  (1)
#define UJ_BOOL_FALSE_SIZE (1)
#define UJ_NULL_SIZE       (1)
#define UJ_UINT8_SIZE      (1)
#define UJ_INT8_SIZE       (1)
#define UJ_UINT16_SIZE     (2)
#define UJ_INT16_SIZE      (2)
#define UJ_UINT32_SIZE     (4)
#define UJ_INT32_SIZE      (4)
#define UJ_UINT64_SIZE     (8)
#define UJ_INT64_SIZE      (8)
#define UJ_STRING_MAX_SIZE (65535)
#define UJ_FLOAT_SIZE      (4)
#define UJ_DOUBLE_SIZE     (8)
#define UJ_ARRAY_MAX_SIZE  (65535)
#define UJ_OBJECT_MAX_SIZE (65535)

#endif
