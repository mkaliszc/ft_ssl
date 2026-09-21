# include "ft_ssl.h"

int	flag_parser(char *str, t_options *opt) {
	char *valid_flag[4] = {"-p", "-q", "-r", "-s"};

	for (size_t i = 0; i < 3; i++)
	{
		if (ft_strcmp(str, valid_flag[i]) == 0) {
			opt->flags |= (1 << i);
			return(IS_FLAG);
		}
	}
	if (ft_strcmp(str, valid_flag[3]) == 0) {
		return(IS_FLAG_S);
	}
	return(IS_TOK);
}

bool digest_parser(int argc, char **argv, t_options *opt) {
	int		i = 2;
	int		test = 0;

	while(argv[i] && test != IS_TOK) {
		test = flag_parser(argv[i], opt);

		if (test == IS_FLAG) {
			i++;
		}
		else if (test == IS_FLAG_S) {
			i++;
			if (!argv[i]) {
				printf_fd(2, "[ERROR] digest_parser : missing argument for -s flag.\n");
				return 1; // a changer en fonction de l'evolution du code
			}
			handle_operand(argv[i], SRC_STRING, &opt->operands);
			i++;
		}
	}

	if (opt->flags & FLAG_P || (i == argc && !opt->operands)) {
		handle_operand("stdin", SRC_STDIN, &opt->operands);
	}
	while (argv[i] && i < argc) {
			handle_operand(argv[i++], SRC_FILE, &opt->operands);
		}
	ft_printf("Digest parser done\n");
	print_op_lst(opt->operands);
	return 0; // a changer en fonction de l'evolution du code
}