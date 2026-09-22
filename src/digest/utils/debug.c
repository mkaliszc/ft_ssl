# include "ft_ssl.h"

static const char	*src_name(t_src src) {
	if (src == SRC_STDIN)
		return ("SRC_STDIN");
	if (src == SRC_FILE)
		return ("SRC_FILE");
	if (src == SRC_STRING)
		return ("SRC_STRING");
	return ("SRC_UNKNOWN");
}

void	print_op_lst(t_operand *lst) {
	int	i = 0;

	if (lst == NULL) {
		ft_printf("[DEBUG] operand list : empty\n");
		return ;
	}
	while (lst) {
		ft_printf("[DEBUG] operand %d : name = \"%s\" | src = %s\n",
			i++, lst->input_name, src_name(lst->src_type));
		lst = lst->next;
	}
}