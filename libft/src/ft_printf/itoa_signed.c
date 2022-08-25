/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_signed.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 00:17:59 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/24 13:06:47 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "ft_printf_internal.h"

void	itoa_sint(t_pdata *loc, int si)
{
	int		i;
	int		size;
	int		sign;
	int		nb[128];

	size = 0;
	sign = (si < 0);
	while (si || !size)
	{
		nb[size++] = sign ? (si % 10) * -1 : si % 10;
		si /= 10;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size + sign);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size + sign] = 48 + nb[i++];
	loc->buf[0] = sign ? '-' : loc->buf[0];
}

void	itoa_slint(t_pdata *loc, long int si)
{
	int		i;
	int		size;
	int		sign;
	int		nb[256];

	size = 0;
	sign = (si < 0);
	while (si || !size)
	{
		nb[size++] = sign ? (si % 10) * -1 : si % 10;
		si /= 10;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size + sign);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size + sign] = 48 + nb[i++];
	loc->buf[0] = sign ? '-' : loc->buf[0];
}

void	itoa_sllint(t_pdata *loc, long long int si)
{
	int		i;
	int		size;
	int		sign;
	int		nb[256];

	size = 0;
	sign = (si < 0);
	while (si || !size)
	{
		nb[size++] = sign ? (si % 10) * -1 : si % 10;
		si /= 10;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size + sign);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size + sign] = 48 + nb[i++];
	loc->buf[0] = sign ? '-' : loc->buf[0];
}

void	itoa_ssize_t(t_pdata *loc, ssize_t si)
{
	int		i;
	int		size;
	int		sign;
	int		nb[128];

	size = 0;
	sign = (si < 0);
	while (si || !size)
	{
		nb[size++] = sign ? (si % 10) * -1 : si % 10;
		si /= 10;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size + sign);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size + sign] = 48 + nb[i++];
	loc->buf[0] = sign ? '-' : loc->buf[0];
}

void	itoa_sintmax_t(t_pdata *loc, intmax_t si)
{
	int		i;
	int		size;
	int		sign;
	int		nb[256];

	size = 0;
	sign = (si < 0);
	while (si || !size)
	{
		nb[size++] = sign ? (si % 10) * -1 : si % 10;
		si /= 10;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size + sign);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size + sign] = 48 + nb[i++];
	loc->buf[0] = sign ? '-' : loc->buf[0];
}
