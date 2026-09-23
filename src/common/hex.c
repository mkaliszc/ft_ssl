# include "ft_ssl.h"

/*
** Raw digest -> lowercase hex string.
** dst must hold len * 2 + 1 bytes. The caller keeps the display logic :
** this helper only formats, it never writes to a fd.
*/

void	digest_to_hex(const uint8_t *digest, size_t len, char *dst) {
	static const char	hex[] = "0123456789abcdef";
	size_t				i = 0;

	if (!digest || !dst)
		return ;
	while (i < len) {
		dst[i * 2] = hex[(digest[i] >> 4) & 0x0f];
		dst[i * 2 + 1] = hex[digest[i] & 0x0f];
		i++;
	}
	dst[len * 2] = '\0';
}
