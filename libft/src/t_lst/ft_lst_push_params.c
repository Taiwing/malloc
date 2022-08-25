/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_push_params.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:44:42 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 15:35:54 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lst_push_params(int ac, char **av)
{
	t_list	*new_list;
	t_list	*new_elem;

	if (!av || !*av)
		return (0);
	new_list = ft_lstnew((void *)av[ac - 1], ft_strlen(av[ac + 1]) + 1);
	new_elem = new_list;
	while (--ac)
	{
		new_elem->next = ft_lstnew((void *)av[ac - 1],
									ft_strlen(av[ac + 1]) + 1);
		new_elem = new_elem->next;
	}
	return (new_list);
}
