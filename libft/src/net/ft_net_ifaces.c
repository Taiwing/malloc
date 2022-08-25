/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_net_ifaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 21:22:13 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/04 08:56:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** e_ftnet_iface: interface types (index in the netconf iface array)
*/
enum e_ftnet_iface	{
	E_FTNET_IFACE_DEFAULT_V4	= 0,	// IPv4 default interface
	E_FTNET_IFACE_DEFAULT_V6	= 1,	// IPv6 default interface
	E_FTNET_IFACE_LOOPBACK_V4	= 2,	// IPv4 loopback interface
	E_FTNET_IFACE_LOOPBACK_V6	= 3,	// IPv6 loopback interface
};

#define FTNET_IFACE_COUNT	4
#define	FTNET_IFACE_TYPE(domain, loopback)\
	((!!loopback * 2) + (domain == AF_INET6))

/*
** t_network_config: Network configuration structure for the libft
**
** iface: table of interfaces
** iface_on: is the corresponding interface type on
** initialized: is the network config initialized or not
*/
typedef struct	s_network_config
{
	t_ifinfo	iface[FTNET_IFACE_COUNT];
	int			iface_on[FTNET_IFACE_COUNT];
	int			initialized;
}				t_network_config;

/*
** g_netconf: current network configuration for the ft_scan functions
**
** Contains the IPv4 and IPv6 default and loopback interfaces. Can only be
** accessed and modified by the user through the ft_net_*iface functions.
*/
#ifdef THREAD_SAFE
	__thread t_network_config	g_netconf = { 0 };
#else
	t_network_config			g_netconf = { 0 };
#endif

/*
** ft_netconf_init: initialize libft's net config
**
** user_iflist: iflist from ft_net_listiface if user has already called it
**
** If user_iflist is not given ft_netconf_init will call ft_net_listiface itself
** and free it once net config is initialized. The initiazation process simply
** copies interfaces selected from iflist to g_netconf.
*/
int			ft_netconf_init(t_list *user_iflist)
{
	t_list	*iflist = user_iflist;

	ft_bzero(&g_netconf, sizeof(t_network_config));
	if (!iflist && !(iflist = ft_net_listiface()))
		return (-1);
	for (t_list *lst = iflist; lst; lst = lst->next)
	{
		t_ifinfo *ifinfo = lst->content;
		int iface_index = FTNET_IFACE_TYPE(ifinfo->ip.family,
			(ifinfo->flags & IFF_LOOPBACK));
		if (!g_netconf.iface_on[iface_index])
		{
			g_netconf.initialized = 1;
			g_netconf.iface_on[iface_index] = 1;
			ft_memcpy(&g_netconf.iface[iface_index], ifinfo, sizeof(t_ifinfo));
		}
	}
	if (!user_iflist)
		ft_net_listiface_free(&iflist);
	return (0);
}

/*
** ft_net_getiface: access libft's net config
**
** dst: a valid interface pointer
** domain: AF_INET or AF_INET6
** loopback: boolean set to true to get a loopback interface
*/
int			ft_net_getiface(t_ifinfo *dst, int domain, int loopback)
{
	int	iface_index;

	if (domain != AF_INET && domain != AF_INET6)
	{
		ft_errno = E_FTERR_NET_INVALID_IP_FAMILY;
		return (-1);
	}
	else if (!g_netconf.initialized && ft_netconf_init(NULL) < 0)
		return (-1);
	iface_index = FTNET_IFACE_TYPE(domain, loopback);
	if (!g_netconf.iface_on[iface_index])
	{
		ft_errno = E_FTERR_NOT_FOUND;
		return (-1);
	}
	ft_memcpy(dst, &g_netconf.iface[iface_index], sizeof(t_ifinfo));
	return (0);
}

/*
** ft_net_setiface: change libft's net config
**
** loopback: change the loopback field
** src: source interface (the domain is taken from its IP)
**
** This will change the source interface for the scan functions. Can be used
** with an item of the list returned by ft_net_listiface.
*/
int			ft_net_setiface(int loopback, const t_ifinfo *src)
{
	int	domain = src->ip.family, iface_index;

	if (domain != AF_INET && domain != AF_INET6)
	{
		ft_errno = E_FTERR_NET_INVALID_IP_FAMILY;
		return (-1);
	}
	iface_index = FTNET_IFACE_TYPE(domain, loopback);
	g_netconf.initialized = 1;
	g_netconf.iface_on[iface_index] = 1;
	ft_memcpy(&g_netconf.iface[iface_index], src, sizeof(t_ifinfo));
	return (0);
}
