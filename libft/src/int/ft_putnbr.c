/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:17:05 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 23:14:41 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int n)
{
	int	ret;

	ret = 0;
	if (n < 0 && ft_putchar(45) != 1)
		return (-1);
	if (n / 10 && (ret = ft_putnbr(n < 0 ? n / -10 : n / 10)) < 0)
		return (-1);
	if (ft_putchar(n < 0 ? 48 - (n % 10) : 48 + (n % 10)) < 0)
		return (-1);
	return (ret + 1 + (n < 0));
}