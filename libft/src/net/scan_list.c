/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 12:30:50 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/06 09:25:47 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

# ifdef THREAD_SAFE
	__thread t_scan_control	**g_scan_list[FTSCAN_TYPE_COUNT] = { 0 };
	__thread uint16_t		g_scan_count[FTSCAN_TYPE_COUNT] = { 0 };
# else
	t_scan_control			**g_scan_list[FTSCAN_TYPE_COUNT] = { 0 };
	uint16_t				g_scan_count[FTSCAN_TYPE_COUNT] = { 0 };
# endif

t_scan_control	*ft_get_scan(t_scan scan)
{
	uint16_t			id = GET_SCAN_ID(scan);
	enum e_ftscan_type	type = GET_SCAN_TYPE(scan);

	if (type < 0 || type > E_FTSCAN_TCP_SYN
		|| !g_scan_list[type] || !g_scan_list[type][id])
	{
		ft_errno = E_FTERR_INVALID_SCAN_HANDLE;
		return (NULL);
	}
	return (g_scan_list[type][id]);
}

uint8_t		ft_get_scan_protocol(enum e_ftscan_type type, int domain)
{
	uint8_t	protocol;

	switch (type)
	{
		case E_FTSCAN_ECHO_PING:
			protocol = domain == AF_INET ? IPPROTO_ICMP : IPPROTO_ICMPV6;
			break;
		case E_FTSCAN_TCP_SYN:
			protocol = IPPROTO_TCP;
			break;
	}
	return (protocol);
}

static int	init_scan_sockets(enum e_ftscan_type type, uint16_t id)
{
	int	domain = g_scan_list[type][id]->ip.family;
	int	protocol = ft_get_scan_protocol(type, domain);

	if ((g_scan_list[type][id]->recvfd = ft_recv_socket_init(domain)) < 0)
		return (-1);
	g_scan_list[type][id]->sendfd = ft_send_socket_init(domain, protocol,
		protocol == IPPROTO_TCP);
	if (g_scan_list[type][id]->sendfd < 0)
	{
		close(g_scan_list[type][id]->recvfd);
		return (-1);
	}
	return (0);
}

t_scan	ft_add_new_scan(enum e_ftscan_type type, t_ip *ip, uint16_t port)
{
	uint16_t	id;
	t_scan		scan;

	for (id = 0; id < MAX_SCAN_COUNT && g_scan_list[type][id]; ++id);
	if (id == MAX_SCAN_COUNT)
	{
		ft_errno = E_FTERR_SCAN_LIST_FULL;
		return (-1);
	}
	if (!(g_scan_list[type][id] = ft_memalloc(sizeof(t_scan_control))))
	{
		ft_errno = -errno;
		return (-1);
	}
	ft_memcpy(&g_scan_list[type][id]->ip, ip, ft_ip_sock_size(ip));
	g_scan_list[type][id]->port = port;
	scan = ((int)type << SCAN_ID_BITLEN) | (int)id;
	if (init_scan_sockets(type, id) < 0)
	{
		ft_memdel((void **)&g_scan_list[type][id]);
		return (-1);
	}
	++g_scan_count[type];
	return (scan);
}
