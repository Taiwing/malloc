/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip_headers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:43:09 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/25 21:30:22 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	init_ipv4_header(struct iphdr *ip, t_iph_args *args)
{

	ft_bzero(ip, sizeof(struct iphdr));
	ip->ihl = 5;
	ip->version = args->version;
	ip->ttl = args->hop_limit;
	if (args->srcip)
		ft_memcpy(&ip->saddr, ft_ip_addr(args->srcip), ft_ip_size(args->srcip));
	if (args->dstip)
		ft_memcpy(&ip->daddr, ft_ip_addr(args->dstip), ft_ip_size(args->dstip));
	ip->tot_len = args->layer5_len + sizeof(struct iphdr);
	ip->protocol = args->protocol;
	if (ip->protocol == IPPROTO_ICMP)
		ip->tot_len += sizeof(struct icmphdr);
	else if (ip->protocol == IPPROTO_TCP)
		ip->tot_len += sizeof(struct tcphdr);
	else if (ip->protocol == IPPROTO_UDP)
		ip->tot_len += sizeof(struct udphdr);
	ip->tot_len = htons(ip->tot_len);
}

static void	init_ipv6_header(struct ipv6hdr *ip, t_iph_args *args)
{
	ft_bzero(ip, sizeof(struct ipv6hdr));
	ip->version = args->version;
	ip->hop_limit = args->hop_limit;
	if (args->srcip)
		ft_memcpy(&ip->saddr, ft_ip_addr(args->srcip), ft_ip_size(args->srcip));
	if (args->dstip)
		ft_memcpy(&ip->daddr, ft_ip_addr(args->dstip), ft_ip_size(args->dstip));
	ip->payload_len = args->layer5_len;
	ip->nexthdr = args->protocol;
	if (ip->nexthdr == IPPROTO_ICMPV6)
		ip->payload_len += sizeof(struct icmp6hdr);
	else if (ip->nexthdr == IPPROTO_TCP)
		ip->payload_len += sizeof(struct tcphdr);
	else if (ip->nexthdr == IPPROTO_UDP)
		ip->payload_len += sizeof(struct udphdr);
	ip->payload_len = htons(ip->payload_len);
}

void		ft_init_ip_header(void *ipptr, t_iph_args *args)
{
	if (args->version == 4)
		init_ipv4_header((struct iphdr *)ipptr, args);
	else
		init_ipv6_header((struct ipv6hdr *)ipptr, args);
}
