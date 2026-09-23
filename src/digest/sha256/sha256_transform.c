# include "ft_ssl.h"

/*
 SHA-256 round constants : the first 32 bits of the fractional parts of the
 cube roots of the first 64 primes. Hardcoded (no libm allowed).
*/
static const uint32_t	g_sha256_k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32_t	rotr32(uint32_t x, uint32_t n) {
	return ((x >> n) | (x << (32 - n)));
}

void	sha256_transform(uint32_t state[8], const uint8_t block[64]) {
	uint32_t	w[64];
	uint32_t	a = state[0];
	uint32_t	b = state[1];
	uint32_t	c = state[2];
	uint32_t	d = state[3];
	uint32_t	e = state[4];
	uint32_t	f = state[5];
	uint32_t	g = state[6];
	uint32_t	h = state[7];
	uint32_t	s0;
	uint32_t	s1;
	uint32_t	ch;
	uint32_t	maj;
	uint32_t	t1;
	uint32_t	t2;

	// --- message words : the block gives the first 16 big-endian words ---
	for (int i = 0; i < 16; i++)
		w[i] = load32_be(block + i * 4);

	// --- message schedule : the 48 remaining words are derived from them ---
	for (int i = 16; i < 64; i++) {
		s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
		s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
	}

	// --- 64 rounds ---
	for (int i = 0; i < 64; i++) {
		s1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
		ch = (e & f) ^ (~e & g);
		t1 = h + s1 + ch + g_sha256_k[i] + w[i];
		s0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
		maj = (a & b) ^ (a & c) ^ (b & c);
		t2 = s0 + maj;

		// --- register rotation : everything shifts down, a and e get t1/t2
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	// --- feed-forward : the block output is added to the incoming state ---
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}
