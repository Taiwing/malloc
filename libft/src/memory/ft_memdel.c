/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:12:08 by yforeau           #+#    #+#             */
/*   Updated: 2019/03/29 19:47:23 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef NO_COLLEC

void	ft_memdel(void **ap)
{
	if (!ap)
		return ;
	if (*ap)
		free(*ap);
	*ap = NULL;
}

#else

void	ft_memdel(void **ap)
{
	if (!ap)
		return ;
	if (*ap)
		free(ft_heap_collector(*ap, FT_COLLEC_GET));
	*ap = NULL;
}

#endif
