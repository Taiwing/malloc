/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopt_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 05:59:39 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 23:03:03 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

int		getopt_puterr(const char *str)
{
	return(write(2, str, ft_strlen(str)));
}

void	option_puterr(const char *prefix, const char *name)
{
	getopt_puterr(" '");
	getopt_puterr(prefix);
	getopt_puterr(name);
	getopt_puterr("'");
}

int		print_short_error(int missing, char *prog_name, char c)
{
	if (getopt_puterr(prog_name) < 0 || getopt_puterr(missing ?
		": option requires an argument -- " : ": illegal option -- ") < 0
		|| write(2, &c, 1) < 0)
		return (-1);
	return (getopt_puterr("\n"));
}
