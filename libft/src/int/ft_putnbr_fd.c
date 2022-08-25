/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:17:50 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 23:19:02 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	int	ret;

	ret = 0;
	if (n < 0 && ft_putchar_fd(45, fd) != 1)
		return (-1);
	if (n / 10 && (ret = ft_putnbr_fd(n < 0 ? n / -10 : n / 10, fd)) < 0)
		return (-1);
	if (ft_putchar_fd(n < 0 ? 48 - (n % 10) : 48 + (n % 10), fd) < 0)
		return (-1);
	return (ret + 1 + (n < 0));
}
