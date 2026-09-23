# include "ft_ssl.h"

static void	print_chunks(t_chunk *lst, bool strip_newline) {
	size_t	len;

	while (lst) {
		len = lst->len;
		if (strip_newline && !lst->next && len > 0 && lst->data[len - 1] == '\n')
			len--;
		write(1, lst->data, len);
		lst = lst->next;
	}
}

static bool	ends_with_newline(t_chunk *lst) {
	if (!lst)
		return (true);	// nothing echoed : no newline to add
	while (lst->next)
		lst = lst->next;
	return (lst->len > 0 && lst->data[lst->len - 1] == '\n');
}

static void	print_stdin(t_options *opt, t_operand *op, const char *hex) {
	bool	echo = opt->flags & FLAG_P;

	if (opt->flags & FLAG_Q) {
		if (echo) {
			print_chunks(op->chunks, false);
			if (!ends_with_newline(op->chunks))
				write(1, "\n", 1);
		}
		ft_printf("%s\n", hex);
	}
	else if (echo) {
		write(1, "(\"", 2);
		print_chunks(op->chunks, true);
		ft_printf("\")= %s\n", hex);
	}
	else
		ft_printf("(stdin)= %s\n", hex);
}

void	print_result(t_options *opt, t_operand *op, const uint8_t *digest) {
	char		hex[DIGEST_MAX_SIZE * 2 + 1];
	const char	*quote = (op->src_type == SRC_STRING) ? "\"" : "";

	digest_to_hex(digest, opt->spec->digest_size, hex);
	if (op->src_type == SRC_STDIN)
		print_stdin(opt, op, hex);
	else if (opt->flags & FLAG_Q)
		ft_printf("%s\n", hex);
	else if (opt->flags & FLAG_R)
		ft_printf("%s %s%s%s\n", hex, quote, op->input_name, quote);
	else
		ft_printf("%s (%s%s%s) = %s\n", opt->spec->label, quote, op->input_name, quote, hex);
}
