/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 16:32:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:30:25 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

static int	doll_sign(char *f)
{
	char	*ref;

	ref = f;
	while (*f > 47 && *f < 58)
		f++;
	return (ref != f && *f == '$');
}

static int	add_flag(char c, t_pconv *conv)
{
	if (c == '#')
		conv->flags |= F_HASH;
	else if (c == '0')
		conv->flags |= F_ZERO;
	else if (c == '-')
		conv->flags |= F_MINUS;
	else if (c == ' ')
		conv->flags |= F_SPACE;
	else if (c == '+')
		conv->flags |= F_PLUS;
	else
		return (0);
	return (1);
}

static int	star_sign(char **f, int *ptr, t_farg *args)
{
	int	nb;
	int	value;

	nb = -1;
	if (**f == 42)
	{
		if (doll_sign(++(*f)))
		{
			if (!(nb = ft_atoi_forward(f)))
				return (-1);
			fetch(args, nb, C_INT, (void *)&value);
			(*f)++;
		}
		else
			fetch(args, 0, C_INT, (void *)&value);
		*ptr = value;
	}
	else if (**f > 47 && **f < 58)
		*ptr = ft_atoi_forward(f);
	else
		return (0);
	return (1);
}

int			get_flags(char **fmt, t_farg *args, t_pconv *conv)
{
	int		i;
	char	*ret;

	ret = *fmt;
	if (doll_sign(*fmt))
	{
		if (!(conv->arg = ft_atoi_forward(fmt)))
			return (-1);
		(*fmt)++;
	}
	while (add_flag(**fmt, conv))
		(*fmt)++;
	if (star_sign(fmt, &(conv->fw), args) < 0)
		return (-1);
	conv->flags |= conv->fw < 0 ? F_MINUS : 0;
	conv->fw *= conv->fw < 0 ? -1 : 1;
	if (**fmt == '.')
	{
		(*fmt)++;
		if ((i = star_sign(fmt, &(conv->prec), args)) < 0)
			return (-1);
		else if (i == 0)
			conv->prec = 0;
	}
	return (ret != *fmt);
}
