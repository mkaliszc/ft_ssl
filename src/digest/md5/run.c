# include "ft_ssl.h"

static void	md5_init(t_digest_ctx *ctx) {
	ctx->state.md5[0] = 0x67452301;
	ctx->state.md5[1] = 0xefcdab89;
	ctx->state.md5[2] = 0x98badcfe;
	ctx->state.md5[3] = 0x10325476;
	ctx->buf_len = 0;
	ctx->total_len = 0;
}

static void	md5_update(t_digest_ctx *ctx, const uint8_t *in, size_t len) {
	digest_update(ctx, ctx->state.md5, md5_transform, in, len);
}

static void	md5_final(t_digest_ctx *ctx, uint8_t *out) {
	digest_pad(ctx, ctx->state.md5, md5_transform, store64_le);
	for (int i = 0; i < 4; i++)
		store32_le(out + i * 4, ctx->state.md5[i]);
}

static const t_digest_spec	g_md5_spec = {
	.label = "MD5",
	.digest_size = 16,
	.init = md5_init,
	.update = md5_update,
	.final = md5_final
};

int	md5_func(int argc, char** argv) {
	t_options	*opt = malloc(sizeof(t_options));

	if (!opt) {
		printf_fd(2, "[ERROR] md5_func : Opt Malloc error.\n");
		return(-1);
	}
	opt->operands = NULL;
	opt->flags = 0;
	opt->spec = &g_md5_spec;

	if (digest_parser(argc, argv, opt)) {
		ssl_digest_cleaner(opt);
		return (1);
	}
	digest_exec(opt);
	return(ssl_digest_cleaner(opt), 0);
}
