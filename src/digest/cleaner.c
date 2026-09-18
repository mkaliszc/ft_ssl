# include "ft_ssl.h"

void	ssl_digest_cleaner(t_options *opt) {
	if (opt) {
		if (opt->operands) {
			free(opt->operands);
		}
		free(opt);
	}
	return;
}