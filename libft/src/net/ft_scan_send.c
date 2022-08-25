/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scan_send.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 17:36:07 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/09 04:38:50 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define	TCP_SYN_HDR_TEMPLATE {\
	.th_sport = htons(DEF_FTSCAN_TCP_SYN_PORT),\
	.th_flags = TH_SYN,\
	.th_off = sizeof(struct tcphdr) / sizeof(uint32_t),\
	.th_win = htons(0x0ff),\
}

#define ECHO_PING_V4_HDR_TEMPLATE { .type = ICMP_ECHO }

#define ECHO_PING_V6_HDR_TEMPLATE { .icmp6_type = ICMPV6_ECHO_REQUEST }

static void	add_echo_ping_v6_header(t_packet *probe,
	t_scan_control *scan_ctrl, size_t ipsize, t_scan scan)
{
	struct icmp6hdr	hdr = ECHO_PING_V6_HDR_TEMPLATE;

	hdr.icmp6_identifier = htons(scan);
	hdr.icmp6_sequence = htons(scan_ctrl->sequence);
	ft_memcpy(probe->buf + ipsize, &hdr, sizeof(hdr));
	if (scan_ctrl->payload_size)
		ft_memcpy(probe->buf + ipsize + sizeof(hdr), scan_ctrl->payload,
			scan_ctrl->payload_size);
}

static void	add_echo_ping_v4_header(t_packet *probe,
	t_scan_control *scan_ctrl, size_t ipsize, t_scan scan)
{
	struct icmphdr	hdr = ECHO_PING_V4_HDR_TEMPLATE;

	hdr.un.echo.id = htons(scan);
	hdr.un.echo.sequence = htons(scan_ctrl->sequence);
	ft_memcpy(probe->buf + ipsize, &hdr, sizeof(hdr));
	if (scan_ctrl->payload_size)
		ft_memcpy(probe->buf + ipsize + sizeof(hdr), scan_ctrl->payload,
			scan_ctrl->payload_size);
	((struct icmphdr *)(probe->buf + ipsize))->checksum =
		ft_checksum((void *)(probe->buf + ipsize),
		sizeof(hdr) + scan_ctrl->payload_size);
}

static void	add_tcp_syn_header(t_packet *probe, t_scan_control *scan_ctrl,
	size_t ipsize, t_scan scan)
{
	struct tcphdr	hdr = TCP_SYN_HDR_TEMPLATE;

	(void)scan;
	hdr.th_dport = htons(scan_ctrl->port);
	hdr.th_seq = htonl(scan_ctrl->sequence);
	ft_memcpy(probe->buf + ipsize, &hdr, sizeof(hdr));
	if (scan_ctrl->payload_size)
		ft_memcpy(probe->buf + ipsize + sizeof(hdr), scan_ctrl->payload,
			scan_ctrl->payload_size);
	((struct tcphdr *)(probe->buf + ipsize))->th_sum = ft_transport_checksum(
		scan_ctrl->ip.family == AF_INET ? 4 : 6, probe->buf,
		probe->buf + ipsize, sizeof(hdr) + scan_ctrl->payload_size);
}

