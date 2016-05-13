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

#include "ujson-render.h"
#include "ujson-extract.h"
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

	uint8_t u8a, u8b;
	int8_t i8a, i8b;
	uint16_t u16a, u16b;
	int16_t i16a, i16b;
	uint32_t u32a, u32b;
	int32_t i32a, i32b;
	uint64_t u64a, u64b;
	int64_t i64a, i64b;
	float fa, fb;
	double da, db;

	#define BUFFER_LENGTH 64
	// Buffer Under Test
	uint8_t but[BUFFER_LENGTH];
	uint8_t* nextbuf;
	// Buffer of Truth
	uint8_t* bot;

	/*********************** endian conversion tests *******/

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

	print("_swap_fpa_64() (only valid for current arch floating-point system)\n");
	u64a = 0x7E57AB1EDEADFA11ull;
	u64b = _swap_fpa_64( u64a );
	assert( u64b == 0xDEADFA117E57AB1Eull );

	/************************ ujson-render tests ***************/

	print("Testing render_* functions...\n");

	print("render_bool() for true\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"t";
	render_bool(&nextbuf, 1);
	assert( buffers_match(but, bot, 1) );

	print("render_bool() for false\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"f";
	render_bool(&nextbuf, 0);
	assert( buffers_match(but, bot, 1) );

	print("render_null()\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"n";
	render_null(&nextbuf);
	assert( buffers_match(but, bot, 1) );

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

	printf("render_uint32() for 0x7fffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"I\x7f\xff\xff\xff";
	render_uint32(&nextbuf, (uint32_t)0x7fffffff);
	assert( buffers_match(but, bot, 5) );

	printf("render_uint32() for 0x80000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"I\x80\x00\x00\x01";
	render_uint32(&nextbuf, (uint32_t)0x80000001);
	assert( buffers_match(but, bot, 5) );

	printf("render_int32() for 0x7fffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"i\x7f\xff\xff\xff";
	render_int32(&nextbuf, (int32_t)0x7fffffff);
	assert( buffers_match(but, bot, 5) );

	printf("render_int32() for 0x80000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"i\x80\x00\x00\x01";
	render_int32(&nextbuf, (int32_t)0x80000001);
	assert( buffers_match(but, bot, 5) );

	printf("render_uint64() for 0x7fffffffffffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"Q\x7f\xff\xff\xff\xff\xff\xff\xff";
	render_uint64(&nextbuf, (uint64_t)0x7fffffffffffffff);
	assert( buffers_match(but, bot, 9) );

	printf("render_uint64() for 0x8000000000000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"Q\x80\x00\x00\x00\x00\x00\x00\x01";
	render_uint64(&nextbuf, (uint64_t)0x8000000000000001);
	assert( buffers_match(but, bot, 9) );

	printf("render_int64() for 0x7fffffffffffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"q\x7f\xff\xff\xff\xff\xff\xff\xff";
	render_int64(&nextbuf, (int64_t)0x7fffffffffffffff);
	assert( buffers_match(but, bot, 9) );

	printf("render_int64() for 0x8000000000000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"q\x80\x00\x00\x00\x00\x00\x00\x01";
	render_int64(&nextbuf, (int64_t)0x8000000000000001);
	assert( buffers_match(but, bot, 9) );

	print("render_string()\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	#define TEST_STRING "my god, it's full of strings!"
	#define TEST_STRING_LEN 32
	bot = (uint8_t*)"s\x00\x1D" TEST_STRING;
	render_string(&nextbuf, (char*)TEST_STRING);
	assert( buffers_match(but, bot, TEST_STRING_LEN) );
	#undef TEST_STRING
	#undef TEST_STRING_LEN
	
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
	#elif !defined(__FLOAT_WORD_ORDER__)
		#warning "Macro __FLOAT_WORD_ORDER__ undefined, assuming non-mixed"
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

	/******************* ujson-extract tests ****************/

	print("Testing extract_* functions...\n");

	print("extract_bool_true()\n");
	bot = (uint8_t*)"t\xAA\xAA";
	nextbuf = bot + 1;
	u8a = 1;
	u8b = 0;
	extract_bool_true(&nextbuf, &u8b);
	assert( u8a == u8b );

	print("extract_bool_false()\n");
	bot = (uint8_t*)"f\xAA\xAA";
	nextbuf = bot + 1;
	u8a = 0;
	u8b = 1;
	extract_bool_false(&nextbuf, &u8b);
	assert( u8a == u8b );

	print("extract_uint8() for 0x7f\n");
	bot = (uint8_t*)"C\x7f\xAA";
	nextbuf = bot + 1;
	u8a = 0x7f;
	u8b = 0;
	extract_uint8(&nextbuf, &u8b);
	assert( u8a == u8b );

	print("extract_uint8 for 0x80\n");
	bot = (uint8_t*)"C\x80\xAA";
	nextbuf = bot + 1;
	u8a = 0x80;
	u8b = 0;
	extract_uint8(&nextbuf, &u8b);
	assert( u8a == u8b );

	print("extract_int8() for 0x7f\n");
	bot = (uint8_t*)"c\x7f\xAA";
	nextbuf = bot + 1;
	i8a = 0x7f;
	i8b = 0;
	extract_int8(&nextbuf, &i8b);
	assert( i8a == i8b );

	print("extract_int8() for 0x80\n");
	bot = (uint8_t*)"c\x80\xAA";
	nextbuf = bot + 1;
	i8a = 0x80;
	i8b = 0;
	extract_int8(&nextbuf, &i8b);
	assert( i8a == i8b );

	print("extract_uint16() for 0x7fff\n");
	bot = (uint8_t*)"W\x7f\xff\xAA";
	nextbuf = bot + 1;
	u16a = 0x7fff;
	u16b = 0;
	extract_uint16(&nextbuf, &u16b);
	assert( u16a == u16b );

	print("extract_uint16() for 0x8001\n");
	bot = (uint8_t*)"W\x80\x01\xAA";
	nextbuf = bot + 1;
	u16a = 0x8001;
	u16b = 0;
	extract_uint16(&nextbuf, &u16b);
	assert( u16a == u16b );

	print("extract_int16() for 0x7fff\n");
	bot = (uint8_t*)"w\x7f\xff\xAA";
	nextbuf = bot + 1;
	i16a = 0x7fff;
	i16b = 0;
	extract_int16(&nextbuf, &i16b);
	assert( i16a == i16b );

	print("extract_int16() for 0x8001\n");
	bot = (uint8_t*)"w\x80\x01\xAA";
	nextbuf = bot + 1;
	i16a = 0x8001;
	i16b = 0;
	extract_int16(&nextbuf, &i16b);
	assert( i16a == i16b );


	print("extract_uint32() for 0x7fffffff\n");
	bot = (uint8_t*)"I\x7f\xff\xff\xff\xAA";
	nextbuf = bot + 1;
	u32a = 0x7fffffff;
	u32b = 0;
	extract_uint32(&nextbuf, &u32b);
	assert( u32a == u32b );

	print("extract_uint32() for 0x80000001\n");
	bot = (uint8_t*)"I\x80\x00\x00\x01\xAA";
	nextbuf = bot + 1;
	u32a = 0x80000001;
	u32b = 0;
	extract_uint32(&nextbuf, &u32b);
	assert( u32a == u32b );

	print("extract_int32() for 0x7fffffff\n");
	bot = (uint8_t*)"i\x7f\xff\xff\xff\xAA";
	nextbuf = bot + 1;
	i32a = 0x7fffffff;
	i32b = 0;
	extract_int32(&nextbuf, &i32b);
	assert( i32a == i32b );

	print("extract_int32() for 0x80000001\n");
	bot = (uint8_t*)"i\x80\x00\x00\x01\xAA";
	nextbuf = bot + 1;
	i32a = 0x80000001;
	i32b = 0;
	extract_int32(&nextbuf, &i32b);
	assert( i32a == i32b );

	
	print("extract_uint64() for 0x7fffffffffffffff\n");
	bot = (uint8_t*)"I\x7f\xff\xff\xff\xff\xff\xff\xff\xAA";
	nextbuf = bot + 1;
	u64a = 0x7fffffffffffffff;
	u64b = 0;
	extract_uint64(&nextbuf, &u64b);
	assert( u64a == u64b );

	print("extract_uint64() for 0x8000000000000001\n");
	bot = (uint8_t*)"I\x80\x00\x00\x00\x00\x00\x00\x01\xAA";
	nextbuf = bot + 1;
	u64a = 0x8000000000000001;
	u64b = 0;
	extract_uint64(&nextbuf, &u64b);
	assert( u64a == u64b );

	print("extract_int64() for 0x7fffffffffffffff\n");
	bot = (uint8_t*)"i\x7f\xff\xff\xff\xff\xff\xff\xff\xAA";
	nextbuf = bot + 1;
	i64a = 0x7fffffffffffffff;
	i64b = 0;
	extract_int64(&nextbuf, &i64b);
	assert( i64a == i64b );

	print("extract_int64() for 0x8000000000000001\n");
	bot = (uint8_t*)"i\x80\x00\x00\x00\x00\x00\x00\x01\xAA";
	nextbuf = bot + 1;
	i64a = 0x8000000000000001;
	i64b = 0;
	extract_int64(&nextbuf, &i64b);
	assert( i64a == i64b );

	print("extract_string()\n");
	zero(but, BUFFER_LENGTH);
	#define TEST_STRING "my god, it's full of strings!"
	#define TEST_STRING_LEN 29
	bot = (uint8_t*)"\x00\x1D" TEST_STRING;
	nextbuf = bot;
	but[TEST_STRING_LEN+1] = '\xAA';
	extract_string(&nextbuf, (char*)but);
	assert( buffers_match(but, TEST_STRING, TEST_STRING_LEN) );
	// check if writing past the end...
	assert( but[TEST_STRING_LEN+1] == '\xAA' );
	#undef TEST_STRING
	#undef TEST_STRING_LEN

	// TODO: more tests!

	print("Tests for ujson-c complete - PASS\n");
	return(0);
}
