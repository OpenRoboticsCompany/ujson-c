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
  * ujson-parse.c
  * Parsing functions - de-serialize elements from ujson format
  * This is appropriate for parsing dynamic objects of unknown size or form,
  * and uses malloc. For a very small embedded system, it's better to use a
  * flat decoder and a fixed format.
  *
  */

#include "endian.h"
#include "movebytes.h"
#include "ujsizes.h"
#include "schematags.h"
#include "ujson-extract.h"
#include "ujson-string.h"
#include "ujson-array.h"
#include "ujson-object.h"
#include "ujson-value.h"
#include "ujson-parse.h"

static void _consume_tag(uint8_t** b)
{
	uint16_t l;
	uint8_t tag = *((*b)++);
	switch(tag) {
		case uj_bool_true_tag:
		case uj_bool_false_tag:
		case uj_null_tag:
			break;
		case uj_uint8_tag:
		case uj_int8_tag:
			*b += 1;
			break;
		case uj_uint16_tag:
		case uj_int16_tag:
			*b += 2;
			break;
		case uj_uint32_tag:
		case uj_int32_tag:
		case uj_float_tag:
			*b += 4;
			break;
		case uj_uint64_tag:
		case uj_int64_tag:
		case uj_double_tag:
			*b += 8;
			break;
		case uj_string_tag:
		case uj_array_tag:
		case uj_object_tag:
			extract_uint16(b, &l);
			*b += l;
			break;
		default:
			break;
	}
}

uint16_t parse_arraylen(uint8_t** buf)
{
	// buf should have the 'a' schematag trimmed
	uint8_t* b = *buf;
	uint16_t size = 0, n = 0;
	extract_uint16(&b, &size);
	while (b < *buf + size + 2) {
		n++;
		_consume_tag(&b);
	}
	return n;
}


uint16_t parse_objectlen(uint8_t** buf)
{
	// buf should have the 'o' schematag trimmed
	uint8_t* b = *buf;
	uint16_t size = 0, n = 0, kl = 0;
	extract_uint16(&b, &size);
	while (b < *buf + size + 2) {
		switch (n++ % 2) {
			case 0:
				extract_uint16(&b, &kl);
				b += kl;
				break;
			case 1:
				_consume_tag(&b);
				break;
		}
	}
	return n >> 1;
}


ujvalue* parse(uint8_t** buf)
{
	uint8_t t;
	uint16_t l, m;
	ujstring* k;
	ujvalue* v;
	v = ujvalue_new();
	t = *((*buf)++);
	switch(t) {
		case uj_bool_true_tag:
			v->type = uj_true;
			break;
		case uj_bool_false_tag:
			v->type = uj_false;
			break;
		case uj_null_tag:
			v->type = uj_null;
			break;
		case uj_uint8_tag:
			v->type = uj_number;
			v->numbertype = uj_uint8;
			extract_uint8(buf, &v->data_as.uint8);
			break;
		case uj_int8_tag:
			v->type = uj_number;
			v->numbertype = uj_int8;
			extract_int8(buf, &v->data_as.int8);
			break;
		case uj_uint16_tag:
			v->type = uj_number;
			v->numbertype = uj_uint16;
			extract_uint16(buf, &v->data_as.uint16);
			break;
		case uj_int16_tag:
			v->type = uj_number;
			v->numbertype = uj_int16;
			extract_int16(buf, &v->data_as.int16);
			break;
		case uj_uint32_tag:
			v->type = uj_number;
			v->numbertype = uj_uint32;
			extract_uint32(buf, &v->data_as.uint32);
			break;
		case uj_int32_tag:
			v->type = uj_number;
			v->numbertype = uj_int32;
			extract_int32(buf, &v->data_as.int32);
			break;
		case uj_uint64_tag:
			v->type = uj_number;
			v->numbertype = uj_uint64;
			extract_uint64(buf, &v->data_as.uint64);
			break;
		case uj_int64_tag:
			v->type = uj_number;
			v->numbertype = uj_int64;
			extract_int64(buf, &v->data_as.int64);
			break;
		case uj_string_tag:
			v->type = uj_string;
			extract_uint16(buf, &l);
			(*buf) -= 2;
			v->data_as.string = string_allot(l);
			v->data_as.string->length = l;
			extract_string(buf, v->data_as.string->data);
			break;
		case uj_float_tag:
			v->type = uj_number;
			v->numbertype = uj_float;
			extract_float(buf, &v->data_as.f);
			break;
		case uj_double_tag:
			v->type = uj_number;
			v->numbertype = uj_double;
			extract_double(buf, &v->data_as.d);
			break;
		case uj_array_tag:
			v->type = uj_array;
			l = parse_arraylen(buf);
			(*buf) += 2;
			v->data_as.array = array_allot(l);
			while (l--) array_push(v->data_as.array, parse(buf));
			break;
		case uj_object_tag:
			v->type = uj_object;
			l = parse_objectlen(buf);
			(*buf) += 2;
			v->data_as.object = object_allot(l);
			while (l--) {
				extract_uint16(buf, &m);
				(*buf) -= 2;
				k = string_allot(m);
				extract_string(buf, k->data);
				object_set(v->data_as.object, k, parse(buf));
			}
			break;
		default:
			break;
	}
	return v;
}






