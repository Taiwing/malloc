/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_packet_filter_echo_ping.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 11:38:59 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/24 08:00:29 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const struct sock_filter	g_bpfcode_ipv4_echo_ping[] = {
	// Load and compare IPv4 protocol (ip[9])
	{ 0x30,  0,  0, 0x00000009 },
	{ 0x15,  0, 13, 0000000000 },	// protocol ICMP (1)

	// Load and compare ICMP id (ip[24])
	{ 0x28,  0,  0, 0x00000018 },
	{ 0x15,  0, 11, 0000000000 },	// ICMP id (3)

	// Load and compare ICMP sequence (ip[26])
	{ 0x28,  0,  0, 0x0000001a },
	{ 0x15,  0,  9, 0000000000 },	// ICMP sequence (5)

	// Load and compare IPv4 source address (ip[12])
	{ 0x20,  0,  0, 0x0000000c },
	// Go to 'error' label if the ip does not match
	{ 0x15,  0,  3, 0000000000 },	// ipv4 source address (7)

	// Load and compare ICMP type (ip[20])
	{ 0x30,  0,  0, 0x00000014 },
	// Go to 'success' label if is a valid reply
	{ 0x15,  4,  0, 0000000000 },	// ICMP type ICMP_ECHOREPLY (9)
	// Else Drop
	{ 0x05,  0,  0, 0x00000004 },

	// Load and compare ICMP type (again, but for the 'error' check) (ip[20])
	{ 0x30,  0,  0, 0x00000014 },
	// If type is not one or the other, Drop
	{ 0x15,  1,  0, 0000000000 },	// ICMP type ICMP_DESTUNREACH (12)
	{ 0x15,  0,  1, 0000000000 },	// ICMP type ICMP_TIME_EXCEEDED (13)
	
	// Return Match (success) or Drop
	{ 0x06,  0,  0, MAX_RAW_DATA_SIZE },
	{ 0x06,  0,  0, 0000000000 },
};

const struct sock_filter	g_bpfcode_ipv6_echo_ping[] = {
	// Load and compare IPv6 protocol (ip6[6])
	{ 0x30,  0,  0, 0x00000006 },
	{ 0x15,  0, 43, 0000000000 },	// protocol ICMP6 (1)

	// Load and compare ICMP id (ip6[44])
	{ 0x28,  0,  0, 0x0000002c },
	{ 0x15,  0, 41, 0000000000 },	// ICMP6 id (3)

	// Load and compare ICMP sequence (ip6[46])
	{ 0x28,  0,  0, 0x0000002e },
	{ 0x15,  0, 39, 0000000000 },	// ICMP6 sequence (5)

	// Load and compare IPv6 source address (ip6[8]-ip6[23])
	{ 0x30,  0,  0, 0x00000008 },
	{ 0x15,  0, 33, 0000000000 },	// first ipv6 source byte (7)
	{ 0x30,  0,  0, 0x00000009 },
	{ 0x15,  0, 31, 0000000000 },
	{ 0x30,  0,  0, 0x0000000a },
	{ 0x15,  0, 29, 0000000000 },
	{ 0x30,  0,  0, 0x0000000b },
	{ 0x15,  0, 27, 0000000000 },
	{ 0x30,  0,  0, 0x0000000c },
	{ 0x15,  0, 25, 0000000000 },
	{ 0x30,  0,  0, 0x0000000d },
	{ 0x15,  0, 23, 0000000000 },
	{ 0x30,  0,  0, 0x0000000e },
	{ 0x15,  0, 21, 0000000000 },
	{ 0x30,  0,  0, 0x0000000f },
	{ 0x15,  0, 19, 0000000000 },
	{ 0x30,  0,  0, 0x00000010 },
	{ 0x15,  0, 17, 0000000000 },
	{ 0x30,  0,  0, 0x00000011 },
	{ 0x15,  0, 15, 0000000000 },
	{ 0x30,  0,  0, 0x00000012 },
	{ 0x15,  0, 13, 0000000000 },
	{ 0x30,  0,  0, 0x00000013 },
	{ 0x15,  0, 11, 0000000000 },
	{ 0x30,  0,  0, 0x00000014 },
	{ 0x15,  0,  9, 0000000000 },
	{ 0x30,  0,  0, 0x00000015 },
	{ 0x15,  0,  7, 0000000000 },
	{ 0x30,  0,  0, 0x00000016 },
	{ 0x15,  0,  5, 0000000000 },
	{ 0x30,  0,  0, 0x00000017 },
	// Go to 'error' label if the ip does not match
	{ 0x15,  0,  3, 0000000000 },	// last ipv6 source byte (37)

	// Load and compare ICMP6 type (ip6[40])
	{ 0x30,  0,  0, 0x00000028 },
	// Go to 'success' label if is a valid reply
	{ 0x15,  4,  0, 0000000000 },	// ICMP6 type ICMP6_ECHOREPLY (39)
	// Else Drop
	{ 0x05,  0,  0, 0x00000004 },

	// Load and compare ICMP6 type (again, but for the 'error' check) (ip6[40])
	{ 0x30,  0,  0, 0x00000028 },
	{ 0x15,  1,  0, 0000000000 },	// ICMP6 type ICMP6_DESTUNREACH (42)
	{ 0x15,  0,  1, 0000000000 },	// ICMP6 type ICMP6_TIME_EXCEEDED (43)

	// Return Match (success) or Drop
	{ 0x06,  0,  0, MAX_RAW_DATA_SIZE },
	{ 0x06,  0,  0, 0000000000 },
};

