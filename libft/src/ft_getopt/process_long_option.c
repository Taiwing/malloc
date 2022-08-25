/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_long_option.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 22:23:19 by yforeau           #+#    #+#             */
/*   Updated: 2019/03/28 12:07:48 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "getopt_errors.h"

static void		add_ambig(t_optdata *d, t_opt *found,
						unsigned char **ambig_set,
						int *ambig_fallback)
{
	if (!*ambig_fallback)
	{
		if (!d->opterr)
			*ambig_fallback = 1;
		else if (!*ambig_set)
		{
			if (!(*ambig_set = (unsigned char *)ft_secmalloc(d->lopts_len)))
				*ambig_fallback = 1;
			else
				ft_memset(*ambig_set, 0, d->lopts_len);
		}
		if (*ambig_set)
			(*ambig_set)[found - d->longopts] = 1;
	}
}

static t_opt	*find_abbrev(char **argv, t_optdata *d,
							char *prefix, size_t namelen)
{
	t_opt			*p;
	t_opt			*found;
	unsigned char	*ambig_set;
	int				ambig_fallback;
	int				match;

	found = 0;
	p = d->longopts;
	ambig_set = 0;
	ambig_fallback = 0;
	while (p->name)
	{
		match = !ft_strncmp(p->name, d->nextchar, namelen);
		if (match && !found)
			found = p;
		else if (match && (d->longonly || found->has_arg != p->has_arg
				|| found->flag != p->flag || found->val != p->val))
			add_ambig(d, p, &ambig_set, &ambig_fallback);
		++p;
	}
	if (ambig_set)
		add_ambig(d, found, &ambig_set, &ambig_fallback);
	if (ambig_set || ambig_fallback)
		print_ambig_error(d, ambig_set, argv, prefix);
	return (ambig_set || ambig_fallback ? p : found);
}

static t_opt	*find_exact_match(t_optdata *d, char **end, size_t *namelen)
{
	t_opt	*p;

	*end = d->nextchar;
	while (**end && **end != '=')
		++(*end);
	*namelen = *end - d->nextchar;
	p = d->longopts;
	while (p->name)
	{
		if (!ft_strncmp(p->name, d->nextchar, *namelen)
			&& *namelen == ft_strlen(p->name))
			return (p);
		++p;
	}
	return (0);
}

int				process_long_option(int argc, char **argv,
									t_optdata *d, char *prefix)
{
	char	*end;
	t_opt	*found;
	size_t	namelen;

	if (!(found = find_exact_match(d, &end, &namelen)))
	{
		if (!(found = find_abbrev(argv, d, prefix, namelen)) || !found->name)
			return (found && !found->name ? '?'
					: unknown_long_option_error(d, argv, prefix));
	}
	++d->optind;
	d->nextchar = "\0";
	if (*end && found->has_arg)
		d->optarg = end + 1;
	else if (*end)
		return (superfluous_arg_error(d, argv, prefix, found));
	else if (found->has_arg && d->optind < argc)
		d->optarg = argv[d->optind++];
	else if (found->has_arg)
		return (missing_arg_error(d, argv, prefix, found));
	if (d->longindex)
		*d->longindex = found - d->longopts;
	if (found->flag)
		*found->flag = found->val;
	return (found->flag ? 0 : found->val);
}
