# include "ft_ssl.h"

int	md5_func(int argc, char** argv) {
	t_options	*opt = malloc(sizeof(t_options));

	if (!opt) {
		printf_fd(2, "[ERROR] md5_func : Opt Malloc error.\n");
		return(-1);
	}
	opt->operands = NULL;
	ft_memset(&(opt->operands), 0 , argc);

	opt->flags = 0;
	opt->label = "MD5";

	if (digest_parser(argc, argv, opt)) {
		ssl_digest_cleaner(opt); // TODO : define this function
		return (1);
	}
	// exec function call
	return(0);
}