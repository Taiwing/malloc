/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 13:27:46 by yforeau           #+#    #+#             */
/*   Updated: 2019/03/29 19:52:11 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef NO_COLLEC

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*new;

	new = ft_strnew(len);
	if (!s || !new)
	{
		if (new)
			free(new);
		return (NULL);
	}
	return (ft_strncpy(new, s + start, len));
}

#else

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*new;

	new = ft_strnew(len);
	if (!s || !new)
	{
		if (new)
			free(ft_heap_collector(new, FT_COLLEC_GET));
		return (NULL);
	}
	return (ft_strncpy(new, s + start, len));
}

#endif
