/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ip_rand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 08:04:22 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/09 04:39:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_ip_is_local: is the given ip on localhost
**
** Only handles when the ip starts with '127' or is zeroed out or is equal to
** '::1' if it is an ipv6 address.
**
** TODO: do this the right way, presumably by listing network interfaces and
** comparing the given IP (possibly taking the mask into account) to their IP
*/
static int	ft_ip_is_local(t_ip *ip)
{
	uint8_t	byte;

	if (ip->family == AF_INET)
	{
		byte = htonl(ip->v4.sin_addr.s_addr) >> 24;
		return (byte == 127 || !ip->v4.sin_addr.s_addr);
	}
	for (int i = 0; i < 15; ++i)
		if (ip->v6.sin6_addr.s6_addr[i])
			return (0);
	return (ip->v6.sin6_addr.s6_addr[15] == 1);
}

static int	init_rand_ipv6(t_ip *ip, int flags)
{
	uint64_t	rand;

	ip->v6.sin6_family = AF_INET6;
	ip->v6.sin6_port = 0;
	ip->v6.sin6_flowinfo = 0;
	ip->v6.sin6_scope_id = 0;
	if (flags & E_IPRAND_ALLOW_LOCAL)
		return (0);
	while (ft_ip_is_local(ip))
	{
		if (!ft_rand_uint64((uint64_t *)&rand, 0, UINT64_MAX))
		{
			ft_errno = E_FTERR_RAND_SOURCE;
			return (-1);
		}
		ft_memcpy(&ip->v6.sin6_addr.s6_addr, &rand, sizeof(rand));
	}
	return (0);
}

static int	init_rand_ipv4(t_ip *ip, int flags)
{
	uint64_t	rand;

	ip->v4.sin_family = AF_INET;
	ip->v4.sin_port = 0;
	if (flags & E_IPRAND_ALLOW_LOCAL)
		return (0);
	while (ft_ip_is_local(ip))
	{
		if (!ft_rand_uint64((uint64_t *)&rand, 0, UINT32_MAX))
		{
			ft_errno = E_FTERR_RAND_SOURCE;
			return (-1);
		}
		ip->v4.sin_addr.s_addr = (uint32_t)rand;
	}
	return (0);
}

static int	set_domain(size_t seed, int flags)
{
	uint64_t	rand = seed;

	if (flags & E_IPRAND_RANDOM_DOMAIN)
	{
		if (!ft_rand_uint64((uint64_t *)&rand, 0, UINT64_MAX))
		{
			ft_errno = E_FTERR_RAND_SOURCE;
			return (-1);
		}
	}
	return (rand & 1 ? AF_INET6 : AF_INET);
}

/*
** ft_ip_rand: generate count random ips on given domain
**
** ip: array of ip structures or single ip pointer
** count: size of the ip array
** domain: family of ips to generate
** flags: configure ip generation
**
** This function technically works with IPv6 but the result is basically
** useless. The IPv6 IP range is big and sparsely used that the chances of
** randomly stumbling upon a running host are infinitesimal. So if this function
** is used for host discovery it should always be done with AF_INET for the
** domain. Otherwise we're just wasting time and CPU cycles.
*/
int			ft_ip_rand(t_ip *ip, size_t count, int domain, int flags)
{
	int	d;

	if (domain != AF_INET && domain != AF_INET6 && domain != AF_UNSPEC)
	{
		ft_errno = E_FTERR_NET_INVALID_IP_FAMILY;
		return (-1);
	}
	else if (count && ft_rand_source(ip, count * sizeof(t_ip), NULL, 0) != ip)
	{
		ft_errno = E_FTERR_RAND_SOURCE;
		return (-1);
	}
	while (count)
	{
		--count;
		d = domain;
		if (d == AF_UNSPEC && (d = set_domain(count, flags)) < 0)
			return (-1);
		else if (d == AF_INET && init_rand_ipv4(ip + count, flags) < 0)
			return (-1);
		else if (d == AF_INET6 && init_rand_ipv6(ip + count, flags) < 0)
			return (-1);
	}
	return (0);
}
