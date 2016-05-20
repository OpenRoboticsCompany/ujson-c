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
  * str.c
  * Simple string handling
  *
  */

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "str.h"

#define UTF8_FINDLEN

str* str_allot(uint16_t len)
{
	str* s = (str*)calloc(1, (size_t)(sizeof(str) + len + 1));
	s->length = len;
	s->data = s->buffer;
	return s;
}

void str_release(str* s)
{
	if (s) free(s);
}

int str_eq(str* a, str* b)
{
	if (a->length != b->length) return 0;
	return !strncmp((char*)a->data, (char*)b->data, a->length);
}

uint16_t str_findlen(const uint8_t* s)
{
#ifdef UTF8_FINDLEN
	uint16_t i, j;
	while (s[i]) if ((s[i++] & 0xC0) != 0x80) j++;
	return j;
#else
	return (uint16_t) strlen(str);
#endif
}
