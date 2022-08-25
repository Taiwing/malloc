/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_foreach_if.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:42:26 by yforeau           #+#    #+#             */
/*   Updated: 2018/10/09 01:42:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_foreach_if(t_list *lst, void (*f)(void *),
		void *content_ref, int (*cmp)())
{
	while (lst)
	{
		if ((*cmp)(content_ref, lst->content) == 0)
			(*f)(lst->content);
		lst = lst->next;
	}
}
