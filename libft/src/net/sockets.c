/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 15:25:44 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 06:36:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_send_socket_init: creates a send socket on domain for given protocol
**
** Returns a raw socket that can be used to send packets. include_ip is set to
** 0 if the user does not want to give an IP header when sending a packet.
** Otherwise ft_send_socket_init will set corresponding HDRINCL socket option
** on the socket. -1 is returned on error and ft_errno is set accordingly.
*/
t_send_socket	ft_send_socket_init(int domain, int protocol, int include_ip)
{
	int	sfd, one = 1;

	if (domain != AF_INET && domain != AF_INET6)
	{
		ft_errno = E_FTERR_NET_INVALID_IP_FAMILY;
		return (-1);
	}
	if ((sfd = socket(domain, SOCK_RAW, protocol)) < 0)
	{
		ft_errno = -errno;
		return (-1);
	}
	if (include_ip && ((domain == AF_INET && setsockopt(sfd, IPPROTO_IP,
		IP_HDRINCL, &one, sizeof(int)) < 0) || (domain == AF_INET6 &&
		setsockopt(sfd, IPPROTO_IPV6, IPV6_HDRINCL, &one, sizeof(int)) < 0)))
	{
		close(sfd);
		ft_errno = -errno;
		return (-1);
	}
	return (sfd);
}

/*
** This filter simply drops every packet. This is set immediately after the
** socket is created so that we can drain the socket before setting the real
** filter we want to set. This makes sure no invalid packet will make it through
** to the packet parsing part of the program. Also, filter switching is done
** atomically which means that there will be no overlap or gap between the
** different filters set during the program's execution.
**
** source: https://natanyellin.com/posts/ebpf-filtering-done-right/
*/
struct sock_filter		g_zero_filter[] = {{ 0x06, 0, 0, 0 }};
const struct sock_fprog	g_zero_bpf = { .len = 1, .filter = g_zero_filter };

/*
** ft_recv_socket_init: creates a recv socket on domain (AF_INET or AF_INET6)
**
** The socket is of type SOCK_DGRAM and protocol ETH_P_IP or ETH_P_IPV6
** (depending on the domain). It can be used with ft_packet_filter functions.
** If an error occurs it returns -1 and sets ft_errno accordingly.
*/
t_recv_socket	ft_recv_socket_init(int domain)
{
	int		sfd;
	char	drain[1];
	int		socket_protocol = domain == AF_INET ? ETH_P_IP : ETH_P_IPV6;

	if ((sfd = socket(AF_PACKET, SOCK_DGRAM, htons(socket_protocol))) < 0)
	{
		ft_errno = -errno;
		return (-1);
	}
	if (setsockopt(sfd, SOL_SOCKET, SO_ATTACH_FILTER,
			&g_zero_bpf, sizeof(g_zero_bpf)) < 0)
	{
		close(sfd);
		ft_errno = -errno;
		return (-1);
	}
	while (recv(sfd, drain, sizeof(drain), MSG_DONTWAIT) >= 0);
	return (sfd);
}
