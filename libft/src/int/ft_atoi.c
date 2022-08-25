/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:06:35 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 18:34:15 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	long long int	nb;
	long long int	sign;

	nb = 0;
	sign = 1;
	while (*nptr == 32 || (*nptr > 8 && *nptr < 14))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
		sign = *nptr++ == '-' ? -1 : sign;
	while (*nptr > 47 && *nptr < 58)
		nb = (nb * 10) + ((*nptr++) - 48);
	return (nb * sign);
}
