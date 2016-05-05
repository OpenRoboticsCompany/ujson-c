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
// TODO: test this thoroughly
#define _swap_fpa_64(x) \
	( ((x) & 0xffffffff00000000ull) >> 32 \
	| ((x) & 0x00000000ffffffffull) << 32 )

#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define _swap_word_endians(x) (x)
#elif __FLOAT_WORD_ORDER == __ORDER_BIG_ENDIAN__
	#define _swap_word_endians(x) (_swap_fpa_64(x))
#else
	#error "Macro __FLOAT_WORD_ORDER__ has unexpected value or is undefined"
#endif

// Only big-endian word order on little-endian byte order ARM FPA needs special handling
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define htojf(x) (_swap_32(x))
	#define htojd(x) (_swap_64(_swap_word_endians(x)))
	#define jtohf(x) (_swap_32(x))
	#define jtohd(x) (_swap_word_endians(_swap_64(x)))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define htojf(x) (x)
	#define htojd(x) (x)
	#define jtohf(x) (x)
	#define jtohd(x) (x)
#else
	#error "Macro __BYTE_ORDER__ has unexpected value or is undefined"
#endif




