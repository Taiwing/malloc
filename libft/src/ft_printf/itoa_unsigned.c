/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_unsigned.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 00:34:41 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/24 13:06:57 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "ft_printf_internal.h"

void	itoa_uint(t_pdata *loc, unsigned int ui, int base, int type)
{
	int		i;
	int		size;
	int		nb[128];
	char	*lstr;
	char	*ustr;

	lstr = "0123456789abcdef";
	ustr = "0123456789ABCDEF";
	if (base < 2 || base > 16)
		return ;
	size = 0;
	while (ui || !size)
	{
		nb[size++] = ui % base;
		ui /= base;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size] = type ? ustr[nb[i++]] : lstr[nb[i++]];
}

void	itoa_ulint(t_pdata *loc, unsigned long int ui, int base, int type)
{
	int		i;
	int		size;
	int		nb[256];
	char	*lstr;
	char	*ustr;

	lstr = "0123456789abcdef";
	ustr = "0123456789ABCDEF";
	if (base < 2 || base > 16)
		return ;
	size = 0;
	while (ui || !size)
	{
		nb[size++] = ui % base;
		ui /= base;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size] = type ? ustr[nb[i++]] : lstr[nb[i++]];
}

void	itoa_ullint(t_pdata *loc, unsigned long long int ui, int base, int type)
{
	int		i;
	int		size;
	int		nb[256];
	char	*lstr;
	char	*ustr;

	lstr = "0123456789abcdef";
	ustr = "0123456789ABCDEF";
	if (base < 2 || base > 16)
		return ;
	size = 0;
	while (ui || !size)
	{
		nb[size++] = ui % base;
		ui /= base;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size] = type ? ustr[nb[i++]] : lstr[nb[i++]];
}

void	itoa_usize_t(t_pdata *loc, size_t ui, int base, int type)
{
	int		i;
	int		size;
	int		nb[128];
	char	*lstr;
	char	*ustr;

	lstr = "0123456789abcdef";
	ustr = "0123456789ABCDEF";
	if (base < 2 || base > 16)
		return ;
	size = 0;
	while (ui || !size)
	{
		nb[size++] = ui % base;
		ui /= base;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size] = type ? ustr[nb[i++]] : lstr[nb[i++]];
}

void	itoa_uintmax_t(t_pdata *loc, uintmax_t ui, int base, int type)
{
	int		i;
	int		size;
	int		nb[256];
	char	*lstr;
	char	*ustr;

	lstr = "0123456789abcdef";
	ustr = "0123456789ABCDEF";
	if (base < 2 || base > 16)
		return ;
	size = 0;
	while (ui || !size)
	{
		nb[size++] = ui % base;
		ui /= base;
	}
	i = 0;
	pdata_add(loc, NULL, 0, size);
	if (loc->n == -1)
		return ;
	while (size--)
		loc->buf[size] = type ? ustr[nb[i++]] : lstr[nb[i++]];
}
