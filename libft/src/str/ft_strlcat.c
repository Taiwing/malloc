/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:25:58 by yforeau           #+#    #+#             */
/*   Updated: 2021/11/18 19:48:14 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t		dlen;
	size_t		slen;

	if ((dlen = (size_t)ft_strlen(dest)) >= size)
		return (size + ft_strlen(src));
	size -= dlen;
	if ((slen = (size_t)ft_strlen(src) + 1) > size)
		slen = size;
	dest += dlen;
	while (*src && size > 1)
	{
		*dest++ = *src++;
		--size;
	}
	*dest = 0;
	return (dlen + slen);
}
