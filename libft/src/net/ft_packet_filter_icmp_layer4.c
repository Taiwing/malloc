/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_packet_filter_icmp_layer4.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 06:42:50 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 06:46:41 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const struct sock_filter	g_bpfcode_ipv4_icmp_layer4[] = {
	// Load and compare IPv4 protocol (ip[9])
	{ 0x30,  0,  0, 0x00000009 },
	{ 0x15,  0, 13, 0000000000 },	// protocol ICMP (1)

	// Load and compare IPv4 source address (ip[12])
	{ 0x20,  0,  0, 0x0000000c },
	{ 0x15,  0, 11, 0000000000 },	// ipv4 source address (3)

	// Load and compare IPv4 destination address (ip[16])
	{ 0x20,  0,  0, 0x00000010 },
	{ 0x15,  0,  9, 0000000000 },	// ipv4 destination address (5)

	// Load and compare ICMP protocol (ip[37])
	{ 0x30,  0,  0, 0x00000025 },
	{ 0x15,  0,  7, 0000000000 },	// ICMP protocol (7)

	// Load and compare ICMP payload TCP or UDP source port (ip[48])
	{ 0x28,  0,  0, 0x00000030 },
	{ 0x35,  0,  5, 0000000000 },	// smallest source port (9)
	{ 0x25,  4,  0, 0000000000 },	// biggest source port (10)

	// Load and compare ICMP payload TCP or UDP destination port (ip[50])
	{ 0x28,  0,  0, 0x00000032 },
	{ 0x35,  0,  2, 0000000000 },	// smallest destination port (12)
	{ 0x25,  1,  0, 0000000000 },	// biggest destination port (13)

	// Return Match or Drop
	{ 0x06,  0,  0, MAX_RAW_DATA_SIZE },
	{ 0x06,  0,  0, 0000000000 },
};

const struct sock_filter	g_bpfcode_ipv6_icmp_layer4[] = {
	// Load and compare IPv6 protocol (ip6[6])
	{ 0x30,  0,  0, 0x00000006 },
	{ 0x15,  0, 73, 0000000000 },	// protocol ICMP6 (1)

	// Load and compare IPv6 source address (ip6[8]-ip6[23])
	{ 0x30,  0,  0, 0x00000008 },
	{ 0x15,  0, 71, 0000000000 },	// first ipv6 source byte (3)
	{ 0x30,  0,  0, 0x00000009 },
	{ 0x15,  0, 69, 0000000000 },
	{ 0x30,  0,  0, 0x0000000a },
	{ 0x15,  0, 67, 0000000000 },
	{ 0x30,  0,  0, 0x0000000b },
	{ 0x15,  0, 65, 0000000000 },
	{ 0x30,  0,  0, 0x0000000c },
	{ 0x15,  0, 63, 0000000000 },
	{ 0x30,  0,  0, 0x0000000d },
	{ 0x15,  0, 61, 0000000000 },
	{ 0x30,  0,  0, 0x0000000e },
	{ 0x15,  0, 59, 0000000000 },
	{ 0x30,  0,  0, 0x0000000f },
	{ 0x15,  0, 57, 0000000000 },
	{ 0x30,  0,  0, 0x00000010 },
	{ 0x15,  0, 55, 0000000000 },
	{ 0x30,  0,  0, 0x00000011 },
	{ 0x15,  0, 53, 0000000000 },
	{ 0x30,  0,  0, 0x00000012 },
	{ 0x15,  0, 51, 0000000000 },
	{ 0x30,  0,  0, 0x00000013 },
	{ 0x15,  0, 49, 0000000000 },
	{ 0x30,  0,  0, 0x00000014 },
	{ 0x15,  0, 47, 0000000000 },
	{ 0x30,  0,  0, 0x00000015 },
	{ 0x15,  0, 45, 0000000000 },
	{ 0x30,  0,  0, 0x00000016 },
	{ 0x15,  0, 43, 0000000000 },
	{ 0x30,  0,  0, 0x00000017 },
	{ 0x15,  0, 41, 0000000000 },	// last ipv6 source byte (33)

	// Load and compare IPv6 destination address (ip6[24]-ip6[39])
	{ 0x30,  0,  0, 0x00000018 },
	{ 0x15,  0, 39, 0000000000 },	// first ipv6 destination byte (35)
	{ 0x30,  0,  0, 0x00000019 },
	{ 0x15,  0, 37, 0000000000 },
	{ 0x30,  0,  0, 0x0000001a },
	{ 0x15,  0, 35, 0000000000 },
	{ 0x30,  0,  0, 0x0000001b },
	{ 0x15,  0, 33, 0000000000 },
	{ 0x30,  0,  0, 0x0000001c },
	{ 0x15,  0, 31, 0000000000 },
	{ 0x30,  0,  0, 0x0000001d },
	{ 0x15,  0, 29, 0000000000 },
	{ 0x30,  0,  0, 0x0000001e },
	{ 0x15,  0, 27, 0000000000 },
	{ 0x30,  0,  0, 0x0000001f },
	{ 0x15,  0, 25, 0000000000 },
	{ 0x30,  0,  0, 0x00000020 },
	{ 0x15,  0, 23, 0000000000 },
	{ 0x30,  0,  0, 0x00000021 },
	{ 0x15,  0, 21, 0000000000 },
	{ 0x30,  0,  0, 0x00000022 },
	{ 0x15,  0, 19, 0000000000 },
	{ 0x30,  0,  0, 0x00000023 },
	{ 0x15,  0, 17, 0000000000 },
	{ 0x30,  0,  0, 0x00000024 },
	{ 0x15,  0, 15, 0000000000 },
	{ 0x30,  0,  0, 0x00000025 },
	{ 0x15,  0, 13, 0000000000 },
	{ 0x30,  0,  0, 0x00000026 },
	{ 0x15,  0, 11, 0000000000 },
	{ 0x30,  0,  0, 0x00000027 },
	{ 0x15,  0,  9, 0000000000 },	// last ipv6 destination byte (65)

	// Load and compare ICMP6 payload protocol (ip6[54])
	{ 0x30,  0,  0, 0x00000036 },
	{ 0x15,  0,  7, 0000000000 },	// protocol (67)

	// Load and compare ICMP6 payload TCP or UDP source port (ip6[88])
	{ 0x28,  0,  0, 0x00000058 },
	{ 0x35,  0,  5, 0000000000 },	// smallest source port (69)
	{ 0x25,  4,  0, 0000000000 },	// biggest source port (70)

	// Load and compare ICMP6 payload TCP or UDP destination port (ip6[90])
	{ 0x28,  0,  0, 0x0000005a },
	{ 0x35,  0,  2, 0000000000 },	// smallest destination port (72)
	{ 0x25,  1,  0, 0000000000 },	// biggest destination port (73)

	// Return Match or Drop
	{ 0x06,  0,  0, MAX_RAW_DATA_SIZE },
	{ 0x06,  0,  0, 0000000000 },
};

