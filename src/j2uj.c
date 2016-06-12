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
  * j2uj.c
  * Simple command-line utility to convert json to microjson
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "ujson-value.h"
#include "ujson-encode.h"
#include "ujson-fromjson.h"

#define ALLOC_CHUNK_SIZE 0x01000
#define MAX_UJ_SIZE      0x10000

static int help(int r)
{
	const char* helptext = \
	"j2uj - JSON to microjson converter\n" \
	"Copyright 2016 Aaron Kondziela <aaron@openrobotics.company>\n" \
	"Usage:\n" \
	"    j2uj < JSON_INPUT > UJ_OUTPUT\n" \
	"    j2uj [-h]\n" \
	"Options:\n" \
	"    -h    Print help text and exit\n" ;
	fwrite(helptext, strlen(helptext), 1, stderr);
	return r;
}

int main(int argc, char** argv)
{
	int o = 0;
	if (argc > 2) return help(1);
	while ((o = getopt(argc, argv, "h")) != -1)
		switch (o) {
			case 'h':
				return help(0);
			default:
				return help(1);
		}
	ujvalue* v = fromjson(stdin);
	uint8_t* ujsonbuf = calloc(1, MAX_UJ_SIZE);
	const char* ujsonbuferr = "ujsonbuf alloc error\n";
	if (ujsonbuf==NULL) return fwrite(ujsonbuferr, strlen(ujsonbuferr), 1, stderr);
	uint16_t s;
	s = encode(ujsonbuf, v);
	ujvalue_release(&v);
	fwrite(ujsonbuf, s, 1, stdout);
	free(ujsonbuf);
	return 0;
}

