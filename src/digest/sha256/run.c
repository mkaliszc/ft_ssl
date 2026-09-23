# include "ft_ssl.h"

static void	sha256_init(t_digest_ctx *ctx) {
	ctx->state.sha256[0] = 0x6a09e667;
	ctx->state.sha256[1] = 0xbb67ae85;
	ctx->state.sha256[2] = 0x3c6ef372;
	ctx->state.sha256[3] = 0xa54ff53a;
	ctx->state.sha256[4] = 0x510e527f;
	ctx->state.sha256[5] = 0x9b05688c;
	ctx->state.sha256[6] = 0x1f83d9ab;
	ctx->state.sha256[7] = 0x5be0cd19;
	ctx->buf_len = 0;
	ctx->total_len = 0;
}

static void	sha256_update(t_digest_ctx *ctx, const uint8_t *in, size_t len) {
	digest_update(ctx, ctx->state.sha256, sha256_transform, in, len);
}

static void	sha256_final(t_digest_ctx *ctx, uint8_t *out) {
	digest_pad(ctx, ctx->state.sha256, sha256_transform, store64_be);
	for (int i = 0; i < 8; i++)
		store32_be(out + i * 4, ctx->state.sha256[i]);
}

static const t_digest_spec	g_sha256_spec = {
	.label = "SHA256",
	.digest_size = 32,
	.init = sha256_init,
	.update = sha256_update,
	.final = sha256_final
};

int	sha256_func(int argc, char **argv) {
	t_options	*opt = malloc(sizeof(t_options));

	if (!opt) {
		printf_fd(2, "[ERROR] sha256_func : Opt Malloc error.\n");
		return(-1);
	}
	opt->operands = NULL;
	opt->flags = 0;
	opt->spec = &g_sha256_spec;

	if (digest_parser(argc, argv, opt)) {
		ssl_digest_cleaner(opt);
		return (1);
	}
	digest_exec(opt);
	return(ssl_digest_cleaner(opt), 0);
}
