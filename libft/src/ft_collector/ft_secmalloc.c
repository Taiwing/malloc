/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_secmalloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 08:10:46 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 06:51:26 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include "ft_collector.h"

#ifdef NO_COLLEC

void	*ft_secmalloc(size_t size)
{
	return (malloc(size));
}

#else

void	*ft_secmalloc(size_t size)
{
	void	*ptr;

	if (!(ptr = malloc(size)))
		ft_exit(EXIT_FAILURE, "malloc: %s", strerror(ENOMEM));
	ft_heap_collector(ptr, FT_COLLEC_ADD);
	return (ptr);
}

#endif
