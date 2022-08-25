/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_find.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:41:51 by yforeau           #+#    #+#             */
/*   Updated: 2018/10/09 01:41:54 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lst_find(t_list *lst, void *content_ref, int (*cmp)())
{
	if (!lst || !content_ref || !cmp)
		return (0);
	while (lst)
	{
		if ((*cmp)(content_ref, lst->content) == 0)
			return (lst);
		lst = lst->next;
	}
	return (0);
}
