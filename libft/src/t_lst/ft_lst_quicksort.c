/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_quicksort.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 18:28:59 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/23 20:43:13 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	separate(t_list *lst, t_list **pivot,
					int len, int (*cmp)(void *, void *))
{
	int		ip;
	t_list	*cur;
	t_list	*big;
	t_list	*last;

	ip = 0;
	cur = lst;
	big = lst;
	last = ft_lst_at(lst, len - 1);
	ft_swap_p((uintptr_t *)&((*pivot)->content), (uintptr_t *)&(last->content));
	while (cur != last)
	{
		if (cmp(cur->content, last->content) <= 0)
		{
			ft_swap_p((uintptr_t *)&(cur->content),
					(uintptr_t *)&(big->content));
			big = big->next;
			++ip;
		}
		cur = cur->next;
	}
	ft_swap_p((uintptr_t *)&(last->content), (uintptr_t *)&(big->content));
	*pivot = big;
	return (ip);
}

void		ft_lst_quicksort(t_list *lst, int len, int (*cmp)(void *, void *))
{
	t_list	*pivot;
	int		ip;

	if (len > 1)
	{
		ip = len / 2;
		pivot = ft_lst_at(lst, ip);
		ip = separate(lst, &pivot, len, cmp);
		ft_lst_quicksort(lst, ip, cmp);
		ft_lst_quicksort(pivot->next, len - ip - 1, cmp);
	}
}
