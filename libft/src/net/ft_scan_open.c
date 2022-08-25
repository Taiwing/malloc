/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scan_open.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 11:07:47 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 06:33:38 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_scan_set_timeout: Set the maximum time to wait for a response
**
** If no packet is received after this amount of time, ft_scan_poll will return
** a timeout event. So the user knows the scan is over. Also, the sent_ts value
** will be reset to indicate that the scan is no longer waiting for a response.
** If the timeout is NULL or zero ft_scan_poll will never return a timeout.
*/
int				ft_scan_set_timeout(t_scan scan, struct timeval *timeout)
{
	t_scan_control	*scan_ctrl;

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	if (!timeout || ft_timeval_is_zero(timeout))
		ft_bzero(&scan_ctrl->timeout, sizeof(scan_ctrl->timeout));
	else
	{
		if (setsockopt(scan_ctrl->recvfd, SOL_SOCKET,
			SO_RCVTIMEO, timeout, sizeof(struct timeval)) < 0)
		{
			ft_errno = -errno;
			return (-1);
		}
		ft_memcpy(&scan_ctrl->timeout, timeout, sizeof(scan_ctrl->timeout));
	}
	return (0);
}

/*
** ft_scan_set_payload: Set a payload for the given scan
**
** The payload will be sent as data for TCP or ICMP probes ("layer5").
*/
int				ft_scan_set_payload(t_scan scan, uint8_t *payload,
	size_t payload_len)
{
	t_scan_control	*scan_ctrl;

	if (!payload || payload_len > MAX_PACKET_PAYLOAD_SIZE)
	{
		ft_errno = E_FTERR_NET_INVALID_PAYLOAD;
		return (-1);
	}
	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	scan_ctrl->payload_size = payload_len;
	ft_memcpy(scan_ctrl->payload, payload, payload_len);
	return (0);
}

/*
** ft_scan_open: Open t_scan handle for the given scan type
**
** It can then be used with ft_echo_ping or ft_tcp_syn for a blocking scan or
** directly with ft_scan_send, ft_scan_poll and ft_scan_recv for an asynchronous
** scan. Then it can be closed with ft_scan_close or ft_scan_close_all.
**
** Returns -1 on error and sets ft_errno else it returns the new t_scan handle.
** This is only used internally, never directly by the user. The scan specific
** functions below are to be used instead to start new scans.
*/
static t_scan	ft_scan_open(enum e_ftscan_type type, t_ip *ip, uint16_t port)
{
	t_scan	scan;
	int		new_list = !g_scan_list[type];

	if (new_list && !(g_scan_list[type] = ft_memalloc(MAX_SCAN_COUNT
		* sizeof(t_scan_control *))))
	{
		ft_errno = -errno;
		return (-1);
	}
	if ((scan = ft_add_new_scan(type, ip, port)) < 0)
	{
		if (new_list)
			ft_memdel((void **)&g_scan_list[type]);
		return (-1);
	}
	return (scan);
}

/*
** ft_echo_ping_open: Open an echo_ping t_scan handle for the given IP
*/
t_scan	ft_echo_ping_open(t_ip *ip, struct timeval *timeout)
{
	t_scan	scan;

	if ((scan = ft_scan_open(E_FTSCAN_ECHO_PING, ip, 0)) < 0 || !timeout)
		return (scan);
	if (ft_scan_set_timeout(scan, timeout) < 0)
	{
		ft_scan_close(scan);
		return (-1);
	}
	return (scan);
}

/*
** ft_tcp_syn_open: Open a tcp_syn t_scan handle for the given IP/port pair
*/
t_scan	ft_tcp_syn_open(t_ip *ip, uint16_t port, struct timeval *timeout)
{
	t_scan	scan;

	if ((scan = ft_scan_open(E_FTSCAN_TCP_SYN, ip, port)) < 0 || !timeout)
		return (scan);
	if (ft_scan_set_timeout(scan, timeout) < 0)
	{
		ft_scan_close(scan);
		return (-1);
	}
	return (scan);
}
