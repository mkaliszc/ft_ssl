# include "ft_ssl.h"

void	free_op(t_operand **op) {
	t_operand	*pos;

	if (op == NULL)
		return ;
	while (*op) {
		pos = (*op)->next;
		free((*op)->data);
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
