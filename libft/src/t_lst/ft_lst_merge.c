/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_merge.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:43:39 by yforeau           #+#    #+#             */
/*   Updated: 2018/10/09 01:43:41 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_merge(t_list **lst1, t_list *lst2)
{
	t_list	*new_lst;

	if (!lst1)
		return ;
	new_lst = *lst1;
	while (new_lst && new_lst->next)
		new_lst = new_lst->next;
	if (new_lst)
		new_lst->next = lst2;
	else
		*lst1 = lst2;
}
