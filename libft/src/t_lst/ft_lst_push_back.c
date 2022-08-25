/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_push_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:44:14 by yforeau           #+#    #+#             */
/*   Updated: 2019/03/02 17:01:09 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lst_push_back(t_list **lst, void *content, size_t content_size)
{
	t_list	*ptr;

	if (!lst)
		return (NULL);
	ptr = *lst;
	while (ptr && ptr->next)
		ptr = ptr->next;
	if (ptr)
		return ((ptr->next = ft_lstnew(content, content_size)));
	else
		return ((*lst = ft_lstnew(content, content_size)));
}
