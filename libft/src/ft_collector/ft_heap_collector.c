/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heap_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:23:28 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/27 06:55:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_collector.h"

#ifdef THREAD_SAFE
# include "thread_safe.h"
MUTEXIFY(void*, ft_heap_collector, void*, ptr, int, hs_do)
#else
void	*ft_heap_collector(void *ptr, int hs_do)
#endif
{
	static void		**heap_stack = NULL;
	static size_t	size = 0;
	static size_t	i = 0;

	if (hs_do == FT_COLLEC_ADD)
	{
		if (!(heap_stack = ft_check_stack_size(heap_stack, &size, i)))
		{
			free(ptr);
			ft_exit(EXIT_FAILURE, "%s: no space left", __func__);
		}
		else
			heap_stack[i++] = ptr;
	}
	else if (hs_do == FT_COLLEC_GET)
		return (ft_get_stack_ptr(heap_stack, &i, ptr));
	else if (hs_do == FT_COLLEC_FREE)
	{
		while (i > 0)
			free(heap_stack[--i]);
		free(heap_stack);
		heap_stack = NULL;
	}
	return (NULL);
}
