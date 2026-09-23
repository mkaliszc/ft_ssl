# include "ft_ssl.h"

static void	free_op(t_operand **op) {
	t_operand	*pos;

	if (op == NULL)
		return ;
	while (*op) {
		pos = (*op)->next;
		free_chunks(&(*op)->chunks);
		free(*op);
		*op = pos;
	}
}

void	ssl_digest_cleaner(t_options *opt) {
	if (opt) {
		free_op(&opt->operands);
		free(opt);
	}
	ft_printf("everything alright\n");
	return;
}
