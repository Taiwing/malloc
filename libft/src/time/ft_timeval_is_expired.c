/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_timeval_is_expired.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:55:52 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/20 06:42:21 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_timeval_is_expired: check if date has passed expiry
**
** More precisely, this function returns 1 (true) if the timestamp of date is
** equal or greater to that of the expiry. If date or expiry is NULL, it is
** gonna be replace by gettimeofday output. It also returns -1 and sets ft_errno
** in case of error.
**
** So here are the possibilies:
**
** -- Basic cases --
** A: date > expiry --> 1
** B: date == expiry --> 1
** C: date < expiry --> 0
**
** -- NULL case --
** D: date == NULL && expiry == NULL --> 1
**
** -- NULL expiry (expiry == now) --
** E: date == yesterday && expiry == NULL --> 0
** F: date == now && expiry == NULL --> 1
** G: date == tomorrow && expiry == NULL --> 1
**
** -- NULL date (date == now) --
** H: date == NULL && expiry == yesterday --> 1
** I: date == NULL && expiry == now --> 1
** J: date == NULL && expiry == tomorrow --> 0
**
** Simple, right ?
*/
int		ft_timeval_is_expired(struct timeval *date, struct timeval *expiry)
{
	struct timeval	now = { 0 };

	if (!date && !expiry)
		return (1);
	else if (!expiry || !date)
	{
		if (gettimeofday(&now, NULL) < 0)
		{
			ft_errno = -errno;
			return (-1);
		}
		date = !date ? &now : date;
		expiry = !expiry ? &now : expiry;
	}
	if (expiry->tv_sec == date->tv_sec)
		return (expiry->tv_usec <= date->tv_usec);
	return (expiry->tv_sec < date->tv_sec);
}
