/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_ip.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 16:28:44 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/15 16:40:20 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_get_ip: Get ip address from given host string using getaddrinfo()
**
** ip: is a valid destination ip to be filled in
** target: string representing the host (IPv6/IPv4 address, hostname, etc...)
** family: AF_INET (IPv4), AF_INET6 (IPv6) or AF_UNSPEC (both)
** return: the error from getaddrinfo if any (can use gai_strerror() on it)
*/
int			ft_get_ip(t_ip *ip, const char *target, int family)
{
	int					ret;
	struct addrinfo		hints = { 0 }, *destinfo = NULL, *host;

	hints.ai_family = family;
	if (!(ret = getaddrinfo(target, NULL, &hints, &destinfo))
		&& destinfo->ai_family != AF_INET && destinfo->ai_family != AF_INET6)
		ret = EAI_FAMILY;
	if (!ret)
	{
		host = destinfo;
		if (family == AF_UNSPEC)
		{
			while (host->ai_family != AF_INET && host->ai_next)
				host = host->ai_next;
			if (host->ai_family != AF_INET)
				host = destinfo;
		}
		ft_memcpy((void *)ip, (void *)host->ai_addr, host->ai_addrlen);
	}
	if (destinfo)
		freeaddrinfo(destinfo);
	return (ret);
}
