/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_sorted_insert.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 15:33:08 by yforeau           #+#    #+#             */
/*   Updated: 2019/04/06 17:33:24 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_sorted_insert(t_list **lst, t_list *new, int (*cmp)())
{
	t_list	*cur;

	if (!lst || !new || !cmp)
		return ;
	if (!(cur = *lst) || cmp(cur->content, new->content) >= 0)
	{
		new->next = cur;
		*lst = new;
		return ;
	}
	while (cur->next && cmp(cur->next->content, new->content) < 0)
		cur = cur->next;
	new->next = cur->next;
	cur->next = new;
}
