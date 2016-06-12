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
  * uj2j.c
  * Simple command-line utility to convert microjson to json
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "ujson-value.h"
#include "ujson-decode.h"
#include "ujson-tojson.h"

#define ALLOC_CHUNK_SIZE 0x01000
#define MAX_UJ_SIZE      0x10000
#define MAX_JSON_SIZE    0x40000

static int help(int r)
{
	const char* helptext = \
	"uj2j - microjson to JSON converter\n" \
	"Copyright 2016 Aaron Kondziela <aaron@openrobotics.company>\n" \
	"Usage:\n" \
	"    uj2j [-t] < UJ_INPUT > JSON_OUTPUT\n" \
	"    uj2j [-h]\n" \
	"Options:\n" \
	"    -t    Output JSON with datatypes. e.g., 123/uint8\n" \
	"    -h    Print help text and exit\n" ;
	fwrite(helptext, strlen(helptext), 1, stderr);
	return r;
}

int main(int argc, char** argv)
{
	int o = 0, t = 0;
	if (argc > 2) return help(1);
	while ((o = getopt(argc, argv, "th")) != -1)
		switch (o) {
			case 'h':
				return help(0);
			default:
				return help(1);
			case 't':
				t = 1;
		}
	int s = 0, n = 0, chunks = 1;
	uint8_t* buf = calloc(1, ALLOC_CHUNK_SIZE);
	uint8_t* next = buf;
	const char* buferr = "ujbuf alloc error\n";
	while (!feof(stdin)) {
		n = fread(next, ALLOC_CHUNK_SIZE, 1, stdin);
		s += n;
		if (s > MAX_UJ_SIZE) return 1;
		if (n < ALLOC_CHUNK_SIZE) break;
		next = realloc(buf, ++chunks * ALLOC_CHUNK_SIZE);
		if (next==NULL) return fwrite(buferr, strlen(buferr), 1, stderr);
		memset(next, '\0', ALLOC_CHUNK_SIZE);
	}
	ujvalue* v = decode(buf);
	free(buf);
	char* jsonbuf = calloc(1, MAX_JSON_SIZE);
	const char* jsonbuferr = "jsonbuf alloc error\n";
	if (jsonbuf==NULL) return fwrite(jsonbuferr, strlen(jsonbuferr), 1, stderr);
	switch (t) {
		case 0:
			tojson(jsonbuf, v);
			break;
		case 1:
			tojson_with_types(jsonbuf, v);
	}
	ujvalue_release(&v);
	fwrite(jsonbuf, strlen(jsonbuf), 1, stdout);
	fwrite("\n", 1, 1, stdout);
	free(jsonbuf);
	return 0;
}

