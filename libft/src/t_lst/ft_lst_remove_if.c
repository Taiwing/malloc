/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_remove_if.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:44:53 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 15:36:40 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_remove_if(t_list **lst, void *content_ref, int (*cmp)())
{
	t_list	*prev;
	t_list	*cur;
	t_list	*next;

	if (!lst || !*lst || !cmp)
		return ;
	prev = NULL;
	cur = *lst;
	next = cur ? cur->next : NULL;
	while (cur)
	{
		if (!cmp(content_ref, cur->content))
		{
			ft_memdel((void **)&cur);
			if (prev)
				prev->next = next;
			else
				*lst = next;
		}
		prev = cur ? cur : prev;
		cur = next;
		next = cur ? cur->next : next;
	}
}