static int	filter_ipv6_echo_ping(t_recv_socket recvfd, t_filter_spec *spec)
{
	uint16_t			length = BPF_FILTER_SIZE(g_bpfcode_ipv6_echo_ping);
	struct sock_filter	filter[BPF_FILTER_SIZE(g_bpfcode_ipv6_echo_ping)];
	struct sock_fprog	bpf = { .len = length, .filter = filter };

	ft_memcpy(filter, g_bpfcode_ipv6_echo_ping, sizeof(filter));
	filter[1].k = IPPROTO_ICMPV6;
	filter[3].k = spec->icmp_echo_id;
	filter[5].k = spec->icmp_echo_sequence;
	for (int i = 0; i < 16; ++i)
		filter[i * 2 + 7].k = spec->src->v6.sin6_addr.s6_addr[i];
	filter[39].k = ICMPV6_ECHO_REPLY;
	filter[42].k = ICMPV6_DEST_UNREACH;
	filter[43].k = ICMPV6_TIME_EXCEED;
	if (setsockopt(recvfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
		return (-1);
	return (0);
}

static int	filter_ipv4_echo_ping(t_recv_socket recvfd, t_filter_spec *spec)
{
	uint16_t			length = BPF_FILTER_SIZE(g_bpfcode_ipv4_echo_ping);
	struct sock_filter	filter[BPF_FILTER_SIZE(g_bpfcode_ipv4_echo_ping)];
	struct sock_fprog	bpf = { .len = length, .filter = filter };

	ft_memcpy(filter, g_bpfcode_ipv4_echo_ping, sizeof(filter));
	filter[1].k = IPPROTO_ICMP;
	filter[3].k = spec->icmp_echo_id;
	filter[5].k = spec->icmp_echo_sequence;
	filter[7].k = htonl(spec->src->v4.sin_addr.s_addr);
	filter[9].k = ICMP_ECHOREPLY;
	filter[12].k = ICMP_DEST_UNREACH;
	filter[13].k = ICMP_TIME_EXCEEDED;
	if (setsockopt(recvfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
		return (-1);
	return (0);
}

/*
** ft_packet_filter_echo_ping: filter icmp response packets for echo ping scan
**
** Structure pointer spec must have src set, dst will be ignored. The imcp echo
** id and sequence are also needed (0 value is valid but not very useful).
**
** Returns 0 on success and -1 otherwise. ft_errno is set appropriately.
*/
int			ft_packet_filter_echo_ping(t_recv_socket recvfd,
	t_filter_spec *spec)
{
	int	ret;

	if (!spec->src)
	{
		ft_errno = E_FTERR_PACKET_FILTER_NULL_IP;
		return (-1);
	}
	if (spec->src->family == AF_INET)
		ret = filter_ipv4_echo_ping(recvfd, spec);
	else
		ret = filter_ipv6_echo_ping(recvfd, spec);
	if (ret)
		ft_errno = -errno;
	return (ret);
}
