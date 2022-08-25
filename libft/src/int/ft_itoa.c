/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:09:04 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 18:45:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		i;
	int		size;
	int		sign;
	int		nb[128];
	char	*str;

	size = 0;
	sign = (n < 0);
	while (n || !size)
	{
		nb[size++] = sign ? (n % 10) * -1 : n % 10;
		n /= 10;
	}
	if (!(str = ft_strnew(size + sign)))
		return (NULL);
	i = 0;
	while (size--)
		str[size + sign] = 48 + nb[i++];
	str[0] = sign ? '-' : str[0];
	return (str);
}
