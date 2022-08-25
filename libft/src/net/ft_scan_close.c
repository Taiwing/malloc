/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scan_close.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 16:41:57 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/19 22:25:32 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_scan_close(t_scan scan)
{
	uint16_t			id = GET_SCAN_ID(scan);
	enum e_ftscan_type	type = GET_SCAN_TYPE(scan);

	if (!g_scan_list[type] || id >= MAX_SCAN_COUNT || !g_scan_list[type][id])
		return;
	if (g_scan_list[type][id]->sendfd >= 0)
		close(g_scan_list[type][id]->sendfd);
	if (g_scan_list[type][id]->recvfd >= 0)
		close(g_scan_list[type][id]->recvfd);
	ft_memdel((void **)&g_scan_list[type][id]);
	if (--g_scan_count[type] == 0)
		ft_memdel((void **)&g_scan_list[type]);
}

static void	ft_scan_close_type(enum e_ftscan_type type)
{
	t_scan	scan;

	if (!g_scan_list[type])
		return;
	for (uint16_t id = 0; id < MAX_SCAN_COUNT && g_scan_count[type]; ++id)
	{
		if (!g_scan_list[type][id])
			continue;
		scan = ((int)type << 16) | (int)id;
		ft_scan_close(scan);
	}
}

void		ft_scan_close_all(void)
{
	for (int i = 0; i < FTSCAN_TYPE_COUNT; ++i)
		ft_scan_close_type(i);
}
