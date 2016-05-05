#include <stdio.h>
#include <stdint.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "ujson.h"
#include "endian.h"

int main(int ARGC, char* ARGV[])
{
	printf("Running ujson-c tests...\n");

	uint16_t u16a, u16b;
	uint32_t u32a, u32b;
	uint64_t u64a, u64b;

	printf("_swap_16()\n");
	u16a = 0xDEAD;
	u16b = _swap_16( u16a );
	assert( u16b == 0xADDEu );

	printf("_swap_32()\n");
	u32a = 0xCAFEBABE;
	u32b = _swap_32( u32a );
	assert( u32b == 0xBEBAFECAu );

	// TODO: add ifdefs for machine width conditional compilation
	printf("_swap_64()\n");
	u64a = 0x5311AB1EC0DEBA5Eull;
	u64b = _swap_64( u64a );
	assert( u64b == 0x5EBADEC01EAB1153ull );

	printf("Tests for ujson-c complete.\n");
	return(0);
}
