/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:12:23 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 22:58:23 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	if (dest < src)
		ft_memcpy(dest, src, n);
	else if (dest > src)
	{
		d = (char *)dest;
		s = (const char *)src;
		while (n--)
			d[n] = s[n];
	}
	return (dest);
}
