/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tcp_syn_parse_reply.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 21:29:39 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/02 21:52:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_tcp_syn_parse_reply_ipv6(t_scan_control *scan_ctrl,
	t_packet *reply)
{
	scan_ctrl->result.ttl = reply->ip->v6.hop_limit;
	scan_ctrl->result.sequence = scan_ctrl->sequence;
	if (reply->nexthdr == E_NH_ICMP6)
	{
		scan_ctrl->result.open = 0;
		scan_ctrl->result.reason = E_FTSCAN_TCP_SYN_ICMP_DEST_UNREACH;
		return (0);
	}
	else if (reply->nexthdr != E_NH_TCP)
	{
		ft_errno = E_FTERR_PACKET_FILTER_FAILURE;
		return (-1);
	}
	scan_ctrl->result.open = !!(reply->next->tcp.th_flags & TH_SYN);
	scan_ctrl->result.reason = scan_ctrl->result.open ?
		E_FTSCAN_TCP_SYN_TCP_SYN : E_FTSCAN_TCP_SYN_TCP_RST;
	return (0);
}

static int	ft_tcp_syn_parse_reply_ipv4(t_scan_control *scan_ctrl,
	t_packet *reply)
{
	scan_ctrl->result.ttl = reply->ip->v4.ttl;
	scan_ctrl->result.sequence = scan_ctrl->sequence;
	if (reply->nexthdr == E_NH_ICMP)
	{
		scan_ctrl->result.open = 0;
		scan_ctrl->result.reason = E_FTSCAN_TCP_SYN_ICMP_DEST_UNREACH;
		return (0);
	}
	else if (reply->nexthdr != E_NH_TCP)
	{
		ft_errno = E_FTERR_PACKET_FILTER_FAILURE;
		return (-1);
	}
	scan_ctrl->result.open = !!(reply->next->tcp.th_flags & TH_SYN);
	scan_ctrl->result.reason = scan_ctrl->result.open ?
		E_FTSCAN_TCP_SYN_TCP_SYN : E_FTSCAN_TCP_SYN_TCP_RST;
	return (0);
}

int	ft_tcp_syn_parse_reply(t_scan_control *scan_ctrl, t_packet *reply,
	struct timeval *recv_ts)
{
	int	ret;

	if (scan_ctrl->ip.family == AF_INET)
		ret = ft_tcp_syn_parse_reply_ipv4(scan_ctrl, reply);
	else
		ret = ft_tcp_syn_parse_reply_ipv6(scan_ctrl, reply);
	if (ret < 0)
		return (-1);
	if (ft_timeval_sub(&scan_ctrl->result.rtt, recv_ts,
		&scan_ctrl->sent_ts) < 0)
		return (-1);
	ft_bzero(&scan_ctrl->sent_ts, sizeof(scan_ctrl->sent_ts));
	return (0);
}