static int	filter_ipv6_icmp_layer4(t_recv_socket recvfd, t_filter_spec *spec)
{
	uint16_t			length = BPF_FILTER_SIZE(g_bpfcode_ipv6_icmp_layer4);
	struct sock_filter	filter[BPF_FILTER_SIZE(g_bpfcode_ipv6_icmp_layer4)];
	struct sock_fprog	bpf = { .len = length, .filter = filter };

	ft_memcpy(filter, g_bpfcode_ipv6_icmp_layer4, sizeof(filter));
	filter[1].k = IPPROTO_ICMPV6;
	for (int i = 0; i < 16; ++i)
		filter[i * 2 + 3].k = spec->src->v6.sin6_addr.s6_addr[i];
	for (int i = 0; i < 16; ++i)
		filter[i * 2 + 35].k = spec->dst->v6.sin6_addr.s6_addr[i];
	filter[67].k = spec->icmp_protocol;
	filter[69].k = spec->min_src_port;
	filter[70].k = spec->max_src_port;
	filter[72].k = spec->min_dst_port;
	filter[73].k = spec->max_dst_port;
	if (setsockopt(recvfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
		return (-1);
	return (0);
}

static int	filter_ipv4_icmp_layer4(t_recv_socket recvfd, t_filter_spec *spec)
{
	uint16_t			length = BPF_FILTER_SIZE(g_bpfcode_ipv4_icmp_layer4);
	struct sock_filter	filter[BPF_FILTER_SIZE(g_bpfcode_ipv4_icmp_layer4)];
	struct sock_fprog	bpf = { .len = length, .filter = filter };

	ft_memcpy(filter, g_bpfcode_ipv4_icmp_layer4, sizeof(filter));
	filter[1].k = IPPROTO_ICMP;
	filter[3].k = htonl(spec->src->v4.sin_addr.s_addr);
	filter[5].k = htonl(spec->dst->v4.sin_addr.s_addr);
	filter[7].k = spec->icmp_protocol;
	filter[9].k = spec->min_src_port;
	filter[10].k = spec->max_src_port;
	filter[12].k = spec->min_dst_port;
	filter[13].k = spec->max_dst_port;
	if (setsockopt(recvfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
		return (-1);
	return (0);
}

int			check_icmp_layer4_filter_spec(t_filter_spec *spec)
{
	if (!spec->src || !spec->dst)
	{
		ft_errno = E_FTERR_PACKET_FILTER_NULL_IP;
		return (1);
	}
	else if (spec->src->family != spec->dst->family
		|| (spec->src->family != AF_INET && spec->src->family != AF_INET6))
	{
		ft_errno = E_FTERR_NET_INVALID_IP_FAMILY;
		return (1);
	}
	else if (spec->min_src_port > spec->max_src_port
		|| spec->min_dst_port > spec->max_dst_port)
	{
		ft_errno = E_FTERR_PACKET_FILTER_INVALID_PORT_RANGE;
		return (1);
	}
	else if (spec->icmp_protocol != IPPROTO_TCP
		&& spec->icmp_protocol != IPPROTO_UDP)
	{
		ft_errno = E_FTERR_NET_INVALID_PROTOCOL;
		return (1);
	}
	return (0);
}

/*
** ft_packet_filter_icmp_layer4: filter icmp response packets with a layer4
** payload on recvfd socket
**
** Structure pointer spec must have both src and dst ip set and with the same
** ip family (either AF_INET or AF_INET6). It also need a valid icmp_protocol
** (IPPROTO_TCP or IPPROTO_UDP), and src-dst port ranges.
**
** Returns 0 on success and -1 otherwise. ft_errno is set appropriately.
*/
int			ft_packet_filter_icmp_layer4(t_recv_socket recvfd,
				t_filter_spec *spec)
{
	int	ret;

	if (check_icmp_layer4_filter_spec(spec))
		return (-1);
	if (spec->src->family == AF_INET)
		ret = filter_ipv4_icmp_layer4(recvfd, spec);
	else
		ret = filter_ipv6_icmp_layer4(recvfd, spec);
	if (ret)
		ft_errno = -errno;
	return (ret);
}
