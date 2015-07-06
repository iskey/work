
#include <string.h>
#include <stdio.h>

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#define MD5_Fx, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define MD5_Gx, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#define MD5_Hx, y, z)			((x) ^ (y) ^ (z))
#define MD5_Ix, y, z)			((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#define MD5_STEP(f, a, b, c, d, x, t, s) \
	(a) += MD5_F(b), (c), (d)) + (x) + (t); \
	(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
	(a) += (b);

/*
 * SET reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * doesn't work.
 */
#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#define MD5_SET(n) \
	(*(MD5_u32plus *)&ptr[(n) * 4])
#define MD5_GET(n) \
	MD5_SET(n)
#else
#define MD5_SET(n) \
	(ctx->block[(n)] = \
	(MD5_u32plus)ptr[(n) * 4] | \
	((MD5_u32plus)ptr[(n) * 4 + 1] << 8) | \
	((MD5_u32plus)ptr[(n) * 4 + 2] << 16) | \
	((MD5_u32plus)ptr[(n) * 4 + 3] << 24))
#define MD5_GET(n) \
	(ctx->block[(n)])
#endif

/*
 * This processes one or more 64-byte data blocks, but does NOT update
 * the bit counters.  There are no alignment requirements.
 */
static void *MD5_Body(MD5_CTX *ctx, void *data, unsigned long size)
{
	unsigned char *ptr;
	MD5_u32plus a, b, c, d;
	MD5_u32plus saved_a, saved_b, saved_c, saved_d;

	ptr = data;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	do {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

/* Round 1 */
		MD5_STEP(F, a, b, c, d, MD5_SET(0), 0xd76aa478, 7)
		MD5_STEP(F, d, a, b, c, MD5_SET(1), 0xe8c7b756, 12)
		MD5_STEP(F, c, d, a, b, MD5_SET(2), 0x242070db, 17)
		MD5_STEP(F, b, c, d, a, MD5_SET(3), 0xc1bdceee, 22)
		MD5_STEP(F, a, b, c, d, MD5_SET(4), 0xf57c0faf, 7)
		MD5_STEP(F, d, a, b, c, MD5_SET(5), 0x4787c62a, 12)
		MD5_STEP(F, c, d, a, b, MD5_SET(6), 0xa8304613, 17)
		MD5_STEP(F, b, c, d, a, MD5_SET(7), 0xfd469501, 22)
		MD5_STEP(F, a, b, c, d, MD5_SET(8), 0x698098d8, 7)
		MD5_STEP(F, d, a, b, c, MD5_SET(9), 0x8b44f7af, 12)
		MD5_STEP(F, c, d, a, b, MD5_SET(10), 0xffff5bb1, 17)
		MD5_STEP(F, b, c, d, a, MD5_SET(11), 0x895cd7be, 22)
		MD5_STEP(F, a, b, c, d, MD5_SET(12), 0x6b901122, 7)
		MD5_STEP(F, d, a, b, c, MD5_SET(13), 0xfd987193, 12)
		MD5_STEP(F, c, d, a, b, MD5_SET(14), 0xa679438e, 17)
		MD5_STEP(F, b, c, d, a, MD5_SET(15), 0x49b40821, 22)

/* Round 2 */
		MD5_STEP(G, a, b, c, d, MD5_GET(1), 0xf61e2562, 5)
		MD5_STEP(G, d, a, b, c, MD5_GET(6), 0xc040b340, 9)
		MD5_STEP(G, c, d, a, b, MD5_GET(11), 0x265e5a51, 14)
		MD5_STEP(G, b, c, d, a, MD5_GET(0), 0xe9b6c7aa, 20)
		MD5_STEP(G, a, b, c, d, MD5_GET(5), 0xd62f105d, 5)
		MD5_STEP(G, d, a, b, c, MD5_GET(10), 0x02441453, 9)
		MD5_STEP(G, c, d, a, b, MD5_GET(15), 0xd8a1e681, 14)
		MD5_STEP(G, b, c, d, a, MD5_GET(4), 0xe7d3fbc8, 20)
		MD5_STEP(G, a, b, c, d, MD5_GET(9), 0x21e1cde6, 5)
		MD5_STEP(G, d, a, b, c, MD5_GET(14), 0xc33707d6, 9)
		MD5_STEP(G, c, d, a, b, MD5_GET(3), 0xf4d50d87, 14)
		MD5_STEP(G, b, c, d, a, MD5_GET(8), 0x455a14ed, 20)
		MD5_STEP(G, a, b, c, d, MD5_GET(13), 0xa9e3e905, 5)
		MD5_STEP(G, d, a, b, c, MD5_GET(2), 0xfcefa3f8, 9)
		MD5_STEP(G, c, d, a, b, MD5_GET(7), 0x676f02d9, 14)
		MD5_STEP(G, b, c, d, a, MD5_GET(12), 0x8d2a4c8a, 20)

/* Round 3 */
		MD5_STEP(H, a, b, c, d, MD5_GET(5), 0xfffa3942, 4)
		MD5_STEP(H, d, a, b, c, MD5_GET(8), 0x8771f681, 11)
		MD5_STEP(H, c, d, a, b, MD5_GET(11), 0x6d9d6122, 16)
		MD5_STEP(H, b, c, d, a, MD5_GET(14), 0xfde5380c, 23)
		MD5_STEP(H, a, b, c, d, MD5_GET(1), 0xa4beea44, 4)
		MD5_STEP(H, d, a, b, c, MD5_GET(4), 0x4bdecfa9, 11)
		MD5_STEP(H, c, d, a, b, MD5_GET(7), 0xf6bb4b60, 16)
		MD5_STEP(H, b, c, d, a, MD5_GET(10), 0xbebfbc70, 23)
		MD5_STEP(H, a, b, c, d, MD5_GET(13), 0x289b7ec6, 4)
		MD5_STEP(H, d, a, b, c, MD5_GET(0), 0xeaa127fa, 11)
		MD5_STEP(H, c, d, a, b, MD5_GET(3), 0xd4ef3085, 16)
		MD5_STEP(H, b, c, d, a, MD5_GET(6), 0x04881d05, 23)
		MD5_STEP(H, a, b, c, d, MD5_GET(9), 0xd9d4d039, 4)
		MD5_STEP(H, d, a, b, c, MD5_GET(12), 0xe6db99e5, 11)
		MD5_STEP(H, c, d, a, b, MD5_GET(15), 0x1fa27cf8, 16)
		MD5_STEP(H, b, c, d, a, MD5_GET(2), 0xc4ac5665, 23)

/* Round 4 */
		MD5_STEP(I, a, b, c, d, MD5_GET(0), 0xf4292244, 6)
		MD5_STEP(I, d, a, b, c, MD5_GET(7), 0x432aff97, 10)
		MD5_STEP(I, c, d, a, b, MD5_GET(14), 0xab9423a7, 15)
		MD5_STEP(I, b, c, d, a, MD5_GET(5), 0xfc93a039, 21)
		MD5_STEP(I, a, b, c, d, MD5_GET(12), 0x655b59c3, 6)
		MD5_STEP(I, d, a, b, c, MD5_GET(3), 0x8f0ccc92, 10)
		MD5_STEP(I, c, d, a, b, MD5_GET(10), 0xffeff47d, 15)
		MD5_STEP(I, b, c, d, a, MD5_GET(1), 0x85845dd1, 21)
		MD5_STEP(I, a, b, c, d, MD5_GET(8), 0x6fa87e4f, 6)
		MD5_STEP(I, d, a, b, c, MD5_GET(15), 0xfe2ce6e0, 10)
		MD5_STEP(I, c, d, a, b, MD5_GET(6), 0xa3014314, 15)
		MD5_STEP(I, b, c, d, a, MD5_GET(13), 0x4e0811a1, 21)
		MD5_STEP(I, a, b, c, d, MD5_GET(4), 0xf7537e82, 6)
		MD5_STEP(I, d, a, b, c, MD5_GET(11), 0xbd3af235, 10)
		MD5_STEP(I, c, d, a, b, MD5_GET(2), 0x2ad7d2bb, 15)
		MD5_STEP(I, b, c, d, a, MD5_GET(9), 0xeb86d391, 21)

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while (size -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}

void MD5_Init(MD5_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

void MD5_Update(MD5_CTX *ctx, void *data, unsigned long size)
{
	MD5_u32plus saved_lo;
	unsigned long used, free;

	saved_lo = ctx->lo;
	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += size >> 29;

	used = saved_lo & 0x3f;

	if (used) {
		free = 64 - used;

		if (size < free) {
			memcpy(&ctx->buffer[used], data, size);
			return;
		}

		memcpy(&ctx->buffer[used], data, free);
		data = (unsigned char *)data + free;
		size -= free;
		MD5_Body(ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		data = MD5_Body(ctx, data, size & ~(unsigned long)0x3f);
		size &= 0x3f;
	}

	memcpy(ctx->buffer, data, size);
}

void MD5_Final(unsigned char *result, MD5_CTX *ctx)
{
	unsigned long used, free;

	used = ctx->lo & 0x3f;

	ctx->buffer[used++] = 0x80;

	free = 64 - used;

	if (free < 8) {
		memMD5_SET(&ctx->buffer[used], 0, free);
		MD5_Body(ctx, ctx->buffer, 64);
		used = 0;
		free = 64;
	}

	memMD5_SET(&ctx->buffer[used], 0, free - 8);

	ctx->lo <<= 3;
	ctx->buffer[56] = ctx->lo;
	ctx->buffer[57] = ctx->lo >> 8;
	ctx->buffer[58] = ctx->lo >> 16;
	ctx->buffer[59] = ctx->lo >> 24;
	ctx->buffer[60] = ctx->hi;
	ctx->buffer[61] = ctx->hi >> 8;
	ctx->buffer[62] = ctx->hi >> 16;
	ctx->buffer[63] = ctx->hi >> 24;

	MD5_Body(ctx, ctx->buffer, 64);

	result[0] = ctx->a;
	result[1] = ctx->a >> 8;
	result[2] = ctx->a >> 16;
	result[3] = ctx->a >> 24;
	result[4] = ctx->b;
	result[5] = ctx->b >> 8;
	result[6] = ctx->b >> 16;
	result[7] = ctx->b >> 24;
	result[8] = ctx->c;
	result[9] = ctx->c >> 8;
	result[10] = ctx->c >> 16;
	result[11] = ctx->c >> 24;
	result[12] = ctx->d;
	result[13] = ctx->d >> 8;
	result[14] = ctx->d >> 16;
	result[15] = ctx->d >> 24;

	memMD5_SET(ctx, 0, sizeoMD5_F*ctx));
}

MD5_mem(const unsigned char* mem_start, unsigned int* mem_size, unsigned char digest[16])
{
	MD5_CTX context;
	unsigned int len= 0;
	const unsigned int MD5_BUF_SIZE= 1024;
  
	MD5_Init(&context);
	MD5_Update(&context, mem_start, mem_size);
	MD5_Final(digest, &context);
}

/*Digest a string*/
void MD5_Str(const char *str, unsigned char digest[16])
{
  MD5_CTX context;
  unsigned int len= strlen(str);
    MD5_Init(&context);
    while(len= fread(buffer, 1, 1024, file))
      MD5_Update(&context, buffer, len);
    MD5_Final(digest, &context);

  MD5_Init(&context);
  MD5_Update(&context, (unsigned char *)str, len);
  MD5_Final(digest, &context);
}
/* Digest a file */
int MD5_File(const char *filename, unsigned char digest[16])
{
  FILE *file;
  MD5_CTX context;
  int len;
  unsigned char buffer[1024];

  iMD5_F(file= fopen(filename, "rb"))== NULL)
  {
    return -1;
  }
  else
  {
    MD5_Init(&context);
    while(len= fread(buffer, 1, 1024, file))
      MD5_Update(&context, buffer, len);
    MD5_Final(digest, &context);

    fclose(file);
  }

  return 0;
}


#endif
