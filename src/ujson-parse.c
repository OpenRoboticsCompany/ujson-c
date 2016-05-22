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
#include "ujson-parse.h"
#include "ujson-extract.h"
#include "ujsizes.h"
#include "schematags.h"
#include "ujson-types.h"
#include "ujson-values.h"

ujvalue* parse(uint8_t** buf, uint16_t len)
{
	ujvalue* v;
	v = ujvalue_new();
	schematag t;

	while (len--) {
		t = (schematag)**buf;
		(*buf)++;
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
			//case uj_uint8_tag:
			//case uj_int8_tag:
			//case uj_uint16_tag:
			//case uj_int16_tag:
			//case uj_uint32_tag:
			//case uj_int32_tag:
			//case uj_uint64_tag:
			//case uj_int64_tag:
			//case uj_string_tag:
			//case uj_float_tag:
			//case uj_double_tag:
			//case uj_array_tag: //parse can call itself here, srcbuf** will step along. just pass new srclen
			//case uj_object_tag:
			default:
				break;
		}
	}
	return v;
}






