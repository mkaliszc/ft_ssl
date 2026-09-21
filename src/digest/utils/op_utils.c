# include "ft_ssl.h"

t_operand	*new_op(char *name, t_src src) {
	t_operand	*return_ptr = malloc(sizeof(t_operand));

	if (return_ptr == NULL)
		return (NULL);
	return_ptr->input_name = name;
	return_ptr->src_type = src;
	return_ptr->next = NULL;

	return (return_ptr);
}

void	add_op(t_operand **lst, t_operand *new) {
	t_operand	*pos;

	if (new == NULL)
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
	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	new->next = *lst;
	lst = new;
}