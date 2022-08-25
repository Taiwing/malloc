/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 12:17:45 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/25 21:32:16 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_iphdr_size(enum e_iphdr iphdr)
{
	switch (iphdr)
	{
		case E_IH_NONE: return (0);							break;
		case E_IH_V4: return (sizeof(struct iphdr));		break;
		case E_IH_V6: return (sizeof(struct ipv6hdr));		break;
	}
	return (0);
}

size_t		ft_nexthdr_size(enum e_nexthdr nexthdr)
{
	switch (nexthdr)
	{
		case E_NH_NONE: return (0);							break;
		case E_NH_ICMP: return (sizeof(struct icmphdr));	break;
		case E_NH_ICMP6: return (sizeof(struct icmp6hdr));	break;
		case E_NH_TCP: return (sizeof(struct tcphdr));		break;
		case E_NH_UDP: return (sizeof(struct udphdr));		break;
	}
	return (0);
}

static void				set_packet_size(t_packet *packet)
{
	size_t	size;

	switch (packet->iphdr)
	{
		case E_IH_V4:
			size = ntohs(packet->ip->v4.tot_len);
			break;
		case E_IH_V6:
			size = ntohs(packet->ip->v6.payload_len) + sizeof(struct ipv6hdr);
			break;
		case E_IH_NONE:
			size = 0;
			break;
	}
	packet->size = size > MAX_RAW_DATA_SIZE ? MAX_RAW_DATA_SIZE : size;
}

static enum e_nexthdr	set_nexthdr(uint8_t type, enum e_iphdr iph)
{
	if (iph == E_IH_NONE)
		return (E_NH_NONE);
	else if (type == IPPROTO_UDP)
		return (E_NH_UDP);
	else if (type == IPPROTO_TCP)
		return (E_NH_TCP);
	else if (type == IPPROTO_ICMP && iph == E_IH_V4)
		return (E_NH_ICMP);
	else if (type == IPPROTO_ICMPV6 && iph == E_IH_V6)
		return (E_NH_ICMP6);
	return (E_NH_NONE);
}

void					ft_packet_reset(t_packet *packet, uint8_t *datap)
{
	packet->iphdr = packet->nextiphdr = E_IH_NONE;
	packet->nexthdr = packet->lasthdr = E_NH_NONE;
	packet->ip = packet->nextip = NULL;
	packet->next = packet->last = NULL;
	packet->size = 0;
	packet->raw_data = datap ? datap : packet->buf;
}

void					ft_packet_init(t_packet *packet,
		enum e_iphdr iph, uint8_t *datap)
{
	uint8_t	type;

	ft_packet_reset(packet, datap);
	if ((packet->iphdr = iph) == E_IH_NONE)
		return;
	packet->ip = (t_iphdr *)packet->raw_data;
	type = iph == E_IH_V4 ? packet->ip->v4.protocol : packet->ip->v6.nexthdr;
	if ((packet->nexthdr = set_nexthdr(type, iph)) == E_NH_NONE)
		return (set_packet_size(packet));
	packet->next = (t_nexthdr *)(packet->raw_data + ft_iphdr_size(iph));
	if (packet->nexthdr == E_NH_TCP || packet->nexthdr == E_NH_UDP)
		return (set_packet_size(packet));
	packet->nextiphdr = iph;
	packet->nextip = (t_iphdr *)((uint8_t *)packet->next
		+ ft_nexthdr_size(packet->nexthdr));
	type = iph == E_IH_V4 ? packet->nextip->v4.protocol
		: packet->nextip->v6.nexthdr;
	if ((packet->lasthdr = set_nexthdr(type, iph)) != E_NH_NONE)
		packet->last = (t_nexthdr *)((uint8_t *)packet->nextip
			+ ft_iphdr_size(iph));
	set_packet_size(packet);
}
