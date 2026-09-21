#pragma once

# include "../OctoLIB/include/libft.h"
# include <stdint.h>

// Used for comparing bitmask information
# define FLAG_P		(1 << 0)
# define FLAG_Q		(1 << 1)
# define FLAG_R		(1 << 2)

// Used to be more clearer, not big signification
# define IS_FLAG	1
# define IS_FLAG_S	2
# define IS_TOK		3

# define COMMAND_INFO "[INFO] Avaible command : md5, sha256\n"

typedef enum e_src
{
	SRC_STDIN,
	SRC_FILE,
	SRC_STRING
}	t_src;

typedef struct s_command_ssl {
	char	*func_name;
	int		(*run)(int argc, char **argv);	
}	t_command_ssl;

typedef struct s_operand {
	char				*input_name; // File name or String
	char				*data; // Used for store data when name is needed
	size_t				len;
	t_src				src_type;
	struct s_operand	*next;
}	t_operand;

typedef struct s_options {
	uint8_t		flags;
	char		*label;
	t_operand	*operands;
}	t_options;

// General
void	dispatch_cmd(int argc, char **argv);

// Digest familly
bool	digest_parser(int argc, char **argv, t_options *opt);
bool	handle_operand(char *name, t_src src, t_operand **operands);
void	free_op(t_operand **op);
void	ssl_digest_cleaner(t_options *opt);

// MD5
int		md5_func(int argc, char **argv);

// sha256
int		sha256_func(int argc, char **argv);

// Utils
t_operand	*new_op(char *name, t_src src);
void		add_op(t_operand **lst, t_operand *new);
void		add_front_op(t_operand **lst, t_operand *new);
void		print_op_lst(t_operand *lst);