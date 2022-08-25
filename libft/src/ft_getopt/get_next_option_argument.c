/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_option_argument.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 17:58:46 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/17 10:53:54 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "permute_order.h"
#include "process_long_option.h"
#include "libft.h"

static void	handle_stop_string(int argc, char **argv, t_optdata *d)
{
	++d->optind;
	if (d->first_nonopt != d->last_nonopt && d->last_nonopt != d->optind)
		permute((uintptr_t *)argv, d);
	else if (d->first_nonopt == d->last_nonopt)
		d->first_nonopt = d->optind;
	d->last_nonopt = argc;
	d->optind = argc;
}

static int	*get_long_option(int argc, char **argv, t_optdata *d, int *c)
{
	/*then the option string begins with "--" and is a long option*/
	if (argv[d->optind][1] == '-')
	{
		d->nextchar = argv[d->optind] + 2;
		*c = process_long_option(argc, argv, d, "--");
		return (c);
	}
	if (d->longonly && (argv[d->optind][2]
		|| !ft_strchr(d->optstring, argv[d->optind][1])))
	{
		d->nextchar = argv[d->optind] + 1;
		if ((*c = process_long_option(argc, argv, d, "-")) != -1)
			return (c);
	}
	return (0);
}

/*is gonna fetch the next argument and check if it is an option*/
int			*get_next_option_argument(int argc, char **argv,
									t_optdata *d, int *c)
{
	/*in case the user moves back optind*/
	if (d->first_nonopt > d->optind)
		d->first_nonopt = d->optind;
	if (d->last_nonopt > d->optind)
		d->last_nonopt = d->optind;
	if (d->order == PERMUTE)
		permute_order(argc, argv, d);
	if (d->optind != argc && !ft_strcmp(argv[d->optind], "--"))
		handle_stop_string(argc, argv, d);
	if (d->optind == argc)
	{
		if (d->first_nonopt != d->last_nonopt)
			d->optind = d->first_nonopt;
		*c = -1;
	}
	else if (argv[d->optind][0] != '-' || !argv[d->optind][1])
	{
		if ((*c = d->order == LEAVE ? -1 : 1) == 1)
			d->optarg = argv[d->optind++];
	}
	else
		return (d->longopts ? get_long_option(argc, argv, d, c) : 0);
	return (c);
}
