/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_pop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 11:56:06 by yforeau           #+#    #+#             */
/*   Updated: 2019/12/03 13:13:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lst_pop(t_list **lst, unsigned int pop)
{
	t_list	*ptr;
	t_list	*prev;

	if (!lst || !*lst)
		return (NULL);
	prev = NULL;
	ptr = *lst;
	while (ptr && pop-- > 0)
	{
		prev = ptr;
		ptr = ptr->next;
	}
	if (ptr)
	{
		if (!prev)
			*lst = ptr->next;
		else
			prev->next = ptr->next;
		ptr->next = NULL;
	}
	return (ptr);
}
