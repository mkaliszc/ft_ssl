# include "ft_ssl.h"

static t_operand	*new_op(char *name, t_src src) {
	t_operand	*return_ptr = malloc(sizeof(t_operand));

	if (return_ptr == NULL)
		return (NULL);
	return_ptr->input_name = name;
	return_ptr->chunks = NULL;
	return_ptr->src_type = src;
	return_ptr->next = NULL;

	return (return_ptr);
}

static void	add_op(t_operand **lst, t_operand *new) {
	t_operand	*pos;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	pos = *lst;
	while (pos->next != NULL)
		pos = pos->next;
	pos->next = new;
}

static void	add_front_op(t_operand **lst, t_operand *new) {
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}

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