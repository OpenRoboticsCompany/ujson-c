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
#include "hash.h"

int main(int argc, char* argv[])
{
	printf("Running ujson dev rig.\n");
	
#define BUFLEN 55
	uint8_t buf[BUFLEN];

	return(0);
}
