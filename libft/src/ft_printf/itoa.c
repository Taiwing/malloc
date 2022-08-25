/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 23:44:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:48:27 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "ft_printf_internal.h"

const int	g_bases[CHAR_MAX + 1] = {
	['i'] = 10,
	['d'] = 10,
	['D'] = 10,
	['b'] = 2,
	['B'] = 2,
	['o'] = 8,
	['O'] = 8,
	['u'] = 10,
	['U'] = 10,
	['x'] = 16,
	['X'] = 16,
	['p'] = 16,
	['P'] = 16,
};

void	itoa_int(t_pdata *loc, t_farg *args, t_pconv *conv)
{
	int				s;
	unsigned int	u;
	int				base;
	int				type;

	type = (conv->type == 'X' || conv->type == 'P');
	base = g_bases[(uint8_t)conv->type];
	if (conv->cast & C_UNSIGNED)
		fetch(args, conv->arg, conv->cast, (void *)&u);
	else
		fetch(args, conv->arg, conv->cast, (void *)&s);
	if (conv->cast == (C_CHAR | C_UNSIGNED))
		itoa_uint(loc, (unsigned char)u, base, type);
	else if (conv->cast == (C_INT | C_SHORT | C_UNSIGNED))
		itoa_uint(loc, (unsigned short int)u, base, type);
	else if (conv->cast == (C_INT | C_UNSIGNED))
		itoa_uint(loc, u, base, type);
	else if (conv->cast & C_CHAR)
		itoa_sint(loc, (char)s);
	else if (conv->cast & C_SHORT)
		itoa_sint(loc, (short int)s);
	else if (conv->cast & C_INT)
		itoa_sint(loc, s);
}

void	itoa_long(t_pdata *loc, t_farg *args, t_pconv *conv)
{
	long int			si;
	unsigned long int	ui;
	int					base;
	int					type;

	if (conv->cast & C_UNSIGNED)
	{
		type = (conv->type == 'X' || conv->type == 'P');
		base = g_bases[(uint8_t)conv->type];
		fetch(args, conv->arg, conv->cast, (void *)&ui);
		itoa_ulint(loc, ui, base, type);
	}
	else
	{
		fetch(args, conv->arg, conv->cast, (void *)&si);
		itoa_slint(loc, si);
	}
}

void	itoa_long_long(t_pdata *loc, t_farg *args, t_pconv *conv)
{
	long long int			si;
	unsigned long long int	ui;
	int						base;
	int						type;

	if (conv->cast & C_UNSIGNED)
	{
		type = (conv->type == 'X' || conv->type == 'P');
		base = g_bases[(uint8_t)conv->type];
		fetch(args, conv->arg, conv->cast, (void *)&ui);
		itoa_ullint(loc, ui, base, type);
	}
	else
	{
		fetch(args, conv->arg, conv->cast, (void *)&si);
		itoa_sllint(loc, si);
	}
}

void	itoa_size_t(t_pdata *loc, t_farg *args, t_pconv *conv)
{
	ssize_t	si;
	size_t	ui;
	int		base;
	int		type;

	if (conv->cast & C_UNSIGNED)
	{
		type = (conv->type == 'X' || conv->type == 'P');
		base = g_bases[(uint8_t)conv->type];
		fetch(args, conv->arg, conv->cast, (void *)&ui);
		itoa_usize_t(loc, ui, base, type);
	}
	else
	{
		fetch(args, conv->arg, conv->cast, (void *)&si);
		itoa_ssize_t(loc, si);
	}
}

void	itoa_intmax_t(t_pdata *loc, t_farg *args, t_pconv *conv)
{
	intmax_t	si;
	uintmax_t	ui;
	int			base;
	int			type;

	if (conv->cast & C_UNSIGNED)
	{
		type = (conv->type == 'X' || conv->type == 'P');
		base = g_bases[(uint8_t)conv->type];
		fetch(args, conv->arg, conv->cast, (void *)&ui);
		itoa_uintmax_t(loc, ui, base, type);
	}
	else
	{
		fetch(args, conv->arg, conv->cast, (void *)&si);
		itoa_sintmax_t(loc, si);
	}
}
