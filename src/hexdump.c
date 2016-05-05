// a hexdumper by Aaron Kondziela <aaron@aaronkondziela.com>

#include <stdio.h>

void hexdump(const void* d, size_t size)
{
	const unsigned char* data = (unsigned char*)d;
	unsigned char txt[17] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	size_t i;
	unsigned char j;
	for (i = 0; i < size; i++) {
		if (i % 16 == 0) printf("%08X  ", i);
		printf("%02X ", data[i]);
		txt[i % 16] = (data[i] >= 0x20 && data[i] <= 0x7E) ? data[i] : '.';
		if ((i+1) % 8 == 0 || i+1 == size) printf(" ");
		if (i+1 == size && (i+1) % 16 != 0 ) {
			txt[(i+1) % 16] = '\0';
			for (j = (i+1) % 16; j < 16; j++) printf("   ");
			if ((i+1) % 16 <= 8) printf(" ");
		}
		if ((i+1) % 16 == 0 || i+1 == size) printf("|%s|\n", txt);
	}
	return;
}
