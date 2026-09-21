# include "ft_ssl.h"

t_operand	*new_op(char *name, t_src src) {
	t_operand	*return_ptr = malloc(sizeof(t_operand));

	if (return_ptr == NULL)
		return (NULL);
	return_ptr->input_name = name;
	return_ptr->data = NULL;
	return_ptr->len = 0;
	return_ptr->src_type = src;
	return_ptr->next = NULL;

	return (return_ptr);
}

void	add_op(t_operand **lst, t_operand *new) {
	t_operand	*pos;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	pos = *lst;
	while (pos -> next != NULL)
		pos = pos -> next;
	pos->next = new;
}

void	add_front_op(t_operand **lst, t_operand *new) {
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}

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