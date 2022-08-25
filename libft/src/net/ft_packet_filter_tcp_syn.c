/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_packet_filter_tcp_syn.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 16:00:05 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/05 09:49:15 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const struct sock_filter	g_bpfcode_ipv4_tcp_syn[] = {
	// Load and compare IPv4 source address (ip[12])
	{ 0x20,  0,  0, 0x0000000c },
	{ 0x15,  0, 31, 0000000000 },	// ipv4 source address (1)

	// Load and compare IPv4 protocol (ip[9])
	{ 0x30,  0,  0, 0x00000009 },
	{ 0x15, 10,  0, 0000000000 },	// protocol ICMP (3)
	{ 0x15,  0, 28, 0000000000 },	// protocol TCP (4)

	// Load and compare TCP source port (ip[20])
	{ 0x28,  0,  0, 0x00000014 },
	{ 0x35,  0, 26, 0000000000 },	// TCP min source port (6)
	{ 0x25, 25,  0, 0000000000 },	// TCP max source port (7)

	// Load and compare TCP destination port (ip[22])
	{ 0x28,  0,  0, 0x00000016 },
	{ 0x35,  0, 23, 0000000000 },	// TCP min destination port (9)
	{ 0x25, 22,  0, 0000000000 },	// TCP max destination port (10)

	// Load and compare TCP flags (ip[33])
	{ 0x30,  0,  0, 0x00000021 },
	{ 0x45, 19,  0, 0000000000 },	// TCP flags TH_SYN | TH_RST (12)
	{ 0x05,  0,  0, 0x00000013 },

	// Load and compare ICMP type (ip[20])
	{ 0x20,  0,  0, 0x00000014 },
	{ 0x15,  0, 17, 0000000000 },	// ICMP type DEST_UNREACH (15)

	// Load and compare ICMP code (ip[21])
	{ 0x20,  0,  0, 0x00000015 },
	{ 0x15, 14,  0, 0000000000 },
	{ 0x15, 13,  0, 0x00000001 },
	{ 0x15, 12,  0, 0x00000002 },
	{ 0x15, 11,  0, 0x00000003 },
	{ 0x15, 10,  0, 0x00000009 },
	{ 0x15,  9,  0, 0x0000000a },
	{ 0x15,  0,  9, 0x0000000d },

	// Load and compare ICMP protocol (ip[37])
	{ 0x20,  0,  0, 0x00000025 },
	{ 0x15,  0,  7, 0000000000 },	// ICMP protocol IPPROTO_TCP (25)

	// Load and compare ICMP TCP payload source port (ip[48])
	{ 0x28,  0,  0, 0x00000030 },
	{ 0x35,  0,  5, 0000000000 },	// TCP min source port (27)
	{ 0x25,  4,  0, 0000000000 },	// TCP max source port (28)

	// Load and compare ICMP TCP payload destination port (ip[50])
	{ 0x28,  0,  0, 0x00000032 },
	{ 0x35,  0,  2, 0000000000 },	// TCP min destination port (30)
	{ 0x25,  1,  0, 0000000000 },	// TCP max destination port (31)

	// Return or Drop
	{ 0x06,  0,  0, MAX_RAW_DATA_SIZE },
	{ 0x06,  0,  0, 0000000000 },
};

const struct sock_filter	g_bpfcode_ipv6_tcp_syn[] = {
	// Load and compare IPv6 source address (ip6[8]-ip6[23])
	{ 0x30,  0,  0, 0x00000008 },
	{ 0x15,  0, 59, 0000000000 },	// first ipv6 source byte (1)
	{ 0x30,  0,  0, 0x00000009 },
	{ 0x15,  0, 57, 0000000000 },
	{ 0x30,  0,  0, 0x0000000a },
	{ 0x15,  0, 55, 0000000000 },
	{ 0x30,  0,  0, 0x0000000b },
	{ 0x15,  0, 53, 0000000000 },
	{ 0x30,  0,  0, 0x0000000c },
	{ 0x15,  0, 51, 0000000000 },
	{ 0x30,  0,  0, 0x0000000d },
	{ 0x15,  0, 49, 0000000000 },
	{ 0x30,  0,  0, 0x0000000e },
	{ 0x15,  0, 47, 0000000000 },
	{ 0x30,  0,  0, 0x0000000f },
	{ 0x15,  0, 45, 0000000000 },
	{ 0x30,  0,  0, 0x00000010 },
	{ 0x15,  0, 43, 0000000000 },
	{ 0x30,  0,  0, 0x00000011 },
	{ 0x15,  0, 41, 0000000000 },
	{ 0x30,  0,  0, 0x00000012 },
	{ 0x15,  0, 39, 0000000000 },
	{ 0x30,  0,  0, 0x00000013 },
	{ 0x15,  0, 37, 0000000000 },
	{ 0x30,  0,  0, 0x00000014 },
	{ 0x15,  0, 35, 0000000000 },
	{ 0x30,  0,  0, 0x00000015 },
	{ 0x15,  0, 33, 0000000000 },
	{ 0x30,  0,  0, 0x00000016 },
	{ 0x15,  0, 31, 0000000000 },
	{ 0x30,  0,  0, 0x00000017 },
	{ 0x15,  0, 29, 0000000000 },

	// Load and compare IPv6 protocol (ip6[6])
	{ 0x30,  0,  0, 0x00000006 },
	{ 0x15, 10,  0, 0000000000 },	// protocol ICMP6 (33)
	{ 0x15,  0, 26, 0000000000 },	// protocol TCP (34)

	// Load and compare TCP source port (ip6[40])
	{ 0x28,  0,  0, 0x00000028 },
	{ 0x35,  0, 24, 0000000000 },	// TCP min source port (36)
	{ 0x25, 23,  0, 0000000000 },	// TCP max source port (37)

	// Load and compare TCP destination port (ip6[42])
	{ 0x28,  0,  0, 0x0000002a },
	{ 0x35,  0, 21, 0000000000 },	// TCP min destination port (39)
	{ 0x25, 20,  0, 0000000000 },	// TCP max destination port (40)

	// Load and compare TCP flags (ip6[53])
	{ 0x30,  0,  0, 0x00000035 },
	{ 0x45, 17,  0, 0000000000 },	// TCP flags TH_SYN | TH_RST (42)
	{ 0x05,  0,  0, 0x00000011 },

	// Load and compare ICMP6 type (ip6[40])
	{ 0x20,  0,  0, 0x00000028 },
	{ 0x15,  0, 15, 0000000000 },	// ICMP type DEST_UNREACH (45)

	// Load and compare ICMP6 type (ip6[41])
	{ 0x20,  0,  0, 0x00000029 },
	{ 0x15, 12,  0, 0000000000 },
	{ 0x15, 11,  0, 0x00000001 },
	{ 0x15, 10,  0, 0x00000002 },
	{ 0x15,  9,  0, 0x00000003 },
	{ 0x15,  0,  9, 0x00000004 },

	// Load and compare ICMP protocol (ip6[54])
	{ 0x20,  0,  0, 0x00000036 },
	{ 0x15,  0,  7, 0000000000 },	// ICMP protocol TCP (53)

	// Load and compare ICMP TCP payload source port (ip[88])
	{ 0x28,  0,  0, 0x00000058 },
	{ 0x35,  0,  5, 0000000000 },	// TCP min source port (55)
	{ 0x25,  4,  0, 0000000000 },	// TCP max source port (56)

	// Load and compare ICMP TCP payload destination port (ip[90])
	{ 0x28,  0,  0, 0x0000005a },
	{ 0x35,  0,  2, 0000000000 },	// TCP min destination port (58)
	{ 0x25,  1,  0, 0000000000 },	// TCP min destination port (59)

	// Return or Drop
	{ 0x06,  0,  0, MAX_RAW_DATA_SIZE },
	{ 0x06,  0,  0, 0000000000 },
};


