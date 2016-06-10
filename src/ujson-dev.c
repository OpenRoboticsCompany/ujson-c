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
  * ujson-dev.c
  *
  * This is only used during development of the ujson-c code, and will
  * most likely be deleted from the final release. Just various internal
  * probing and testing and whatnot. Nothing to see here, move along.
  *
  */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ujson-encode.h"
#include "hexdump.h"
#include "ujson-endian.h"
#include "udp.h"
#include "ujson-hash.h"
#include "ujson-string.h"
#include "ujson-types.h"
#include "schematags.h"
#include "ujson-value.h"
#include "ujson-array.h"
#include "ujson-decode.h"
#include "ujson-hash.h"
#include "ujson-object.h"
#include "ujson-tojson.h"
#include "ujson-dump.h"
#include "ujson-fromjson.h"
#include <assert.h>

int main(int argc, char* argv[])
{
	printf("\n\n*********************\nRunning ujson dev rig.\n");

	ujvalue* vv;
	FILE* fp;
	fp = fopen("test.json", "r");
	vv = fromjson(fp);
	fclose(fp);
	ujdump(vv);
	ujvalue_release(&vv);
	return 0;

	#define BUFLEN 2048
	char buffer[BUFLEN] = {0};
	ujvalue* decodedv;

	uint8_t* a = "\x00\x03\x01\x02\x03";
	uint8_t* s = "accc";

	decodedv = extract(a, s);
	ujdump(decodedv);
	ujvalue_release(&decodedv);

	a = "\x00\x0e\x00\x04\x61\x73\x64\x66\x01\x00\x04\x71\x77\x65\x72\x02";
	s = "occ";

	decodedv = extract(a, s);
	ujdump(decodedv);
	ujvalue_release(&decodedv);

	uint8_t dat[1024];
	uint8_t schema[1024];

	while(1) {
		int n, i;
		unsigned char buffer[65536];
		uint8_t* buf;
		printf("Listening for udp...\n");
		n = rec(buffer, 65536);
		printf("recevied %u bytes\n",n);
		hexdump(buffer, n);
		i = 0;
		//while (i < n) printf("\\x%02X", buffer[i++]);
		//printf("\n");
		buf = (uint8_t*)buffer;
		decodedv = decode(buf);
		ujdump(decodedv);

		buf = (uint8_t*)buffer;
		memset(buffer, 0, 65536);
		n = format(buf, decodedv);
		printf("%s\n",buffer);

		memset(buffer, '\0', 65536);
		n = 0;
		n = encode(buffer, decodedv);
		hexdump(buffer, n);
		ujvalue_release(&decodedv);
		decodedv = NULL;

		buf = (uint8_t*)buffer;
		decodedv = decode(buf);
		ujdump(decodedv);

		n = data(dat, decodedv);
		hexdump(dat, n);
		i = 0;
		while (i < n) printf("\\x%02X", dat[i++]);
		printf("\n");
		
		n = format(schema, decodedv);
		printf("%s\n",schema);

		ujvalue_release(&decodedv);
		decodedv = extract(dat, schema);

		ujdump(decodedv);
		ujvalue_release(&decodedv);

		printf("---\n");
	}

	return 0;
}




