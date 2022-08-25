/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uni_conversions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 16:37:37 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:30:38 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

static int	byte_len(wint_t c)
{
	int l;

	l = 0;
	if (c < 0x80)
		l = 1;
	else if (c < 0x800)
		l = 2;
	else if (c < 0x10000)
		l = 3;
	else if (c < 0x200000)
		l = 4;
	return (l);
}

static void	conv_wint(char *str, int l, wint_t c)
{
	str[0] = l > 1 ? 0xf0 << (4 - l) : 0;
	while (--l > -1)
	{
		str[l] |= !l ? c : 0x80 | (c & 0x3f);
		c >>= 6;
	}
}

void		lc_conversion(t_pdata *loc, t_farg *args,
				t_pconv *conv, char **fmt)
{
	int		l;
	wint_t	nb;

	(void)fmt;
	fetch(args, conv->arg, C_WINT_T, (void *)(&nb));
	l = byte_len(nb);
	pdata_add(loc, NULL, 0, l);
	if (loc->n == -1)
		return ;
	conv_wint(loc->buf, l, nb);
}

void		ls_conversion(t_pdata *loc, t_farg *args,
				t_pconv *conv, char **fmt)
{
	int			l;
	int			i;
	wchar_t		*wstr;

	(void)fmt;
	fetch(args, conv->arg, C_WCHAR_T_P, (void *)(&wstr));
	if (!wstr)
		pdata_add(loc, "(null)", 0,
		(conv->prec > 5 || conv->prec < 0) ? 6 : conv->prec);
	else if (wstr)
	{
		i = 0;
		while (*wstr && (i < conv->prec || conv->prec == -1) && loc->n != -1)
		{
			l = byte_len(*wstr);
			if (conv->prec == -1 || (i + l) <= conv->prec)
			{
				pdata_add(loc, NULL, 0, l);
				if (loc->n != -1)
					conv_wint(loc->buf + loc->n - l, l, *wstr++);
			}
			i += l;
		}
	}
}
