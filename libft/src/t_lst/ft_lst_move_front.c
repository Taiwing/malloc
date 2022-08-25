/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_move_front.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:43:54 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 15:35:17 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_move_front(t_list **lst, t_list *elem)
{
	t_list *prev;

	if (!lst || !*lst || !elem || *lst == elem)
		return ;
	prev = *lst;
	while (prev && prev->next != elem)
		prev = prev->next;
	if (!prev)
		return ;
	prev->next = elem->next;
	elem->next = *lst;
	*lst = elem;
}
