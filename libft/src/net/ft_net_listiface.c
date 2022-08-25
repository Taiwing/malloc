/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_net_listiface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 06:01:06 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/04 07:18:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ifaddrs.h>

static int		add_interface(t_list **iflist, struct ifaddrs *ifap)
{
	t_ifinfo		interface = { 0 };

	ft_strcpy(interface.name, ifap->ifa_name);
	interface.flags = ifap->ifa_flags;
	ft_memcpy(&interface.ip, ifap->ifa_addr,
		ifap->ifa_addr->sa_family == AF_INET ?
		sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6));
	ft_memcpy(&interface.netmask, ifap->ifa_netmask,
		ifap->ifa_netmask->sa_family == AF_INET ?
		sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6));
	if (!ft_lst_push_back(iflist, &interface, sizeof(interface)))
	{
		ft_errno = -errno;
		return (-1);
	}
	return (0);
}

/*
** g_ifap: this is simply kept to be freed in case of failure from add_interface
*/
#ifdef THREAD_SAFE
	__thread struct ifaddrs	*g_ifap = NULL;
#else
	struct ifaddrs			*g_ifap = NULL;
#endif

static void		ft_ifap_free(void)
{
	if (g_ifap)
	{
		freeifaddrs(g_ifap);
		g_ifap = NULL;
	}
}

static void		ft_delone_iface(void *iface, size_t size)
{
	(void)size;
	ft_memdel(&iface);
}

/*
** ft_net_listiface_free: free list output from ft_net_listiface
*/
void			ft_net_listiface_free(t_list **iflist)
{
	ft_lstdel(iflist, ft_delone_iface);
}

/*
** ft_net_listiface: list current interfaces with getifaddrs()
**
** Returns NULL on failure and sets ft_errno in this case. Otherwise, a list of
** interfaces (t_ifinfo structures) is returned.
*/
t_list		*ft_net_listiface(void)
{
	t_list			*iflist = NULL;

	if (getifaddrs(&g_ifap) < 0)
	{
		ft_errno = -errno;
		return (NULL);
	}
#ifndef NO_COLLEC
	ft_atexit(ft_ifap_free);
#endif
	for (struct ifaddrs *ifap = g_ifap; ifap; ifap = ifap->ifa_next)
	{
		if (!((ifap->ifa_flags & IFF_LOWER_UP) && (ifap->ifa_flags & IFF_UP)
			&& !(ifap->ifa_flags & IFF_DORMANT) && ifap->ifa_name
			&& ft_strlen(ifap->ifa_name) < IFNAMSIZ
			&& ifap->ifa_addr && ifap->ifa_netmask
			&& (ifap->ifa_addr->sa_family == AF_INET
			|| ifap->ifa_addr->sa_family == AF_INET6)
			&& ifap->ifa_addr->sa_family == ifap->ifa_netmask->sa_family))
			continue;
		if (add_interface(&iflist, ifap) < 0)
		{
			ft_net_listiface_free(&iflist);
			break;
		}
	}
	ft_ifap_free();
	return (iflist);
}
