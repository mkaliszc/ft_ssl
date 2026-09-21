# include "ft_ssl.h"

bool	handle_operand(char *name, t_src src, t_operand **operands) {
	t_operand	*new = new_op(name, src);

	if (!new) {
		printf_fd(2, "[ERROR] handle operand : new_op malloc error for %s\n", name);
		return 1;
	}
	if (src == SRC_STDIN) {
		add_front_op(operands, new);
	}
	else {
		add_op(operands, new);
	}
	return 0;
}