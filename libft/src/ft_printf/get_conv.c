/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_conv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 18:34:46 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:30:46 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

static int	get_type(char **fmt, t_pconv *conv)
{
	char	*t;

	if ((t = ft_strchr(FTP_CONVERSIONS, **fmt)) && !*t)
		return (0);
	conv->type = *(*fmt)++;
	if (ft_strchr(FTP_INT_CONV, conv->type))
	{
		if (ft_toupper(conv->type) == conv->type && conv->type != 'X')
			conv->cast = C_LONG;
		if (!ft_strchr(FTP_SINT_CONV, conv->type))
			conv->cast |= C_UNSIGNED;
	}
	else if ((conv->type == 'c' || conv->type == 's') && conv->cast & C_LONG)
		conv->type -= 32;
	return (1);
}

static int	get_cast(char **fmt, t_pconv *conv)
{
	int	ret;

	ret = 0;
	while (ft_strchr("hljzL", **fmt) && **fmt)
	{
		if (**fmt == 'h' && !(conv->cast & C_SHORT))
			conv->cast |= C_SHORT;
		else if (**fmt == 'h')
			conv->cast = (conv->cast | C_CHAR) & ~C_SHORT;
		else if (**fmt == 'l' && !(conv->cast & C_LONG))
			conv->cast |= C_LONG;
		else if (**fmt == 'l' || **fmt == 'L')
			conv->cast = (conv->cast | C_LONG_LONG) & ~C_LONG;
		else if (**fmt == 'j')
			conv->cast |= C_INTMAX_T;
		else if (**fmt == 'z')
			conv->cast |= C_SIZE_T;
		(*fmt)++;
		ret = 1;
	}
	return (ret);
}

int			get_conv(char **fmt, t_farg *args, t_pconv *conv)
{
	int		f;
	int		c;

	(*fmt)++;
	f = 1;
	c = 1;
	while (f || c)
	{
		if ((f = get_flags(fmt, args, conv)) < 0)
			return (0);
		c = get_cast(fmt, conv);
	}
	return (get_type(fmt, conv));
}
