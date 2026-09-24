# include "ft_ssl.h"

int	main(int argc, char **argv) {
	if (argc == 1) {
		ft_printf("[INFO] Usage: ft_ssl command [flags] [file/string]\n");
		ft_printf(COMMAND_INFO);
		ft_printf(FLAGS_INFO);
		return(1);
	}
	else {
		dispatch_cmd(argc, argv);
	}
	return(0);
}