/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_move_to_front.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:10:22 by yforeau           #+#    #+#             */
/*   Updated: 2019/02/20 16:34:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_move_to_front(t_list *prev, t_list **alst1, t_list **alst2)
{
	t_list	*next;

	if (!alst1 || !*alst1 || !alst2)
		return ;
	next = (*alst1)->next;
	(*alst1)->next = (*alst2);
	*alst2 = *alst1;
	*alst1 = next;
	if (prev)
		prev->next = next;
}
