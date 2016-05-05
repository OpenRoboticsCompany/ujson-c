#include <stdio.h>
#include <stdint.h>

#include "ujson.h"
#include "hexdump.h"
#include "endian.h"
#include "udp.h"

int main(int argc, char* argv[])
{
	printf("Running ujson test rig.\n");
	
	char testdata[] = "\0o\101CB";
	int tdlen = 5;
	hexdump(testdata, tdlen);

#define BUFLEN 55

	uint8_t buf[BUFLEN];
	int i;
	for (i = 0; i < BUFLEN; i++) buf[i] = 0;

	uint8_t* bp = buf;
	uint8_t* nextbuf;
	nextbuf = buf;

	char string[] = "ABCDEFG oh my god it's full of stars";
	printf("buffer before:\n");
	hexdump(buf, BUFLEN);
	printf("buffer after:\n");
	render_string(&nextbuf, string);
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
	float f = 1.0f;
	double d = 1.0f;

	printf("float 1.0 hexdump:\n");
	hexdump(&f, 4);
	printf("double 1.0 hexdump:\n");
	hexdump(&d, 8);
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
