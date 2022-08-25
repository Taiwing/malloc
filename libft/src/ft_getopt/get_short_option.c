/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_short_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 19:37:35 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/17 13:48:13 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "getopt_errors.h"

/*if current option requires an argument, this function gets it*/
static int	get_argument(int argc, char **argv, t_optdata *d, char *tmp)
{
	if ((d->optarg = *d->nextchar ? d->nextchar : NULL))
		++d->optind;
	/*else if the option is mandatory*/
	else if (tmp[2] != ':')
	{
		/*if next argument is the end of argv*/
		if (d->optind == argc)
		{
			if (d->opterr)
				print_short_error(1, *argv, *tmp);
			d->optopt = *tmp;
			return (*d->optstring == ':' ? ':' : '?');
		}
		else
			d->optarg = argv[d->optind++];
	}
	d->nextchar = "\0";
	return (*tmp);
}

int			get_short_option(int argc, char **argv, t_optdata *d)
{
	int			c;
	char		*tmp;

	c = *d->nextchar++;
	if (!*d->nextchar)
		++d->optind;
	tmp = ft_strchr(d->optstring, c);
	if (!tmp || c == ':')
	{
		if (d->opterr)
			print_short_error(0, *argv, c);
		d->optopt = c;
		return ('?');
	}
	if (tmp[1] == ':')
		c = get_argument(argc, argv, d, tmp);
	return (c);
}
