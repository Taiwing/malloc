/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:11:49 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 16:26:39 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*m;

	m = (unsigned char *)s;
	while (n && *m != (unsigned char)c)
	{
		n--;
		m++;
	}
	return (n ? (void *)m : NULL);
}