static int	filter_ipv6_tcp_syn(t_recv_socket recvfd, t_filter_spec *spec)
{
	uint16_t			length = BPF_FILTER_SIZE(g_bpfcode_ipv6_tcp_syn);
	struct sock_filter	filter[BPF_FILTER_SIZE(g_bpfcode_ipv6_tcp_syn)];
	struct sock_fprog	bpf = { .len = length, .filter = filter };

	ft_memcpy(filter, g_bpfcode_ipv6_tcp_syn, sizeof(filter));
	for (int i = 0; i < 16; ++i)
		filter[i * 2 + 1].k = spec->src->v6.sin6_addr.s6_addr[i];
	filter[33].k = IPPROTO_ICMPV6;
	filter[34].k = IPPROTO_TCP;
	filter[36].k = spec->min_src_port;
	filter[37].k = spec->max_src_port;
	filter[39].k = spec->min_dst_port;
	filter[40].k = spec->max_dst_port;
	filter[42].k = TH_SYN | TH_RST;
	filter[45].k = ICMPV6_DEST_UNREACH;
	filter[53].k = IPPROTO_TCP;
	filter[55].k = spec->min_dst_port;
	filter[56].k = spec->max_dst_port;
	filter[58].k = spec->min_src_port;
	filter[59].k = spec->max_src_port;
	if (setsockopt(recvfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
		return (-1);
	return (0);
}

static int	filter_ipv4_tcp_syn(t_recv_socket recvfd, t_filter_spec *spec)
{
	uint16_t			length = BPF_FILTER_SIZE(g_bpfcode_ipv4_tcp_syn);
	struct sock_filter	filter[BPF_FILTER_SIZE(g_bpfcode_ipv4_tcp_syn)];
	struct sock_fprog	bpf = { .len = length, .filter = filter };

	ft_memcpy(filter, g_bpfcode_ipv4_tcp_syn, sizeof(filter));
	filter[1].k = htonl(spec->src->v4.sin_addr.s_addr);
	filter[3].k = IPPROTO_ICMP;
	filter[4].k = IPPROTO_TCP;
	filter[6].k = spec->min_src_port;
	filter[7].k = spec->max_src_port;
	filter[9].k = spec->min_dst_port;
	filter[10].k = spec->max_dst_port;
	filter[12].k = TH_SYN | TH_RST;
	filter[15].k = ICMP_DEST_UNREACH;
	filter[25].k = IPPROTO_TCP;
	filter[27].k = spec->min_dst_port;
	filter[28].k = spec->max_dst_port;
	filter[30].k = spec->min_src_port;
	filter[31].k = spec->max_src_port;
	if (setsockopt(recvfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
		return (-1);
	return (0);
}

/*
** ft_packet_filter_tcp_syn: filter tcp/icmp packets for tcp syn scan
**
** Structure pointer spec must have src set, dst will be ignored. The imcp echo
** id and sequence are also needed (0 value is valid but not very useful).
**
** Returns 0 on success and -1 otherwise. ft_errno is set appropriately.
*/
int			ft_packet_filter_tcp_syn(t_recv_socket recvfd,
	t_filter_spec *spec)
{
	int	ret;

	if (!spec->src)
	{
		ft_errno = E_FTERR_PACKET_FILTER_NULL_IP;
		return (-1);
	}
	if (spec->src->family == AF_INET)
		ret = filter_ipv4_tcp_syn(recvfd, spec);
	else
		ret = filter_ipv6_tcp_syn(recvfd, spec);
	if (ret)
		ft_errno = -errno;
	return (ret);
}
