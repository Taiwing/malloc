/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_packet_send.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 09:52:26 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 06:48:02 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_header_level(uint8_t **data, size_t *size,
	t_packet *packet, int header_level)
{
	uint8_t	*raw_data = *data;

	switch (header_level)
	{
		case 1: *data = packet->nexthdr != E_NH_NONE ? (uint8_t *)packet->next :
			(uint8_t *)packet->ip + ft_iphdr_size(packet->iphdr);
			break;
		case 2: *data = packet->nextiphdr != E_IH_NONE ?
			(uint8_t *)packet->nextiphdr :
			(uint8_t *)packet->next + ft_nexthdr_size(packet->nexthdr);
			break;
		case 3: *data = packet->lasthdr != E_NH_NONE ? (uint8_t *)packet->last :
			(uint8_t *)packet->nextip + ft_iphdr_size(packet->nextiphdr);
			break;
		case 4: *data = (uint8_t *)packet->last
			+ ft_nexthdr_size(packet->lasthdr);
			break;
		default: return (-1);
	}
	*size -= *data - raw_data;
	return (0);
}

/*
** ft_packet_send: send an IP packet to dst IP via sendfd
**
** sendfd: initialized send socket
** dst: IP address to send the packet to
** packet: IP packet
** header_level: from 0 to 4 where 0 is the entire packet, 1 is from the
** nexthdr, 2 is from nextiphdr, 3 is from lasthdr and 4 is the payload
**
** Returns 0 if packet was successfully sent, -1 otherwise. Sets ft_errno
** appropriatly in this case.
*/
int	ft_packet_send(t_send_socket sendfd, t_ip *dst, t_packet *packet,
	int header_level)
{
	struct sockaddr	*sockdst = (struct sockaddr *)dst;
	size_t			size = packet->size;
	uint8_t			*data = packet->raw_data;

	if (header_level != MIN_HEADER_LEVEL
		&& get_header_level(&data, &size, packet, header_level) < 0)
		ft_errno = E_FTERR_PACKET_INVALID_HEADER_LEVEL;
	else if (sendto(sendfd, data, size, 0, sockdst, ft_ip_sock_size(dst)) < 0)
		ft_errno = -errno;
	else
		return (0);
	return (-1);
}
