/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scans.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 13:57:40 by yforeau           #+#    #+#             */
/*   Updated: 2022/02/19 22:24:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_scan_result: Get the result for scan after a poll event blocking recv
*/
int			ft_scan_result(t_scanres *result, t_scan scan)
{
	t_scan_control	*scan_ctrl;

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	if (!ft_timeval_is_zero(&scan_ctrl->sent_ts))
	{
		ft_errno = E_FTERR_NET_SCAN_IN_PROGRESS;
		return (-1);
	}
	ft_memcpy(result, &scan_ctrl->result, sizeof(scan_ctrl->result));
	ft_bzero(&scan_ctrl->result, sizeof(scan_ctrl->result));
	return (0);
}

/*
** blocking_scan: Start given scan in blocking mode
**
** Returns -1 on error and sets ft_errno, 0 on success and 1 on scan failure.
** Scan information is set in result if no error occured.
*/
static int	blocking_scan(t_scanres *result, t_scan scan)
{
	t_scan_control	*scan_ctrl;

	if (!(scan_ctrl = ft_get_scan(scan)))
		return (-1);
	if (ft_scan_send(scan) < 0)
		return (-1);
	if (ft_scan_recv(scan, 1, NULL) < 0)
		return (-1);
	return (ft_scan_result(result, scan));
}

/*
** ft_echo_ping: start a blocking echo_ping scan
*/
int			ft_echo_ping(t_scanres *result, t_scan scan)
{
	return (blocking_scan(result, scan));
}

/*
** ft_tcp_syn: start a blocking tcp_syn scan
*/
int			ft_tcp_syn(t_scanres *result, t_scan scan)
{
	return (blocking_scan(result, scan));
}
