/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_conversions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 20:36:04 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:29:00 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

void	c_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt)
{
	unsigned char	nb;

	(void)fmt;
	fetch(args, conv->arg, C_CHAR | C_UNSIGNED, (void *)(&nb));
	pdata_add(loc, NULL, (char)nb, 1);
}

void	s_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt)
{
	char	*str;

	(void)fmt;
	fetch(args, conv->arg, C_CHAR_P, (void *)(&str));
	if (!str)
		pdata_add(loc, "(null)", 0,
		(conv->prec > 5 || conv->prec < 0) ? 6 : conv->prec);
	else if (str && (conv->prec < 0 || ft_memchr((void *)str, 0, conv->prec)))
		pdata_add(loc, str, 0, ft_strlen(str));
	else if (str)
		pdata_add(loc, str, 0, conv->prec);
}

void	p_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt)
{
	unsigned long int	ptr;

	(void)fmt;
	fetch(args, conv->arg, C_INT | C_LONG | C_UNSIGNED, (void *)(&ptr));
	itoa_ulint(loc, ptr, 16, conv->type == 'P');
}
