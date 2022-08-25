/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 11:13:12 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/11 09:16:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_option_argument.h"
#include "get_short_option.h"

static int	ft_getopt_internal(int argc, char **argv, t_optdata *d)
{
	int	c;

	if (argc < 1)
		return (-1);
	d->optarg = 0;
	if (!*d->nextchar)
	{
		if (get_next_option_argument(argc, argv, d, &c))
			return (c);
		d->nextchar = argv[d->optind] + 1;
	}
	return (get_short_option(argc, argv, d));
}

int			ft_getopt(int argc, char **argv, t_optdata *d)
{
	d->order = d->order == DEFAULT ? LEAVE : d->order;
	return (ft_getopt_internal(argc, argv, d));
}

int			ft_getopt_long(int argc, char **argv, t_optdata *d)
{
	d->order = d->order == DEFAULT ? PERMUTE : d->order;
	return (ft_getopt_internal(argc, argv, d));
}

int			ft_getopt_long_only(int argc, char **argv, t_optdata *d)
{
	d->longonly = 1;
	d->order = d->order == DEFAULT ? PERMUTE : d->order;
	return (ft_getopt_internal(argc, argv, d));
}
