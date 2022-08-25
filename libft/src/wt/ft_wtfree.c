/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wtfree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:04:19 by yforeau           #+#    #+#             */
/*   Updated: 2019/04/29 18:36:43 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_collector.h"

#ifdef NO_COLLEC

void	ft_wtfree(char **wt)
{
	char	**ptr;

	if (!wt)
		return ;
	ptr = wt;
	while (*wt)
		free(*wt++);
	free(*wt);
	free(ptr);
}

#else

void	ft_wtfree(char **wt)
{
	char	**ptr;

	if (!wt)
		return ;
	ptr = wt;
	while (*wt)
		free(ft_heap_collector(*wt++, FT_COLLEC_GET));
	free(ft_heap_collector(*wt, FT_COLLEC_GET));
	free(ft_heap_collector(ptr, FT_COLLEC_GET));
}

#endif
