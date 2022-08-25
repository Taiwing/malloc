/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_discard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 18:21:10 by yforeau           #+#    #+#             */
/*   Updated: 2019/03/29 19:43:11 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef NO_COLLEC

void	ft_lst_discard(t_list *prev, t_list **alst)
{
	t_list	*next;

	if (!alst || !*alst)
		return ;
	next = (*alst)->next;
	free((*alst)->content);
	free(*alst);
	*alst = next;
	if (prev)
		prev->next = next;
}

#else

void	ft_lst_discard(t_list *prev, t_list **alst)
{
	t_list	*next;

	if (!alst || !*alst)
		return ;
	next = (*alst)->next;
	free(ft_heap_collector((*alst)->content, FT_COLLEC_GET));
	free(ft_heap_collector(*alst, FT_COLLEC_GET));
	*alst = next;
	if (prev)
		prev->next = next;
}

#endif
