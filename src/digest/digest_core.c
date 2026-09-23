# include "ft_ssl.h"

/*
** Buffering and padding shared by every "64-byte block / 32-bit word"
** algorithm. Each algorithm only provides its transform, its state and the
** endianness of the length field : the logic itself is written once.
*/

void	digest_update(t_digest_ctx *ctx, uint32_t *state, t_transform tf,
			const uint8_t *in, size_t len) {
	size_t	fill;

	ctx->total_len += len;

	// 1. complete the pending block first, if there is one
	if (ctx->buf_len > 0) {
		fill = BLOCK_SIZE - ctx->buf_len;
		if (len < fill) {
			ft_memcpy(ctx->buffer + ctx->buf_len, in, len);
			ctx->buf_len += len;
			return ;
		}
		ft_memcpy(ctx->buffer + ctx->buf_len, in, fill);
		tf(state, ctx->buffer);
		ctx->buf_len = 0;
		in += fill;
		len -= fill;
	}

	// 2. full blocks are transformed straight from the input, no copy
	while (len >= BLOCK_SIZE) {
		tf(state, in);
		in += BLOCK_SIZE;
		len -= BLOCK_SIZE;
	}

	// 3. keep the tail (< 64 bytes) for the next update or for final
	ft_memcpy(ctx->buffer, in, len);
	ctx->buf_len = len;
}

void	digest_pad(t_digest_ctx *ctx, uint32_t *state, t_transform tf,
			t_store64 store_len) {
	uint64_t	bit_len = ctx->total_len * 8;

	// 1. mandatory '1' bit right after the message
	ctx->buffer[ctx->buf_len++] = 0x80;

	// 2. no room left for the 8-byte length : pad this block, open a new one
	if (ctx->buf_len > LEN_OFFSET) {
		ft_memset(ctx->buffer + ctx->buf_len, 0, BLOCK_SIZE - ctx->buf_len);
		tf(state, ctx->buffer);
		ctx->buf_len = 0;
	}

	// 3. zeros up to byte 56, then the message length in bits
	ft_memset(ctx->buffer + ctx->buf_len, 0, LEN_OFFSET - ctx->buf_len);
	store_len(ctx->buffer + LEN_OFFSET, bit_len);
	tf(state, ctx->buffer);
	ctx->buf_len = 0;
}
