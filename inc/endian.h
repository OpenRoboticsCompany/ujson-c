#define _swap_16(x) \
	( ((x) & 0xff00u) >> 8 \
	| ((x) & 0x00ffu) << 8 )

#define _swap_32(x) \
	( ((x) & 0xff000000u) >> 24 \
	| ((x) & 0x00ff0000u) >> 8  \
	| ((x) & 0x0000ff00u) << 8  \
	| ((x) & 0x000000ffu) << 24 )

#define _swap_64(x) \
	( ((x) & 0xff00000000000000ull) >> 56 \
	| ((x) & 0x00ff000000000000ull) >> 40 \
	| ((x) & 0x0000ff0000000000ull) >> 24 \
	| ((x) & 0x000000ff00000000ull) >> 8  \
	| ((x) & 0x00000000ff000000ull) << 8  \
	| ((x) & 0x0000000000ff0000ull) << 24 \
	| ((x) & 0x000000000000ff00ull) << 40 \
	| ((x) & 0x00000000000000ffull) << 56 )

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define htoj16(x) (_swap_16(x))
	#define htoj32(x) (_swap_32(x))
	#define htoj64(x) (_swap_64(x))
	#define jtoh16(x) (_swap_16(x))
	#define jtoh32(x) (_swap_32(x))
	#define jtoh64(x) (_swap_64(x))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define htoj16(x) (x)
	#define htoj32(x) (x)
	#define htoj64(x) (x)
	#define jtoh16(x) (x)
	#define jtoh32(x) (x)
	#define jtoh64(x) (x)
#else
	#error "Macro __BYTE_ORDER__ has unexpected value or is undefined"
#endif

// This is for wacky ARM FPA doubles, which have both byte- and word-order considerations.
// Recommend RTFM if you are working with doubles on an FPA-flavoured ARM core.
// TODO: test this floating-point stuff thoroughly
#define _swap_fpa_64(x) \
	( ((x) & 0xffffffff00000000ull) >> 32 \
	| ((x) & 0x00000000ffffffffull) << 32 )

// These are helpers to convince C to treat a series of bytes that were once a float or uint
// as the other type, while avoiding the conversion inherent in C's typecasting. This can be
// somewhat tricky at the machine code level as well due to separate integer and floating-point
// registers, possibly needing a trip in and out of memory, or some funky cpu instructions.
//
// Warning: tangled typecasting and conversions below!
//
#ifdef __GNUC__
	#define _FUNC_MAYBE_NOT_USED __attribute__ ((unused))
#else
	#define _FUNC_MAYBE_NOT_USED
#endif

static float _FUNC_MAYBE_NOT_USED _bytewise_uint32_to_float( uint32_t u )
{
	return (float)*(float*)&u;
}
#define _bytewise_float_to_uint32(x) ( (uint32_t)*(uint32_t*)&x )

static float _FUNC_MAYBE_NOT_USED _bytewise_uint64_to_double( uint64_t u )
{
	return (double)*(double*)&u;
}
#define _bytewise_double_to_uint64(x) ( (uint64_t)*(uint64_t*)&x )

#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define _swap_word_endians(x) (x)
#elif __FLOAT_WORD_ORDER == __ORDER_BIG_ENDIAN__
	#define _swap_word_endians(x) (_swap_fpa_64(x))
#else
	#error "Macro __FLOAT_WORD_ORDER__ has unexpected value or is undefined"
#endif

// Only big-endian word order on little-endian byte order ARM FPA needs special handling
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define htojf(x) (_bytewise_uint32_to_float(_swap_32(_bytewise_float_to_uint32(x))))
	#define htojd(x) (_bytewise_uint64_to_double(_swap_64(_swap_word_endians(_bytewise_double_to_uint64(x)))))
	#define jtohf(x) (_bytewise_uint32_to_float(_swap_32(_bytewise_float_to_uint32(x))))
	#define jtohd(x) (_bytewise_uint64_to_double(_swap_word_endians(_swap_64(_bytewise_double_to_uint64(x)))))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define htojf(x) (x)
	#define htojd(x) (x)
	#define jtohf(x) (x)
	#define jtohd(x) (x)
#else
	#error "Macro __BYTE_ORDER__ has unexpected value or is undefined"
#endif




