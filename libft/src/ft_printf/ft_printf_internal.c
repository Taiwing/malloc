/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 06:01:28 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/25 06:24:56 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

int	ft_printf_internal(t_pdata *data, const char *format, t_farg *args)
{
	while (*format && data->n >= 0)
		parser(data, (char **)&format, args);
	if (data->n >= 0)
	{
		if (data->flags & PDATA_RETBUF)
			data->buf[data->n] = 0;
		else
			pdata_print(data);
		return (data->n + data->flushed);
	}
	else
		ft_memdel((void **)&data->abuf);
	return (data->n);
}
