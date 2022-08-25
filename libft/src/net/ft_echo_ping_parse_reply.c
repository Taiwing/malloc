/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_ping_parse_reply.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 21:27:41 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/02 21:52:23 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_echo_ping_parse_reply_ipv6(t_scan_control *scan_ctrl,
	t_packet *reply)
{
	switch (reply->next->icmp6.icmp6_type)
	{
		case ICMPV6_TIME_EXCEED:
			scan_ctrl->result.open = 0;
			scan_ctrl->result.reason = E_FTSCAN_ECHO_PING_TTL;
			break;
		case ICMPV6_DEST_UNREACH:
			scan_ctrl->result.open = 0;
			scan_ctrl->result.reason = E_FTSCAN_ECHO_PING_DEST_UNREACH;
			break;
		case ICMPV6_ECHO_REPLY:
			scan_ctrl->result.open = 1;
			scan_ctrl->result.reason = E_FTSCAN_ECHO_PING_ECHO_REPLY;
			break;
		default:
			ft_errno = E_FTERR_PACKET_FILTER_FAILURE;
			return (-1);
			break;
	}
	scan_ctrl->result.ttl = reply->ip->v6.hop_limit;
	scan_ctrl->result.sequence = ntohs(reply->next->icmp6.icmp6_sequence);
	return (0);
}

static int	ft_echo_ping_parse_reply_ipv4(t_scan_control *scan_ctrl,
	t_packet *reply)
{
	switch (reply->next->icmp.type)
	{
		case ICMP_TIME_EXCEEDED:
			scan_ctrl->result.open = 0;
			scan_ctrl->result.reason = E_FTSCAN_ECHO_PING_TTL;
			break;
		case ICMP_DEST_UNREACH:
			scan_ctrl->result.open = 0;
			scan_ctrl->result.reason = E_FTSCAN_ECHO_PING_DEST_UNREACH;
			break;
		case ICMP_ECHOREPLY:
			scan_ctrl->result.open = 1;
			scan_ctrl->result.reason = E_FTSCAN_ECHO_PING_ECHO_REPLY;
			break;
		default:
			ft_errno = E_FTERR_PACKET_FILTER_FAILURE;
			return (-1);
			break;
	}
	scan_ctrl->result.ttl = reply->ip->v4.ttl;
	scan_ctrl->result.sequence = ntohs(reply->next->icmp.un.echo.sequence);
	return (0);
}

int		ft_echo_ping_parse_reply(t_scan_control *scan_ctrl, t_packet *reply,
	struct timeval *recv_ts)
{
	int	ret;

	if (scan_ctrl->ip.family == AF_INET)
		ret = ft_echo_ping_parse_reply_ipv4(scan_ctrl, reply);
	else
		ret = ft_echo_ping_parse_reply_ipv6(scan_ctrl, reply);
	if (ret < 0)
		return (-1);
	if (ft_timeval_sub(&scan_ctrl->result.rtt, recv_ts,
		&scan_ctrl->sent_ts) < 0)
		return (-1);
	ft_bzero(&scan_ctrl->sent_ts, sizeof(scan_ctrl->sent_ts));
	return (0);
}
