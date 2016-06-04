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
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "ujson-encode.h"
#include "ujson-extract.h"
#include "endian.h"
#include "movebytes.h"
#include "ujson-hash.h"
#include "ujson-string.h"
#include "ujson-decode.h"
#include "ujson-value.h"
#include "ujson-array.h"
#include "ujson-tojson.h"

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

//TODO factor tests out to smaller files.
//TODO assert() is ghetto as a test system. add more awesome sauce.
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

	/************************ movebytes tests ***************/

	print("Testing movebytes...\n");
	uint8_t* srcbuf = (uint8_t*)"1234567890";
	zero(but, BUFFER_LENGTH);
	but[10] = (uint8_t)'\xAA'; //overrun canary
	movebytes(but, srcbuf, 10); //not including that trailing null
	assert( buffers_match(but, srcbuf, 10) );
	assert( but[10] = (uint8_t)'\xAA' );

	/************************ ujson-encode tests ***************/

	print("Testing encode_* functions...\n");

	print("encode_bool() for true\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"t";
	encode_bool(&nextbuf, 1);
	assert( buffers_match(but, bot, 1) );

	print("encode_bool() for false\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"f";
	encode_bool(&nextbuf, 0);
	assert( buffers_match(but, bot, 1) );

	print("encode_null()\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"n";
	encode_null(&nextbuf);
	assert( buffers_match(but, bot, 1) );

	print("encode_uint8() for 0x7B (123)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"C\x7B";
	encode_uint8(&nextbuf, (uint8_t)0x7B);
	assert( buffers_match(but, bot, 2) );

	print("encode_uint8() for 0x82 (130)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"C\x82";
	encode_uint8(&nextbuf, (uint8_t)0x82);
	assert( buffers_match(but, bot, 2) );

	print("encode_int8() for 0x7B (123)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"c\x7B";
	encode_int8(&nextbuf, (int8_t)0x7B);
	assert( buffers_match(but, bot, 2) );

	print("encode_int8() for 0x82 (-126)\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"c\x82";
	encode_int8(&nextbuf, (int8_t)0x82);
	assert( buffers_match(but, bot, 2) );

	print("encode_uint16() for 0x7FFF\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"W\x7f\xff";
	encode_uint16(&nextbuf, (uint16_t)0x7fff);
	assert( buffers_match(but, bot, 3) );

	print("encode_uint16() for 0x8001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"W\x80\x01";
	encode_uint16(&nextbuf, (uint16_t)0x8001);
	assert( buffers_match(but, bot, 3) );

	print("encode_int16() for 0x7FFF\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"w\x7f\xff";
	encode_int16(&nextbuf, (uint16_t)0x7fff);
	assert( buffers_match(but, bot, 3) );

	print("encode_int16() for 0x8001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"w\x80\x01";
	encode_int16(&nextbuf, (uint16_t)0x8001);
	assert( buffers_match(but, bot, 3) );

	printf("encode_uint32() for 0x7fffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"I\x7f\xff\xff\xff";
	encode_uint32(&nextbuf, (uint32_t)0x7fffffff);
	assert( buffers_match(but, bot, 5) );

	printf("encode_uint32() for 0x80000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"I\x80\x00\x00\x01";
	encode_uint32(&nextbuf, (uint32_t)0x80000001);
	assert( buffers_match(but, bot, 5) );

	printf("encode_int32() for 0x7fffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"i\x7f\xff\xff\xff";
	encode_int32(&nextbuf, (int32_t)0x7fffffff);
	assert( buffers_match(but, bot, 5) );

	printf("encode_int32() for 0x80000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"i\x80\x00\x00\x01";
	encode_int32(&nextbuf, (int32_t)0x80000001);
	assert( buffers_match(but, bot, 5) );

	printf("encode_uint64() for 0x7fffffffffffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"Q\x7f\xff\xff\xff\xff\xff\xff\xff";
	encode_uint64(&nextbuf, (uint64_t)0x7fffffffffffffff);
	assert( buffers_match(but, bot, 9) );

	printf("encode_uint64() for 0x8000000000000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"Q\x80\x00\x00\x00\x00\x00\x00\x01";
	encode_uint64(&nextbuf, (uint64_t)0x8000000000000001);
	assert( buffers_match(but, bot, 9) );

	printf("encode_int64() for 0x7fffffffffffffff\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"q\x7f\xff\xff\xff\xff\xff\xff\xff";
	encode_int64(&nextbuf, (int64_t)0x7fffffffffffffff);
	assert( buffers_match(but, bot, 9) );

	printf("encode_int64() for 0x8000000000000001\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"q\x80\x00\x00\x00\x00\x00\x00\x01";
	encode_int64(&nextbuf, (int64_t)0x8000000000000001);
	assert( buffers_match(but, bot, 9) );

	print("encode_string()\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	#define TEST_STRING "my god, it's full of strings!"
	#define TEST_STRING_LEN 32
	bot = (uint8_t*)"s\x00\x1D" TEST_STRING;
	encode_string(&nextbuf, (char*)TEST_STRING);
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

	print("encode_float() for 12345.6789\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"d\x46\x40\xe6\xb7";
	encode_float(&nextbuf, (float)12345.6789);
	assert( buffers_match(but, bot, 5) );

	print("encode_double() for 12345.6789\n");
	zero(but, BUFFER_LENGTH);
	nextbuf = but;
	bot = (uint8_t*)"D\x40\xc8\x1c\xd6\xe6\x31\xf8\xa1";
	encode_double(&nextbuf, (double)12345.6789);
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
	bot = (uint8_t*)"Q\x7f\xff\xff\xff\xff\xff\xff\xff\xAA";
	nextbuf = bot + 1;
	u64a = 0x7fffffffffffffff;
	u64b = 0;
	extract_uint64(&nextbuf, &u64b);
	assert( u64a == u64b );

	print("extract_uint64() for 0x8000000000000001\n");
	bot = (uint8_t*)"Q\x80\x00\x00\x00\x00\x00\x00\x01\xAA";
	nextbuf = bot + 1;
	u64a = 0x8000000000000001;
	u64b = 0;
	extract_uint64(&nextbuf, &u64b);
	assert( u64a == u64b );

	print("extract_int64() for 0x7fffffffffffffff\n");
	bot = (uint8_t*)"q\x7f\xff\xff\xff\xff\xff\xff\xff\xAA";
	nextbuf = bot + 1;
	i64a = 0x7fffffffffffffff;
	i64b = 0;
	extract_int64(&nextbuf, &i64b);
	assert( i64a == i64b );

	print("extract_int64() for 0x8000000000000001\n");
	bot = (uint8_t*)"q\x80\x00\x00\x00\x00\x00\x00\x01\xAA";
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
	extract_string(&nextbuf, but);
	assert( buffers_match(but, (uint8_t*)TEST_STRING, TEST_STRING_LEN) );
	// check if writing past the end...
	assert( but[TEST_STRING_LEN+1] == '\xAA' );
	#undef TEST_STRING
	#undef TEST_STRING_LEN

	print("extract_float() for 12345.6789\n");
	bot = (uint8_t*)"\x46\x40\xe6\xb7";
	fa = (float)12345.6789;
	nextbuf = bot;
	extract_float(&nextbuf, &fb);
	assert( fa == fb );

	print("extract_double() for 12345.6789\n");
	bot = (uint8_t*)"\x40\xc8\x1c\xd6\xe6\x31\xf8\xa1";
	da = (double)12345.6789;
	nextbuf = bot;
	extract_double(&nextbuf, &db);
	assert( da == db );

	/********************** str tests ************/

	print("ujson-string\n");

	ujstring* test_str1;
	ujstring* test_str2;
	#define TEST_STRING_1 "this is a test string, ain't it grand?"
	#define TEST_STRING_2 "this is a test string, but shorter."
	bot = (uint8_t*)TEST_STRING_1;
	zero(but, BUFFER_LENGTH);
	test_str1 = string_allot( strlen((char*)bot) );
	assert( test_str1->length == strlen((char*)bot) );
	assert( strlen((char*)test_str1->data) == 0 );
	// yes s->length should be alloted size and strlen zero, here. assuming strings
	// will be assigned upon creation and not mutated later.

	print("string_release()\n");
	string_release(&test_str1);
	assert( test_str1 == NULL );

	print("string_from()\n");
	test_str1 = string_from((uint8_t*)TEST_STRING_1);
	assert( test_str1->length = strlen(TEST_STRING_1) );
	assert( strcmp((char*)test_str1->data, TEST_STRING_1) == 0 );

	print("string_eq()\n");
	test_str2 = string_from( (uint8_t*)TEST_STRING_2 );
	assert( ! string_eq(test_str1, test_str2) );
	assert( test_str1->length > test_str2->length );
	string_release(&test_str2);
	test_str2 = string_from( (uint8_t*)TEST_STRING_1 );
	assert( string_eq(test_str1, test_str2) );
	
	string_release(&test_str1);
	string_release(&test_str2);
	

	/****************** array ************/

	print("decode_arraylen()\n");
	bot = (uint8_t*)"\x61\x00\x4e\x63\x01\x77\x01\x00\x69\x00\x01\x00\x00\x51\x00\x00\x00\x01\x00\x00\x00\x00\x73\x00\x04\x61\x73\x64\x66\x63\xff\x77\xff\x00\x69\xff\xff\x00\x00\x71\xff\xff\xff\xff\x00\x00\x00\x00\x61\x00\x04\x63\x01\x63\x02\x6f\x00\x0b\x00\x03\x6b\x65\x79\x73\x00\x03\x76\x61\x6c\x63\x01\x74\x66\x6e\x73\x00\x04\x71\x77\x65\x72";
	// [1,256,65536,4294967296,<<"asdf">>,-1,-256,-65536,-4294967296,[1,2],[{<<"key">>,<<"val">>}],1,true,false,null,<<"qwer">>]
	bot += 1; // trim the 'a' schematag
	#define ARRAY_TERMS 16
	u16a = 0;
	u16a = decode_arraylen(&bot);
	assert( u16a == ARRAY_TERMS );

	{
	print("array_allot()\n");
	ujarray* uja = 0;
	uja = array_allot(3);
	assert(uja->size == 3);
	assert(uja->start == 0);
	assert(uja->end == 0);
	assert(array_length(uja) == 0);

	print("array_push()\n");
	ujvalue* v;
	v = ujvalue_new();
	v->type = uj_number;
	v->numbertype = uj_uint8;
	v->data_as.uint8 = 0xAA;
	array_push(uja, v);
	assert(uja->size == 3);
	assert(uja->start == 0);
	assert(uja->end == 1);
	assert(uja->values[0]->data_as.uint8 == 0xAA);
	assert(array_length(uja) == 1);

	print("array_push()\n");
	v = ujvalue_new();
	v->type = uj_number;
	v->numbertype = uj_uint32;
	v->data_as.uint32 = 0xBBBBBBBB;
	array_push(uja, v);
	assert(uja->size == 3);
	assert(uja->start == 0);
	assert(uja->end == 2);
	assert(uja->values[0]->data_as.uint8 == 0xAA);
	assert(uja->values[1]->data_as.uint32 == 0xBBBBBBBB);
	assert(array_length(uja) == 2);

	print("array_push()\n");
	v = ujvalue_new();
	v->type = uj_number;
	v->numbertype = uj_uint16;
	v->data_as.uint16 = 0xCCCC;
	array_push(uja, v);
	assert(uja->size == 3);
	assert(uja->start == 0);
	assert(uja->end == 3);
	assert(uja->values[0]->data_as.uint8 == 0xAA);
	assert(uja->values[1]->data_as.uint32 == 0xBBBBBBBB);
	assert(uja->values[2]->data_as.uint16 == 0xCCCC);
	assert(array_length(uja) == 3);

	print("array_each()\n");
	// anon func is gcc only
	void (*f)(ujvalue** v) =
		({ void __fn__ (ujvalue** v) {
		 	switch((*v)->numbertype) {
				case uj_uint8:
					(*v)->data_as.uint8++; break;
				case uj_uint16:
					(*v)->data_as.uint16++; break;
				case uj_uint32:
					(*v)->data_as.uint32++; break;
			}
		}__fn__;
		});
	array_each(uja, f);
	assert(uja->values[0]->data_as.uint8 == 0xAB);
	assert(uja->values[1]->data_as.uint32 == 0xBBBBBBBC);
	assert(uja->values[2]->data_as.uint16 == 0xCCCD);

	print("array_map()\n");
	// anon func is gcc only
	ujvalue* (*m)(ujvalue* v) =
		({ ujvalue* __fn__ (ujvalue* v) {
		 	ujvalue* v2;
			v2 = ujvalue_new();
			v2->type = uj_number;
		 	switch(v->numbertype) {
				case uj_uint8:
					v2->numbertype = uj_uint8;
					v2->data_as.uint8 = v->data_as.uint8 + 1; break;
				case uj_uint16:
					v2->numbertype = uj_uint16;
					v2->data_as.uint16 = v->data_as.uint16 + 1; break;
				case uj_uint32:
					v2->numbertype = uj_uint32;
					v2->data_as.uint32 = v->data_as.uint32 + 1; break;
			}
			return v2;
		}__fn__;
		});
	ujarray* ujb;
	ujb = array_map(uja, m);
	assert(ujb->size == 3);
	assert(array_length(ujb) == 3);
	assert(ujb->values[0]->data_as.uint8 == 0xAC);
	assert(ujb->values[1]->data_as.uint32 == 0xBBBBBBBD);
	assert(ujb->values[2]->data_as.uint16 == 0xCCCE);
	array_release(&ujb);

	print("array_pop()\n");
	v = NULL;
	v = array_pop(uja);
	assert(uja->size == 3);
	assert(uja->start == 0);
	assert(uja->end == 2);
	assert(uja->values[0]->data_as.uint8 == 0xAB);
	assert(uja->values[1]->data_as.uint32 == 0xBBBBBBBC);
	assert(uja->values[2] == NULL);
	assert(v->data_as.uint16 = 0xCCCD);
	assert(array_length(uja) == 2);
	ujvalue_release(&v);
	assert(v == NULL);

	print("array_shift()\n");
	v = array_shift(uja);
	assert(uja->size == 3);
	assert(uja->start == 1);
	assert(uja->end == 2);
	assert(uja->values[0] == NULL);
	assert(uja->values[1]->data_as.uint32 == 0xBBBBBBBC);
	assert(uja->values[2] == NULL);
	assert(v->data_as.uint8 == 0xAB);
	assert(array_length(uja) == 1);
	ujvalue_release(&v);
	assert(v == NULL);

	print("array_release()\n");
	array_release(&uja);
	assert(uja == NULL);

	}

	/****************** object ***************/

	{
	print("object\n");

	#define OLEN 10
	ujobject* o;
	ujstring* k;
	ujvalue* v;
	char sb[255];
	int i;
	print("object_allot()\n");
	o = object_allot(OLEN);
	assert( o->size == OLEN );

	print("object_set()\n");
	for(i = 0; i < 10; i++) {
		sprintf(sb, "%c", (char)i+0x40);
		k = string_from((uint8_t*)sb);
		v = ujvalue_new();
		v->type = uj_number;
		v->numbertype = uj_uint32;
		v->data_as.uint32 = (uint32_t)(i<<24 | i<<16 | i<<8 | i);
		object_set(o, k, v);
	}

	print("object_get()\n");
	for(i = 9; i >= 0; i--) {
		sprintf(sb, "%c", (char)i+0x40);
		k = string_from((uint8_t*)sb);
		v = object_get(o, k);
		string_release(&k);
		assert( v->data_as.uint32 == (uint32_t)(i<<24 | i<<16 | i<<8 | i) );
	}

	print("object_release()\n");
	object_release(&o);
	}

	/****************** decode *************/

	ujvalue* v;
	v = NULL;

	print("decode(true)\n");
	bot = (uint8_t*)"t";
	v = decode(&bot);
	assert( v->type == uj_true );

	print("ujvalue_release()\n");
	ujvalue_release(&v);
	assert( v == NULL );

	print("decode(false)\n");
	bot = (uint8_t*)"f";
	v = decode(&bot);
	assert( v->type == uj_false );
	ujvalue_release(&v);

	print("decode(null)\n");
	bot = (uint8_t*)"n";
	v = decode(&bot);
	assert( v->type == uj_null );
	ujvalue_release(&v);

	print("decode(uint8)\n");
	bot = (uint8_t*)"C\x7f\xAA";
	u8a = 0x7f;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_uint8 );
	assert( v->data_as.uint8 == u8a );
	ujvalue_release(&v);

	print("decode(int8)\n");
	bot = (uint8_t*)"c\x81\xAA";
	i8a = 0x81;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_int8 );
	assert( v->data_as.int8 == i8a );
	ujvalue_release(&v);

	print("decode(uint16)\n");
	bot = (uint8_t*)"W\x7f\xff\xAA";
	u16a = 0x7fff;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_uint16 );
	assert( v->data_as.uint16 == u16a );
	ujvalue_release(&v);

	print("decode(int16)\n");
	bot = (uint8_t*)"w\x80\x02\xAA";
	i16a = 0x8002;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_int16 );
	assert( v->data_as.int16 == i16a );
	ujvalue_release(&v);

	print("decode(uint32)\n");
	bot = (uint8_t*)"I\x7f\xff\xff\xff\xAA";
	u32a = 0x7fffffff;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_uint32 );
	assert( v->data_as.uint32 == u32a );
	ujvalue_release(&v);

	print("decode(int32)\n");
	bot = (uint8_t*)"i\x80\x00\x00\x01\xAA";
	i32a = 0x80000001;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_int32 );
	assert( v->data_as.int32 == i32a );
	ujvalue_release(&v);

	print("decode(uint64)\n");
	bot = (uint8_t*)"Q\x7f\xff\xff\xff\xff\xff\xff\xff\xAA";
	u64a = 0x7fffffffffffffff;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_uint64 );
	assert( v->data_as.uint64 == u64a );
	ujvalue_release(&v);

	print("decode(int64)\n");
	bot = (uint8_t*)"q\x80\x00\x00\x00\x00\x00\x00\x03\xAA";
	i64a = 0x8000000000000003;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_int64 );
	assert( v->data_as.int64 == i64a );
	ujvalue_release(&v);


	print("decode(string)\n");
	#define TEST_STRING "if you have two strings do you call it twine?"
	#define TEST_STRING_LEN 45
	bot = (uint8_t*)"s\x00\x2D" TEST_STRING;
	v = decode(&bot);
	assert( v->type == uj_string );
	assert( v->data_as.string );
	assert( v->data_as.string->data );
	assert( v->data_as.string->length = TEST_STRING_LEN );
	assert( buffers_match(v->data_as.string->data, (uint8_t*)TEST_STRING, TEST_STRING_LEN) );
	#undef TEST_STRING
	#undef TEST_STRING_LEN

	ujvalue_release(&v);

	print("decode(float)\n");
	bot = (uint8_t*)"d\x46\x40\xe6\xb7";
	fa = (float)12345.6789;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_float );
	assert( v->data_as.f == fa );
	ujvalue_release(&v);

	print("decode(double)\n");
	bot = (uint8_t*)"D\x40\xc8\x1c\xd6\xe6\x31\xf8\xa1";
	da = (double)12345.6789;
	v = decode(&bot);
	assert( v->type == uj_number );
	assert( v->numbertype == uj_double );
	assert( v->data_as.d == da );
	ujvalue_release(&v);

	print("decode(array)\n");
	bot = (uint8_t*)"a\x00\x0c\x43\xaa\x74\x57\xbb\xbb\x66\x49\xcc\xcc\xcc\xcc";
	// [16#AA,true,16#BBBB,false,16#CCCCCCCC]
	v = decode(&bot);
	assert(v->type == uj_array);
	assert(v->data_as.array->size == 5);
	assert(v->data_as.array->values[0]->type == uj_number);
	assert(v->data_as.array->values[0]->numbertype == uj_uint8);
	assert(v->data_as.array->values[0]->data_as.uint8 == 0xAA);
	assert(v->data_as.array->values[1]->type == uj_true);
	assert(v->data_as.array->values[2]->type == uj_number);
	assert(v->data_as.array->values[2]->numbertype == uj_uint16);
	assert(v->data_as.array->values[2]->data_as.uint16 == 0xBBBB);
	assert(v->data_as.array->values[3]->type == uj_false);
	assert(v->data_as.array->values[4]->type == uj_number);
	assert(v->data_as.array->values[4]->numbertype == uj_uint32);
	assert(v->data_as.array->values[4]->data_as.uint32 == 0xCCCCCCCC);
	ujvalue_release(&v);

	print("decode(array of arrays)\n");
	bot = (uint8_t*)"a\x00\x0e\x61\x00\x04\x63\x01\x63\x02\x61\x00\x04\x63\x03\x63\x04";
	// [[1,2],[3,4]]
	v = decode(&bot);
	assert(v->type == uj_array);
	assert(v->data_as.array->size == 2);
	assert(v->data_as.array->values[0]->type == uj_array);
	assert(v->data_as.array->values[1]->type == uj_array);
	assert(v->data_as.array->values[0]->data_as.array->values[0]->type == uj_number);
	assert(v->data_as.array->values[0]->data_as.array->values[0]->numbertype == uj_int8);
	assert(v->data_as.array->values[0]->data_as.array->values[0]->data_as.int8 == 1);
	assert(v->data_as.array->values[0]->data_as.array->values[1]->type == uj_number);
	assert(v->data_as.array->values[0]->data_as.array->values[1]->numbertype == uj_int8);
	assert(v->data_as.array->values[0]->data_as.array->values[1]->data_as.int8 == 2);
	assert(v->data_as.array->values[1]->data_as.array->values[0]->type == uj_number);
	assert(v->data_as.array->values[1]->data_as.array->values[0]->numbertype == uj_int8);
	assert(v->data_as.array->values[1]->data_as.array->values[0]->data_as.int8 == 3);
	assert(v->data_as.array->values[1]->data_as.array->values[1]->type == uj_number);
	assert(v->data_as.array->values[1]->data_as.array->values[1]->numbertype == uj_int8);
	assert(v->data_as.array->values[1]->data_as.array->values[1]->data_as.int8 == 4);
	ujvalue_release(&v);

	/************************************* tojson *****************/

	{
	print("tojson functions\n");
	bot = (uint8_t*)"\x61\x00\x84\x73\x00\x05\x68\x65\x6C\x6C\x6F\x74\x63\x7B\x61\x00\x68\x77\x16\x2E\x6E\x73\x00\x05\x77\x6F\x72\x6C\x64\x61\x00\x06\x63\x05\x63\x06\x63\x07\x6F\x00\x50\x00\x07\x6B\x65\x79\x20\x6F\x6E\x65\x63\x2A\x00\x0A\x6E\x65\x73\x74\x65\x64\x20\x61\x72\x72\x61\x00\x06\x63\x01\x63\x02\x63\x03\x00\x0A\x73\x74\x72\x69\x6E\x67\x20\x6B\x65\x79\x73\x00\x0B\x73\x70\x65\x63\x74\x61\x63\x75\x6C\x61\x72\x00\x0A\x6E\x65\x73\x74\x65\x64\x20\x6F\x62\x6A\x6F\x00\x07\x00\x03\x79\x61\x79\x63\x45\x44\x45\x59\x87\xBF\x7C\xB8\xEC\x63\x64\x3F\x9D\xF3\xB6";
	#define TOJSON_OUTPUT "[\"hello\",true,123,[5678,null,\"world\",[5,6,7],{\"nested arr\":[1,2,3],\"string key\":\"spectacular\",\"nested obj\":{\"yay\":69},\"key one\":42}],123456789123456705438416896.00000000000000000,1.233999968]"
	#define TOJSON_WITH_TYPES_OUTPUT "[\"hello\",true,123/int8,[5678/int16,null,\"world\",[5/int8,6/int8,7/int8],{\"nested arr\":[1/int8,2/int8,3/int8],\"string key\":\"spectacular\",\"nested obj\":{\"yay\":69/int8},\"key one\":42/int8}],123456789123456705438416896.00000000000000000/double,1.233999968/float]"
	char jsonbuf[1024] = {0};
	v = decode(&bot);
	print("tojson()\n");
	tojson(jsonbuf, v);
	assert( strcmp(jsonbuf, TOJSON_OUTPUT) == 0 );
	print("tojson_with_types()\n");
	tojson_with_types(jsonbuf, v);
	assert( strcmp(jsonbuf, TOJSON_WITH_TYPES_OUTPUT) == 0 );
	}

	// TODO: more tests!

	print("Tests for ujson-c complete - PASS\n");
	return(0);
}



