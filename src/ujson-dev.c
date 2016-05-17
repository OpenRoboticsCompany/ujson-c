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

#include "ujson-encode.h"
#include "hexdump.h"
#include "endian.h"
#include "udp.h"

int main(int argc, char* argv[])
{
	printf("Running ujson dev rig.\n");
	
#define BUFLEN 55

	uint8_t buf[BUFLEN];
	int i;
	for (i = 0; i < BUFLEN; i++) buf[i] = 0;

	uint8_t* bp = buf;
	uint8_t* nextbuf;
	nextbuf = buf;

	float ff = 123.456f;
	double dd = 789.654f;

	printf("buffer before:\n");
	hexdump(buf, BUFLEN);

	encode_double(&nextbuf, dd);

	printf("buffer after:\n");
	hexdump(buf, BUFLEN);
	printf("buffer pointers:\n");
	hexdump(&bp, sizeof(bp));
	hexdump(&nextbuf, sizeof(nextbuf));
	printf("size is %u\n", (nextbuf - bp));
	printf("sending udp\n");
	xmit(buf, (nextbuf - buf));
	printf("done\n");

	printf("sizeof float %d\n", sizeof(float));
	printf("sizeof double %d\n", sizeof(double));

	printf("float %.10f hexdump:\n", ff);
	hexdump(&ff, 4);
	printf("double %.10f hexdump:\n", dd);
	hexdump(&dd, 8);
	dd = htojd(dd);
	hexdump(&dd, 8);

#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
	printf("float word order little endian\n");
#else
	printf("float word order big endian\n");
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	printf("byte order little endian\n");
#else
	printf("byte order big endian\n");
#endif

	return(0);
}
