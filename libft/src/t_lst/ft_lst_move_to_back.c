/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_move_to_back.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:13:05 by yforeau           #+#    #+#             */
/*   Updated: 2019/02/20 16:34:21 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_move_to_back(t_list *prev, t_list **alst1, t_list **alst2)
{
	t_list	*next;

	if (!alst1 || !*alst1 || !alst2)
		return ;
	next = (*alst1)->next;
	if (!*alst2)
		*alst2 = *alst1;
	else
		ft_lst_last(*alst2)->next = *alst1;
	(*alst1)->next = NULL;
	*alst1 = next;
	if (prev)
		prev->next = next;
}
