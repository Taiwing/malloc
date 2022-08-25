/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scan_set_filter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:29:35 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 13:06:19 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_tcp_syn_set_filter(t_scan scan)
{
	t_scan_control	*scan_ctrl;
	t_filter_spec	spec = { 0 };

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	spec.min_src_port = scan_ctrl->port;
	spec.max_src_port = scan_ctrl->port;
	spec.min_dst_port = DEF_FTSCAN_TCP_SYN_PORT;
	spec.max_dst_port = DEF_FTSCAN_TCP_SYN_PORT;
	spec.src = &scan_ctrl->ip;
	return (ft_packet_filter_tcp_syn(scan_ctrl->recvfd, &spec));
}

static int		ft_echo_ping_set_filter(t_scan scan)
{
	t_scan_control	*scan_ctrl;
	t_filter_spec	spec = { 0 };

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	spec.icmp_echo_id = scan;
	spec.icmp_echo_sequence = scan_ctrl->sequence;
	spec.src = &scan_ctrl->ip;
	return (ft_packet_filter_echo_ping(scan_ctrl->recvfd, &spec));
}

int				ft_scan_set_filter(t_scan scan)
{
	switch (GET_SCAN_TYPE(scan))
	{
		case E_FTSCAN_ECHO_PING:
			return (ft_echo_ping_set_filter(scan));		break;
		case E_FTSCAN_TCP_SYN:
			return (ft_tcp_syn_set_filter(scan));		break;
	}
	return (-1);	// Once again, WTF gcc ?
}
