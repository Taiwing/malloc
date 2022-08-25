/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 17:34:35 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:30:06 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "ft_printf_internal.h"
#include "libft.h"

const t_convf	g_conversions[CHAR_MAX + 1] = {
	['i'] = itoa_cast,
	['d'] = itoa_cast,
	['D'] = itoa_cast,
	['b'] = itoa_cast,
	['B'] = itoa_cast,
	['o'] = itoa_cast,
	['O'] = itoa_cast,
	['u'] = itoa_cast,
	['U'] = itoa_cast,
	['x'] = itoa_cast,
	['X'] = itoa_cast,
	['p'] = p_conversion,
	['P'] = p_conversion,
	['e'] = efg_conversions,
	['E'] = efg_conversions,
	['f'] = efg_conversions,
	['F'] = efg_conversions,
	['g'] = efg_conversions,
	['G'] = efg_conversions,
	['c'] = c_conversion,
	['C'] = lc_conversion,
	['s'] = s_conversion,
	['S'] = ls_conversion,
	['t'] = t_conversion,
};

static void	convert(t_pdata *data, t_farg *args, t_pconv *conv, char **fmt)
{
	t_pdata	loc_data;
	t_convf	convf;

	pdata_init(&loc_data, data->flags, data->fd);
	pdata_local_set_buf(&loc_data);
	if ((convf = g_conversions[(uint8_t)conv->type]))
		convf(&loc_data, args, conv, fmt);
	else
		pdata_add(&loc_data, NULL, conv->type, 1);
	if (loc_data.n != -1)
		format_data(data, &loc_data, conv);
	data->n = loc_data.n == -1 ? -1 : data->n;
	ft_memdel((void **)&loc_data.abuf);
}

void		parser(t_pdata *data, char **fmt, t_farg *args)
{
	t_pconv	conv = CONV_INIT;
	char		*mod;

	mod = ft_strchr(*fmt, '%');
	if (mod == *fmt)
	{
		if (get_conv(fmt, args, &conv))
			convert(data, args, &conv, fmt);
	}
	else if (mod)
	{
		pdata_add(data, *fmt, 0, mod - *fmt);
		*fmt = mod;
	}
	else
	{
		pdata_add(data, *fmt, 0, ft_strlen(*fmt));
		*fmt = ft_strchr(*fmt, 0);
	}
}
