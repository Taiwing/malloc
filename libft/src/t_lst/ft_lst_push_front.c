/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_push_front.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:44:27 by yforeau           #+#    #+#             */
/*   Updated: 2019/03/02 17:02:57 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lst_push_front(t_list **lst, void *content, size_t content_size)
{
	t_list	*ptr;

	if (!lst)
		return (NULL);
	if ((ptr = ft_lstnew(content, content_size)))
		ft_lstadd(lst, ptr);
	return (ptr);
}
