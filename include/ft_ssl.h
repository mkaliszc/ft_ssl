#pragma once

# include "../OctoLIB/include/libft.h"

# define COMMAND_INFO "[INFO] Avaible command : md5, sha256\n"
# define FLAGS_MD5 "[INFO] Avaible flags (optionnal) :\n\t-p = echo STDIN to STDOUT and append the checksum to STDOUT.\n\t-q = quiet mode.\n\t-r = reverse the format of the output.\n\t-s = print the sum of the given string.\n"
# define FLAGS_SHA256 "[INFO] Avaible flags (optionnal) :\n\t-p = echo STDIN to STDOUT and append the checksum to STDOUT.\n\t-q = quiet mode.\n\t-r = reverse the format of the output.\n\t-s = print the sum of the given string.\n"

typedef enum e_input_type {
	FILE,
	STDIN,
	STRING
} t_input_type;

typedef struct s_command_ssl {
	char	*func_name;
	int		(*run)(int argc, char **argv);
	
} t_command_ssl;

typedef struct s_input {
	char			*input_name;
	t_input_type	input_type;
} t_input;

// General
void	dispatch_cmd(int argc, char **argv);

// MD5
int		md5_func(int argc, char **argv);

// sha256
int		sha256_func(int argc, char **argv);