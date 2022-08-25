/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memswap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 15:12:25 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/29 15:18:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memswap(void *dest, size_t n)
{
	unsigned char	*dst;
	unsigned char	*src;
	unsigned char	tmp;

	if (n)
	{
		src = dest;
		dst = src + n - 1;
		while (dst > src)
		{
			tmp = *src;
			*src++ = *dst;
			*dst-- = tmp;
		}
	}
	return (dest);
}
