# include "ft_ssl.h"

void	ssl_digest_cleaner(t_options *opt) {
	if (opt) {
		if (opt->operands) {
			free_op(opt->operands);
		}
		free(opt);
	}
	return;
}

void	free_op(t_operand **op) {
	if (op && *op) {
		t_operand	*pos;

		while(*op) {
			pos = (*op)->next;
			free(*op);
			*op = pos;
		}
	}
}