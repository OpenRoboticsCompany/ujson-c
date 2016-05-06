#include <stdio.h>
#include <stdint.h>

#include "endian.h"
#include "ujson.h"

// TODO factor out below
void movebytes(uint8_t* to, uint8_t* from, uint16_t n)
{
	while ( n-- )
		to[n] = from[n];
}

void render_uint8(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = 'C';
	(*nextbuf)[1] = val;
	(*nextbuf) += 2;
}

void render_int8(uint8_t** nextbuf, uint8_t val)
{
	(*nextbuf)[0] = 'c';
	(*nextbuf)[1] = val;
	(*nextbuf) += 2;
}

void render_uint16(uint8_t** nextbuf, uint16_t val)
{
	val = htoj16(val);
	(*nextbuf)[0] = 'W';
	movebytes( &((*nextbuf)[1]), (uint8_t*)&val, 2 );
	(*nextbuf) += 3;
}

void render_int16(uint8_t** nextbuf, int16_t val)
{
	val = htoj16(val);
	(*nextbuf)[0] = 'w';
	(*nextbuf)[1] = ((uint8_t*)&val)[0];
	(*nextbuf)[2] = ((uint8_t*)&val)[1];
	(*nextbuf) += 3;
}

void render_uint32(uint8_t** nextbuf, uint32_t val)
{
	(*nextbuf)[0] = 'I';
	(*nextbuf)[1] = ((uint8_t*)&val)[0];
	(*nextbuf)[2] = ((uint8_t*)&val)[1];
	(*nextbuf)[3] = ((uint8_t*)&val)[2];
	(*nextbuf)[4] = ((uint8_t*)&val)[3];
	(*nextbuf) += 5;
}

void render_int32(uint8_t** nextbuf, int32_t val)
{
	val = htoj32(val);
	(*nextbuf)[0] = 'i';
	(*nextbuf)[1] = ((uint8_t*)&val)[0];
	(*nextbuf)[2] = ((uint8_t*)&val)[1];
	(*nextbuf)[3] = ((uint8_t*)&val)[2];
	(*nextbuf)[4] = ((uint8_t*)&val)[3];
	(*nextbuf) += 5;
}

void render_uint64(uint8_t** nextbuf, uint64_t val)
{
	val = htoj64(val);
	(*nextbuf)[0] = 'Q';
	(*nextbuf)[1] = ((uint8_t*)&val)[0];
	(*nextbuf)[2] = ((uint8_t*)&val)[1];
	(*nextbuf)[3] = ((uint8_t*)&val)[2];
	(*nextbuf)[4] = ((uint8_t*)&val)[3];
	(*nextbuf)[5] = ((uint8_t*)&val)[4];
	(*nextbuf)[6] = ((uint8_t*)&val)[5];
	(*nextbuf)[7] = ((uint8_t*)&val)[6];
	(*nextbuf)[8] = ((uint8_t*)&val)[7];
	(*nextbuf) += 9;
}

void render_int64(uint8_t** nextbuf, int64_t val)
{
	val = htoj64(val);
	(*nextbuf)[0] = 'q';
	(*nextbuf)[1] = ((uint8_t*)&val)[0];
	(*nextbuf)[2] = ((uint8_t*)&val)[1];
	(*nextbuf)[3] = ((uint8_t*)&val)[2];
	(*nextbuf)[4] = ((uint8_t*)&val)[3];
	(*nextbuf)[5] = ((uint8_t*)&val)[4];
	(*nextbuf)[6] = ((uint8_t*)&val)[5];
	(*nextbuf)[7] = ((uint8_t*)&val)[6];
	(*nextbuf)[8] = ((uint8_t*)&val)[7];
	(*nextbuf) += 9;
}

void render_string(uint8_t** nextbuf, char* str)
{
	uint16_t len = 0;
	while (str[len++]);
	len--;
	(*nextbuf)[0] = 's';
	uint16_t i = 0;
	for (i = 0; i < len; i++)
		(*nextbuf)[3+i] = ((uint8_t*)str)[i];
	(*nextbuf) += 3 + len;
	len = htoj16(len);
	(*nextbuf)[1] = ((uint8_t*)&len)[0];
	(*nextbuf)[2] = ((uint8_t*)&len)[1];
}

void render_float(uint8_t** nextbuf, float val)
{
	val = htojf(val);
	(*nextbuf)[0] = 'd';
	// TODO fill this out
}

void render_double(uint8_t** nextbuf, double val)
{
	val = htojd(val);
	(*nextbuf)[0] = 'D';
	// TODO fill this out
}


