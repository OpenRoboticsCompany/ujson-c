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
  * ujson-tojson.c
  * Tranforms a ujson object (ujvalue tree) to json text
  * Requires a buffer of sufficient size to hold the json
  *
  */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
// math.h for float rendering
#include <math.h>

#include "ujson-value.h"
#include "ujson-tojson.h"

static void _unicode(char** buf, uint8_t* ujstringdata, uint16_t* n)
{
	uint16_t a, b, c, d;
	uint32_t e;
	if ((ujstringdata[*n] & 0xE0) == 0xC0) {
		a = (ujstringdata[(*n)++] & 0x1F);
		b = (ujstringdata[(*n)++] & 0x3F) | ((a & 0x03) << 6);
		a = a >> 2;
		(*buf) += sprintf(*buf, "\\u%2X%2X", a, b);
	} else if ((ujstringdata[*n] & 0xF0) == 0xE0) {
		a = (ujstringdata[(*n)++] & 0x0F);
		b = (ujstringdata[(*n)++] & 0x3F);
		c = (ujstringdata[(*n)++] & 0x3F);
		a = (a << 4) | ((b & 0x3C) >> 2);
		b = ((b & 0x03) << 6) | (c & 0x3F);
		(*buf) += sprintf(*buf, "\\u%02X%02X", a, b);
	} else if ((ujstringdata[*n] & 0xF8) == 0xF0) {
		// oh snap! it's utf16 surrogate time!
		a = (ujstringdata[(*n)++] & 0x07);
		b = (ujstringdata[(*n)++] & 0x3F);
		c = (ujstringdata[(*n)++] & 0x3F);
		d = (ujstringdata[(*n)++] & 0x3F);
		e = ((a & 0x07) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (d & 0x3F);
		e -= 0x010000;
		a = (e & 0x000FFC00) >> 10;
		b = (e & 0x000003FF);
		a += 0xD800;
		b += 0xDC00;
		(*buf) += sprintf(*buf, "\\u%04X\\u%04X", a, b);
	}
}

static void _escaped_strcpy(char** buf, uint8_t* ujstringdata)
{
	uint16_t n = 0;
	if (ujstringdata[0]) *((*buf)++) = '"';
	while (ujstringdata[n]) {
		switch (ujstringdata[n]) {
			case '"':
			case '\\':
			case '\x00': case '\x01': case '\x02': case '\x03':
			case '\x04': case '\x05': case '\x06': case '\x07': 
			                                       case '\x0B':
			                          case '\x0E': case '\x0F': 
			case '\x10': case '\x11': case '\x12': case '\x13':
			case '\x14': case '\x15': case '\x16': case '\x17': 
			case '\x18': case '\x19': case '\x1A': case '\x1B':
			case '\x1C': case '\x1D': case '\x1E': case '\x1F': 
				*((*buf)++) = '\\';
				*((*buf)++) = (char)ujstringdata[n++];
				continue;
			case '\b':
				*((*buf)++) = '\\';
				*((*buf)++) = 'b';
				n++;
				continue;
			case '\f':
				*((*buf)++) = '\\';
				*((*buf)++) = 'f';
				n++;
				continue;
			case '\n':
				*((*buf)++) = '\\';
				*((*buf)++) = 'n';
				n++;
				continue;
			case '\r':
				*((*buf)++) = '\\';
				*((*buf)++) = 'r';
				n++;
				continue;
			case '\t':
				*((*buf)++) = '\\';
				*((*buf)++) = 't';
				n++;
				continue;
			default:
				if (ujstringdata[n] > 0x7F) {
					_unicode(buf, ujstringdata, &n);
				} else {
					*((*buf)++) = (char)ujstringdata[n++];
				}
				continue;
		}
	}
	if (ujstringdata[0]) *((*buf)++) = '"';
}

static void _tojson(char** buf, ujvalue* v, int i, uint8_t opts)
{
	uint16_t n;
	if (v == NULL) {
		memcpy(*buf, "null", 4);
		(*buf) += 4;
		return;
	}
	switch(v->type) {
		case uj_true:
			memcpy(*buf, "true", 4);
			(*buf) += 4;
			break;
		case uj_false:
			memcpy(*buf, "false", 5);
			(*buf) += 5;
			break;
		case uj_null:
			memcpy(*buf, "null", 4);
			(*buf) += 4;
			break;
		case uj_number:
			switch(v->numbertype) {
				case uj_uint8:
					(*buf) += sprintf(*buf, "%u", v->data_as.uint8);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/uint8", 6);
						(*buf) += 6;
					}
					break;
				case uj_int8:
					(*buf) += sprintf(*buf, "%d", v->data_as.int8);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/int8", 5);
						(*buf) += 5;
					}
					break;
				case uj_uint16:
					(*buf) += sprintf(*buf, "%u", v->data_as.uint16);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/uint16", 7);
						(*buf) += 7;
					}
					break;
				case uj_int16:
					(*buf) += sprintf(*buf, "%d", v->data_as.int16);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/int16", 6);
						(*buf) += 6;
					}
					break;
				case uj_uint32:
					(*buf) += sprintf(*buf, "%u", v->data_as.uint32);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/uint32", 7);
						(*buf) += 7;
					}
					break;
				case uj_int32:
					(*buf) += sprintf(*buf, "%d", v->data_as.int32);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/int32", 6);
						(*buf) += 6;
					}
					break;
				case uj_uint64:
					(*buf) += sprintf(*buf, "%llu", v->data_as.uint64);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/uint64", 7);
						(*buf) += 7;
					}
					break;
				case uj_int64:
					(*buf) += sprintf(*buf, "%lld", v->data_as.int64);
					if (opts & uj_showtypes) {
						memcpy(*buf, "/int64", 6);
						(*buf) += 7;
					}
					break;
				case uj_float:
					if (isfinite(v->data_as.f)) {
						(*buf) += sprintf(*buf, "%.9f", v->data_as.f);
					} else if (isnan(v->data_as.f)) {
						memcpy(*buf, "NaN", 3);
						(*buf) += 3;
					} else if (isinf(v->data_as.f) == -1) {
						memcpy(*buf, "-Infinity", 9);
						(*buf) += 9;
					} else if (isinf(v->data_as.f) == 1) {
						memcpy(*buf, "Infinity", 8);
						(*buf) += 8;
					}
					if (opts & uj_showtypes) {
						memcpy(*buf, "/float", 6);
						(*buf) += 6;
					}
					break;
				case uj_double:
					if (isfinite(v->data_as.d)) {
						(*buf) += sprintf(*buf, "%.17f", v->data_as.d);
					} else if (isnan(v->data_as.d)) {
						memcpy(*buf, "NaN", 3);
						(*buf) += 3;
					} else if (isinf(v->data_as.d) == -1) {
						memcpy(*buf, "-Infinity", 9);
						(*buf) += 9;
					} else if (isinf(v->data_as.d) == 1) {
						memcpy(*buf, "Infinity", 8);
						(*buf) += 8;
					}
					if (opts & uj_showtypes) {
						memcpy(*buf, "/double", 7);
						(*buf) += 7;
					}
					break;
			}
			break;
		case uj_string:
			_escaped_strcpy(buf, v->data_as.string->data);
			break;
		case uj_array:
			*((*buf)++) = '[';
			for (n = 0; n < array_length(v->data_as.array); n++) {
				_tojson(buf, v->data_as.array->values[n], i+1, opts);
				if (n < array_length(v->data_as.array) - 1) *((*buf)++) = ',';
			}
			*((*buf)++) = ']';
			break;
		case uj_object:
			*((*buf)++) = '{';
			for (n = 0; n < v->data_as.object->size; n++) {
				_escaped_strcpy(buf, ((ujstring*)v->data_as.object->data[n*2])->data);
				*((*buf)++) = ':';
				_tojson(buf, (ujvalue*)v->data_as.object->data[n*2+1], i+1, opts);
				if (n < v->data_as.object->size - 1) *((*buf)++) = ',';
			}
			*((*buf)++) = '}';
			break;
	}
	if (i == 0) *((*buf)++) = '\0';
}

void tojson(char* buf, ujvalue* v)
{
	_tojson(&buf, v, 0, uj_plainjson);
}

void tojson_with_types(char* buf, ujvalue* v)
{
	_tojson(&buf, v, 0, uj_showtypes);
}
