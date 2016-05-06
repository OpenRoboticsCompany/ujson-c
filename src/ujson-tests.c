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

	print("Tests for ujson-c complete - PASS\n");
	return(0);
}
