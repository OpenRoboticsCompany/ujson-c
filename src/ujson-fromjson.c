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
  * ujson-fromjson.c
  * Parses JSON into a ujvalue
  *
  */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ujson-value.h"
#include "ujson-types.h"
#include "ujson-fromjson.h"

#define UJ_FROMJSON_MAXSTRINGSIZE 1024
#define UJ_FROMJSON_NUMBUFSIZE 25

FILE* f;

static int next()
{
	int c;
	do c = getc(f); while (c == ' ' || c == '\n' || c == '\r' || c == '\t');
	return c;
}

static int skip(uint8_t n)
{
	int c = 0;
	while (n-- && c != EOF) c = next();
	return c;
}

static void skipnumber()
{
	int c = 0;
	while (1) {
		switch (c = next()) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'X':
			case 'a': case 'b': case 'c': case 'd':           case 'f': case 'x':
			case '.': case '-': case '+': case 'e':
				break;
			case EOF:
				return;
			default:
				ungetc(c,f);
				return;
		}
	}
}

// This will also accept C-style hex and octal integers in addition to legal JSON integers
static ujvalue* readnumber()
{
	ujvalue* v = ujvalue_new();
	char numbuf[UJ_FROMJSON_NUMBUFSIZE + 1] = {0};
	char* tail = numbuf;
	int c;
	uint8_t isfloat = 0;
	uint8_t issigned = 0;
	uint8_t i = 0;

	double d64;
	uint64_t u64;
	int64_t i64;

	while (i < UJ_FROMJSON_NUMBUFSIZE) {
		switch (c = next()) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'X':
			case 'a': case 'b': case 'c': case 'd':           case 'f': case 'x':
			case '+': case 'e':
				numbuf[i++] = c;
				break;
			case '-':
				issigned = 1;
				numbuf[i++] = c;
				break;
			case '.':
				isfloat = 1;
				numbuf[i++] = c;
				break;
			case EOF:
			default:
				if (i==0) goto LaFinDuMonde;
				if (c != EOF && ungetc(c,f) == EOF) goto LaFinDuMonde;
				i = UJ_FROMJSON_NUMBUFSIZE;
				break;
		}
	}
	// consume rest of too-long number if present
	skipnumber();

	v->type = uj_number;

	if (isfloat) {
		d64 = strtod(numbuf, &tail);
		if (tail==numbuf) goto LaFinDuMonde;
		if (d64 > 1.0e8 || d64 < -1.0e8) {
			v->numbertype = uj_double;
			v->data_as.d = d64;
		} else {
			v->numbertype = uj_float;
			v->data_as.f = (float)d64;
		}
	} else if(issigned) {
		i64 = strtoll(numbuf, &tail, 0);
		if (tail==numbuf) goto LaFinDuMonde;
		if (i64 >= -128 && i64 <= 127) {
			v->numbertype = uj_int8;
			v->data_as.int8 = (int8_t)i64;
		} else if (i64 >= -32768 && i64 <= 32767) {
			v->numbertype = uj_int16;
			v->data_as.int16 = (int16_t)i64;
		} else if (i64 >= -2147483648 && i64 <= 2147483647) {
			v->numbertype = uj_int32;
			v->data_as.int32 = (int32_t)i64;
		} else {
			v->numbertype = uj_int64;
			v->data_as.int64 = i64;
		}
	} else {
		u64 = strtoull(numbuf, &tail, 0);
		if (tail==numbuf) goto LaFinDuMonde;
		if (u64 <= 0xFF) {
			v->numbertype = uj_uint8;
			v->data_as.uint8 = (uint8_t)u64;
		} else if (u64 <= 0xFFFF) {
			v->numbertype = uj_uint16;
			v->data_as.uint16 = (uint16_t)u64;
		} else if (u64 <= 0xFFFFFFFF) {
			v->numbertype = uj_uint32;
			v->data_as.uint32 = (uint32_t)u64;
		} else {
			v->numbertype = uj_uint64;
			v->data_as.uint64 = u64;
		}
	}

	return v;

LaFinDuMonde:
	ujvalue_release(&v);
	return NULL;
}

static ujvalue* readstring()
{
	ujvalue* v = ujvalue_new();
	char strbuf[UJ_FROMJSON_MAXSTRINGSIZE + 1] = {0};

	// TODO here next

	return v;

LaFinDuMonde:
	ujvalue_release(&v);
	return NULL;
}

static ujvalue* readvalue()
{
	ujvalue* v;
	int c;
	switch (c = next()) {
		case EOF:
			return NULL;
			break;
		case '{':
			break;
		case '[':
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '-':
			if (ungetc(c, f)==EOF) return NULL;
			v = readnumber();
			break;
		case '"':
			if (ungetc(c, f)==EOF) return NULL;
			v = readstring();
			break;
		case 't':
			if (skip(3)==EOF) return NULL;
			v = ujvalue_new();
			v->type = uj_true;
			break;
		case 'f':
			if (skip(4)==EOF) return NULL;
			v = ujvalue_new();
			v->type = uj_false;
			break;
		case 'n':
			if (skip(3)==EOF) return NULL;
			v = ujvalue_new();
			break;
	}
	return v;
}

ujvalue* fromjson(FILE* fp)
{
	f = fp;
	return readvalue();
}
