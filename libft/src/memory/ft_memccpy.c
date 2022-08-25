/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:11:25 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 18:53:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	char		*d;
	const char	*s;

	s = src;
	d = dest;
	while (n--)
	{
		*d++ = *s++;
		if ((unsigned char)*(d - 1) == (unsigned char)c)
			return ((void *)d);
	}
	return (NULL);
}
