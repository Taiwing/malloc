/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ip_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 22:06:24 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/28 22:10:22 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_ip_str: turns given IP into a string
**
** It's a simple convenience wrapper around the inet_ntop function for the
** t_ip type. It returns a pointer to a static buffer so it's content needs
** to be copied if the function is used multiple times (this function can be
** thread-safe). If inet_ntop fails it returns NULL and errno is set.
*/
char		*ft_ip_str(const t_ip *ip)
{
#ifdef THREAD_SAFE
	static __thread char	buf[INET6_ADDRSTRLEN];
#else
	static char				buf[INET6_ADDRSTRLEN];
#endif

	return ((char *)inet_ntop(ip->family, ft_ip_addr(ip),
		buf, INET6_ADDRSTRLEN));
}
