/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_and_ceil.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 16:23:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/07/05 15:39:14 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

int			ft_ceil(double d)
{
	if (d - (int)d == 0.0f || d < 0.0f)
		return ((int)d);
	else
		return ((int)d + 1);
}

uint32_t	logbase2_32(uint32_t nbr)
{
	uint32_t	tmp;

	tmp = nbr >> 24;
	if (tmp)
		return (24 + g_logtable[tmp]);
	tmp = nbr >> 16;
	if (tmp)
		return (16 + g_logtable[tmp]);
	tmp = nbr >> 8;
	if (tmp)
		return (8 + g_logtable[tmp]);
	return (g_logtable[nbr]);
}

uint32_t	logbase2_64(uint64_t nbr)
{
	if (nbr >> 32)
		return (32 + logbase2_32((uint32_t)(nbr >> 32)));
	return (logbase2_32((uint32_t)nbr));
}
