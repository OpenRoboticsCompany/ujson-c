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
  * ujson-tests.c
  *
  * Runs a basic set of tests, use via "make tests"
  * If running on embedded, you may want to redefine the output mechanism
  * in the print func below, and the assert call, depending on what
  * resources are available to you.
  *
  */

#include <stdio.h>
#include <stdint.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "ujson.h"
#include "endian.h"

// Change output routine here for serial output on embedded, etc.
void print(char* s)
{
	printf(s);
}

int buffers_match( uint8_t* b1, uint8_t* b2, uint16_t len )
{
	while (len--) if ( b1[len] != b2[len] ) return 0;
	return -1;
}

void zero( uint8_t* b, uint16_t len )
{
	while (len--) b[len] = 0;
}

int main(int ARGC, char* ARGV[])
{
	print("Running ujson-c tests...\n");

	uint16_t u16a, u16b;
	uint32_t u32a, u32b;
	uint64_t u64a, u64b;

	print("Testing endian conversions...\n");

	print("_swap_16()\n");
	u16a = 0xDEAD;
	u16b = _swap_16( u16a );
	assert( u16b == 0xADDEu );

	print("_swap_32()\n");
	u32a = 0xCAFEBABE;
	u32b = _swap_32( u32a );
	assert( u32b == 0xBEBAFECAu );

	// TODO: add ifdefs for machine width conditional compilation
	print("_swap_64()\n");
	u64a = 0x5311AB1EC0DEBA5Eull;
	u64b = _swap_64( u64a );
	assert( u64b == 0x5EBADEC01EAB1153ull );

	print("Testing render_* functions...\n");

	#define BUFFER_LENGTH 64
	// Buffer Under Test
	uint8_t but[BUFFER_LENGTH];
	uint8_t* nextbuf;
	// Buffer of Truth
	uint8_t* bot;

	print("render_uint8() for 0x7B (123)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"C\x7B";
	render_uint8(&nextbuf, (uint8_t)0x7B);
	assert( buffers_match(but, bot, 2) );

	print("render_uint8() for 0x82 (130)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"C\x82";
	render_uint8(&nextbuf, (uint8_t)0x82);
	assert( buffers_match(but, bot, 2) );

	print("render_int8() for 0x7B (123)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"c\x7B";
	render_int8(&nextbuf, (int8_t)0x7B);
	assert( buffers_match(but, bot, 2) );

	print("render_int8() for 0x82 (-126)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"c\x82";
	render_int8(&nextbuf, (int8_t)0x82);
	assert( buffers_match(but, bot, 2) );

	print("render_uint16() for 0x7FFF\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"W\x7f\xff";
	render_uint16(&nextbuf, (uint16_t)0x7fff);
	assert( buffers_match(but, bot, 3) );

	print("render_uint16() for 0x8001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"W\x80\x01";
	render_uint16(&nextbuf, (uint16_t)0x8001);
	assert( buffers_match(but, bot, 3) );

	print("render_int16() for 0x7FFF\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"w\x7f\xff";
	render_int16(&nextbuf, (uint16_t)0x7fff);
	assert( buffers_match(but, bot, 3) );

	print("render_int16() for 0x8001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"w\x80\x01";
	render_int16(&nextbuf, (uint16_t)0x8001);
	assert( buffers_match(but, bot, 3) );

	print("render_string()\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	#define TEST_STRING "my god, it's full of strings!"
	#define TEST_STRING_LEN 32
	bot = (uint8_t*)"s\x00\x1D" TEST_STRING;
	render_string(&nextbuf, (char*)TEST_STRING);
	assert( buffers_match(but, bot, TEST_STRING_LEN) );
	
	print("Note: floating-point tests presently are only valid for current build target arch.\n");
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		print("Byte order is little endian\n");
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		print("Byte order is big endian\n");
	#else
		#error "Macro __BYTE_ORDER__ has unexpected value or is undefined"
	#endif
	#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
		print("Float word order is little endian\n");
	#elif __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__
		print("Float word order is big endian\n");
	#else
		#error "Macro __FLOAT_WORD_ORDER__ has unexpected value or is undefined"
	#endif

	print("render_float() for 12345.6789\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"d\x46\x40\xe6\xb7";
	render_float(&nextbuf, (float)12345.6789);
	assert( buffers_match(but, bot, 5) );

	print("render_double() for 12345.6789\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"D\x40\xc8\x1c\xd6\xe6\x31\xf8\xa1";
	render_double(&nextbuf, (double)12345.6789);
	assert( buffers_match(but, bot, 9) );

	// TODO: more tests!

	print("Tests for ujson-c complete - PASS\n");
	return(0);
}
