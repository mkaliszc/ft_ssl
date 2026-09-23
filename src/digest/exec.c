# include "ft_ssl.h"

static int	feed_fd(t_options *opt, t_operand *op, t_digest_ctx *ctx, int fd) {
	uint8_t	buf[READ_SIZE];
	ssize_t	n;
	bool	keep = (op->src_type == SRC_STDIN && (opt->flags & FLAG_P));

	while ((n = read(fd, buf, READ_SIZE)) > 0) {
		opt->spec->update(ctx, buf, (size_t)n);
		if (keep && add_chunk(&op->chunks, buf, (size_t)n)) {
			printf_fd(2, "[ERROR] feed_fd : chunk malloc error.\n");
			return (1);
		}
	}
	if (n < 0) {
		printf_fd(2, "[ERROR] feed_fd : %s: %s\n", op->input_name, strerror(errno));
		return (1);
	}
	return (0);
}

static int	hash_operand(t_options *opt, t_operand *op, uint8_t *digest) {
	t_digest_ctx	ctx;
	int				fd;
	int				ret = 0;

	opt->spec->init(&ctx);
	if (op->src_type == SRC_STRING) {
		opt->spec->update(&ctx, (const uint8_t *)op->input_name,
			ft_strlen(op->input_name));
	}
	else if (op->src_type == SRC_STDIN) {
		ret = feed_fd(opt, op, &ctx, 0);
	}
	else {
		fd = open(op->input_name, O_RDONLY);
		if (fd < 0) {
			printf_fd(2, "[ERROR] hash_operand : %s: %s\n", op->input_name, strerror(errno));
			return (1);
		}
		ret = feed_fd(opt, op, &ctx, fd);
		close(fd);
	}
	if (ret)
		return (ret);
	opt->spec->final(&ctx, digest);
	return (0);
}

int	digest_exec(t_options *opt) {
	t_operand	*pos = opt->operands;
	uint8_t		digest[DIGEST_MAX_SIZE];
	int			ret = 0;

	while (pos) {
		if (hash_operand(opt, pos, digest))
			ret = 1;	// one source failed : report it, keep going (like openssl)
		else
			print_result(opt, pos, digest);
		pos = pos->next;
	}
	return (ret);
}
