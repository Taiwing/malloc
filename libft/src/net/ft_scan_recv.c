/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scan_recv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 20:28:37 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/22 06:53:49 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	scan_parse_reply(t_scan_control *scan_ctrl, enum e_ftscan_type type,
	t_packet *reply, struct timeval *recv_ts)
{
	struct timeval	now;
	enum e_iphdr	iph = scan_ctrl->ip.family == AF_INET ? E_IH_V4 : E_IH_V6;
	
	if (!recv_ts && gettimeofday(&now, NULL) < 0)
	{
		ft_errno = -errno;
		return (-1);
	}
	recv_ts = recv_ts ? recv_ts : &now;
	ft_packet_init(reply, iph, NULL);
	switch (type)
	{
		case E_FTSCAN_ECHO_PING:
			return (ft_echo_ping_parse_reply(scan_ctrl, reply, recv_ts));
			break;
		case E_FTSCAN_TCP_SYN:
			return (ft_tcp_syn_parse_reply(scan_ctrl, reply, recv_ts));
			break;
	}
	return (-1);	// Will never reach this line. WTF gcc ???
}

int	ft_scan_recv(t_scan scan, int wait, struct timeval *recv_ts)
{
	ssize_t			rd;
	t_packet		rep;
	t_scan_control	*scan_ctrl;
	int				flags = wait ? 0 : MSG_DONTWAIT;

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	if ((rd = recv(scan_ctrl->recvfd, rep.buf, sizeof(rep.buf), flags)) > 0)
		return (scan_parse_reply(scan_ctrl,
			GET_SCAN_TYPE(scan), &rep, recv_ts));
	else if (rd < 0 && wait && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		scan_ctrl->result.sequence = scan_ctrl->sequence;
		ft_bzero(&scan_ctrl->sent_ts, sizeof(scan_ctrl->sent_ts));
		return (0);
	}
	ft_errno = -errno;
	return (-1);
}