/*
** set_interface: choose interface for TCP scans according to network config
**
** src: source ip to be set in the IP header
** dest: the ip of the host to be scanned.
**
** This function sucks. There should be a function 'ft_ip_is_local' instead
** which would check every available interface and use their netmask/ip address
** to decide if the ip is local or not, which would guide the interface choice.
** I mean at least that's how I think it should be. I'm not really sure. However
** it would be called every time a probe is sent which would suck very much
** because it would need to list interfaces each time (a long operation, with
** a call to an external function and memory allocation). Or the interface list
** would need to be cached (but how ? and how do we free it afterwards ?), or
** the source IP should be set for the scan (it should be added in the scan
** control structure just for the TCP scan...).
**
** What really sucks is that we have to use a function like this in the first
** place. With the ping echo scan we don't need to set the ip source address,
** the kernel does it for us. It can do that for TCP packets too, but the
** problem is that it does not compute the TCP checksum afterwards (which
** of course depends on both IPs in the ip header -_-). TCP offload could be
** used but it's not enabled by default so it requires changing the system's
** config... This is really annoying. That's why I made this function, to
** mimic the behavior of ft_nmap regarding interface choice. It's lame.
**
** TODO: try to find a more elegant solution to this
*/
static int	set_interface(t_ip *src, t_ip *dest)
{
	int			nodef, noloop;
	t_ifinfo	ifdef = { 0 }, ifloop = { 0 };

	if ((nodef = ft_net_getiface(&ifdef, dest->family, 0)) < 0
		&& ft_errno != E_FTERR_NOT_FOUND)
		return (-1);
	if ((noloop = ft_net_getiface(&ifloop, dest->family, 1)) < 0
		&& ft_errno != E_FTERR_NOT_FOUND)
		return (-1);
	if (nodef && noloop)
		return (-1);
	if ((!nodef && !ft_ip_cmp(dest, &ifdef.ip))
		|| (!noloop && !ft_ip_cmp(dest, &ifloop.ip)))
	{
		if (noloop)
			return (-1);
		ft_memcpy(src, &ifloop.ip, sizeof(t_ip));
	}
	ft_memcpy(src, &ifdef.ip, sizeof(t_ip));
	return (0);
}

static int	add_ip_header(t_packet *probe, t_scan_control *scan_ctrl,
	uint8_t protocol)
{
	t_ip		src = { 0 };
	uint8_t		version = scan_ctrl->ip.family == AF_INET ? 4 : 6;
	t_iph_args	ipargs = {
		.version = version, .dstip = &scan_ctrl->ip, .protocol = protocol,
		.hop_limit = 255, .layer5_len = scan_ctrl->payload_size,
	};

	if (protocol == IPPROTO_TCP)
	{
		if (set_interface(&src, &scan_ctrl->ip) < 0)
			return (-1);
		ipargs.srcip = &src;
	}
	ft_init_ip_header(probe->buf, &ipargs);
	return (0);
}

static int	scan_build_probe_headers(t_packet *probe, t_scan_control *scan_ctrl,
	t_scan scan, enum e_ftscan_type type)
{
	uint8_t				protocol = ft_get_scan_protocol(type,
		scan_ctrl->ip.family);
	size_t				size = scan_ctrl->ip.family == AF_INET ?
		sizeof(struct iphdr) : sizeof(struct ipv6hdr);

	if (add_ip_header(probe, scan_ctrl, protocol) < 0)
		return (-1);
	switch (type)
	{
		case E_FTSCAN_ECHO_PING:
			if (protocol == IPPROTO_ICMP)
				add_echo_ping_v4_header(probe, scan_ctrl, size, scan);
			else
				add_echo_ping_v6_header(probe, scan_ctrl, size, scan);
			break;
		case E_FTSCAN_TCP_SYN:
			add_tcp_syn_header(probe, scan_ctrl, size, scan);
			break;
	}
	return (0);
}

/*
** ft_scan_send: Build a probe for the given scan and send it
**
** The sent_ts field of the t_scan_control structure will be overwritten to
** reset the timeout for this scan. It also will be used for RTT computation.
*/
int	ft_scan_send(t_scan scan)
{
	enum e_iphdr		iph;
	enum e_ftscan_type	type;
	t_scan_control		*scan_ctrl;
	t_packet			probe = { 0 };
	struct timeval		sent_ts = { 0 };

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	++scan_ctrl->sequence;
	type = GET_SCAN_TYPE(scan);
	iph = scan_ctrl->ip.family == AF_INET ? E_IH_V4 : E_IH_V6;
	if (ft_scan_set_filter(scan) < 0
		|| scan_build_probe_headers(&probe, scan_ctrl, scan, type) < 0)
		return (-1);
	ft_packet_init(&probe, iph, NULL);
	if (gettimeofday(&sent_ts, NULL) < 0)
	{
		ft_errno = -errno;
		return (-1);
	}
	if (ft_packet_send(scan_ctrl->sendfd, &scan_ctrl->ip,
		&probe, type == E_FTSCAN_ECHO_PING) < 0)
		return (-1);
	ft_memcpy(&scan_ctrl->sent_ts, &sent_ts, sizeof(sent_ts));
	return (0);
}
