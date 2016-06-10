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
  * Parses JSON into a ujvalue. Assumes if we are reading JSON text, that
  * we are running a larger system that isn't very tightly resource-constrained.
  *
  */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ujson-value.h"
#include "ujson-types.h"
#include "ujson-string.h"
#include "ujson-array.h"
#include "ujson-object.h"
#include "ujson-fromjson.h"

#define UJ_FROMJSON_MAXSTRINGSIZE 1024
#define UJ_FROMJSON_NUMBUFSIZE 25

FILE* f;
static ujvalue* readvalue();

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

static int skipnumber()
{
	int c = 0;
	while (1) {
		switch (c = next()) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'X':
			case 'a': case 'b': case 'c': case 'd':           case 'f': case 'x':
			case '.': case '-': case '+': case 'e':
				continue;
			case EOF:
				return 1;
			default:
				if (ungetc(c,f)==EOF) return 1;
				return 0;
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
	if (skipnumber()) goto LaFinDuMonde;

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

static int unicode_decode(char* s, int* i)
{
	// TODO write this
	long int a = 0, b = 0;
	int j = 0, c = 0;
	char buf[5] = {0};
	char* tail;

	for (j = 0; j < 4; j++) if ((buf[j]=next()) == EOF) return 1;
	a = strtoul(buf, &tail, 16);
	if (tail==buf) return 1;

	if (a >= 0 && a < 0x80) {
		s[(*i)++] = (char)a;
	} else if (a >= 0x80 && a < 0x800) {
		s[(*i)++] = (char)(0xC0 | (a >> 6));
		s[(*i)++] = (char)(0x80 | (a & 0x3F));
	} else if ((a >= 0x800 && a < 0xD800) || (a >= 0xE000 && a < 0x10000)) {
		s[(*i)++] = (char)(0xE0 | (a >> 12));
		s[(*i)++] = (char)(0x80 | ((a >> 6) & 0x3F));
		s[(*i)++] = (char)(0x80 | (a & 0x3F));
	} else if (a >= 0xD800 && a < 0xE000) {
		// surrogate pair
		if ((c=next()) != '\\') return 1;
		if ((c=next()) != 'u') return 1;
		for (j = 0; j < 4; j++) if ((buf[j]=next()) == EOF) return 1;
		b = strtoul(buf, &tail, 16);
		if (tail==buf) return 1;
		a -= 0xD800;
		b -= 0xDC00;
		a = (a << 10) | b;
		a += 0x010000;
		s[(*i)++] = (char)(0xF0 | (a >> 18));
		s[(*i)++] = (char)(0x80 | ((a >> 12) & 0x3F));
		s[(*i)++] = (char)(0x80 | ((a >> 6) & 0x3F));
		s[(*i)++] = (char)(0x80 | (a & 0x3F));
	}
	return 0;
}

static int skipstring()
{
	int c, e = 0;
	while ((c = next()) != EOF) {
		if (e) {
			switch (c) {
				case 'u':
					for (e = 4; e > 0; e--) if (next()==EOF) return 1;
					continue;
				default:
					e = 0;
					continue;
			}
		}
		switch (c) {
			case '\\':
				e = 1;
				continue;
			case '"':
				return 0;
		}
	}
	return 1;
}

static ujvalue* readstring()
{
	ujvalue* v = ujvalue_new();
	char s[UJ_FROMJSON_MAXSTRINGSIZE + 1] = {0};
	int c, i = 0, e = 0;

	while ((c = next()) != EOF && i < UJ_FROMJSON_MAXSTRINGSIZE) {
		if (e) {
			e = 0;
			switch (c) {
				case '"':
				case '\\':
				case '/':
				case '\x00': case '\x01': case '\x02': case '\x03':
				case '\x04': case '\x05': case '\x06': case '\x07': 
				case '\x08': case '\x09': case '\x0A': case '\x0B':
				case '\x0C': case '\x0D': case '\x0E': case '\x0F': 
				case '\x10': case '\x11': case '\x12': case '\x13':
				case '\x14': case '\x15': case '\x16': case '\x17': 
				case '\x18': case '\x19': case '\x1A': case '\x1B':
				case '\x1C': case '\x1D': case '\x1E': case '\x1F': 
					s[i++] = (char)c;
					continue;
				case 'b':
					s[i++] = '\b';
					continue;
				case 'f':
					s[i++] = '\f';
					continue;
				case 'n':
					s[i++] = '\n';
					continue;
				case 'r':
					s[i++] = '\r';
					continue;
				case 't':
					s[i++] = '\t';
					continue;
				case 'u':
					if (unicode_decode(s, &i)) goto LaFinDuMonde;
					continue;
				default:
					continue;
			}
		}
		switch (c) {
			case '\\':
				e = 1;
				continue;
			case '"':
				goto Done;
			default:
				s[i++] = (char)c;
		}
	}

Done:
	if (c != '"' && c != EOF)
		if (skipstring()) goto LaFinDuMonde;

	v->type = uj_string;
	v->data_as.string = string_from((uint8_t*)s);
	return v;

LaFinDuMonde:
	ujvalue_release(&v);
	return NULL;
}

static void ujtmpa_push(ujtmpa** head, ujvalue* v)
{
	if (*head==NULL) {
		*head = calloc(1, sizeof(ujtmpa));
		(*head)->v = v;
		return;
	}
	ujtmpa* cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = calloc(1, sizeof(ujtmpa));
	cur->next->v = v;
}

static ujvalue* ujtmpa_get(ujtmpa* l, int i)
{
	while (i--) l = l->next;
	return l->v;
}

static void ujtmpa_release(ujtmpa** l, int chain)
{
	ujtmpa* cur = *l;
	ujtmpa* next;
	while (cur) {
		next = cur->next;
		if (chain) ujvalue_release(&cur->v);
		free(cur);
		cur = next;
	}
	*l = NULL;
}

static ujvalue* readarray()
{
	ujtmpa* list = NULL;
	ujvalue* v;
	int c, i = 0;
	while ((c = next()) != ']') {
		if (c==EOF) goto LaFinDuMonde;
		if (ungetc(c,f)==EOF) goto LaFinDuMonde;
		if ((v=readvalue())==NULL) goto LaFinDuMonde;
		switch (c=next()) {
			case ',':
				break;
			case ']':
				if (ungetc(c,f)==EOF) goto LaFinDuMonde;
				break;
			case EOF:
			default:
				goto LaFinDuMonde;
		}
		ujtmpa_push(&list, v);
		i++;
	}
	v = ujvalue_new();
	v->type = uj_array;
	v->data_as.array = array_allot(i);
	c = 0;
	while (c < i) array_push(v->data_as.array, ujtmpa_get(list, c++));
	ujtmpa_release(&list, 0);
	return v;

LaFinDuMonde:
	ujtmpa_release(&list, 1);
	return NULL;
}

static void ujtmpo_push(ujtmpo** head, ujstring* k, ujvalue* v)
{
	if (*head==NULL) {
		*head = calloc(1, sizeof(ujtmpo));
		(*head)->k = k;
		(*head)->v = v;
		return;
	}
	ujtmpo* cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = calloc(1, sizeof(ujtmpo));
	cur->next->k = k;
	cur->next->v = v;
}

static ujtmpo* ujtmpo_get(ujtmpo* l, int i)
{
	while (i--) l = l->next;
	return l;
}

static void ujtmpo_release(ujtmpo** l, int chain)
{
	ujtmpo* cur = *l;
	ujtmpo* next;
	while (cur) {
		next = cur->next;
		if (chain) string_release(&cur->k), ujvalue_release(&cur->v);
		free(cur);
		cur = next;
	}
	*l = NULL;
}

static ujvalue* readobject()
{
	ujtmpo* list = NULL;
	ujstring* k;
	ujvalue* v;
	int c, i = 0;
	while ((c = next()) != '}') {
		if (c==EOF) goto LaFinDuMonde;
		if (ungetc(c,f)==EOF) goto LaFinDuMonde;
		if ((v=readvalue())==NULL) goto LaFinDuMonde;
		k = string_from(v->data_as.string->data);
		ujvalue_release(&v);
		if ((c=next()) != ':') goto LaFinDuMonde;
		if ((v=readvalue())==NULL) goto LaFinDuMonde;
		switch (c=next()) {
			case ',':
				break;
			case '}':
				if (ungetc(c,f)==EOF) goto LaFinDuMonde;
				break;
			case EOF:
			default:
				goto LaFinDuMonde;
		}
		ujtmpo_push(&list, k, v);
		i++;
	}
	v = ujvalue_new();
	v->type = uj_object;
	v->data_as.object = object_allot(i);
	while (i--) object_set(v->data_as.object, ujtmpo_get(list,i)->k, ujtmpo_get(list,i)->v);
	ujtmpo_release(&list, 0);
	return v;

LaFinDuMonde:
	ujtmpo_release(&list, 1);
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
			v = readobject();
			break;
		case '[':
			v = readarray();
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '-':
			if (ungetc(c, f)==EOF) return NULL;
			v = readnumber();
			break;
		case '"':
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
