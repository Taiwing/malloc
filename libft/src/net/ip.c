/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:58:23 by yforeau           #+#    #+#             */
/*   Updated: 2022/06/13 21:49:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_ip_sock_size(const t_ip *ip)
{
	return (ip->family == AF_INET ? sizeof(struct sockaddr_in)
		: ip->family == AF_INET6 ? sizeof(struct sockaddr_in6) : 0);
}

size_t		ft_ip_size(const t_ip *ip)
{
	return (ip->family == AF_INET ? sizeof(struct in_addr)
		: ip->family == AF_INET6 ? sizeof(struct in6_addr) : 0);
}

uint8_t		*ft_ip_addr(const t_ip *ip)
{
	return (ip->family == AF_INET ? (uint8_t *)&ip->v4.sin_addr
		: ip->family == AF_INET6 ? (uint8_t *)&ip->v6.sin6_addr : NULL);
}

int			ft_ip_cmp(const t_ip *a, const t_ip *b)
{
	size_t	size = ft_ip_size(a);

	if (size != ft_ip_size(b))
		return ((int)size - (int)ft_ip_size(b));
	return (ft_memcmp(ft_ip_addr(a), ft_ip_addr(b), size));
}

void		ft_ip_set_port(t_ip *ip, uint16_t port)
{
	if (ip->family == AF_INET)
		ip->v4.sin_port = port;
	else if (ip->family == AF_INET6)
		ip->v6.sin6_port = port;
}

int			ft_ip_apply_mask(t_ip *dest, const t_ip *mask)
{
	size_t	size;
	uint8_t	*d;
	uint8_t	*m;

	if ((size = ft_ip_size(dest)) != ft_ip_size(mask))
		return (1);
	if (!(d = ft_ip_addr(dest)) || !(m = ft_ip_addr(dest)))
		return (1);
	for (size_t i = 0; i < size; ++i)
		d[i] &= m[i];
	return (0);
}

int			ft_ip_same_subnet(const t_ip *a, const t_ip *b, const t_ip *mask)
{
	t_ip	masked_a;
	t_ip	masked_b;

	if (a->family != b->family || mask->family != a->family)
		return (0);
	ft_memcpy(&masked_a, a, sizeof(masked_a));
	ft_memcpy(&masked_b, b, sizeof(masked_b));
	if (ft_ip_apply_mask(&masked_a, mask) || ft_ip_apply_mask(&masked_b, mask))
		return (0);
	return (!ft_ip_cmp(&masked_a, &masked_b));
}
