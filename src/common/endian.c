# include "ft_ssl.h"

/*
** Words are always rebuilt byte by byte : casting the buffer to uint32_t *
** would depend on the host endianness and on the pointer alignment.
*/

uint32_t	load32_le(const uint8_t *src) {
	return ((uint32_t)src[0]
		| ((uint32_t)src[1] << 8)
		| ((uint32_t)src[2] << 16)
		| ((uint32_t)src[3] << 24));
}

uint32_t	load32_be(const uint8_t *src) {
	return (((uint32_t)src[0] << 24)
		| ((uint32_t)src[1] << 16)
		| ((uint32_t)src[2] << 8)
		| (uint32_t)src[3]);
}

void	store32_le(uint8_t *dst, uint32_t value) {
	dst[0] = (uint8_t)(value & 0xff);
	dst[1] = (uint8_t)((value >> 8) & 0xff);
	dst[2] = (uint8_t)((value >> 16) & 0xff);
	dst[3] = (uint8_t)((value >> 24) & 0xff);
}

void	store32_be(uint8_t *dst, uint32_t value) {
	dst[0] = (uint8_t)((value >> 24) & 0xff);
	dst[1] = (uint8_t)((value >> 16) & 0xff);
	dst[2] = (uint8_t)((value >> 8) & 0xff);
	dst[3] = (uint8_t)(value & 0xff);
}

// Length field of the padding : MD5 stores it little-endian
void	store64_le(uint8_t *dst, uint64_t value) {
	for (int i = 0; i < 8; i++)
		dst[i] = (uint8_t)((value >> (8 * i)) & 0xff);
}

// Length field of the padding : SHA-256 stores it big-endian
void	store64_be(uint8_t *dst, uint64_t value) {
	for (int i = 0; i < 8; i++)
		dst[i] = (uint8_t)((value >> (56 - 8 * i)) & 0xff);
}
