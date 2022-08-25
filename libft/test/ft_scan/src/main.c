/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 05:43:30 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/05 10:33:13 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define DEF_PORT	80

static t_scan	open_scan(t_ip *ip, struct timeval *timeout,
	enum e_ftscan_type scan_type, uint16_t port)
{
	t_scan	scan;

	switch (scan_type)
	{
		case E_FTSCAN_ECHO_PING:
			if ((scan = ft_echo_ping_open(ip, timeout)) < 0)
				ft_exit(EXIT_FAILURE, "ft_echo_ping_open: %s",
					ft_strerror(ft_errno));
			break;
		case E_FTSCAN_TCP_SYN:
			if ((scan = ft_tcp_syn_open(ip, port, timeout)) < 0)
				ft_exit(EXIT_FAILURE, "ft_tcp_syn_open: %s",
					ft_strerror(ft_errno));
			break;
		default:
			ft_exit(EXIT_FAILURE, "impossible error");
			break;
	}
	return (scan);
}

static void	print_result(t_scanres *result, t_scan scan)
{
	t_scan_control	*scan_ctrl;

	if (!(scan_ctrl = ft_get_scan(scan)))
		ft_exit(EXIT_FAILURE, "ft_get_scan: %s", ft_strerror(ft_errno));
	ft_printf("\nResult (%d: %s):\n", scan, ft_ip_str(&scan_ctrl->ip));
	ft_printf("rtt: tv_sec = %lld, tv_usec = %lld\n",
		result->rtt.tv_sec, result->rtt.tv_usec);
	ft_printf("ttl: %hhu\n", result->ttl);
	ft_printf("open: %s\n", result->open ? "true" : "false");
	ft_printf("reason: %d\n", result->reason);
	ft_printf("sequence: %hu\n", result->sequence);
	ft_printf("sequence from scan_control: %hu\n", scan_ctrl->sequence);
}

static void	print_events(uint16_t events)
{
	uint16_t	bit;
	int			is_first = 1;

	ft_printf("event:");
	if (!events)
		ft_printf(" NONE");
	for (int i = 0; i < 16 && events; ++i)
	{
		bit = events & 1;
		events >>= 1;
		switch (bit << i)
		{
			case E_POLLSC_NONE:											break;
			case E_POLLSC_REPLY:
				ft_printf("%s REPLY", is_first ? "" : "|");				break;
			case E_POLLSC_TIMEOUT:
				ft_printf("%s TIMEOUT", is_first ? "" : "|");			break;
			case E_POLLSC_ERROR:
				ft_printf("%s ERROR", is_first ? "" : "|");				break;
			default:
				ft_printf("%s unknown event %d", is_first ? "" : "|");	break;
		}
		if (bit)
			is_first = 0;
	}
	ft_printf("\n");
}

static void	send_probes(t_pollsc *scans, int host_count)
{
	for (int i = 0; i < host_count; ++i)
		if (ft_scan_send(scans[i].scan) < 0)
			ft_exit(EXIT_FAILURE, "ft_scan_send: %s", ft_strerror(ft_errno));
}

static int	get_results(int *done, t_pollsc *scans, int host_count)
{
	int			count = 0;
	t_scanres	result = { 0 };

	for (int i = 0; i < host_count; ++i)
	{
		if (scans[i].events || done[i])
			++count;
		if (!scans[i].events || done[i])
			continue;
		if (!(scans[i].events & E_POLLSC_TIMEOUT))
		{
			ft_printf("\n");
			print_events(scans[i].events);
		}
		if (ft_scan_result(&result, scans[i].scan) < 0)
			ft_exit(EXIT_FAILURE, "ft_scan_result: %s", ft_strerror(ft_errno));
		if (!(scans[i].events & E_POLLSC_TIMEOUT))
			print_result(&result, scans[i].scan);
		done[i] = 1;
		scans[i].scan *= -1;
	}
	return (count == host_count);
}

#define	MAX_ARGS	256
struct timeval		g_timeout = { 1, 500000 };

