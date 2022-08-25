/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_pop_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 09:53:10 by yforeau           #+#    #+#             */
/*   Updated: 2022/01/18 09:56:14 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_lst_pop_if: pop first matching element in list if any
*/
t_list	*ft_lst_pop_if(t_list **lst, void *content_ref, int (*cmp)())
{
	int	index;

	if ((index = ft_lst_find_index(*lst, content_ref, cmp)) < 0)
		return (NULL);
	return (ft_lst_pop(lst, index));
}
