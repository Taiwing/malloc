/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   efg_conversions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 20:36:57 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:30:52 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "log_and_ceil.h"

static int	check_infnan(t_pdata *loc, t_bitd *raw, int is_long, int maj)
{
	if ((is_long && raw->ld.exponent == LDBL_EXP_MAX)
		|| (!is_long && raw->sd.exponent == DBL_EXP_MAX))
	{
		if ((is_long && !(raw->ld.mantissa & ~LDBL_INT_BIT))
			|| (!is_long && !raw->sd.mantissa))
		{
			if ((is_long && raw->ld.sign) || (!is_long && raw->sd.sign))
				pdata_add(loc, NULL, '-', 1);
			pdata_add(loc, maj ? "INF" : "inf", 0, 3);
		}
		else
			pdata_add(loc, maj ? "NAN" : "nan", 0, 3);
		return (1);
	}
	if ((is_long && raw->ld.sign) || (!is_long && raw->sd.sign))
		pdata_add(loc, NULL, '-', 1);
	return (0);
}

static void	init_fltinf(t_pconv *conv, t_fltinf *info, t_bitd *raw)
{
	if (conv->cast & C_LONG_LONG)
	{
		info->sign = raw->ld.sign;
		info->exp = raw->ld.exponent ? (int)raw->ld.exponent - 16446 : -16447;
		info->mantissa = raw->ld.mantissa;
		info->log2 = raw->ld.exponent ? 64 : logbase2_64(raw->ld.mantissa);
	}
	else
	{
		info->sign = raw->sd.sign;
		info->exp = raw->sd.exponent ? (int)raw->sd.exponent - 1075 : -1074;
		info->mantissa = raw->sd.exponent ?
						(1ull << 52) | raw->sd.mantissa : raw->sd.mantissa;
		info->log2 = raw->sd.exponent ? 52 : logbase2_64(raw->sd.mantissa);
	}
	info->prec = conv->prec < 0 ? 6 : conv->prec;
	info->prec = (conv->type == 'g' || conv->type == 'G') && info->prec > 0 ?
				info->prec - 1 : info->prec;
	info->conv = conv->type;
	info->digit_exp = 0;
	info->digit = 0;
	info->exp10 = 0;
	info->flags = conv->flags;
}

void		efg_conversions(t_pdata *loc, t_farg *args,
				t_pconv *conv, char **fmt)
{
	char		buf[16384];
	int			size;
	t_fltinf	info;
	t_bitd		raw;

	(void)fmt;
	if (conv->cast & C_LONG_LONG)
		fetch(args, conv->arg, C_DOUBLE | C_LONG, (void *)&raw);
	else
		fetch(args, conv->arg, C_DOUBLE, (void *)&raw);
	if (!check_infnan(loc, &raw, conv->cast & C_LONG_LONG,
		conv->type > 64 && conv->type < 91))
	{
		init_fltinf(conv, &info, &raw);
		size = dragon4(&info, buf);
		if (conv->type == 'f' || conv->type == 'F')
			f_format(loc, buf, size, &info);
		else if (conv->type == 'e' || conv->type == 'E')
			e_format(loc, buf, size, &info);
		else if (conv->type == 'g' || conv->type == 'G')
			g_format(loc, buf, size, &info);
	}
}
