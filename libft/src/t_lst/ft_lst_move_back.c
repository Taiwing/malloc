/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_move_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 11:00:13 by yforeau           #+#    #+#             */
/*   Updated: 2019/11/22 11:37:02 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_move_back(t_list **lst, t_list *elem)
{
	t_list *prev;

	if (!lst || !*lst || !elem)
		return ;
	if ((prev = *lst) != elem)
		while (prev && prev->next != elem)
			prev = prev->next;
	if (!prev || !elem->next)
		return ;
	if (prev == elem)
		*lst = prev->next;
	else
		prev->next = elem->next;
	while (prev->next)
		prev = prev->next;
	prev->next = elem;
	elem->next = NULL;
}
