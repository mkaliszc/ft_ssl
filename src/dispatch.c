# include "ft_ssl.h"

static const t_command_ssl command_list[] = { 
	{"md5", md5_func},
	{"sha256", sha256_func},
	{NULL, NULL}
};


void	dispatch_cmd(int argc, char **argv) {
	int i = 0;

	while(command_list[i].func_name) {
		if (ft_strcmp(argv[1], command_list[i].func_name)) {
			command_list[i].run(argc, argv);
			exit(0);
		}
		i++;
	}
	printf_fd(2, "[INFO] dispatch_cmd : %s doesn't exist in this version.\n");
	ft_printf(COMMAND_INFO);
	exit(0);
}