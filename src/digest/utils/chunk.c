# include "ft_ssl.h"

bool	add_chunk(t_chunk **lst, const uint8_t *data, size_t len) {
	t_chunk	*new = malloc(sizeof(t_chunk));
	t_chunk	*pos;

	if (!new)
		return (1);
	ft_memcpy(new->data, data, len);
	new->len = len;
	new->next = NULL;
	if (!*lst) {
		*lst = new;
		return (0);
	}
	pos = *lst;
	while (pos->next)
		pos = pos->next;
	pos->next = new;
	return (0);
}

void	free_chunks(t_chunk **lst) {
	t_chunk	*next;

	if (!lst)
		return ;
	while (*lst) {
		next = (*lst)->next;
		free(*lst);
		*lst = next;
	}
}
