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
#include "ujson-values.h"
#include "str.h"
#include "ujson-parse.h"


uint16_t parse_arraylen(uint8_t** buf)
{
	// buf should have the 'a' schematag trimmed
	uint8_t* b;
	b = *buf;
	uint16_t size = 0, l = 0;
	extract_uint16(&b, &size);
	uint16_t n = 0;
	schematag t;
	while (b < *buf + size) {
		t = (schematag)*b;
		b++;
		n++;
		switch(t) {
			case uj_bool_true_tag:
			case uj_bool_false_tag:
			case uj_null_tag:
				break;
			case uj_uint8_tag:
			case uj_int8_tag:
				b += 1;
				break;
			case uj_uint16_tag:
			case uj_int16_tag:
				b += 2;
				break;
			case uj_uint32_tag:
			case uj_int32_tag:
			case uj_float_tag:
				b += 4;
				break;
			case uj_uint64_tag:
			case uj_int64_tag:
			case uj_double_tag:
				b += 8;
				break;
			case uj_string_tag:
			case uj_array_tag:
			case uj_object_tag:
				extract_uint16(&b, &l);
				b += l;
				break;
			default:
				break;
		}
	}
	return n;
}

ujvalue* parse(uint8_t** buf, uint16_t len)
{
	ujvalue* v;
	schematag t;
	uint8_t* start;
	start = *buf;
	uint16_t sl;

	while (*buf < start + len) {
		t = (schematag)**buf;
		(*buf)++;
		switch(t) {
			case uj_bool_true_tag:
				v = ujvalue_new();
				v->type = uj_true;
				break;
			case uj_bool_false_tag:
				v = ujvalue_new();
				v->type = uj_false;
				break;
			case uj_null_tag:
				v = ujvalue_new();
				v->type = uj_null;
				break;
			case uj_uint8_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_uint8;
				extract_uint8(buf, &v->data_as.uint8);
				break;
			case uj_int8_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_int8;
				extract_int8(buf, &v->data_as.int8);
				break;
			case uj_uint16_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_uint16;
				extract_uint16(buf, &v->data_as.uint16);
				break;
			case uj_int16_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_int16;
				extract_int16(buf, &v->data_as.int16);
				break;
			case uj_uint32_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_uint32;
				extract_uint32(buf, &v->data_as.uint32);
				break;
			case uj_int32_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_int32;
				extract_int32(buf, &v->data_as.int32);
				break;
			case uj_uint64_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_uint64;
				extract_uint64(buf, &v->data_as.uint64);
				break;
			case uj_int64_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_int64;
				extract_int64(buf, &v->data_as.int64);
				break;
			case uj_string_tag:
				v = ujvalue_new();
				v->type = uj_string;
				extract_uint16(buf, &sl);
				v->data_as.string = str_allot(sl);
				(*buf) -= 2;
				extract_string(buf, v->data_as.string->data);
				break;
			case uj_float_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_float;
				extract_float(buf, &v->data_as.f);
				break;
			case uj_double_tag:
				v = ujvalue_new();
				v->type = uj_number;
				v->numbertype = uj_double;
				extract_double(buf, &v->data_as.d);
				break;
			//case uj_array_tag: //parse can call itself here, srcbuf** will step along. just pass new srclen
			//case uj_object_tag:
			default:
				break;
		}
	}
	return v;
}






