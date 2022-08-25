/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scan_poll.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 15:11:43 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/05 10:54:27 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <poll.h>

static int	ft_scan_check_timeout(t_scan scan, struct timeval *now)
{
	int				ret;
	struct timeval	expiry;
	t_scan_control	*scan_ctrl;

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	if (ft_timeval_add(&expiry, &scan_ctrl->sent_ts, &scan_ctrl->timeout) < 0)
		return (-1);
	if ((ret = ft_timeval_is_expired(now, &expiry)) < 0)
		return (-1);
	else if (ret)
	{
		scan_ctrl->result.sequence = scan_ctrl->sequence;
		ft_bzero(&scan_ctrl->sent_ts, sizeof(scan_ctrl->sent_ts));
	}
	return (ret);
}

static int	get_polling_results(t_pollsc *scans, int count,
	struct pollfd *pfds)
{
	struct timeval	now;
	int				timeout, events = 0;
	
	if (gettimeofday(&now, NULL) < 0)
	{
		ft_errno = -errno;
		return (-1);
	}
	for (int i = 0; i < count; ++i)
	{
		if (scans[i].scan < 0) continue;
		if (pfds[i].revents & ~POLLIN)
			scans[i].events |= E_POLLSC_ERROR;
		if (pfds[i].revents & POLLIN
			&& ft_scan_recv(scans[i].scan, 0, &now) < 0)
			return (-1);
		else if (pfds[i].revents & POLLIN)
			scans[i].events |= E_POLLSC_REPLY;
		else if ((timeout = ft_scan_check_timeout(scans[i].scan, &now)) < 0)
			return (-1);
		else if (timeout)
			scans[i].events |= E_POLLSC_TIMEOUT;
		events += !!scans[i].events;
	}
	return (events);
}

static int	scan_poll_setup(struct pollfd *pfds, t_pollsc *scans, int count)
{
	t_scan_control	*scan_ctrl;

	if (count > MAX_GLOBAL_SCAN_COUNT || count <= 0)
	{
		ft_errno = E_FTERR_NET_INVALID_SCAN_COUNT;
		return (-1);
	}
	ft_bzero(pfds, count * sizeof(struct pollfd));
	for (int i = 0; i < count; ++i)
	{
		if (scans[i].scan < 0)
		{
			pfds[i].fd = -1;
			continue;
		}
		if (!(scan_ctrl = ft_get_scan(scans[i].scan)))
			return (-1);
		pfds[i].fd = scan_ctrl->recvfd;
		pfds[i].events = POLLIN;
		scans[i].events = 0;
	}
	return (0);
}

/*
** ft_scan_poll: Check if there is any event for the given scans
**
** Events will be returned in the event field of each t_pollsc in the list.
** scans is an array of t_pollsc structures specifying which scans to listen
** for. If the t_scan value is negative the scan will be ignored (like in the
** original poll() function). count is the length of the scans array. timeout
** is the amount of time to poll for events.
**
** If events are catched they will be set in the scan_control structure and
** ft_scan_poll will return immediately. Otherwise it will wait for the timeout
** and return 0. If no timeout is given, it will only return a non-zero
** value if events are detected right away (or an error occurs).
** The timeout value is rounded up to ms.
*/
int	ft_scan_poll(t_pollsc *scans, int count, struct timeval *timeout)
{
	int				ms_timeout = 0;
	struct pollfd	pfds[MAX_GLOBAL_SCAN_COUNT];

	if (scan_poll_setup(pfds, scans, count) < 0)
		return (-1);
	if (timeout)
		ms_timeout = timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
	if (poll(pfds, count, ms_timeout) < 0)
	{
		ft_errno = -errno;
		return (-1);
	}
	return (get_polling_results(scans, count, pfds));
}