static void	mono_scan(char *host, int domain, enum e_ftscan_type scan_type)
{
	t_ip		ip;
	int			ret;
	t_scan		scan;
	t_scanres	result;

	if (ft_get_ip(&ip, host, domain) < 0)
		ft_exit(EXIT_FAILURE, "ft_get_ip: %s", gai_strerror(ret));
	ft_printf("IP: %s\n", ft_ip_str(&ip));
	scan = open_scan(&ip, &g_timeout, scan_type, DEF_PORT);
	while (!(ret = ft_echo_ping(&result, scan)))
	{
		print_result(&result, scan);
		sleep(1);
	}
	ft_scan_close(scan);
	if (ret < 0)
		ft_exit(EXIT_FAILURE, "ft_echo_ping: %s", ft_strerror(ft_errno));
}

static void	print_timeout_count(t_pollsc scans[MAX_ARGS], int host_count)
{
	int	count = 0;

	for (int i = 0; i < host_count; ++i)
		if (scans[i].events & E_POLLSC_TIMEOUT)
			++count;
	ft_printf("\nTIMEOUTS: %d/%d\n", count, host_count);
}

static void	multi_scan(char **hosts, int host_count, int domain,
	enum e_ftscan_type scan_type)
{
	int				ret;
	int				done[MAX_ARGS] = { 0 };
	t_ip			ip[MAX_ARGS] = { 0 };
	t_pollsc		scans[MAX_ARGS] = { 0 };

	if (!hosts[0] && ft_ip_rand(ip, host_count, domain, 0) < 0)
		ft_exit(EXIT_FAILURE, "ft_ip_rand: %s", ft_strerror(ft_errno));
	for (int i = 0; i < host_count; ++i)
	{
		if (hosts[0] && (ret = ft_get_ip(ip + i, hosts[i], domain)) < 0)
			ft_exit(EXIT_FAILURE, "ft_get_ip: %s", gai_strerror(ret));
		ft_printf("IP: %s\n", ft_ip_str(ip + i));
		if ((scans[i].scan = ft_echo_ping_open(ip + i, &g_timeout)) < 0)
			ft_exit(EXIT_FAILURE, "ft_echo_ping_open: %s",
				ft_strerror(ft_errno));
		scans[i].scan = open_scan(ip + i, &g_timeout, scan_type, DEF_PORT);
	}
	send_probes(scans, host_count);
	while ((ret = ft_scan_poll(scans, host_count, NULL)) >= 0)
	{
		if (!ret) continue;
		if (get_results(done, scans, host_count))
		{
			print_timeout_count(scans, host_count);
			ft_bzero(done, sizeof(done));
			for (int i = 0; i < host_count; ++i)
				scans[i].scan *= -1;
			sleep(1);
			send_probes(scans, host_count);
		}
	}
	ft_exit(EXIT_FAILURE, "ft_scan_poll: %s", ft_strerror(ft_errno));
}

# define	DEF_RANDOM_IP_COUNT	128

int	main(int argc, char **argv)
{
	int					host_count;
	int					first_host = 1;
	int					domain = AF_UNSPEC;
	enum e_ftscan_type	scan_type = E_FTSCAN_ECHO_PING;

	while (argv[first_host] && argv[first_host][0] == '-')
	{
		if (!ft_strcmp(argv[first_host], "-4"))
			domain = AF_INET;
		else if (!ft_strcmp(argv[first_host], "-6"))
			domain = AF_INET6;
		else if (!ft_strcmp(argv[first_host], "--syn"))
			scan_type = E_FTSCAN_TCP_SYN;
		else if (!ft_strcmp(argv[first_host], "--ping"))
			scan_type = E_FTSCAN_ECHO_PING;
		else
			ft_exit(EXIT_FAILURE, "unknown option %s", argv[first_host]);
		++first_host;
	}
	host_count = argc - first_host;
	if (host_count > MAX_ARGS)
		ft_exit(EXIT_FAILURE, "no more than %d hosts", MAX_ARGS);
	ft_atexit(ft_scan_close_all);
	if (!host_count)
		host_count = DEF_RANDOM_IP_COUNT;
	if (host_count == 1)
		mono_scan(argv[first_host], domain, scan_type);
	else
		multi_scan(argv + first_host, host_count, domain, scan_type);
	return (EXIT_SUCCESS);
}
