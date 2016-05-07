/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Aaron Kondziela <aaron@aaronkondziela.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

 /*
  * Part of hexdump - Simple C function to print some memory as a
  * standard hexdump
  *
  * https://github.com/aaronkondziela/hexdump/
  *
  * hexdump.c
  *
  */

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
		if (i+1 == size && (i+1) % 16 != 0) {
			txt[(i+1) % 16] = '\0';
			for (j = (i+1) % 16; j < 16; j++) printf("   ");
			if ((i+1) % 16 <= 8) printf(" ");
		}
		if ((i+1) % 16 == 0 || i+1 == size) printf("|%s|\n", txt);
	}
	return;
}
